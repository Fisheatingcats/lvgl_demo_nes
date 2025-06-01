#include "lv_porting.h"
#include "lvgl.h"

#define MONITOR_HOR_RES  800
#define MONITOR_VER_RES  480
#define MONITOR_BUF_SIZE MONITOR_HOR_RES * 10

#ifdef MCU_ENV

#include "gt911.h"
#include "lcd.h"
#include "timer.h"
#include "systick.h"

void disp_drv_init(void);
void indev_drv_init(void);

void lv_porting_init(void)
{
    lv_init();
    disp_drv_init();          // 显示设备驱动初始化
    indev_drv_init();         // 输入设备驱动初始化
    timer2_it_init(7199, 49); // 时钟中断初始化
}

void TIM2_IRQHandler(void)
{
    lv_tick_inc(5);
    timer2_clearFlag();
}

void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    LCD_draw_area(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);
    lv_disp_flush_ready(disp_drv);
}

// 显示设备驱动初始化
void disp_drv_init(void)
{

    lcd_init();
    LCD_WriteReg(0x3600, 0x60); // 横屏

    // 初始化图像缓冲区, 第二个缓冲区(可选)可以传入NULL
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf_1[MONITOR_BUF_SIZE];
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, MONITOR_BUF_SIZE);

    // 初始化显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = MONITOR_HOR_RES;
    disp_drv.ver_res  = MONITOR_VER_RES;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);
}

void indev_read(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{

    GT911_touch_point *touchPoints = gt911_scan();
    if (touchPoints != NULL) {
        // 横屏坐标转换
        data->point.x = touchPoints->y;
        data->point.y = MONITOR_HOR_RES - touchPoints->x - 1;
        data->state   = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// 输入设备驱动初始化
void indev_drv_init(void)
{
    // 初始化触摸屏
    gt911_init();

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = indev_read;
    lv_indev_drv_register(&indev_drv);
}

void inline lv_porting_delay(void)
{
    delay_ms(5);
}

#else

#define _DEFAULT_SOURCE  /* needed for usleep() */
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "sdl/sdl.h"

static int tick_thread(void *data)
{
    LV_UNUSED(data);

    while (1) {
        SDL_Delay(5);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

void lv_porting_init(void)
{
    lv_init();
    monitor_init();
    // SDL创建线程
    SDL_CreateThread(tick_thread, "tick", NULL);

    // 初始化图像缓冲区, 第二个缓冲区(可选)可以传入NULL
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf_1[MONITOR_BUF_SIZE];
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, MONITOR_BUF_SIZE);

    /* 注册显示驱动 */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf     = &disp_buf;
    disp_drv.flush_cb     = monitor_flush;
    disp_drv.hor_res      = MONITOR_HOR_RES;
    disp_drv.ver_res      = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;
    lv_disp_t *disp       = lv_disp_drv_register(&disp_drv);

    /* 注册鼠标驱动 */
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type        = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb     = sdl_mouse_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
}

void inline lv_porting_delay(void)
{
    SDL_Delay(5);
}

#endif