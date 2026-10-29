//includes
#include "video.h"
#include "main.h"
#include "keyboard.h"
#include "keyboard_input.h"
#include "main_ui.h"
#include "desktop_ui.h"
#include "file_viewer.h"
#include "bpb.h"
#include "disk.h"
#include "fat32_fs.h"
#include "fat32.h"
#include "sleep.h"
#include "fat32_utils.h"
#include <string.h>
#include <stddef.h>
#include "memory.h"

char fat2_name[12];
#define EXPLORER_MAX_DEPTH 16

typedef struct {
    uint32_t cluster;
} explorer_path_entry_t;

//modules
extern fat32_bpb_t g_bpb;

// UI

void debug_start_ui();
void fat32_tests_icon();


// debug_software_for_D.eSys_modules

void debug_start();
void open_debug();
void fat32_debug_menu();


//test_read
void test_read_db();
void test_read_2_db();
void test_read_3_db();
void test_read_4_db();
void test_read_5_db();
void test_read_6_db();
void test_read_7_db();

//test_write
void test_write_db(void);
void test_write2_db(void);
int test_write3_3_db(void);
void test_write_3_db(void);
int test_fat32_data_db(void);
//void test_write3();



void test_find_free_cluster_db();


// read tests

void test_read_db()
{
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 500, 50, "TEST 1: find file", 0xFFFFFF);
    g_bpb = fat32_read_bpb();
    fat32_file_t file;

    if (!fat32_find_file("TEST    TXT", &g_bpb, &file)) {
        draw_text(framebuffer, 500, 70, "File NOT found!", 0xFFFFFF);
        sleep(2000);
        return;
    }

uint32_t cluster = file.start_cluster;

    if (!cluster) {
        draw_text(framebuffer, 500, 70, "File NOT found!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 70, "File FOUND!", 0xFFFFFF);
    sleep(2000);
    return;
}

void test_read_2_db()
{
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 500, 50, "TEST 2: start", 0xFFFFFF);

    uint8_t buffer[512];

    draw_text(framebuffer, 500, 70, "before read", 0xFFFFFF);
    sleep(1000);

    int result = read_sector(0, buffer);

    draw_text(framebuffer, 500, 90, "after read", 0xFFFFFF);
    sleep(1000);

    if (result != 0) {
        draw_text(framebuffer, 500, 110, "Disk read FAILED!", 0xFFFFFF);

        sleep(1000);
        return;
    }

    draw_text(framebuffer, 500, 110, "Disk sector 0 OK!", 0xFFFFFF);
    sleep(1000);
    return;
            
}


void test_read_3_db() {
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 500, 50, "TEST 2: BPB test", 0xFFFFFF);

    fat32_bpb_t bpb = fat32_read_bpb();

    draw_text(framebuffer, 500, 70, "BPB read OK!", 0xFFFFFF);
    sleep(1000);
}


void test_read_4_db()
{
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 500, 50, "TEST 4: read file", 0xFFFFFF);

    g_bpb = fat32_read_bpb();

    fat32_file_t file;

    if (!fat32_find_file("TEST    TXT", &g_bpb, &file)) {
        draw_text(framebuffer, 500, 70, "File NOT found!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 70, "File FOUND!", 0xFFFFFF);
    sleep(1000);

    uint8_t buffer[512];

    int result = fat32_read_file(
        &file,
        &g_bpb,
        buffer
        );

    if (result != 0) {
        draw_text(framebuffer, 500, 90, "File READ FAILED!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 90, "File READ OK!", 0xFFFFFF);
    sleep(2000);

    draw_text(framebuffer, 500, 110, (char *)buffer, 0xFFFFFF);

    sleep(5000);
    return;
}



void test_read_5_db() {
    clear_screen(framebuffer, 0x0078D7);

    draw_text(framebuffer, 500, 50,
              "TEST 5: FAT cluster chain", 0xFFFFFF);

    g_bpb = fat32_read_bpb();

    fat32_file_t file;

    if (!fat32_find_file("BIG     TXT", &g_bpb, &file)) {
        draw_text(framebuffer, 500, 70,
                  "File NOT found!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 70,
              "File FOUND!", 0xFFFFFF);

    sleep(1000);

    uint32_t cluster = file.start_cluster;

for (int i = 0; i < 8; i++) {

    uint32_t next = fat32_get_next_cluster(
        cluster,
        &g_bpb
    );

    if (next == 0xFFFFFFFF) {
        draw_text(framebuffer, 500, 90,
                  "FAT READ FAILED!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    if (next >= 0x0FFFFFF8) {
        draw_text(framebuffer, 500, 90,
                  "EOC REACHED!", 0xFFFFFF);
        sleep(5000);
        return;
    }

    cluster = next;
}

}




void test_read_6_db()
{
    clear_screen(framebuffer, 0x0078D7);

    draw_text(framebuffer, 500, 50,
              "TEST 6: multi-cluster read", 0xFFFFFF);

    g_bpb = fat32_read_bpb();

    fat32_file_t file;

    char fat_name[12];

    if (fat32_make_83_name("BIG.TXT", fat_name) != 0) {
        draw_text(framebuffer, 500, 70,
                  "83 NAME ERROR!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    if (!fat32_find_file(fat_name, &g_bpb, &file)) {
        draw_text(framebuffer, 500, 70,
                  "File NOT found!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 70,
              "File FOUND!", 0xFFFFFF);

    sleep(1000);

    uint8_t buffer[2048];

    int result = fat32_read_file(
        &file,
        &g_bpb,
        buffer
    );

    if (result != 0) {
        draw_text(framebuffer, 500, 90,
                  "File READ FAILED!", 0xFFFFFF);
        sleep(2000);
        return;
    }

    draw_text(framebuffer, 500, 90,
              "MULTI-CLUSTER READ OK!", 0xFFFFFF);

    sleep(2000);
}

void test_read_7_db()
{
    clear_screen(framebuffer, 0xFFFFFF);

    g_bpb = fat32_read_bpb();

    fat32_dir_entry_t entries[32];
    uint32_t count = 0;

    int result = fat32_list_directory(
        g_bpb.root_cluster,
        &g_bpb,
        entries,
        32,
        &count
    );

    if (result != 0) {
        draw_text(
            framebuffer,
            0, 160,
            "Directory READ FAILED!",
            0xFF0000
        );

        explorer_loop();
        sleep(2000);
        return;
    }

    explorer_ui(framebuffer);

    for (uint32_t i = 0; i < count; i++) {
        draw_text(
            framebuffer,
            50,
            200 + i * 20,
            entries[i].name,
            0x000000
        );
    }

    //explorer_loop();
}

//write tests

void test_write_db(void)
{
    uint8_t buffer[512];

    for (int i = 0; i < 512; i++) {
        buffer[i] = 'A';
    }

    if (write_sector(3000, buffer) != 0) {
        draw_text(framebuffer, 0, 160, "WRITE FAILED!", 0xFF0000);
        sleep(3000);
        return;
    }

    draw_text(framebuffer, 0, 160, "WRITE OK!", 0xFF0000);
    sleep(3000);
}

void test_write2_db(void)
{
    uint8_t write_buffer[512];
    uint8_t read_buffer[512];
    for (int i = 0; i < 512; i++) {
        write_buffer[i] = (uint8_t)(i % 256);
    }

    if (write_sector(3000, write_buffer) != 0) {
        draw_text(
            framebuffer, 50, 160,
            "WRITE FAILED!", 0xFF0000);
        sleep(3000);
        return;
    }

    if (read_sector(3000, read_buffer) != 0) {
        draw_text(
            framebuffer, 50, 160,
            "READ FAILED!", 0xFF0000);
        sleep(3000);
        return;
    }
    for (int i = 0; i < 512; i++) {
        if (write_buffer[i] != read_buffer[i]) {
            draw_text(
                framebuffer, 50, 160,
                "COMPARE FAILED!", 0xFF0000);
            sleep(3000);
            return;
        }
    }

    draw_text(
        framebuffer, 0, 160,"READ/WRITE TEST OK!", 0x000000);
        sleep(3000);
        return;
}






//find free cluster test


// debug_main_modules


void open_debug(struct limine_framebuffer *fb) {
    clear_screen(framebuffer, 0x00FF00);

    debug_start();
}

void debug_start() {
    debug_start_ui(framebuffer);
    key = 0;
    while (key == 0) {
        key = ps2_read_key();
        
    }
    if (key == 27) {
        return;

    } else if (key == '1') {
        fat32_debug_menu();

    }
}


void debug_start_ui(struct limine_framebuffer *fb) {
    clear_screen(framebuffer, 0xFFFFFF);
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 50;
    int y_2 = y - 300;


    draw_rect(fb, x_2, y_2, 100, 100, 0xFF0000);



    draw_rect(fb,x_2 + 15, y_2 + 5, 5, 5, 0xFFFFFF);
    draw_rect(fb,x + 20, y_2 + 10, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 25, y_2 + 15, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 30, y_2 + 20, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 35, y_2 + 25, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 40, y_2 + 30, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 45, y_2 + 35, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 50, y_2 + 40, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 55, y_2 + 45, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 60, y_2 + 50, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 65, y_2 + 55, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 70, y_2 + 60, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 75, y_2 + 65, 5, 5, 0xFFFFFF);

    draw_rect(fb,x_2 + 75, y_2 + 5, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 70, y_2 + 10, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 65, y_2 + 15, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 60, y_2 + 20, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 55, y_2 + 25 , 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 50, y_2 + 30, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 45, y_2 + 35, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 40, y_2 + 40, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 35, y_2 + 45, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 30, y_2 + 50 , 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 25, y_2 + 55, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 20, y_2 + 60, 5, 5, 0xFFFFFF);
    draw_rect(fb,x_2 + 15, y_2 + 65, 5, 5, 0xFFFFFF);
    draw_text(fb,x_2 + 5, y_2 + 85, "Press [Esc]", 0xFFFFFF);

    fat32_tests_icon(framebuffer);

}


void fat32_tests_icon(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;

    uint32_t x = (W / 2) - (icon_w / 2);
    uint32_t y = (H / 2) - (icon_h / 2) + 330;
    int x_2 = x - 200;
    int y_2 = y - 300;


    draw_rect(fb, x_2 , y_2, 100, 100, 0x000000);

    draw_rect(fb, x_2 + 10, y_2 + 35, 25, 25, 0xC0C0C0 );
    draw_rect(fb, x_2 +  15, y_2 + 40, 10, 5, 0x000000);
    draw_rect(fb, x_2 + 15, y_2 + 50, 10, 5, 0x000000);
    draw_rect(fb, x_2 + 35, y_2 + 30, 60, 35, 0xFFFFFF );
    draw_text(fb,x_2 + 5, y_2 + 75, "FAT32_debug [1]", 0xFFFFFF);
    draw_text(fb,x_2 + 5, y_2 + 85, " Press [1]", 0xFFFFFF);

}














void fat32_debug_menu() {
    clear_screen(framebuffer,0x0078D7 );
    draw_text(framebuffer, 0, 100, "Press [Esc] to close this menu!!", 0xFF0000 );
    draw_text(framebuffer, 0, 110, "Press [1] to run test_read.", 0xFFFFFF);
    draw_text(framebuffer, 0, 120, "Press [2] to run test_read_2.",0xFFFFFF);
    draw_text(framebuffer, 0, 130, "Press [3] to run test_read_3.",0xFFFFFF);
    draw_text(framebuffer, 0, 140, "Press [4] to run test_read_4.",0xFFFFFF);
    draw_text(framebuffer, 0, 150, "Press [5] to run test_read_5.", 0xFFFFFF);
    draw_text(framebuffer, 0, 160, "Press [6] to run test_read_6.", 0xFFFFFF);
    draw_text(framebuffer, 0, 170, "Press [7] to run test_write.", 0xFFFFFF);
    draw_text(framebuffer, 0, 180, "Press [8] to run test_write_2.", 0xFFFFFF);
    

    key = 0;
    while(key == 0) {
        key = ps2_read_key();
    }

    if(key == 27) {
        return;
    } else if(key == '1') {
        test_read_db();
        
    }else if(key == '2') {
        test_read_2_db();
    }else if(key == '3') {
        test_read_3_db();
    }else if(key == '4') {
        test_read_4_db();
    }else if (key == '5') {
        test_read_5_db();
    } else if(key == '6') {
        test_read_6_db();
    } else if(key == '7') {
        test_write_db();
    } else if(key == '8') {
        test_write2_db();
    } else {
        draw_text(framebuffer, 0, 160,"                               | You pressed an unknown key!! Closing debug mode in 3 secconds!", 0xFFFFFF);
        sleep(3000);
        return;
    }



}