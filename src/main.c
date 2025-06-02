/*
 * @Desc: 
 * @Author: LIUBIN
 * @version: 0.1
 * @Date: 2025-06-01 17:58:29
 */
#include "porting/lv_porting.h"
#include "lvgl.h"

/* nes demo */
#include "lv_demo_nes.h"

#include "SuperMario.h"
#include "nes.h"



int main(int argc, char const *argv[])
{
    lv_porting_init();

    lv_demo_nes();
    
    nes_t *nes = nes_init();
    if (argc == 1)
    {
        // int ret = nes_load_file(nes, nes_file_path);
        int ret = nes_load_rom(nes, acSuperMario);
        if (ret)
        {
            NES_LOG_ERROR("nes load rom fail\n");
            goto error;
        }
        nes_run(nes);
        nes_unload_rom(nes);
        nes_deinit(nes);

        return 0;
    }

    if (argc == 2)
    {
        const char *nes_file_path = argv[1];
        size_t nes_file_path_len = strlen(nes_file_path);
        if (nes_memcmp(nes_file_path + nes_file_path_len - 4, ".nes", 4) == 0 || nes_memcmp(nes_file_path + nes_file_path_len - 4, ".NES", 4) == 0)
        {
            NES_LOG_INFO("nes_file_path:%s\n", nes_file_path);

            int ret = nes_load_file(nes, nes_file_path);
            // int ret = nes_load_rom(nes, acSuperMario);
            if (ret)
            {
                NES_LOG_ERROR("nes load file fail\n");
                goto error;
            }
            nes_run(nes);
            nes_unload_file(nes);
            nes_deinit(nes);
            return 0;
        }
        else
        {
            goto error;
        }
    }
    else
    {

        return 0;
    }
error:
    nes_deinit(nes);
    getchar();
    return -1;

}
