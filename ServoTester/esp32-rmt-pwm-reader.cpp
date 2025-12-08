/**
*********************************************************************************
* @file:	esp32_rmt_pwm_reader.cpp
* @originalauthor:	R.W.
* @author:  R.M.
* @date:	08.12.25
* @brief:   Compatible with both ESP-IDF v4 and v5 RMT API
* @history:
*********************************************************************************
**/
#include "esp32-rmt-pwm-reader.h"

// globals
int _numberOfChannels;
static pwm_channel_t *pwm_channels;

static const char *PwmStateStrings[] = {
  "NOT_CONNECTED",
  "UNSTABLE",
  "STABLE"
};

// Common handles for cleanup
xQueueHandle interruptQueue;
TaskHandle_t controlTask;

// Version-specific handles
#ifdef USE_RMT_V5_API
static rmt_channel_handle_t *rmt_rx_channels = NULL;
static rmt_symbol_word_t **rx_buffers = NULL;
#else
rmt_isr_handle_t rmt_isr_handle;
#endif

// data to send from isr/callback to control task
struct isr_data {
  uint8_t channel;
  int64_t currentPwmInterruptTime;
  uint32_t highus;
};

// declare internal functions
static void pwm_control_task(void *parms);
PwmState getNewState(const uint8_t channel);
uint16_t calculateChannelZero(const uint8_t channel);
void calculateChannelMinMax(const uint8_t channel);

// ============================================================================
// VERSION-SPECIFIC ISR/CALLBACK IMPLEMENTATIONS
// ============================================================================

#ifdef USE_RMT_V5_API
/**
 * RMT receive done callback for IDF v5
 */
static bool IRAM_ATTR rmt_rx_done_callback(rmt_channel_handle_t channel_handle,
                                           const rmt_rx_done_event_data_t *edata,
                                           void *user_data) {
  BaseType_t high_task_wakeup = pdFALSE;
  uint8_t channel = *(uint8_t *)user_data;

  if (edata->num_symbols > 0) {
    uint32_t highus = edata->received_symbols[0].duration0;

    isr_data isrData = {
      .channel = channel,
      .currentPwmInterruptTime = esp_timer_get_time(),
      .highus = highus
    };

    xQueueSendFromISR(interruptQueue, &isrData, &high_task_wakeup);
  }

  // Restart reception immediately
  rmt_receive_config_t receive_config = {
    .signal_range_min_ns = 1000,
    .signal_range_max_ns = 3500000,
  };

  rmt_receive(channel_handle, rx_buffers[channel],
              64 * sizeof(rmt_symbol_word_t), &receive_config);

  return high_task_wakeup == pdTRUE;
}

#else
/**
 * Interrupt service routine handler for the RMT peripheral (IDF v4)
 */
static void IRAM_ATTR rmt_isr_handler(void *arg) {
  uint32_t intr_st = RMT.int_st.val;

  for (uint8_t channel = 0; channel < _numberOfChannels; channel++) {
    uint32_t channel_mask = BIT(channel * 3 + 1);

    if (!(intr_st & channel_mask))
      continue;

    RMT.conf_ch[channel].conf1.rx_en = 0;
    RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_TX;

    volatile rmt_item32_t *item = RMTMEM.chan[channel].data32;

    isr_data isrData = {};
    if (item) {
      isrData = {
        .channel = channel,
        .currentPwmInterruptTime = esp_timer_get_time(),
        .highus = item->duration0
      };

      xQueueSendFromISR(interruptQueue, &isrData, NULL);
    }

    RMT.conf_ch[channel].conf1.mem_wr_rst = 1;
    RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_RX;
    RMT.conf_ch[channel].conf1.rx_en = 1;

    RMT.int_clr.val = channel_mask;
  }
}
#endif

// ============================================================================
// COMMON CONTROL TASK
// ============================================================================

static void pwm_control_task(void *parms) {
  isr_data isrData = {};
  while (true) {
    if (xQueueReceive(interruptQueue, &isrData, portMAX_DELAY)) {
      uint8_t channel = isrData.channel;
      auto &data = pwm_channels[channel].data;

      uint64_t totalus = isrData.currentPwmInterruptTime - data.lastIr;
      data.lastTotalus = data.totalus;
      data.totalus = totalus;

      data.frq = (totalus > 0) ? ((10000000 / totalus) + 5) / 10 : 0;  // truncate to nearest integer without using float functions

      data.highus = isrData.highus;
      data.lastIr = isrData.currentPwmInterruptTime;

      getNewState(channel);
    }
    vTaskDelay(1);
  }
}

// ============================================================================
// COMMON INITIALIZATION
// ============================================================================

void pwm_reader_init(const uint8_t channelPins[], const int numberOfChannels) {
  _numberOfChannels = numberOfChannels;

  pwm_channels = new pwm_channel_t[_numberOfChannels * sizeof(pwm_channel_t)];

  struct pwm_config default_config = {
    .pulse_min = SCALE_PULSE_MIN,
    .pulse_max = SCALE_PULSE_MAX,
    .pulse_neutral = SCALE_PULSE_NEUTRAL,
    .period_max = 2 * PERIOD_MIN_VALID,
    .auto_zero = AUTO_ZERO_CALIBRATION,
    .auto_min_max = AUTO_MIN_MAX_CALIBRATION,
    .failsafe = GET_FAILESAFE
  };

  struct pwm_data initial_data;
  for (int channel = 0; channel < _numberOfChannels; channel++) {
    pwm_channels[channel].pin = channelPins[channel];
    pwm_channels[channel].channel = channel;
    pwm_channels[channel].config = default_config;
    pwm_channels[channel].data = initial_data;
    if (pwm_channels[channel].pin>34) {
        pinMode(pwm_channels[channel].pin, INPUT);
    } else {
        pinMode(pwm_channels[channel].pin, INPUT_PULLDOWN);
    }
  }

  ESP_LOGI(TAG, "Initialized with ESP-IDF v%d.%d.%d",
           ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
}

// ============================================================================
// VERSION-SPECIFIC BEGIN IMPLEMENTATIONS
// ============================================================================

#ifdef USE_RMT_V5_API
esp_err_t pwm_reader_begin() {
  esp_err_t ret;

  interruptQueue = xQueueCreate(8, sizeof(isr_data));
  if (interruptQueue == NULL) {
    ESP_LOGE(TAG, "Failed to create queue");
    return ESP_FAIL;
  }

  if (xTaskCreate(pwm_control_task, "pwm_control_task", 2048, NULL, 1, &controlTask) != pdPASS) {
    ESP_LOGE(TAG, "Failed to create task");
    vQueueDelete(interruptQueue);
    return ESP_FAIL;
  }

  rmt_rx_channels = (rmt_channel_handle_t *)malloc(_numberOfChannels * sizeof(rmt_channel_handle_t));
  if (rmt_rx_channels == NULL) {
    ESP_LOGE(TAG, "Failed to allocate channel handles");
    vQueueDelete(interruptQueue);
    vTaskDelete(controlTask);
    return ESP_FAIL;
  }

  rx_buffers = (rmt_symbol_word_t **)malloc(_numberOfChannels * sizeof(rmt_symbol_word_t *));
  if (rx_buffers == NULL) {
    ESP_LOGE(TAG, "Failed to allocate buffer array");
    free(rmt_rx_channels);
    vQueueDelete(interruptQueue);
    vTaskDelete(controlTask);
    return ESP_FAIL;
  }

  for (uint8_t channel = 0; channel < _numberOfChannels; channel++) {
    rmt_rx_channel_config_t rx_chan_config = {
      .gpio_num = (gpio_num_t)pwm_channels[channel].pin,
      .clk_src = RMT_CLK_SRC_DEFAULT,
      .resolution_hz = 1000000,
      .mem_block_symbols = 64,
      .flags = {
        .invert_in = false,
        .with_dma = false,
        .io_loop_back = false,
      }
    };

    ret = rmt_new_rx_channel(&rx_chan_config, &rmt_rx_channels[channel]);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to create RX channel %d: %s", channel, esp_err_to_name(ret));
      for (uint8_t i = 0; i < channel; i++) {
        rmt_del_channel(rmt_rx_channels[i]);
        free(rx_buffers[i]);
      }
      free(rx_buffers);
      free(rmt_rx_channels);
      vQueueDelete(interruptQueue);
      vTaskDelete(controlTask);
      return ESP_FAIL;
    }

    rmt_rx_event_callbacks_t cbs = {
      .on_recv_done = rmt_rx_done_callback,
    };

    ret = rmt_rx_register_event_callbacks(rmt_rx_channels[channel], &cbs,
                                          &pwm_channels[channel].channel);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to register callback for channel %d", channel);
      for (uint8_t i = 0; i <= channel; i++) {
        rmt_del_channel(rmt_rx_channels[i]);
        if (i < channel) free(rx_buffers[i]);
      }
      free(rx_buffers);
      free(rmt_rx_channels);
      vQueueDelete(interruptQueue);
      vTaskDelete(controlTask);
      return ESP_FAIL;
    }

    ret = rmt_enable(rmt_rx_channels[channel]);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to enable channel %d", channel);
      for (uint8_t i = 0; i <= channel; i++) {
        rmt_del_channel(rmt_rx_channels[i]);
        if (i < channel) free(rx_buffers[i]);
      }
      free(rx_buffers);
      free(rmt_rx_channels);
      vQueueDelete(interruptQueue);
      vTaskDelete(controlTask);
      return ESP_FAIL;
    }

    rx_buffers[channel] = (rmt_symbol_word_t *)malloc(64 * sizeof(rmt_symbol_word_t));
    if (rx_buffers[channel] == NULL) {
      ESP_LOGE(TAG, "Failed to allocate RX buffer for channel %d", channel);
      for (uint8_t i = 0; i <= channel; i++) {
        rmt_disable(rmt_rx_channels[i]);
        rmt_del_channel(rmt_rx_channels[i]);
        if (i < channel) free(rx_buffers[i]);
      }
      free(rx_buffers);
      free(rmt_rx_channels);
      vQueueDelete(interruptQueue);
      vTaskDelete(controlTask);
      return ESP_FAIL;
    }

    rmt_receive_config_t receive_config = {
      .signal_range_min_ns = 1000,
      .signal_range_max_ns = 3500000,
    };

    ret = rmt_receive(rmt_rx_channels[channel], rx_buffers[channel],
                      64 * sizeof(rmt_symbol_word_t), &receive_config);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "Failed to start receiving on channel %d: %s", channel, esp_err_to_name(ret));
      for (uint8_t i = 0; i <= channel; i++) {
        rmt_disable(rmt_rx_channels[i]);
        rmt_del_channel(rmt_rx_channels[i]);
        free(rx_buffers[i]);
      }
      free(rx_buffers);
      free(rmt_rx_channels);
      vQueueDelete(interruptQueue);
      vTaskDelete(controlTask);
      return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Channel %d initialized on GPIO %d (IDF v5 API)", channel, pwm_channels[channel].pin);
  }

  return ESP_OK;
}

#else
esp_err_t pwm_reader_begin() {
  interruptQueue = xQueueCreate(8, sizeof(isr_data));
  if (interruptQueue == NULL) {
    ESP_LOGE(TAG, "Failed to create queue");
    return ESP_FAIL;
  }

  if (xTaskCreate(pwm_control_task, "pwm_control_task", 2048, NULL, 1, &controlTask) != pdPASS) {
    ESP_LOGE(TAG, "Failed to create task");
    vQueueDelete(interruptQueue);
    return ESP_FAIL;
  }

  for (uint8_t channel = 0; channel < _numberOfChannels; channel++) {
    rmt_config_t rxConfig = {};

    rxConfig.rmt_mode = RMT_MODE_RX;
    rxConfig.channel = (rmt_channel_t)pwm_channels[channel].channel;
    rxConfig.clk_div = 80;
    rxConfig.gpio_num = (gpio_num_t)pwm_channels[channel].pin;
    rxConfig.mem_block_num = 1;
    rxConfig.rx_config.filter_en = true;
    rxConfig.rx_config.filter_ticks_thresh = 100;
    rxConfig.rx_config.idle_threshold = IDLE_TRESHOLD;

    if (rxConfig.gpio_num == GPIO_NUM_NC) {
      ESP_LOGE(TAG, "Invalid GPIO for channel %d", channel);
      return ESP_FAIL;
    }

    if (rmt_config(&rxConfig) != ESP_OK) {
      ESP_LOGE(TAG, "Failed to configure RMT channel %d", channel);
      return ESP_FAIL;
    }

    if (rmt_set_rx_intr_en(rxConfig.channel, true) != ESP_OK) {
      ESP_LOGE(TAG, "Failed to enable interrupt for channel %d", channel);
      return ESP_FAIL;
    }

    if (rmt_rx_start(rxConfig.channel, 1) != ESP_OK) {
      ESP_LOGE(TAG, "Failed to start RX for channel %d", channel);
      return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Channel %d initialized on GPIO %d (IDF v4 API)", channel, pwm_channels[channel].pin);
  }

  if (rmt_isr_register(rmt_isr_handler, NULL, 0, &rmt_isr_handle) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to register ISR handler");
    return ESP_FAIL;
  }

  return ESP_OK;
}
#endif

// ============================================================================
// VERSION-SPECIFIC CLEANUP IMPLEMENTATIONS
// ============================================================================

#ifdef USE_RMT_V5_API
esp_err_t pwm_cleanup() {
  if (rmt_rx_channels != NULL) {
    for (uint8_t channel = 0; channel < _numberOfChannels; channel++) {
      rmt_disable(rmt_rx_channels[channel]);
      rmt_del_channel(rmt_rx_channels[channel]);
      pinMode(pwm_channels[channel].pin, INPUT);
      if (rx_buffers && rx_buffers[channel]) {
        free(rx_buffers[channel]);
      }
    }
    free(rmt_rx_channels);
    rmt_rx_channels = NULL;
  }

  if (rx_buffers != NULL) {
    free(rx_buffers);
    rx_buffers = NULL;
  }

  if (interruptQueue != NULL) {
    vQueueDelete(interruptQueue);
    interruptQueue = NULL;
  }

  if (controlTask != NULL) {
    vTaskDelete(controlTask);
    controlTask = NULL;
  }

  if (pwm_channels != NULL) {
    delete[] pwm_channels;
    pwm_channels = NULL;
  }

  return ESP_OK;
}

#else
esp_err_t pwm_cleanup() {
  for (uint8_t channel = 0; channel < _numberOfChannels; channel++) {
    RMT.conf_ch[channel].conf1.mem_wr_rst = 1;
    RMT.conf_ch[channel].conf1.rx_en = 0;
    RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_TX;
    if (rmt_rx_stop((rmt_channel_t)channel) != ESP_OK) {
      return ESP_FAIL;
    }
    pinMode(pwm_channels[channel].pin, INPUT);
  }

  if (rmt_isr_deregister(rmt_isr_handle) != ESP_OK) {
    return ESP_FAIL;
  }

  vQueueDelete(interruptQueue);
  vTaskDelete(controlTask);
  delete[] pwm_channels;

  return ESP_OK;
}
#endif

// ============================================================================
// COMMON GETTER AND SETTER FUNCTIONS (unchanged)
// ============================================================================

const pwm_channel_t *pwm_get_whole_channels_object() {
  return pwm_channels;
}

const pwm_data *pwm_get_channel_data(const uint8_t channel) {
  return &pwm_channels[channel].data;
}

const pwm_config *pwm_get_channel_config(const uint8_t channel) {
  return &pwm_channels[channel].config;
}

void pwm_set_channel_rmtChannel(const uint8_t channel, const uint8_t rmtChannel) {
  pwm_channels[channel].channel = rmtChannel;
}

void pwm_set_channel_pulse_min(const uint8_t channel, const uint16_t pulse_min) {
  pwm_channels[channel].config.pulse_min = pulse_min;
}

void pwm_set_channel_pulse_max(const uint8_t channel, const uint16_t pulse_max) {
  pwm_channels[channel].config.pulse_max = pulse_max;
}

void pwm_set_channel_pulse_neutral(const uint8_t channel, const uint16_t pulse_neutral) {
  pwm_channels[channel].config.pulse_neutral = pulse_neutral;
}

void pwm_set_channel_period_max(const uint8_t channel, const uint16_t period_max) {
  pwm_channels[channel].config.period_max = period_max;
}

void pwm_set_auto_zero(const uint8_t channel, const boolean auto_zero) {
  pwm_channels[channel].config.auto_zero = auto_zero;
}

void pwm_set_auto_min_max(const uint8_t channel, const boolean auto_min_max) {
  pwm_channels[channel].config.auto_min_max = auto_min_max;
}

void pwm_set_failsafe(const uint8_t channel, const boolean failsafe) {
  pwm_channels[channel].config.failsafe = failsafe;
}

const PwmState pwm_get_state(const uint8_t channel) {
  return pwm_channels[channel].data.state;
}

void pwm_set_state(const uint8_t channel, const PwmState state) {
  pwm_channels[channel].data.state = state;
}

const char *pwm_get_state_name(const uint8_t channel) {
  return PwmStateStrings[static_cast<size_t>(pwm_channels[channel].data.state)];
}

const uint16_t pwm_get_frequency(const uint8_t channel) {
  return pwm_channels[channel].data.frq;
}

const int32_t pwm_get_rawPwm(const uint8_t channel) {
  const auto &data = pwm_channels[channel].data;
  const auto &config = pwm_channels[channel].config;

  if (data.state == PwmState::STABLE) {
    return static_cast<int32_t>(GET_FAILESAFE_DURING_MIN_MAX && config.auto_min_max)
             ? config.pulse_neutral
             : data.highus;
  }
  return static_cast<int32_t>(config.failsafe)
           ? config.pulse_neutral
           : -1;
}

int32_t calculateOffset(const uint8_t channel) {
  const auto &data = pwm_channels[channel].data;

  if ((data.state != PwmState::STABLE) || (pwm_channels[0].data.state != PwmState::STABLE)) {
    return 0;
  }

  int frq_diff = static_cast<int>((data.frq) - (pwm_channels[0].data.frq));
  if (channel == 0 || (abs(frq_diff) > 2)) {
    return 0;
  }

  int64_t time_diff = (data.lastIr - pwm_channels[0].data.lastIr);
  int64_t impulse_diff = static_cast<int64_t>(pwm_channels[0].data.highus) - static_cast<int64_t>(data.highus);

  if ((abs(time_diff) << 1) > static_cast<int64_t>(data.totalus)) {
    time_diff = (time_diff < 0)
                  ? static_cast<int64_t>(data.totalus) % time_diff
                  : -(static_cast<int64_t>(data.totalus) % time_diff);
  }

  return time_diff + impulse_diff;
}

const int32_t pwm_get_offset(const uint8_t channel) {
  return calculateOffset(channel);
}

const int32_t pwm_get_scaledPwm(const uint8_t channel) {
  const auto &data = pwm_channels[channel].data;
  const auto &config = pwm_channels[channel].config;

  if (data.state == PwmState::STABLE) {
    if (GET_FAILESAFE_DURING_MIN_MAX && config.auto_min_max) {
      return config.pulse_neutral;
    }

    if (data.highus <= config.pulse_neutral) {
      return map(data.highus,
                 config.pulse_min,
                 config.pulse_neutral,
                 SCALE_PULSE_MIN,
                 SCALE_PULSE_NEUTRAL);
    }

    return map(data.highus,
               config.pulse_neutral,
               config.pulse_max,
               SCALE_PULSE_NEUTRAL,
               SCALE_PULSE_MAX);
  }

  return (config.failsafe) ? config.pulse_neutral : -1;
}

const float_t pwm_get_rc_duty_cycle(const uint16_t pulse_min, const uint16_t pulse_max, const uint16_t pulse_current) {
  if ((pulse_min + pulse_max) == 0) {
    return 0;
  }
  return 100.0 * pulse_current / (pulse_min + pulse_max);
}

void calculateChannelMinMax(const uint8_t channel) {
  auto &data = pwm_channels[channel].data;
  auto &config = pwm_channels[channel].config;

  uint16_t lowerMid = (config.pulse_neutral - VALID_MIN_MAX_MID);
  uint16_t upperMid = (config.pulse_neutral + VALID_MIN_MAX_MID);

  uint16_t lowerLimitMin = lowerMid - (MIN_MAX_CALIBRATE_LOWER_LIMIT * (lowerMid / 100));
  uint16_t upperLimitMin = lowerMid + (MIN_MAX_CALIBRATE_LOWER_LIMIT * (lowerMid / 100));

  uint16_t lowerLimitMax = upperMid - (MIN_MAX_CALIBRATE_UPPER_LIMIT * (upperMid / 100));
  uint16_t upperLimitMax = upperMid + (MIN_MAX_CALIBRATE_UPPER_LIMIT * (upperMid / 100));

  if (((esp_timer_get_time() - data.stable_time) / 1e6) >= AUTO_MIN_MAX_CALIBRATE_TIME_SEC) {
    if (data.auto_pulse_min > lowerLimitMin && data.auto_pulse_min < upperLimitMin) {
      config.pulse_min = data.auto_pulse_min;
    }
    if (data.auto_pulse_max > lowerLimitMax && data.auto_pulse_max < upperLimitMax) {
      config.pulse_max = data.auto_pulse_max;
    }
    config.auto_min_max = false;
  } else {
    data.auto_pulse_min = data.auto_pulse_min > 0
                            ? MIN(data.highus, data.auto_pulse_min)
                            : data.highus;
    data.auto_pulse_max = MAX(data.highus, data.auto_pulse_max);
  }
}

uint16_t calculateChannelZero(const uint8_t channel) {
  const auto &data = pwm_channels[channel].data;
  const auto &config = pwm_channels[channel].config;

  uint16_t lowerLimit = config.pulse_neutral - ((ZERO_CALC_LOWER_LIMIT * config.pulse_neutral) / 100);
  uint16_t upperLimit = config.pulse_neutral + ((ZERO_CALC_UPPER_LIMIT * config.pulse_neutral) / 100);

  if (data.highus > lowerLimit && data.highus < upperLimit) {
    return data.highus;
  }

  return config.pulse_neutral;
}

PwmState getNewState(const uint8_t channel) {
#define PWM_OUT_OF_RANGE ((data.totalus > PERIOD_MIN_VALID) || (data.totalus < PERIOD_MAX_VALID) && (data.totalus <= 0.8 * data.lastTotalus || data.totalus >= 1.2 * data.lastTotalus))
#define NO_SIGNAL ((currentTime - data.lastIr) > (2 * (uint64_t)config.period_max))

  auto &data = pwm_channels[channel].data;
  auto &config = pwm_channels[channel].config;

  PwmState newState = data.state;
  int64_t currentTime = esp_timer_get_time();

  switch (data.state) {
    case PwmState::NOT_CONNECTED:
      newState = PwmState::UNSTABLE;
      data.stableCounter = 0;
      break;

    case PwmState::UNSTABLE:
      if (NO_SIGNAL) {
        newState = PwmState::NOT_CONNECTED;
        break;
      }
      if (PWM_OUT_OF_RANGE) {
        newState = PwmState::UNSTABLE;
        data.stableCounter = 0;
        break;
      }
      if (data.stableCounter == STABLE_COUNTER) {
        data.stableCounter = 0;
        data.stable_time = currentTime;
        newState = PwmState::STABLE;
        if (config.auto_zero == true) {
          config.pulse_neutral = calculateChannelZero(channel);
        }
        break;
      }
      data.stableCounter++;
      break;

    case PwmState::STABLE:
      if (NO_SIGNAL) {
        newState = PwmState::NOT_CONNECTED;
        break;
      }
      if (PWM_OUT_OF_RANGE) {
        newState = PwmState::UNSTABLE;
        data.stableCounter = 0;
        break;
      }
      if (config.auto_min_max) {
        calculateChannelMinMax(channel);
        break;
      }
  }
  data.state = newState;
  return newState;
}