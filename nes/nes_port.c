/*
 * @Desc:
 * @Author: LIUBIN
 * @version: 0.1
 * @Date: 2025-05-17 02:11:50
 */
/*
 * Copyright 2023-2024 Dozingfiretruck
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "nes.h"
#include "lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include "lv_demo_nes.h"

#include <SDL2/SDL.h>
// #include "../lv_drivers/sdl/sdl.h"
// #include "lv_drv_conf.h"
/* memory */
void *nes_malloc(int num)
{
    return malloc(num);
}

void nes_free(void *address)
{
    free(address);
}

void *nes_memcpy(void *str1, const void *str2, size_t n)
{
    return memcpy(str1, str2, n);
}

void *nes_memset(void *str, int c, size_t n)
{
    return memset(str, c, n);
}

int nes_memcmp(const void *str1, const void *str2, size_t n)
{
    return memcmp(str1, str2, n);
}

#if (NES_USE_FS == 1)
/* io */
FILE *nes_fopen(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

size_t nes_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

size_t nes_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int nes_fseek(FILE *stream, long int offset, int whence)
{
    return fseek(stream, offset, whence);
}

int nes_fclose(FILE *stream)
{
    return fclose(stream);
}
#endif

// int nes_log_printf(const char *format, ...){
//     va_list args;
//     va_start(args, format);
//     vprintf(format, args);
//     va_end(args);

//     size_t len;
//     va_list args;
//     if (luat_log_level_cur > level) return;
//     char log_printf_buff[LOGLOG_SIZE]  = {0};
//     va_start(args, format);
//     len = vsnprintf_(log_printf_buff, LOGLOG_SIZE - 2, format, args);
//     va_end(args);
//     if (len > 0) {
//         log_printf_buff[len] = '\n';
//         luat_log_write(log_printf_buff, len + 1);
//     }

// }

extern void keyboard_handler(SDL_Event *event);

static void sdl_event(nes_t *nes)
{
    SDL_Event event;
    keyboard_handler(&event);
    if (SDL_PollEvent(&event)) {

        switch (event.type) {

            case SDL_KEYDOWN:
                // printf("key:%d\n", event.key.keysym.scancode);
                switch (event.key.keysym.scancode) {
                    case 26: // W
                        nes->nes_cpu.joypad.U1 = 1;
                        break;
                    case 22: // S
                        nes->nes_cpu.joypad.D1 = 1;
                        break;
                    case 4: // A
                        nes->nes_cpu.joypad.L1 = 1;
                        break;
                    case 7: // D
                        nes->nes_cpu.joypad.R1 = 1;
                        break;
                    case 13: // J
                        nes->nes_cpu.joypad.A1 = 1;
                        break;
                    case 14: // K
                        nes->nes_cpu.joypad.B1 = 1;
                        break;
                    case 25: // V
                        nes->nes_cpu.joypad.SE1 = 1;
                        break;
                    case 5: // B
                        nes->nes_cpu.joypad.ST1 = 1;
                        break;
                    case 82: // ↑
                        nes->nes_cpu.joypad.U2 = 1;
                        break;
                    case 81: // ↓
                        nes->nes_cpu.joypad.D2 = 1;
                        break;
                    case 80: // ←
                        nes->nes_cpu.joypad.L2 = 1;
                        break;
                    case 79: // →
                        nes->nes_cpu.joypad.R2 = 1;
                        break;
                    case 93: // 5
                        nes->nes_cpu.joypad.A2 = 1;
                        break;
                    case 94: // 6
                        nes->nes_cpu.joypad.B2 = 1;
                        break;
                    case 89: // 1
                        nes->nes_cpu.joypad.SE2 = 1;
                        break;
                    case 90: // 2
                        nes->nes_cpu.joypad.ST2 = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case 26: // W
                        nes->nes_cpu.joypad.U1 = 0;
                        break;
                    case 22: // S
                        nes->nes_cpu.joypad.D1 = 0;
                        break;
                    case 4: // A
                        nes->nes_cpu.joypad.L1 = 0;
                        break;
                    case 7: // D
                        nes->nes_cpu.joypad.R1 = 0;
                        break;
                    case 13: // J
                        nes->nes_cpu.joypad.A1 = 0;
                        break;
                    case 14: // K
                        nes->nes_cpu.joypad.B1 = 0;
                        break;
                    case 25: // V
                        nes->nes_cpu.joypad.SE1 = 0;
                        break;
                    case 5: // B
                        nes->nes_cpu.joypad.ST1 = 0;
                        break;
                    case 82: // ↑
                        nes->nes_cpu.joypad.U2 = 0;
                        break;
                    case 81: // ↓
                        nes->nes_cpu.joypad.D2 = 0;
                        break;
                    case 80: // ←
                        nes->nes_cpu.joypad.L2 = 0;
                        break;
                    case 79: // →
                        nes->nes_cpu.joypad.R2 = 0;
                        break;
                    case 93: // 5
                        nes->nes_cpu.joypad.A2 = 0;
                        break;
                    case 94: // 6
                        nes->nes_cpu.joypad.B2 = 0;
                        break;
                    case 89: // 1
                        nes->nes_cpu.joypad.SE2 = 0;
                        break;
                    case 90: // 2
                        nes->nes_cpu.joypad.ST2 = 0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                nes_deinit(nes);
                return;
        }
    }
}

#if (NES_ENABLE_SOUND == 1)

static SDL_AudioDeviceID nes_audio_device;
#define SDL_AUDIO_NUM_CHANNELS (1)

static uint8_t apu_output = 0;
static void AudioCallback(void *userdata, Uint8 *stream, int len)
{
    (void)len;
    nes_t *nes = (nes_t *)userdata;
    // printf("%s\n",nes->nes_apu.sample_buffer[0]);
    if (apu_output) {
        nes_memcpy(stream, &nes->nes_apu.sample_buffer, NES_APU_SAMPLE_PER_SYNC);
        apu_output = 0;
    }
}

int nes_sound_output(uint8_t *buffer, size_t len)
{
    (void)buffer;
    (void)len;
    apu_output = 1;
    return 0;
}
#endif

int nes_initex(nes_t *nes)
{

    printf("nes init");

    if (SDL_Init(SDL_INIT_AUDIO)) {
        printf("Can not init audio, %s", SDL_GetError());
        return -1;
    }
#if (NES_ENABLE_SOUND == 1)
    SDL_AudioSpec desired = {
        .freq     = NES_APU_SAMPLE_RATE,
        .format   = AUDIO_S8,
        .channels = SDL_AUDIO_NUM_CHANNELS,
        .samples  = NES_APU_SAMPLE_PER_SYNC,
        .callback = AudioCallback,
        .userdata = nes};
    nes_audio_device = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desired, NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (!nes_audio_device) {
        printf("Couldn't open audio: %s\n", SDL_GetError());
    }
    SDL_PauseAudio(0);

    SDL_PauseAudioDevice(nes_audio_device, SDL_FALSE);
#endif
    return 0;
}

int nes_deinitex(nes_t *nes)
{
    printf("nes deinit");

    return 0;
}

int nes_draw(int x1, int y1, int x2, int y2, nes_color_t *color_data)
{
    // 将 NES 颜色格式转换为 LVGL 格式（假设颜色格式为 RGB565）
    lv_canvas_set_buffer(my_nes, color_data, (x2 - x1 + 1), (y2 - y1 + 1), LV_IMG_CF_TRUE_COLOR);

    return 0;
}

#define FRAMES_PER_SECOND 1000 / 60

void nes_frame(nes_t *nes)
{
    sdl_event(nes);
    
    SDL_Delay(FRAMES_PER_SECOND);
    // vTaskDelay(pdMS_TO_TICKS(10));
}
