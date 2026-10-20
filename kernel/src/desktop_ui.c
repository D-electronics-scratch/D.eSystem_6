#include "video.h"
#include "ui_desktop.h"
#include "main_ui.h"
#include "main.h"
#include "wallpaper.h"

int wallpaper_num;

void ui_desktop();

void render_ui();

void render_desktop();




void ui_desktop() {
    clear_screen(framebuffer, 0x000000);  
    render_desktop();
    render_gui(); 

    render_ui();                          
}


void render_desktop() {
    draw_wallpaper_blue_gradient(framebuffer);


    if(wallpaper_num == 1) {

        draw_wallpaper_blue_gradient(framebuffer);

    } else if(wallpaper_num == 2) {

        draw_wallpaper_red_gradient(framebuffer);
    } else {
        wallpaper_num = 1;
        draw_wallpaper_blue_gradient(framebuffer);

    }


    
}


void render_ui() {
    //draw_text(framebuffer, 500, 50, "--------------------------------", 0xFFFFFF);
    //draw_text(framebuffer, 500, 60, "| [Esc] Shutdown               |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 70, "| [2] calculator               |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 80, "| [3] clock                    |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 90, "| [4] info                     |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 100, "| [5] D.eShell                 |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 110, "| [6] thundercalculator        |", 0xFFFFFF);
    //draw_text(framebuffer, 500, 120, "--------------------------------", 0xFFFFFF);

    print_main_ui();
}



