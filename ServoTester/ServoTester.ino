#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
#include <EEPROM.h>
#include "esp32-rmt-pwm-reader.h"
#include "Servo.h"
Servo myservo;  // create servo object to control a servo
#define LEDR 4
#define LEDB 16
#define LEDG 17

#define RxSerial Serial1
#define TxSerial Serial2
#define TXSERIAL_PIN 22  // pin for trasmitting command to servo and PWM signal
#define IGNRXSER_PIN 27  // unused pin for serial rx
#define READ_PIN 35      // pin used to read answers from SERVO

const uint8_t channelPins[]={READ_PIN};

// Set the following to true for serial debugging
#define DEBUG false
#define DEBUGREADCHARS false
char buffer[30];
uint8_t redrawScreen = 0;
uint8_t readServo = 0;
uint8_t writeServo = 0;
uint8_t ServoTestOn = 0;
uint8_t ServoAttached = 0;
uint8_t MeterOn = 0;
uint8_t MeterAttached = 0;
uint8_t CalUpdated = 0;
unsigned long lasteepromwrite;
float ServoPosition = 1500;
float ServoIncrement = 0;
uint8_t SweepStarted = 0;
int16_t SweepDirection = 1;

#include <TFT_eSPI.h>

#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
//https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default
// SPI pins

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 400, touchScreenMaximumX = 3000, touchScreenMinimumY = 500, touchScreenMaximumY = 3000;

extern objects_t objects;
/*Set to your screen resolution*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))


#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
#if DEBUG
  Serial.println(buf);
  Serial.flush();
#endif
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if (touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if (p.x < touchScreenMinimumX) {
      touchScreenMinimumX = p.x;
      CalUpdated = 1;
#if DEBUG
      Serial.printf("Recalibrating touchScreenMinimumX %d\n", touchScreenMinimumX);
#endif
    }
    if (p.x > touchScreenMaximumX) {
      touchScreenMaximumX = p.x;
      CalUpdated = 1;
#if DEBUG
      Serial.printf("Recalibrating touchScreenMaximumX %d\n", touchScreenMaximumX);
#endif
    }
    if (p.y < touchScreenMinimumY) {
      touchScreenMinimumY = p.y;
      CalUpdated = 1;
#if DEBUG
      Serial.printf("Recalibrating touchScreenMinimumY %d\n", touchScreenMinimumY);
#endif
    }
    if (p.y > touchScreenMaximumY) {
      touchScreenMaximumY = p.y;
      CalUpdated = 1;
#if DEBUG
      Serial.printf("Recalibrating touchScreenMaximumY %d\n", touchScreenMaximumY);
#endif
    }
    //Map this to the pixel position
    data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 0, TFT_HOR_RES - 1); /* Touchscreen X calibration */
    data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 0, TFT_VER_RES - 1); /* Touchscreen Y calibration */
#if DEBUG
    Serial.printf("X: %d, Y: %d\n", data->point.x, data->point.y);
#endif
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

lv_indev_t *indev;      //Touchscreen input device
uint8_t *draw_buf;      //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer
uint32_t currentTime, displayTime;

struct servostruct {
  uint8_t min_angle;
  uint8_t max_angle;
  uint16_t neutral_angle;
  uint16_t min_pulse;
  uint16_t max_pulse;
  uint8_t deadband;
  uint8_t gain;
  uint8_t dumping;
  uint16_t max_power;
  uint16_t oltime;
  uint16_t olpower;
  uint8_t inversion;
  uint8_t softstart;
};
servostruct servo;
servostruct servoquery;

struct stteststruct {
  uint16_t min_pulse;
  uint16_t max_pulse;
  uint16_t sweeptime;
  uint8_t sweepon;
  uint16_t position;
};
stteststruct sttest;

struct pulsemeterstruct {
  uint16_t min_pulse;
  uint16_t max_pulse;
  uint16_t center_pulse;
  unsigned long lastinput;
  unsigned long pulseInValue;
  unsigned long frameInValue;
};
volatile pulsemeterstruct pulseMeter;

void loadservodefault() {
  servo.min_angle = 100;
  servo.max_angle = 100;
  servo.neutral_angle = 4096;
  servo.min_pulse = 500;
  servo.max_pulse = 2500;
  servo.deadband = 15;
  servo.gain = 29;
  servo.dumping = 12;
  servo.max_power = 1200;
  servo.oltime = 2000;
  servo.olpower = 1200;
  servo.inversion = 0;
  servo.softstart = 0;
  sttest.min_pulse = 500;
  sttest.max_pulse = 2500;
  sttest.sweeptime = 5;
  sttest.sweepon = 0;
  sttest.position = 1500;
}
uint8_t basecommand[] = { 175, 250, 0, 10, 0, 0, 0, 0, 0, 0 };
uint8_t readcommands[] = { 56, 61, 63, 80, 81, 82, 83, 84 };
uint8_t writecommands[] = { 1, 5, 18, 24, 64, 65, 66, 67, 68, 69, 72, 70 };

void ServoWrite(uint8_t command, uint16_t payloadh, uint16_t payloadl) {
  uint8_t checksum = 0;
  basecommand[4] = command;
  basecommand[5] = (payloadh >> 8);
  basecommand[6] = (payloadh % 256);
  basecommand[7] = (payloadl >> 8);
  basecommand[8] = (payloadl % 256);
  for (int i = 2; i < 9; i++) {
    checksum += basecommand[i];
  }
  basecommand[9] = checksum;
  TxSerial.write(basecommand, 10);
  TxSerial.flush();
}

bool ReadAnswer(uint8_t command) {
  uint8_t ch;
  uint8_t checksum = 0;
  int8_t readchars = 0;
  char buffer[30];
  while (RxSerial.available() && readchars < 30) {
    ch = (byte)RxSerial.read();
    buffer[readchars] = ch;
#if DEBUGREADCHARS
    Serial.printf("%d ,", ch);
#endif
    readchars++;
  }
  Serial.println();
  if (readchars != 19 || buffer[4] != command) {
#if DEBUG
    Serial.printf("Readdchars %d \n\r", readchars);
    Serial.printf("Buffer 4 %d \n\r", buffer[4]);
#endif
    return false;
  }
  for (int i = 12; i < 18; i++) {
    checksum += buffer[i];
  }
  if (checksum != buffer[18]) {
#if DEBUG
    Serial.println("Checksum failed");
#endif
    return false;
  }
  switch (command) {
    case 56:
      servoquery.olpower = buffer[14];
      servoquery.olpower *= 256;
      servoquery.olpower += buffer[15];
      servoquery.oltime = buffer[16];
      servoquery.oltime *= 256;
      servoquery.oltime += buffer[17];
#if DEBUG
      Serial.print("OLPOWER:");
      Serial.println(servoquery.olpower);
      Serial.print("OLTIME:");
      Serial.println(servoquery.oltime);
#endif
      break;
    case 61:
      servoquery.gain = buffer[14];
      servoquery.gain *= 256;
      servoquery.gain += buffer[15];
      servoquery.dumping = buffer[16];
      servoquery.dumping *= 256;
      servoquery.dumping += buffer[17];
#if DEBUG
      Serial.print("GAIN:");
      Serial.println(servoquery.gain);
      Serial.print("DUMPING:");
      Serial.println(servoquery.dumping);
#endif
      break;
    case 63:
      servoquery.max_power = buffer[14];
      servoquery.max_power *= 256;
      servoquery.max_power += buffer[15];
#if DEBUG
      Serial.print("MAXPOWER:");
      Serial.println(servoquery.max_power);
#endif
      break;
    case 80:
      servoquery.min_pulse = buffer[14];
      servoquery.min_pulse *= 256;
      servoquery.min_pulse += buffer[15];
      servoquery.min_pulse /= 4;
      servoquery.max_pulse = buffer[16];
      servoquery.max_pulse *= 256;
      servoquery.max_pulse += buffer[17];
      servoquery.max_pulse /= 4;
#if DEBUG
      Serial.print("MIN_PULSE:");
      Serial.println(servoquery.min_pulse);
      Serial.print("MAX_PULSE:");
      Serial.println(servoquery.max_pulse);
#endif
      break;
    case 81:
      servoquery.min_angle = buffer[15];
      servoquery.max_angle = buffer[17];
#if DEBUG
      Serial.print("MIN_ANGLE:");
      Serial.println(servoquery.min_angle);
      Serial.print("MAX_ANGLE:");
      Serial.println(servoquery.max_angle);
#endif
      break;
    case 82:
      servoquery.neutral_angle = buffer[14];
      servoquery.neutral_angle *= 256;
      servoquery.neutral_angle += buffer[15];
#if DEBUG
      Serial.print("NEUTRAL_ANGLE:");
      Serial.println(servoquery.neutral_angle);
#endif
      break;
    case 83:
      servoquery.deadband = buffer[15];
#if DEBUG
      Serial.print("DEADBAND:");
      Serial.println(servoquery.deadband);
#endif
      break;
    case 84:
      servoquery.inversion = buffer[15];
      servoquery.softstart = buffer[17];
#if DEBUG
      Serial.print("INVERSION:");
      Serial.println(servoquery.inversion);
      Serial.print("SOFTSTART:");
      Serial.println(servoquery.softstart);
#endif
      break;
    default:
      return false;
      break;
  }
  return true;
}

bool ServoRead() {
  uint8_t command;
  uint8_t index = 0;
  while (index < 8) {
    ServoWrite(readcommands[index], 0, 0);
    delay(5);
    if (!ReadAnswer(readcommands[index])) {
#if DEBUG
      Serial.println("Read Failed");
#endif
      break;
    } else {
#if DEBUG
      Serial.println("Read Success");
#endif
      index++;
    }
    delay(50);
  }
  if (index == 8) {
    servo = servoquery;
    return true;
  } else {
    return false;
  }
}

void UpdateServo() {
  uint8_t command;
  uint8_t index = 0;
  uint16_t payloadh = 0;
  uint16_t payloadl = 0;
  while (index < 12) {
    payloadh = 0;
    payloadl = 0;
    command = writecommands[index];
    switch (command) {
      case 1:
        payloadh = servo.gain;
        break;
      case 5:
        payloadh = servo.dumping;
        break;
      case 18:
        payloadh = servo.oltime;
        payloadl = servo.olpower;
        break;
      case 24:
        payloadh = servo.max_power;
        break;
      case 64:
        payloadl = servo.min_angle;
        break;
      case 65:
        payloadl = servo.max_angle;
        break;
      case 66:
        payloadl = servo.neutral_angle;
        break;
      case 67:
        payloadl = servo.deadband;
        break;
      case 68:
        payloadl = servo.inversion;
        break;
      case 69:
        break;
      case 72:
        payloadh = servo.min_pulse * 4;
        payloadl = servo.max_pulse * 4;
        break;
      case 70:
        payloadl = servo.softstart;
        break;
    }
    ServoWrite(command, payloadh, payloadl);
    delay(50);
    index++;
  }
}

void setup() {
  // Initialization function
  // Some basic info on the Serial console
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

#if DEBUG
  delay(2000);
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
#endif
  if (!EEPROM.begin(100)) {
#if DEBUG
    Serial.println("Failed to initialize EEPROM");
    Serial.println("Restarting...");
#endif
    delay(1000);
    ESP.restart();
  }
  int address = 0;
  int value;
  value = EEPROM.readUInt(address);
  if (value > 100) {
    touchScreenMinimumX = value;
  }
  address += sizeof(unsigned int);
  value = EEPROM.readUInt(address);
  if (value > 100) {
    touchScreenMaximumX = value;
  }
  address += sizeof(unsigned int);
  value = EEPROM.readUInt(address);
  if (value > 100) {
    touchScreenMinimumY = value;
  }
  address += sizeof(unsigned int);
  value = EEPROM.readUInt(address);
  if (value > 100) {
    touchScreenMaximumY = value;
  }

  // Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
  touchscreen.setRotation(2);                                                /* Inverted landscape orientation to match screen */

  // Initialise LVGL GUI
  lv_init();

  // Setup LVGL display and input devices
  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t *disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
  // Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // Integrate EEZ Studio GUI (initialize UI elements based on definition)
  ui_init();
  digitalWrite(21, HIGH);  // Turn on display
  pinMode(READ_PIN, INPUT);
  pinMode(TXSERIAL_PIN, INPUT_PULLUP);

  /*
  TxSerial.begin(115200, EspSoftwareSerial::SWSERIAL_8N1, READ_PIN, READ_PIN , false, 256);
	// high speed half duplex, turn off interrupts during tx
	TxSerial.enableIntTx(false);
*/
  loadservodefault();
  redrawScreen = 1;
}

unsigned long now;
unsigned long lastsweep;
unsigned long nowmicros;
unsigned long lastservo;
unsigned long lastinputredraw;

void loop() {
  // Main program loop
  now = millis();
  nowmicros = micros();
  if ((CalUpdated == 1) && (now - lasteepromwrite) > 30000ull) {
#if DEBUG
    Serial.println("Save settings");
#endif
    int address = 0;
    EEPROM.writeUInt(address, touchScreenMinimumX);  // -2^31
    address += sizeof(unsigned int);
    EEPROM.writeUInt(address, touchScreenMaximumX);  // -2^31
    address += sizeof(unsigned int);
    EEPROM.writeUInt(address, touchScreenMinimumY);  // -2^31
    address += sizeof(unsigned int);
    EEPROM.writeUInt(address, touchScreenMaximumY);  // -2^31
    EEPROM.commit();
    lasteepromwrite = now;
    CalUpdated = 0;
  }

  if (readServo == 1 || writeServo == 1) {
    ServoTestOn = 0;
    MeterOn = 0;
  }

  if (MeterOn == 1) {
    ServoTestOn = 0;
  }

  if (ServoTestOn == 1) {
    if (ServoAttached == 0) {
      myservo.attach(TXSERIAL_PIN, Servo::CHANNEL_NOT_ATTACHED, 10, 170, 500, 2500);
      ServoAttached = 1;
    }
    if (sttest.sweepon == 0) {
      myservo.writeMicroseconds(sttest.position);
      delay(15);
    } else {
      if (SweepStarted == 0) {
#if DEBUG
        Serial.println("Doing Servo Sweep");
#endif
        ServoPosition = (sttest.min_pulse);
        myservo.writeMicroseconds((uint16_t)ServoPosition);
        delay(200);
        SweepStarted = 1;
        SweepDirection = 1;
        lastsweep = nowmicros;
      } else {
        if ((nowmicros - lastsweep) > 19900) {
          lastsweep = nowmicros;
          ServoIncrement = ((float)(sttest.max_pulse - sttest.min_pulse)) / (5 * sttest.sweeptime);
          if (SweepDirection == 1) {
            ServoPosition += ServoIncrement;
            if (ServoPosition > ((float)sttest.max_pulse)) {
              ServoPosition = sttest.max_pulse;
              SweepDirection = -1;
            }
          } else {
            ServoPosition -= ServoIncrement;
            if (ServoPosition < ((float)sttest.min_pulse)) {
              ServoPosition = sttest.min_pulse;
              SweepDirection = 1;
            }
          }
          myservo.writeMicroseconds((uint16_t)ServoPosition);
        }
      }
    }
  } else if (ServoAttached == 1) {
    myservo.detach();
    pinMode(TXSERIAL_PIN, INPUT_PULLUP);
    ServoAttached = 0;
    SweepStarted = 0;
    SweepDirection = 1;
    lv_obj_remove_state(objects.ststartbt, LV_STATE_DISABLED);
    lv_obj_set_state(objects.ststopbt, LV_STATE_DISABLED, LV_ANIM_ON);
    delay(100);
  }
  if (MeterOn == 1) {
    if (MeterAttached==0) {
      pwm_reader_init(channelPins, 1);
      pwm_set_failsafe(0, false);
      pwm_set_auto_zero(0, true);     // set channel to auto zero
      pwm_set_auto_min_max(0, true);  // set channel to auto min/max calibration
      pwm_reader_begin();      
      pulseMeter.min_pulse=2500;
      pulseMeter.max_pulse=0;
      pulseMeter.center_pulse=0;      
      MeterAttached=1;
    }
    if (pwm_get_state(0)==PwmState::STABLE) {
      pulseMeter.pulseInValue=pwm_get_rawPwm(0);
      if (pulseMeter.pulseInValue==-1) {
        pulseMeter.pulseInValue=0;
      }
      pulseMeter.frameInValue=pwm_get_frequency(0);
      pulseMeter.lastinput=now;
    }
    if ((now - pulseMeter.lastinput) > 200) {
      pulseMeter.pulseInValue = 0;
      pulseMeter.frameInValue = 0;
    } 
    if (pulseMeter.pulseInValue!=0) {
      if (pulseMeter.pulseInValue>pulseMeter.max_pulse) {
        pulseMeter.max_pulse=pulseMeter.pulseInValue;
        sprintf(buffer, "%d", pulseMeter.pulseInValue);
        lv_label_set_text(objects.metermaxvaluelbl, buffer);
      }
      if (pulseMeter.pulseInValue<pulseMeter.min_pulse) {
        pulseMeter.min_pulse=pulseMeter.pulseInValue;
        sprintf(buffer, "%d", pulseMeter.pulseInValue);
        lv_label_set_text(objects.meterminvaluelbl, buffer);
      }
    }
    if ((now - lastinputredraw) > 100ull) {
      if (pulseMeter.pulseInValue != 0) {
        sprintf(buffer, "%d", pulseMeter.pulseInValue);
        lv_label_set_text(objects.pulsevaluelbl, buffer);
      } else {
        lv_label_set_text(objects.pulsevaluelbl, "----");
      }
      if (pulseMeter.frameInValue != 0) {
        sprintf(buffer, "%d", pulseMeter.frameInValue);
        lv_label_set_text(objects.frequencyvaluelbl, buffer);
      } else {
        lv_label_set_text(objects.frequencyvaluelbl, "---");
      }
      lastinputredraw = now;
    }
  } else {
    if (MeterAttached==1) {
      pwm_cleanup();
      MeterAttached=0;
    }
  }
  if (readServo == 1) {
    RxSerial.begin(115200, SERIAL_8N1, READ_PIN, -1);
    RxSerial.setTimeout(0);
    TxSerial.begin(115200, SERIAL_8N1, IGNRXSER_PIN, TXSERIAL_PIN);
    TxSerial.setTimeout(0);
    while (RxSerial.available()) {
      uint8_t receivedChar = RxSerial.read();
    }
    if (ServoRead()) {
      lv_label_set_text(objects.msglbl, "Read Success");
    } else {
      lv_label_set_text(objects.msglbl, "Read Failed");
    }
    lastservo = now;
    redrawScreen = 1;
    readServo = 0;
    delay(100);
    TxSerial.end();
    pinMode(TXSERIAL_PIN, INPUT_PULLUP);
    RxSerial.end();
    pinMode(READ_PIN, INPUT);
  }
  if (writeServo == 1) {
    RxSerial.begin(115200, SERIAL_8N1, READ_PIN, -1);
    RxSerial.setTimeout(0);
    TxSerial.begin(115200, SERIAL_8N1, IGNRXSER_PIN, TXSERIAL_PIN);
    TxSerial.setTimeout(0);
    while (RxSerial.available()) {
      uint8_t receivedChar = RxSerial.read();
    }
    UpdateServo();
    lv_label_set_text(objects.msglbl, "Update done.");
    lastservo = now;
    redrawScreen = 1;
    writeServo = 0;
    delay(100);
    TxSerial.end();
    pinMode(TXSERIAL_PIN, INPUT_PULLUP);
    RxSerial.end();
    pinMode(READ_PIN, INPUT);
  }

  if (now - lastservo > 5000ull) {
    lv_obj_remove_state(objects.readservo, LV_STATE_DISABLED);
    lv_obj_remove_state(objects.writeservo, LV_STATE_DISABLED);
    lv_label_set_text(objects.msglbl, "");
    lastservo = now;
  }

  if (redrawScreen == 1) {
    sprintf(buffer, "%d", servo.min_angle);
    lv_label_set_text(objects.minanglelbl, buffer);
    sprintf(buffer, "%d", servo.max_angle);
    lv_label_set_text(objects.maxanglelbl, buffer);
    sprintf(buffer, "%d", servo.neutral_angle);
    lv_label_set_text(objects.neutrallbl, buffer);
    sprintf(buffer, "%d", servo.deadband);
    lv_label_set_text(objects.deadbandlbl, buffer);
    sprintf(buffer, "%d", servo.dumping);
    lv_label_set_text(objects.dumpinglbl, buffer);
    sprintf(buffer, "%d", servo.gain);
    lv_label_set_text(objects.gainlbl, buffer);
    sprintf(buffer, "%d", servo.min_pulse);
    lv_label_set_text(objects.pulseminlbl, buffer);
    sprintf(buffer, "%d", servo.max_pulse);
    lv_label_set_text(objects.pulsemaxlbl, buffer);
    sprintf(buffer, "%d", servo.max_power);
    lv_label_set_text(objects.powerlbl, buffer);
    sprintf(buffer, "%d", servo.oltime);
    lv_label_set_text(objects.oltimelbl, buffer);
    sprintf(buffer, "%d", servo.olpower);
    lv_label_set_text(objects.ololpowerlbl, buffer);
    if (servo.inversion == 1) {
      lv_obj_set_state(objects.inversionsw, LV_STATE_CHECKED, LV_ANIM_ON);
    } else {
      lv_obj_remove_state(objects.inversionsw, LV_STATE_CHECKED);
    }
    if (servo.softstart == 1) {
      lv_obj_set_state(objects.softstartsw, LV_STATE_CHECKED, LV_ANIM_ON);
    } else {
      lv_obj_remove_state(objects.softstartsw, LV_STATE_CHECKED);
    }
    sprintf(buffer, "%d", sttest.min_pulse);
    lv_label_set_text(objects.stpulseminlbl, buffer);
    sprintf(buffer, "%d", sttest.max_pulse);
    lv_label_set_text(objects.stpulsemaxlbl, buffer);
    sprintf(buffer, "%0.1f", ((float)sttest.sweeptime) / 10);
    lv_label_set_text(objects.stsweeplbl, buffer);
    if (sttest.sweepon == 1) {
      lv_obj_set_state(objects.stsweepsw, LV_STATE_CHECKED, LV_ANIM_ON);
    } else {
      lv_obj_remove_state(objects.stsweepsw, LV_STATE_CHECKED);
    }
    sprintf(buffer, "%d", sttest.position);
    lv_label_set_text(objects.stcenterlbl, buffer);
    redrawScreen = 0;
  }
  lv_tick_inc(now - lastTick);  // Update the LVGL tick timer
  lastTick = now;
  lv_timer_handler();  // Update the UI (LVGL)
}
