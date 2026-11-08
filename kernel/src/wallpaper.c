#include <stdint.h>
#include <stddef.h>
#include "video.h"

static uint32_t gradient_color(
    uint32_t y,
    uint32_t height,
    int top_r, int top_g, int top_b,
    int mid_r, int mid_g, int mid_b,
    int bot_r, int bot_g, int bot_b)
{
    uint32_t t = (height > 1)
        ? (uint32_t)(((uint64_t)y * 1000U) / (height - 1))
        : 0;

    int r, g, b;
    uint32_t u;

    if (t < 500U) {
        u = t * 2U;

        r = top_r + (mid_r - top_r) * (int)u / 1000;
        g = top_g + (mid_g - top_g) * (int)u / 1000;
        b = top_b + (mid_b - top_b) * (int)u / 1000;
    } else {
        u = (t - 500U) * 2U;

        r = mid_r + (bot_r - mid_r) * (int)u / 1000;
        g = mid_g + (bot_g - mid_g) * (int)u / 1000;
        b = mid_b + (bot_b - mid_b) * (int)u / 1000;
    }

    return ((uint32_t)r << 16)
         | ((uint32_t)g << 8)
         | (uint32_t)b;
}

static void draw_gradient(
    struct limine_framebuffer *fb,
    int top_r, int top_g, int top_b,
    int mid_r, int mid_g, int mid_b,
    int bot_r, int bot_g, int bot_b)
{
    if (fb == NULL || fb->address == NULL)
        return;

    uint32_t width = (uint32_t)fb->width;
    uint32_t height = (uint32_t)fb->height;
    uint32_t pitch = (uint32_t)(fb->pitch / 4);

    if (width == 0 || height == 0 || pitch < width)
        return;

    uint32_t *pixels = (uint32_t *)fb->address;

    for (uint32_t y = 0; y < height; y++) {
        uint32_t color = gradient_color(
            y, height,
            top_r, top_g, top_b,
            mid_r, mid_g, mid_b,
            bot_r, bot_g, bot_b);

        for (uint32_t x = 0; x < width; x++) {
            pixels[(size_t)y * pitch + x] = color;
        }
    }
}

void draw_wallpaper_blue_gradient(struct limine_framebuffer *fb)
{
    draw_gradient(fb,
        8, 18, 65,
        15, 75, 175,
        55, 165, 245);
}

void draw_wallpaper_red_gradient(struct limine_framebuffer *fb)
{
    draw_gradient(fb,
        65, 5, 20,
        190, 15, 35,
        255, 75, 35);
}