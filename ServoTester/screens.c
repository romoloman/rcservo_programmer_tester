#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_gesture, LV_EVENT_GESTURE, (void *)0);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 533);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // readservo
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.readservo = obj;
                    lv_obj_set_pos(obj, 5, 17);
                    lv_obj_set_size(obj, 100, 30);
                    lv_obj_add_event_cb(obj, action_read_servo, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff24921c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Read Servo");
                        }
                    }
                }
                {
                    // writeservo
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.writeservo = obj;
                    lv_obj_set_pos(obj, 198, 17);
                    lv_obj_set_size(obj, 100, 30);
                    lv_obj_add_event_cb(obj, action_write_servo, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffe20d0d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Write Servo");
                        }
                    }
                }
                {
                    // msglbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.msglbl = obj;
                    lv_obj_set_pos(obj, 60, 56);
                    lv_obj_set_size(obj, 200, 25);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 18, 85);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 2);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 280, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 99);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Min angle (0-255)");
                }
                {
                    // minangledec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.minangledec = obj;
                    lv_obj_set_pos(obj, 178, 95);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // minanglelbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.minanglelbl = obj;
                    lv_obj_set_pos(obj, 208, 95);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "100");
                }
                {
                    // minangleinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.minangleinc = obj;
                    lv_obj_set_pos(obj, 278, 95);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 129);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Max angle (0-255)");
                }
                {
                    // maxangledec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.maxangledec = obj;
                    lv_obj_set_pos(obj, 178, 125);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // maxanglelbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.maxanglelbl = obj;
                    lv_obj_set_pos(obj, 208, 125);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "100");
                }
                {
                    // maxangleinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.maxangleinc = obj;
                    lv_obj_set_pos(obj, 278, 125);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    // neutraldec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.neutraldec = obj;
                    lv_obj_set_pos(obj, 178, 157);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // neutrallbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.neutrallbl = obj;
                    lv_obj_set_pos(obj, 208, 157);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "4096");
                }
                {
                    // neutralinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.neutralinc = obj;
                    lv_obj_set_pos(obj, 278, 155);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 159);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Neutral (2000-6000)");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 18, 189);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 2);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 280, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 199);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pulse min (500-1500)");
                }
                {
                    // pulsemindec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pulsemindec = obj;
                    lv_obj_set_pos(obj, 178, 197);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // pulseminlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pulseminlbl = obj;
                    lv_obj_set_pos(obj, 208, 197);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "500");
                }
                {
                    // pulsemininc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pulsemininc = obj;
                    lv_obj_set_pos(obj, 278, 197);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 229);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pulse max (1500-2500)");
                }
                {
                    // pulsemaxdec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pulsemaxdec = obj;
                    lv_obj_set_pos(obj, 178, 227);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // pulsemaxlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pulsemaxlbl = obj;
                    lv_obj_set_pos(obj, 208, 227);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "2500");
                }
                {
                    // pulsemaxinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pulsemaxinc = obj;
                    lv_obj_set_pos(obj, 278, 227);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 259);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Dead Band (0-255)");
                }
                {
                    // deadbanddec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.deadbanddec = obj;
                    lv_obj_set_pos(obj, 178, 257);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // deadbandlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.deadbandlbl = obj;
                    lv_obj_set_pos(obj, 208, 257);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "15");
                }
                {
                    // deadbandinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.deadbandinc = obj;
                    lv_obj_set_pos(obj, 278, 257);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 289);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Gain (0-30)");
                }
                {
                    // gaindec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.gaindec = obj;
                    lv_obj_set_pos(obj, 178, 287);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // gainlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gainlbl = obj;
                    lv_obj_set_pos(obj, 208, 287);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "29");
                }
                {
                    // gaininc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.gaininc = obj;
                    lv_obj_set_pos(obj, 278, 287);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 319);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Dumping (0-255)");
                }
                {
                    // dumpingdec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.dumpingdec = obj;
                    lv_obj_set_pos(obj, 178, 317);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // dumpinglbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.dumpinglbl = obj;
                    lv_obj_set_pos(obj, 208, 317);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "12");
                }
                {
                    // dumpinginc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.dumpinginc = obj;
                    lv_obj_set_pos(obj, 278, 317);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 349);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Power (0-1200)");
                }
                {
                    // powerdec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.powerdec = obj;
                    lv_obj_set_pos(obj, 178, 347);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // powerlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.powerlbl = obj;
                    lv_obj_set_pos(obj, 208, 347);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "1200");
                }
                {
                    // powerinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.powerinc = obj;
                    lv_obj_set_pos(obj, 278, 347);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 18, 374);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 2);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 280, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 379);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "OVERLOAD PROTECTION");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 405);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Time (0-60000)");
                }
                {
                    // oltimedec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.oltimedec = obj;
                    lv_obj_set_pos(obj, 178, 403);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // oltimelbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.oltimelbl = obj;
                    lv_obj_set_pos(obj, 208, 403);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "2000");
                }
                {
                    // oltimeinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.oltimeinc = obj;
                    lv_obj_set_pos(obj, 278, 403);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 435);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Power (0-1200)");
                }
                {
                    // olpowerdec
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.olpowerdec = obj;
                    lv_obj_set_pos(obj, 178, 433);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "-");
                        }
                    }
                }
                {
                    // ololpowerlbl
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ololpowerlbl = obj;
                    lv_obj_set_pos(obj, 208, 433);
                    lv_obj_set_size(obj, 60, 20);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "1200");
                }
                {
                    // ololpowerinc
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.ololpowerinc = obj;
                    lv_obj_set_pos(obj, 278, 433);
                    lv_obj_set_size(obj, 20, 21);
                    lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "+");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 18, 459);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 1);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 280, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 470);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Inversion");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 500);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Soft start");
                }
                {
                    // inversionsw
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.inversionsw = obj;
                    lv_obj_set_pos(obj, 246, 466);
                    lv_obj_set_size(obj, 50, 25);
                    lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                }
                {
                    // softstartsw
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.softstartsw = obj;
                    lv_obj_set_pos(obj, 246, 496);
                    lv_obj_set_size(obj, 50, 25);
                    lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_gesture, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_spangroup_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 40);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00c6ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 91, 11);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_decor(obj, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "SERVO TESTER");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 50);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pulse min (500-1500)");
        }
        {
            // stpulsemindec
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stpulsemindec = obj;
            lv_obj_set_pos(obj, 185, 48);
            lv_obj_set_size(obj, 20, 20);
            lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "-");
                }
            }
        }
        {
            // stpulseminlbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.stpulseminlbl = obj;
            lv_obj_set_pos(obj, 215, 48);
            lv_obj_set_size(obj, 60, 20);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "500");
        }
        {
            // stpulsemininc
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stpulsemininc = obj;
            lv_obj_set_pos(obj, 285, 48);
            lv_obj_set_size(obj, 20, 21);
            lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "+");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 80);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pulse max (1500-2500)");
        }
        {
            // stpulsemaxdec
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stpulsemaxdec = obj;
            lv_obj_set_pos(obj, 185, 78);
            lv_obj_set_size(obj, 20, 20);
            lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "-");
                }
            }
        }
        {
            // stpulsemaxlbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.stpulsemaxlbl = obj;
            lv_obj_set_pos(obj, 215, 78);
            lv_obj_set_size(obj, 60, 20);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "2500");
        }
        {
            // stpulsemaxinc
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stpulsemaxinc = obj;
            lv_obj_set_pos(obj, 285, 78);
            lv_obj_set_size(obj, 20, 21);
            lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "+");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Sweep time (1-60 sec)");
        }
        {
            // stsweepdec
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stsweepdec = obj;
            lv_obj_set_pos(obj, 185, 108);
            lv_obj_set_size(obj, 20, 20);
            lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "-");
                }
            }
        }
        {
            // stsweeplbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.stsweeplbl = obj;
            lv_obj_set_pos(obj, 215, 108);
            lv_obj_set_size(obj, 60, 20);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "30");
        }
        {
            // stsweepinc
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stsweepinc = obj;
            lv_obj_set_pos(obj, 285, 108);
            lv_obj_set_size(obj, 20, 21);
            lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "+");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 170);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Position (500-2500)");
        }
        {
            // stcenterdec
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stcenterdec = obj;
            lv_obj_set_pos(obj, 185, 168);
            lv_obj_set_size(obj, 20, 20);
            lv_obj_add_event_cb(obj, action_dec_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_dec_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "-");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 140);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Sweep ON");
        }
        {
            // stcenterlbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.stcenterlbl = obj;
            lv_obj_set_pos(obj, 215, 168);
            lv_obj_set_size(obj, 60, 20);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xff2a76cf), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "1500");
        }
        {
            // stcenterinc
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.stcenterinc = obj;
            lv_obj_set_pos(obj, 285, 168);
            lv_obj_set_size(obj, 20, 21);
            lv_obj_add_event_cb(obj, action_inc_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_inc_longpressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "+");
                }
            }
        }
        {
            // stsweepsw
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.stsweepsw = obj;
            lv_obj_set_pos(obj, 255, 136);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // ststartbt
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.ststartbt = obj;
            lv_obj_set_pos(obj, 20, 195);
            lv_obj_set_size(obj, 100, 36);
            lv_obj_add_event_cb(obj, action_ststart, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff32121), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Start");
                }
            }
        }
        {
            // ststopbt
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.ststopbt = obj;
            lv_obj_set_pos(obj, 200, 195);
            lv_obj_set_size(obj, 100, 36);
            lv_obj_add_event_cb(obj, action_ststop, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_state(obj, LV_STATE_DISABLED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff21f34a), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Stop");
                }
            }
        }
    }
    
    tick_screen_settings();
}

void tick_screen_settings() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_settings,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_settings();
}
