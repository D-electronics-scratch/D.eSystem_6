#include "video.h"
#include "background.h"



static uint32_t lerp_color(uint32_t a, uint32_t b, uint32_t t, uint32_t max) {
    uint8_t ar = (a >> 16) & 0xFF;
    uint8_t ag = (a >> 8) & 0xFF;
    uint8_t ab = a & 0xFF;

    uint8_t br = (b >> 16) & 0xFF;
    uint8_t bg = (b >> 8) & 0xFF;
    uint8_t bb = b & 0xFF;

    uint8_t r = ar + ((br - ar) * t) / max;
    uint8_t g = ag + ((bg - ag) * t) / max;
    uint8_t b2 = ab + ((bb - ab) * t) / max;

    return (r << 16) | (g << 8) | b2;
}

void draw_wallpaper_gradient(struct limine_framebuffer *fb) {
    uint32_t top = 0xFF00FF;     
    uint32_t bottom = 0x00FFFF;  

    for (uint32_t y = 0; y < fb->height; y++) {
        uint32_t color = lerp_color(top, bottom, y, fb->height);

        for (uint32_t x = 0; x < fb->width; x++) {
            putpixel(fb, x, y, color);
        }
    }
}






void draw_cube_wallpaper(struct limine_framebuffer *fb) {
    draw_rect(fb, 10, 10, 20, 40, 0x00FF00 );
    draw_rect(fb, 60, 50, 40,60, 0xFF0000 );
    draw_rect(fb, 40, 100 ,50,50, 0x0000FF );
    draw_rect(fb, 100,300, 60, 46, 0xFF00FF );
    draw_rect(fb, 150, 200, 100,130, 0xFFFF00);
    draw_rect(fb, 312, 323, 120, 25, 0x00FFFF );
    draw_rect(fb, 563, 455, 43,156, 0x8800FF);
    draw_rect(fb, 742, 756, 80, 130, 0x00D4FF);
    draw_rect(fb, 800, 200, 40,90,  0x0000FF);
    draw_rect(fb, 1000, 100, 342,432, 0xFF0000);
    draw_rect(fb, 856, 457, 170, 160, 0x000088);
    draw_rect(fb, 30, 600, 234,123, 0x000088);
}