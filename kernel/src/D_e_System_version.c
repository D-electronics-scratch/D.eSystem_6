#include "D_e_System_version.h"
#include "ui_desktop.h"
#include "desktop_ui.h"

#include "video.h"
#include "keyboard.h"
#include "desktop_ui.h"
#include "main_ui.h"
#include "keyboard_input.h"
#include "system_info.h"
#include "main.h"
#include "memory.h"
#include <string.h>
#include "bpb.h"
#include "string.h"



void D_e_Sys_v_run();
void GUI();
void print_sys_specs();
extern int ui_mode;


void D_e_Sys_v_run() {
    clear_screen(framebuffer, 0x404040);
    int key = 0;
    


    print_main_ui();
    //GUI


    GUI(framebuffer);



    while(key == 0) {
        key = ps2_read_key();
    }
    if (key == 27) {
        ui_desktop();
        return;
    }


    
    



}




void GUI(struct limine_framebuffer *fb) {





    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 800;
    uint32_t icon_h = 200;

    uint32_t x = (W / 2) - (icon_w / 2) + 0;
    uint32_t y = (H / 2) - (icon_h / 2) - 250;

    draw_rect(fb, x, y, icon_w, icon_h, 0xFFFFFF);
    draw_text(fb, x+ 10, y + 10, "Press [Esc] to close", 0xFF0000);

    draw_text(fb,x + 10, y + 30, "Kernel version: D.Kernel 0.1.3", 0x404040);
    draw_text(fb,x + 10, y + 40, "Operatingsystem: D.eSystem 6.1.8 beta", 0x404040);
    if(ui_mode == 1) {
        draw_text(fb,x + 10, y + 50, "UI version: D.UI 12.1", 0x404040);
    } else {
        draw_text(fb,x + 10, y + 50, "UI version: D.Touch UI (ASCII edition 2.0)", 0x404040);
    }
    if(ui_mode == 1) {
        draw_text(fb,x + 10, y + 60, "UX version: UX version: Color UX D.eSystem 6 edition", 0x404040);
    } else{
        draw_text(fb,x + 10, y + 60, "UX version: UX version: Text UX (ASCII)", 0x404040);
    }
    
    draw_text(fb, x + 10, y + 70,"Brand: D.electronics", 0x404040);
    draw_text(fb,x + 10, y + 80, "Codename: Everest-Mount", 0x404040);
    draw_text(fb,x + 10, y + 90, "CPU:", 0x404040);
    draw_text(fb, x + 10, y + 100, "RAM (MB)", 0x404040);

    print_sys_specs(framebuffer);
    


}




void print_sys_specs(struct limine_framebuffer *fb) {


    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 800;
    uint32_t icon_h = 200;

    uint32_t x = (W / 2) - (icon_w / 2) + 0;
    uint32_t y = (H / 2) - (icon_h / 2) - 250;

    int key = 0;

    char cpu_name[64];
    

    get_cpu_brand(cpu_name);
    draw_text(fb, x + 43, y + 90, cpu_name, 0x404040);

    uint64_t ram = get_total_ram();
    char ram_text[32];
    itoa(ram / (1024 * 1024), ram_text, 10);
    draw_text(fb, x + 75, y + 100, ram_text, 0x404040);

}