#include "video.h"
#include "main.h"
#include "ui_desktop.h"
#include "main_ui.h"
#include "sleep.h"

void messagefiel_backgroud();
void basic_messagefield();
void messagefield_welcome();
void show_welcome();
void show_clock();





void basic_messagefield(struct limine_framebuffer *fb)
{
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2) - 500;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);

    draw_rect(
        fb,
        x + 480,
        y - 150,
        150,
        30,
        0x000000
    );
}



void messagefield_welcome(struct limine_framebuffer *fb, int *x_b, int *y_b) {
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2) - 500;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);
    draw_rect(fb, x + 480, y - 150, 150, 30, 0x000000);
    draw_text(fb, x+ 520, y - 140, "Welcome!!", 0xFFFFFF);
    sleep(3000);
    basic_messagefield(fb);
}


void show_clock(struct limine_framebuffer *fb, int *x_b, int *y_b) {
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2) - 500;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);
    draw_rect(fb, x + 480, y - 150, 150, 30, 0x000000);
    draw_text(fb, x+ 535, y - 140, "Clock", 0xFFFFFF);
    sleep(2000);
    basic_messagefield(fb);
}

