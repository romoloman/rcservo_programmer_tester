#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_screen_loaded(lv_event_t * e);
extern void action_read_servo(lv_event_t * e);
extern void action_write_servo(lv_event_t * e);
extern void action_dec_pressed(lv_event_t * e);
extern void action_inc_pressed(lv_event_t * e);
extern void action_switch_changed(lv_event_t * e);
extern void action_gesture(lv_event_t * e);
extern void action_ststart(lv_event_t * e);
extern void action_ststop(lv_event_t * e);
extern void action_inc_longpressed(lv_event_t * e);
extern void action_dec_longpressed(lv_event_t * e);
extern void action_load_programmer(lv_event_t * e);
extern void action_load_tester(lv_event_t * e);
extern void action_setposition(lv_event_t * e);
extern void action_load_meter(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/