#include "Arduino.h"
#include "actions.h"
#include "screens.h"
#include <lvgl.h>

//extern objects_t objects;; // Dichiarazione degli oggetti generati da EEZ Studio
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

extern servostruct servo;

struct stteststruct {
  uint16_t min_pulse;
  uint16_t max_pulse;
  uint16_t sweeptime;
  uint8_t sweepon;
  uint16_t position;
};

extern stteststruct sttest;

extern uint readServo;
extern uint writeServo;
extern uint ServoTestOn;

extern uint8_t redrawScreen;
int currentScreenIndex = 0;  // 0: main, 1: settings

// Funzione per caricare uno schermo in base all'indice
void loadScreen(int screenIndex) {
  if (screenIndex == 0) {
    lv_scr_load(objects.main);
    currentScreenIndex = 0;
  } else if (screenIndex == 1) {
    lv_scr_load(objects.programmer);
    currentScreenIndex = 1;
  } else if (screenIndex == 2) {
    lv_scr_load(objects.tester);
    currentScreenIndex = 2;
  } else {
    LV_LOG_WARN("Indice schermo non valido");
  }
}

void action_gesture(lv_event_t *e) {
  lv_indev_t *indev = lv_event_get_indev(e);
  lv_dir_t dir = lv_indev_get_gesture_dir(indev);
  switch (dir) {
    case LV_DIR_LEFT:
      if (currentScreenIndex != 0) {
        loadScreen(0);  // Vai menu
      }
      break;
      break;
    default:
      break;
  }
}

void action_load_programmer(lv_event_t * e) {
  loadScreen(1);  // Load Programmer screen
};
void action_load_tester(lv_event_t * e) {
  loadScreen(2);  // Load servo tester screen
};

void action_dec_pressed(lv_event_t *e) {
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);  // Cast esplicito
  if (button == objects.minangledec) {
    if (servo.min_angle > 0) {
      servo.min_angle--;
      redrawScreen = 1;
    }
  } else if (button == objects.maxangledec) {
    if (servo.max_angle > 0) {
      servo.max_angle--;
      redrawScreen = 1;
    }
  } else if (button == objects.neutraldec) {
    if (servo.neutral_angle > 2000) {
      servo.neutral_angle--;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemindec) {
    if (servo.min_pulse > 500) {
      servo.min_pulse--;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemaxdec) {
    if (servo.max_pulse > 1500) {
      servo.max_pulse--;
      redrawScreen = 1;
    }
  } else if (button == objects.deadbanddec) {
    if (servo.deadband > 0) {
      servo.deadband--;
      redrawScreen = 1;
    }
  } else if (button == objects.gaindec) {
    if (servo.gain > 0) {
      servo.gain--;
      redrawScreen = 1;
    }
  } else if (button == objects.dumpingdec) {
    if (servo.dumping > 0) {
      servo.dumping--;
      redrawScreen = 1;
    }
  } else if (button == objects.powerdec) {
    if (servo.max_power > 0) {
      servo.max_power--;
      redrawScreen = 1;
    }
  } else if (button == objects.oltimedec) {
    if (servo.oltime > 0) {
      servo.oltime--;
      redrawScreen = 1;
    }
  } else if (button == objects.olpowerdec) {
    if (servo.olpower > 0) {
      servo.olpower--;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemindec) {
    if (sttest.min_pulse > 500) {
      sttest.min_pulse--;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemaxdec) {
    if (sttest.max_pulse > 1500) {
      sttest.max_pulse--;
      redrawScreen = 1;
    }
  } else if (button == objects.stsweepdec) {
    if (sttest.sweeptime > 1) {
      sttest.sweeptime--;
      redrawScreen = 1;
    }
  } else if (button == objects.stcenterdec) {
    if (sttest.position > 500) {
      sttest.position--;
      redrawScreen = 1;
    }
  }
};

void action_dec_longpressed(lv_event_t *e) {
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);  // Cast esplicito
  if (button == objects.minangledec) {
    if (servo.min_angle > 0) {
      servo.min_angle--;
      redrawScreen = 1;
    }
  } else if (button == objects.maxangledec) {
    if (servo.max_angle > 0) {
      servo.max_angle--;
      redrawScreen = 1;
    }
  } else if (button == objects.neutraldec) {
    if (servo.neutral_angle > 2005) {
      servo.neutral_angle -= 5;
      redrawScreen = 1;
    } else if (servo.neutral_angle > 2000) {
      servo.neutral_angle = 2000;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemindec) {
    if (servo.min_pulse > 505) {
      servo.min_pulse -= 5;
      redrawScreen = 1;
    } else if (servo.min_pulse > 500) {
      servo.min_pulse = 500;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemaxdec) {
    if (servo.max_pulse > 1505) {
      servo.max_pulse-=5;
      redrawScreen = 1;
    } else if (servo.max_pulse > 1500) {
      servo.max_pulse = 1500;
      redrawScreen = 1;
    }
  } else if (button == objects.powerdec) {
    if (servo.max_power > 5) {
      servo.max_power -= 5;
      redrawScreen = 1;
    } else if (servo.max_power > 0) {
      servo.max_power = 0;
      redrawScreen = 1;
    }
  } else if (button == objects.oltimedec) {
    if (servo.oltime > 5) {
      servo.oltime -= 5;
      redrawScreen = 1;
    } else if (servo.oltime > 0) {
      servo.oltime = 0;
      redrawScreen = 1;
    }
  } else if (button == objects.olpowerdec) {
    if (servo.olpower > 5) {
      servo.olpower -= 5;
      redrawScreen = 1;
    } else if (servo.olpower > 0) {
      servo.olpower = 0;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemindec) {
    if (sttest.min_pulse > 510) {
      sttest.min_pulse -= 10;
      redrawScreen = 1;
    } else if (sttest.min_pulse > 500) {
      sttest.min_pulse = 500;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemaxdec) {
    if (sttest.max_pulse > 1510) {
      sttest.max_pulse -= 10;
      redrawScreen = 1;
    } else if (sttest.max_pulse > 1500) {
      sttest.max_pulse = 1500;
      redrawScreen = 1;
    }
  } else if (button == objects.stsweepdec) {
    if (sttest.sweeptime > 5) {
      sttest.sweeptime-=5;
      redrawScreen = 1;
    } else if ((sttest.sweeptime <= 5) ) {
      sttest.sweeptime=1;
      redrawScreen = 1;
    }
  } else if (button == objects.stcenterdec) {
    if (sttest.position > 510) {
      sttest.position -= 10;
      redrawScreen = 1;
    } else if (sttest.position > 500) {
      sttest.position--;
      redrawScreen = 1;
    }
  }
};

void action_inc_pressed(lv_event_t *e) {
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);  // Cast esplicito
  if (button == objects.minangleinc) {
    if (servo.min_angle < 255) {
      servo.min_angle++;
      redrawScreen = 1;
    }
  } else if (button == objects.maxangleinc) {
    if (servo.max_angle < 255) {
      servo.max_angle++;
      redrawScreen = 1;
    }
  } else if (button == objects.neutralinc) {
    if (servo.neutral_angle < 6000) {
      servo.neutral_angle++;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemininc) {
    if (servo.min_pulse < 1500) {
      servo.min_pulse++;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemaxinc) {
    if (servo.max_pulse < 2500) {
      servo.max_pulse++;
      redrawScreen = 1;
    }
  } else if (button == objects.deadbandinc) {
    if (servo.deadband < 255) {
      servo.deadband++;
      redrawScreen = 1;
    }
  } else if (button == objects.gaininc) {
    if (servo.gain < 30) {
      servo.gain++;
      redrawScreen = 1;
    }
  } else if (button == objects.dumpinginc) {
    if (servo.dumping < 255) {
      servo.dumping++;
      redrawScreen = 1;
    }
  } else if (button == objects.powerinc) {
    if (servo.max_power < 1200) {
      servo.max_power++;
      redrawScreen = 1;
    }
  } else if (button == objects.oltimeinc) {
    if (servo.oltime < 60000) {
      servo.oltime++;
      redrawScreen = 1;
    }
  } else if (button == objects.ololpowerinc) {
    if (servo.olpower < 1200) {
      servo.olpower++;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemininc) {
    if (sttest.min_pulse < 1500) {
      sttest.min_pulse++;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemaxinc) {
    if (sttest.max_pulse < 2500) {
      sttest.max_pulse++;
      redrawScreen = 1;
    }
  } else if (button == objects.stsweepinc) {
    if (sttest.sweeptime < 600) {
      sttest.sweeptime++;
      redrawScreen = 1;
    }
  } else if (button == objects.stcenterinc) {
    if (sttest.position < 2500) {
      sttest.position++;
      redrawScreen = 1;
    }
  }
};


void action_inc_longpressed(lv_event_t *e) {
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);  // Cast esplicito
  if (button == objects.minangleinc) {
    if (servo.min_angle < 255) {
      servo.min_angle++;
      redrawScreen = 1;
    }
  } else if (button == objects.maxangleinc) {
    if (servo.max_angle < 255) {
      servo.max_angle++;
      redrawScreen = 1;
    }
  } else if (button == objects.neutralinc) {
    if (servo.neutral_angle < 5990) {
      servo.neutral_angle += 10;
      redrawScreen = 1;
    } else if (servo.neutral_angle < 6000) {
      servo.neutral_angle = 6000;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemininc) {
    if (servo.min_pulse < 1495) {
      servo.min_pulse += 5;
      redrawScreen = 1;
    } else if (servo.min_pulse < 1500) {
      servo.min_pulse = 1500;
      redrawScreen = 1;
    }
  } else if (button == objects.pulsemaxinc) {
    if (servo.max_pulse < 2495) {
      servo.max_pulse += 5;
      redrawScreen = 1;
    } else if (servo.max_pulse < 2500) {
      servo.max_pulse = 2500;
      redrawScreen = 1;
    }
  } else if (button == objects.powerinc) {
    if (servo.max_power < 1195) {
      servo.max_power += 5;
      redrawScreen = 1;
    } else if (servo.max_power < 1200) {
      servo.max_power = 1200;
      redrawScreen = 1;
    }
  } else if (button == objects.oltimeinc) {
    if (servo.oltime < 59990) {
      servo.oltime += 10;
      redrawScreen = 1;
    } else if (servo.oltime < 60000) {
      servo.oltime = 60000;
      redrawScreen = 1;
    }
  } else if (button == objects.ololpowerinc) {
    if (servo.olpower < 1195) {
      servo.olpower += 5;
      redrawScreen = 1;
    } else if (servo.olpower < 1200) {
      servo.olpower = 1200;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemininc) {
    if (sttest.min_pulse < 1490) {
      sttest.min_pulse+=10;
      redrawScreen = 1;
    } else if (sttest.min_pulse < 1500) {
      sttest.min_pulse=1500;
      redrawScreen = 1;
    }
  } else if (button == objects.stpulsemaxinc) {
    if (sttest.max_pulse < 2490) {
      sttest.max_pulse+=10;
      redrawScreen = 1;
    } else if (sttest.max_pulse < 2500) {
      sttest.max_pulse=2500;
      redrawScreen = 1;
    }
  } else if (button == objects.stsweepinc) {
    if (sttest.sweeptime < 595) {
      sttest.sweeptime+=5;
      redrawScreen = 1;
    }  else if (sttest.sweeptime<600) {
      sttest.sweeptime=600;
      redrawScreen = 1;
    }
  } else if (button == objects.stcenterinc) {
    if (sttest.position < 2490) {
      sttest.position+=10;
      redrawScreen = 1;
    } else if (sttest.position < 2500) {
      sttest.position=2500;
      redrawScreen = 1;
    }
  }
};

void action_setposition(lv_event_t * e) {
  lv_obj_t *lbl = (lv_obj_t *)lv_event_get_target(e);
  if (lbl == objects.stpulseminlbl) {
    sttest.position=sttest.min_pulse;
    redrawScreen = 1;
  } else if (lbl == objects.stpulsemaxlbl) {
    sttest.position=sttest.max_pulse;
    redrawScreen = 1;
  } else if (lbl == objects.stcenterlbl) {
    sttest.position=1500;
    redrawScreen = 1;
  }
};

void action_switch_changed(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_target(e);
  if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
    if (sw == objects.inversionsw) {
      servo.inversion = 1;
    } else if (sw == objects.softstartsw) {
      servo.softstart = 1;
    } else if (sw == objects.stsweepsw) {
      sttest.sweepon = 1;
    }
  } else {
    if (sw == objects.inversionsw) {
      servo.inversion = 0;
    } else if (sw == objects.softstartsw) {
      servo.softstart = 0;
    } else if (sw == objects.stsweepsw) {
      sttest.sweepon = 0;
    }
  }
}



void action_read_servo(lv_event_t *e) {
  lv_obj_set_state(objects.readservo, LV_STATE_DISABLED, LV_ANIM_ON);
  lv_obj_set_state(objects.writeservo, LV_STATE_DISABLED, LV_ANIM_ON);
  readServo = 1;
};

void action_write_servo(lv_event_t *e) {
  lv_obj_set_state(objects.readservo, LV_STATE_DISABLED, LV_ANIM_ON);
  lv_obj_set_state(objects.writeservo, LV_STATE_DISABLED, LV_ANIM_ON);
  writeServo = 1;
};

void action_ststart(lv_event_t *e) {
  ServoTestOn = 1;
  lv_obj_remove_state(objects.ststopbt, LV_STATE_DISABLED);
  lv_obj_set_state(objects.ststartbt, LV_STATE_DISABLED, LV_ANIM_ON);
}

void action_ststop(lv_event_t *e) {
  lv_obj_remove_state(objects.ststartbt, LV_STATE_DISABLED);
  lv_obj_set_state(objects.ststopbt, LV_STATE_DISABLED, LV_ANIM_ON);
  ServoTestOn = 0;
}

void action_screen_loaded(lv_event_t *e) {
  lv_obj_t *screen = (lv_obj_t *)lv_event_get_target(e);
  if (screen == objects.main) {
    lv_obj_scroll_to_y(objects.main, 0, LV_ANIM_OFF);
  }  
  if (screen == objects.programmer) {
    lv_obj_scroll_to_y(objects.programmer, 0, LV_ANIM_OFF);
  }
  if (screen == objects.tester) {
    lv_obj_scroll_to_y(objects.tester, 0, LV_ANIM_OFF);
  }
}