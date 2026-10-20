#include "video.h"
#include "main_ui.h"
#include "messagefield.h"

void print_main_ui();

void print_main_ui() {
    draw_text(framebuffer, 550, 10, "D.eSystem 6 (build 6.1.8 beta)", 0x0000FF); //render D.eSystem version
    draw_text(framebuffer, 10, 10, "kernel version: D.Kernel 0.1.3", 0x00FF00); //render D.Kernel version
    //basic_messagefield(framebuffer);
}