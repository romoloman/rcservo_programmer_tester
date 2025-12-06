#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *programmer;
    lv_obj_t *tester;
    lv_obj_t *meter;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *readservo;
    lv_obj_t *writeservo;
    lv_obj_t *msglbl;
    lv_obj_t *minangledec;
    lv_obj_t *minanglelbl;
    lv_obj_t *minangleinc;
    lv_obj_t *maxangledec;
    lv_obj_t *maxanglelbl;
    lv_obj_t *maxangleinc;
    lv_obj_t *neutraldec;
    lv_obj_t *neutrallbl;
    lv_obj_t *neutralinc;
    lv_obj_t *pulsemindec;
    lv_obj_t *pulseminlbl;
    lv_obj_t *pulsemininc;
    lv_obj_t *pulsemaxdec;
    lv_obj_t *pulsemaxlbl;
    lv_obj_t *pulsemaxinc;
    lv_obj_t *deadbanddec;
    lv_obj_t *deadbandlbl;
    lv_obj_t *deadbandinc;
    lv_obj_t *gaindec;
    lv_obj_t *gainlbl;
    lv_obj_t *gaininc;
    lv_obj_t *dumpingdec;
    lv_obj_t *dumpinglbl;
    lv_obj_t *dumpinginc;
    lv_obj_t *powerdec;
    lv_obj_t *powerlbl;
    lv_obj_t *powerinc;
    lv_obj_t *oltimedec;
    lv_obj_t *oltimelbl;
    lv_obj_t *oltimeinc;
    lv_obj_t *olpowerdec;
    lv_obj_t *ololpowerlbl;
    lv_obj_t *ololpowerinc;
    lv_obj_t *inversionsw;
    lv_obj_t *softstartsw;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *stpulsemindec;
    lv_obj_t *stpulseminlbl;
    lv_obj_t *stpulsemininc;
    lv_obj_t *stpulsemaxdec;
    lv_obj_t *stpulsemaxlbl;
    lv_obj_t *stpulsemaxinc;
    lv_obj_t *stsweepdec;
    lv_obj_t *stsweeplbl;
    lv_obj_t *stsweepinc;
    lv_obj_t *stcenterdec;
    lv_obj_t *stcenterlbl;
    lv_obj_t *stcenterinc;
    lv_obj_t *stsweepsw;
    lv_obj_t *ststartbt;
    lv_obj_t *ststopbt;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *pulsevaluelbl;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_PROGRAMMER = 2,
    SCREEN_ID_TESTER = 3,
    SCREEN_ID_METER = 4,
};

void create_screen_main();
void tick_screen_main();

void create_screen_programmer();
void tick_screen_programmer();

void create_screen_tester();
void tick_screen_tester();

void create_screen_meter();
void tick_screen_meter();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/