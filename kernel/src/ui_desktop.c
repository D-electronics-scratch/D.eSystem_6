#include "ui_desktop.h"
#include "video.h"

void render_gui();
void clock_icon();
void shutdown_icon();
void calculator_icon();
void info_icon();
void d_e_shell_icon();
void d_menu_icon();
void files_icon();

void render_gui() {

    clock_icon(framebuffer);
    shutdown_icon(framebuffer);
    calculator_icon(framebuffer);
    info_icon(framebuffer);
    d_e_shell_icon(framebuffer);
    files_icon(framebuffer);


}

void clock_icon(struct limine_framebuffer *fb) {
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 500;
    int y_2 = y - 600;
     

    draw_rect(fb, x_2, y_2, 100, 100, 0xFFFFFF);

    draw_rect(fb, x_2 + 10, y_2 + 10, 10, 60, 0x202020);
    draw_rect(fb, x_2 + 80, y_2 + 10, 10, 60, 0x202020);

    draw_rect(fb, x_2 + 20, y_2 + 10, 60, 10, 0x202020);
    draw_rect(fb, x_2 + 20, y_2 + 60, 60, 10, 0x202020);

  
    draw_rect(fb, x_2 + 50, y_2 + 37, 4, 17, 0x202020);
    draw_rect(fb, x_2 + 50, y_2 + 37, 20, 4, 0x202020);

    draw_text(fb, x_2 + 15, y_2 + 85, "Press [3]", 0xFF0000);
}



void shutdown_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x -500;
    int y_2 = y -450;
    

    draw_rect(fb, x_2, y_2, icon_w, icon_h, 0x0094FF);
    draw_rect(fb, x_2 + 20, y_2 + 10, 10, 60, 0xEEEEEE);
    draw_rect(fb, x_2 + 70, y_2 + 10, 10, 60, 0xEEEEEE);
    draw_rect(fb, x_2 + 20, y_2 + 60, 60, 10, 0xEEEEEE);
    draw_rect(fb, x_2 + 45, y_2 + 10, 10, 30, 0xEEEEEE);
    draw_text(fb, x_2 + 5, y_2 + 85, "Powerswitch", 0xFFFFFF);
}



void calculator_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 500;
    int y_2 = y - 300;



    draw_rect(fb, x_2, y_2, icon_w, icon_h, 0x99FF99);

    draw_rect(fb, x_2 + 15, y_2 + 9, 5, 20, 0xFF9999);
    draw_rect(fb, x_2 + 8, y_2 + 16 , 20, 5, 0xFF9999);
    //draw_rect(fb, 730, 690, 5, 20, 0xFF99FF);

    draw_rect(fb,x_2 + 60, y_2 + 15, 20, 5, 0x9999FF);

    draw_rect(fb, x_2 + 10, y_2 + 50, 20, 5, 0xFF99FF);
    draw_rect(fb, x_2 + 10, y_2 + 60, 20, 5, 0xFF99FF);

    draw_rect(fb, x_2 + 70, y_2 + 50, 5, 5, 0x202020);
    draw_rect(fb, x_2 + 70, y_2 + 60, 5, 5, 0x202020);
    draw_text(fb, x_2 + 15, y_2 + 85, "Press [2]", 0xFF0000);
    



    


}

void info_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 500;
    int y_2 = y - 150;

    
    draw_rect(fb, x_2, y_2, icon_w, icon_h, 0xFFFF00);


    draw_rect(fb, x_2 + 45, y_2 + 10, 10, 10, 0xFF3B3B);   // Punkt
    draw_rect(fb, x_2 + 45, y_2 + 30, 10, 40, 0xFF3B3B);   // Strich

    
    draw_text(fb, x_2 + 15, y_2 + 85, "Press [4]", 0xFF0000);
}


void d_e_shell_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 350;
    int y_2 = y - 600;

    draw_rect(fb, x_2, y_2, icon_w, icon_h,  0x000088);

    
    draw_rect(fb, x_2 + 10, y_2 + 60, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 15, y_2 + 55, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 20, y_2 + 50, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 25, y_2 + 45, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 30, y_2 + 40, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 25, y_2 + 35, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 20, y_2 + 30, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 15, y_2 + 25, 5, 5, 0xFFFFFF);
    draw_rect(fb, x_2 + 10, y_2 + 20, 5, 5, 0xFFFFFF);

    draw_rect(fb, x_2 + 35, y_2 + 60, 55, 5, 0xFFFFFF);
    draw_text(fb, x_2 + 15, y_2 + 85, "Press [5]", 0xFF0000);
}

void d_menu_icon() {
    draw_rect(framebuffer, 10, 680, 100, 100, 0xCCCCCC);
    draw_rect(framebuffer, 20, 700, 5, 65, 0xFFFFFF);

    draw_rect(framebuffer, 25, 705, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 30, 710, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 35, 715, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 40, 720, 5, 5, 0xFFFFFF);

    draw_rect(framebuffer, 25, 755, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 30, 750, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 35, 745, 5, 5, 0xFFFFFF);
    draw_rect(framebuffer, 40, 740, 5, 5, 0xFFFFFF);

    draw_rect(framebuffer, 40, 720, 5, 20, 0xFFFFFF);

    draw_rect(framebuffer, 60, 760, 5, 5, 0xFFFFFF);
    

}



void files_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 350;
    int y_2 = y - 450;

    draw_rect(fb, x_2, y_2, icon_w, icon_h,  0xFFFFFF);
    draw_rect(fb, x_2 + 15, y_2 + 30,70,35, 0x0094FF);
    draw_rect(fb, x_2 + 15, y_2 + 15,40,20, 0x0094FF);
    draw_rect(fb , x_2 + 20, y_2 + 40 ,55 ,5, 0xFFFFFF);
    draw_text(fb, x_2 + 15, y_2 + 85, "Press [6]", 0xFF0000); 

}