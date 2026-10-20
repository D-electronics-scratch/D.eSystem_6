#include "introduction.h"
#include "video.h"
#include "main.h"
#include "keyboard.h"
#include "keyboard_input.h"
#include "util.h"

#include <limine.h>
#include "main_ui.h"
#include "desktop_ui.h"
#include "calculator_input.h"
#include "sleep.h"
#include "messagefield.h"
#include "wallpaper.h"
extern int setup_yes_no;
extern int ui_mode;
extern int ui_mode_2;


void run_setup_start();
void render_interface();
void page_2();


void run_setup_start() {
    clear_screen(framebuffer, 0x303030);
    render_interface(framebuffer);
}





void render_interface(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 750;
    uint32_t icon_h = 400;
    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H * 2) / 4 - (icon_h / 2);

    draw_wallpaper_blue_gradient(framebuffer);
    draw_rect(fb, x, y, 750, 400, 0xFFFFFF);
    draw_text(fb, x + 300, y + 20, "Welcome in D.eSystem 6", 0x303030 );
    draw_text(fb, x + 220, y + 40, "press 1 to continue/press 2 to leave", 0x303030);
    //messagefield_welcome(framebuffer);
    char key = 0;
    while(key == 0) {
        key = ps2_read_key();
        
    }

    if(key == '2') {
        clear_screen(framebuffer, 0x303030);
        setup_yes_no = 0;
        ui_mode = 2;
        return;
        
    } else if(key == '1') {
        clear_screen(fb, 0x303030);
        draw_wallpaper_blue_gradient(framebuffer);
        print_main_ui();

        draw_rect(fb, x, y, 750, 400, 0xFFFFFF);
        draw_text(fb, x + 250, y + 20, "What is D.eSystem?", 0x303030 );
        draw_text(fb, x + 130, y + 40, "D.eSystem 6 is an Operatingsystem fro D.electronics.", 0x303030);
        draw_text(fb, x + 130, y + 50, "It runs on the D.Kernel which is written in bare metall C.", 0x303030);
        draw_text(fb, x + 250, y + 60, "D.eSystem 6 is the best system what D.electronics ever built.", 0x303030 );
        draw_text(fb, x + 130, y + 150, "1: next/2:close", 0x303030);
        key = 0;
        while(key == 0) {
            key = ps2_read_key();
        }
        if(key == '2') {
            setup_yes_no = 0;
            ui_mode = 1;
            return;
        } else if(key == '1') {
            clear_screen(fb, 0x303030);
            draw_wallpaper_blue_gradient(framebuffer);
            print_main_ui();
            draw_rect(fb, x, y, 750, 400, 0xFFFFFF);
            draw_text(fb, x + 250, y + 20, "Sellect your homescreen interface", 0x303030 );
            draw_text(fb, x + 130, y + 40, "1: D.UI 11(Graphical intaerface)", 0x303030);
            draw_text(fb,x + 130,y + 50, "2: ASCII interface(text interface)", 0x303030);
            draw_text(fb, x + 130, y + 60, "3: close", 0x303030);
            key = 0;
            while(key == 0) {
                key = ps2_read_key();
            }
            if(key == '3') {
                setup_yes_no = 0;
                ui_mode = 3;
                return;
            } else if(key == '1') {
                ui_mode = 1;
                ui_mode_2 = 1;
                setup_yes_no = 0;
                return;
            } else if (key == '2'){
                ui_mode = 2;
                ui_mode_2 = 2;
                setup_yes_no = 0;
                return;
            }

        }
        
        

    }
}





