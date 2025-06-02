/*
 * @Desc:
 * @Author: LIUBIN
 * @version: 0.1
 * @Date: 2025-05-25 21:12:08
 */
#include "lv_demo_nes.h"
#include "nes.h"
#include "lvgl.h"

#include <stdio.h>

lv_obj_t *my_nes;

static void lvgl_keypad_cb(lv_event_t *e)
{
    lv_obj_t *obj        = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_KEY) {
        uint32_t key = lv_indev_get_key(lv_indev_get_act());
        printf("lvgl key : %c\n", key);
        switch (key) {
            case 'W':
                
                break;
            case LV_KEY_UP:
                lv_group_focus_prev(lv_group_get_default());
                break;
            case LV_KEY_DOWN:
                lv_group_focus_next(lv_group_get_default());
                break;
            case LV_KEY_ENTER:
                lv_event_send(obj, LV_EVENT_PRESSED, NULL);
                break;
        }
    }
}

void lv_demo_nes(void)
{
    // 在当前屏幕上创建NES游戏实例
    my_nes = lv_canvas_create(lv_scr_act());
    lv_obj_set_size(my_nes, 256, 240);
    lv_obj_align(my_nes, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_zoom(my_nes, 512); // 两倍分辨率

    // lv_obj_add_event_cb(lv_scr_act(), lvgl_keypad_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(my_nes, lvgl_keypad_cb, LV_EVENT_KEY, NULL);
    lv_group_add_obj(lv_group_get_default(), my_nes);
}