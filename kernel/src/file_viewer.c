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

char fat_name[12];
#define EXPLORER_MAX_DEPTH 16

typedef struct {
    uint32_t cluster;
} explorer_path_entry_t;

//modules
extern fat32_bpb_t g_bpb;
void file_viewer_start();

//test_read
void test_read();
void test_read_2();
void test_read_3();
void test_read_4();
void test_read_5();
void test_read_6();
void test_read_7();

//test_write
void test_write(void);
void test_write2(void);
int test_write3_3(void);
void test_write_3(void);
int test_fat32_data(void);
//void test_write3();



void test_find_free_cluster();


void file_viewer_explorer();
void file_viewer_open();
void explorer_ui();
void explorer_loop();
void explorer_dir();
void create_new_file(struct limine_framebuffer *fb);


//static uint32_t explorer_current_cluster = 2;
//static uint32_t explorer_parent_cluster = 0;
//static int explorer_in_directory = 0;
//static uint32_t explorer_selected = 0;
static int explorer_enter_directory(uint32_t cluster);
static int explorer_go_back(void);
static fat32_dir_entry_t explorer_entries[32];
//static uint32_t explorer_entry_count = 0;
static explorer_path_entry_t explorer_path[EXPLORER_MAX_DEPTH];
//static int explorer_depth = 0;
static uint32_t explorer_current_cluster = 2;
static uint32_t explorer_selected = 0;
//static fat32_dir_entry_t explorer_entries[32];
static uint32_t explorer_entry_count = 0;
int test_fat32_data(void);

//char input_filename
//char content_file



void file_viewer_start() {
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 0, 50, "press 1 for test_1 (find file)               | press B for test_fat32_data", 0xFFFFFF);
    draw_text(framebuffer, 0, 60, "press 2 for test_2                           | press C for test_write 3   ", 0xFFFFFF);
    draw_text(framebuffer, 0, 70, "press 3 for test_3", 0xFFFFFF);
    draw_text(framebuffer, 0, 80, "press 4 for test_4 (read_file)", 0xFFFFFF);
    draw_text(framebuffer, 0, 90, "press 5 for test_5", 0xFFFFFF);
    draw_text(framebuffer, 0, 100, "press 6 for test_6", 0xFFFFFF);
    draw_text(framebuffer, 0, 110, "press 7 for test_7", 0xFFFFFF);
    draw_text(framebuffer, 0, 120, "press 8 for test_write 1", 0xFFFFFF);
    draw_text(framebuffer, 0, 130, "press 9 for test_write 2", 0xFFFFFF);
    draw_text(framebuffer, 0, 140, "press 0 for test_find_free_cluster", 0xFFFFFF);
    draw_text(framebuffer, 0, 150, "press A for test_write 3", 0xFFFFFF);

    key = 0;
    while (key == 0) {
        key = ps2_read_key();
        if(key == '1') {
            test_read();
        } else if(key == '2') {
            test_read_2();
        } else if(key == '3') {
            test_read_3();
        } else if(key == '4') {
            test_read_4();
        } else if(key == '5') {
            test_read_5();
        }else if(key == '6') {
            test_read_6();
        } else if(key == '7') {
            test_read_7();
        } else if(key == '8') {
            test_write();
        } else if(key == '9') {
            test_write2();
        } else if(key == '0') {
            test_find_free_cluster();
        } else if(key == 'a') {
            test_write_3();
        } else if(key == 'b') {
            test_fat32_data();
        } else if(key == 'c') {
            test_write_3();
        }
    }
}


//test mode
void test_read()
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

void test_read_2()
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


void test_read_3() {
    clear_screen(framebuffer, 0x0078D7);
    draw_text(framebuffer, 500, 50, "TEST 2: BPB test", 0xFFFFFF);

    fat32_bpb_t bpb = fat32_read_bpb();

    draw_text(framebuffer, 500, 70, "BPB read OK!", 0xFFFFFF);
    sleep(1000);
}


void test_read_4()
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



void test_read_5() {
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




void test_read_6()
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

void test_read_7()
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

    explorer_loop();
}

//write tests

void test_write(void)
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

void test_write2(void)
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

void test_find_free_cluster(void)
{
    uint32_t cluster = 0;

    if (g_bpb.bytes_per_sector != 512) {
        draw_text(framebuffer, 0, 160,"BPB BYTES ERROR", 0xFF0000);

        sleep(3000);
        return;
    }

    if (g_bpb.sectors_per_cluster == 0 ||
        g_bpb.fat_size_32 == 0 ||
        g_bpb.num_fats == 0) {
        draw_text(framebuffer, 0, 160,
                  "BPB VALUES ERROR", 0xFF0000);
                  sleep(3000);
        return;
    }

    if (fat32_find_free_cluster(&g_bpb, &cluster) != 0) {
        draw_text(framebuffer, 0, 160,
                  "CLUSTER SEARCH FAILED", 0xFF0000);
                  sleep(3000);
        return;
    }

    draw_text(framebuffer, 0, 160,"FREE CLUSTER FOUND!", 0x000000);
    sleep(3000);
    return;

}

    int test_fat32_data(void)
{
    uint32_t cluster;
    uint8_t write_buf[512] = {0};
    uint8_t read_buf[512] = {0};

    const char test_data[] = "Hello from D.eSystem!";
    sleep(500);

    if (fat32_find_free_cluster(&g_bpb, &cluster) != 0) {
        draw_text(framebuffer, 0, 160, "DATA TEST: NO FREE CLUSTER", 0xFF0000);
        sleep(3000);
        return -1;
    }
    if (fat32_set_fat_entry(
            &g_bpb, cluster, 0x0FFFFFFF) != 0) {
        draw_text(framebuffer, 0, 160, "DATA TEST: FAT WRITE FAILED", 0xFF0000);
        sleep(3000);
        return -2;
    }
    memcpy(write_buf, test_data, sizeof(test_data));

    uint32_t lba = fat32_cluster_to_lba(cluster, g_bpb);

    if (write_sector(lba, write_buf) != 0) {
        draw_text(framebuffer, 0, 160, "DATA TEST: WRITE FAILED", 0xFF0000);
        return -3;
        sleep(3000);
    }
    if (read_sector(lba, read_buf) != 0) {
        draw_text(framebuffer, 0, 160, "DATA TEST: READ FAILED", 0xFF0000);
        sleep(3000);
        return -4;
    }

    if (memcmp(write_buf, read_buf, sizeof(write_buf)) != 0) {
        draw_text(framebuffer, 0, 160, "DATA TEST: VERIFY FAILED", 0xFF0000);
        sleep(3000);
        return -5;
    }

    draw_text(framebuffer, 0, 160, "FAT32 DATA WRITE/READ OK!", 0x00FF00);
    sleep(3000);

    return 0;
}


void test_write_3(void)
{
    const char filename[12] = "TE03    TXT";
    const uint8_t content[] = "Hello from D.eSystem!";

    int result = fat32_create_file(
        g_bpb.root_cluster,
        filename,
        &g_bpb,
        content,
        sizeof(content) - 1
    );

    if (result == 0) {
        draw_text(framebuffer, 0, 220,
                  "TE03.TXT CREATED OK!", 0x00FF00);
                  sleep(3000);
                  return;
    } else {
        draw_text(framebuffer, 0, 220,
                  "FILE CREATE FAILED!", 0xFF0000);
                  sleep(3000);
                  return;
    }
}




//explorer

void file_viewer_explorer() {

    clear_screen(framebuffer, 0x808080);
    print_main_ui();
    explorer_ui(framebuffer);
    explorer_dir();
    explorer_loop();
    


}


static void explorer_open_file(fat32_dir_entry_t *entry) {
    clear_screen(framebuffer, 0x808080);
    print_main_ui();
    explorer_ui(framebuffer);

    g_bpb = fat32_read_bpb();

    fat32_file_t file;

    //char fat_name[12];

    //if (fat32_make_83_name(entry->name, fat_name) != 0) {
    //    draw_text(
    //        framebuffer,
    //        50,
    //        100,
    //        "Invalid 8.3 filename!",
    //        0xFF0000
    //    );
    //    sleep(2000);
    //    return;
    //}

    if (!fat32_find_file_in_directory(
        explorer_current_cluster,
        entry->name,
        &g_bpb,
        &file)) {

        draw_text(framebuffer,50,100,"File not found",0xFF0000);
        return;
    }

    uint8_t buffer[4096];

    if (file.size >= sizeof(buffer)) {
        draw_text(framebuffer, 50, 100, "File too large for viewer!", 0xFF0000);
        sleep(2000);
        return;
    }

    if (fat32_read_file(&file, &g_bpb, buffer) != 0) {
        draw_text(framebuffer, 50, 100, "File READ FAILED!", 0xFF0000);
        sleep(2000);
        return;
    }

    buffer[file.size] = '\0';

    draw_text(
        framebuffer,
        50,
        140,
        (char *)buffer,
        0x000000
    );

    while (1) {
        uint8_t key = ps2_read_key();

        if (key == 27)
            return;
    }
}

void explorer_loop() {
    uint8_t key = 0;

    while (1) {
        key = ps2_read_key();

        if (key == 27)
            return;

        if (key == KEY_UP) {
            if (explorer_selected > 0)
                explorer_selected--;

            explorer_dir();
        }

        if (key == 'n') {
            create_new_file(framebuffer);
        }

        else if (key == KEY_DOWN) {
            if (explorer_entry_count > 0 &&
                explorer_selected + 1 < explorer_entry_count)
                explorer_selected++;

            explorer_dir();
        }

        else if (key == 8) {
            if (explorer_go_back() == 0)
                explorer_dir();
        }

        else if (key == '\n' || key == '\r') {
            if (explorer_entry_count == 0 ||
                explorer_selected >= explorer_entry_count)
                continue;

            fat32_dir_entry_t *entry =
                &explorer_entries[explorer_selected];

            if (fat32_is_directory(entry)) {


                if (explorer_enter_directory(entry->start_cluster) == 0) {

                    explorer_dir();
                }

            } else {

                explorer_open_file(entry);
                explorer_dir();
            }
        }
    }
}


void file_viewer_open() {
    clear_screen(framebuffer, 0x808080);
    print_main_ui();
    draw_text(framebuffer, 0, 90, "Press 1 to start explorer!! press 2 to open debug mode!!", 0x000000);
    char key = 0;
    while(key == 0) {
        key = ps2_read_key();
    }
    if(key == '1') {
        file_viewer_explorer();
    } else if(key == '2') {
        file_viewer_start();
    } else {
        draw_text(framebuffer, 0, 100, "You pressed an other key,explorer will launch in 2 secconds!!", 0x000000);
        sleep(2000);
        file_viewer_explorer();

    }
    
}

void explorer_ui(struct limine_framebuffer *fb) {

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2) - 100;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);
    draw_rect(fb, x - 60, y - 110 , 500,50, 0xFFFF00);
    draw_text(fb, x - 50, y - 90, "Welcome in D.electronics file viewer (press [Esc] to close! Press [N] to create new file!)", 0x000000);
    int y_2 = y - 110;
    int x_2 = x - 450;
    draw_rect(fb, x_2, y_2, 170, H - y_2,0xFFFF00);
    draw_text(fb, x_2 + 10, y - 90, "Files:", 0x000000);

}


void explorer_dir() {
    clear_screen(framebuffer, 0x808080);
    print_main_ui();
    explorer_ui(framebuffer);

    g_bpb = fat32_read_bpb();

    explorer_entry_count = 0;

    int result = fat32_list_directory(
        explorer_current_cluster,
        &g_bpb,
        explorer_entries,
        32,
        &explorer_entry_count
    );

    if (result != 0) {
        draw_text(
            framebuffer,
            50,
            100,
            "Directory READ FAILED!",
            0xFF0000
        );
        return;
    }

    char display_name[13];

    for (uint32_t i = 0; i < explorer_entry_count; i++) {

        if (fat32_format_83_name(
                explorer_entries[i].name,
                display_name) != 0) {
            continue;
        }

        if (i == explorer_selected) {
            draw_text(
                framebuffer,
                35,
                200 + i * 20,
                ">",
                0x000000
            );
        }

        draw_text(
            framebuffer,
            50,
            200 + i * 20,
            display_name,
            0x000000
        );

        if (fat32_is_directory(&explorer_entries[i])) {
            draw_text(
                framebuffer,
                250,
                200 + i * 20,
                "[DIR]",
                0x0000FF
            );
        }
    }
}


#define EXPLORER_MAX_DEPTH 16

//typedef struct {
//    uint32_t cluster;
//} explorer_path_entry_t;

//static explorer_path_entry_t explorer_path[EXPLORER_MAX_DEPTH];
static int explorer_depth = 0;

//static uint32_t explorer_current_cluster = 2;
//static uint32_t explorer_selected = 0;





static int explorer_enter_directory(uint32_t cluster) {
    if (explorer_depth >= EXPLORER_MAX_DEPTH)
        return -1;

    explorer_path[explorer_depth].cluster =
        explorer_current_cluster;

    explorer_depth++;

    explorer_current_cluster = cluster;
    explorer_selected = 0;

    return 0;
}



static int explorer_go_back(void) {
    if (explorer_depth <= 0)
        return -1;

    explorer_depth--;

    explorer_current_cluster =
        explorer_path[explorer_depth].cluster;

    explorer_selected = 0;

    return 0;
}

//void create_new_file() {
//    clear_screen(framebuffer, 0x0078D7);
//    keyboard_read_string(input_filename);
//    const char filename[12] = input_filename;
    //const char filename[12] = "TE03    TXT";

//    keyboard_read_string(content_file);
//    const uint8_t content[] = content_file;
    //const uint8_t content[] = "Hello from D.eSystem!";

//    int result = fat32_create_file(g_bpb.root_cluster, filename, &g_bpb, content, sizeof(content) - 1);

//    if (result == 0) {
//        draw_text(framebuffer, 0, 220,"File was created", 0x00FF00);
//                  sleep(3000);
//                  return;
//    } else {
//        draw_text(framebuffer, 0, 220,"File was not created!", 0xFF0000);
//                  sleep(3000);
//                  return;
//    }
//}






static int convert_filename83(const char *input, char output[11])
{
    if (input == 0 || input[0] == '\0')
        return -1;

    for (int i = 0; i < 11; i++)
        output[i] = ' ';

    int name_len = 0;
    int ext_len = 0;
    int in_ext = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        // Manche Tastaturfunktionen hängen einen Zeilenumbruch an.
        if (c == '\n' || c == '\r')
            break;

        if (c == '.') {
            if (in_ext || name_len == 0)
                return -1;

            in_ext = 1;
            continue;
        }

        if (c >= 'a' && c <= 'z')
            c = c - 'a' + 'A';

        if (!((c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') ||
              c == '_' || c == '-'))
            return -1;

        if (!in_ext) {
            if (name_len >= 8)
                return -1;

            output[name_len++] = c;
        } else {
            if (ext_len >= 3)
                return -1;

            output[8 + ext_len++] = c;
        }
    }

    return name_len > 0 ? 0 : -1;
}


static uint32_t string_length(const char *str)
{
    uint32_t len = 0;

    while (str[len] != '\0') {
        len++;
    }

    return len;
}

void create_new_file(struct limine_framebuffer *fb)
{
    char input_filename[64] = {0};
    char content_file[512] = {0};
    char filename[11];

    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2) - 100;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);

    clear_screen(framebuffer, 0x808080);

    draw_rect(fb, x - 60, y - 100 , 500,50, 0xFFFF00);
    draw_text(framebuffer, x + 100, y - 80 ,"Filename (e.g. NOTE.TXT):", 0x000000);

    keyboard_read_string(input_filename, 51);

    clear_screen(framebuffer, 0x808080);

    if (convert_filename83(input_filename, filename) != 0) {
        draw_rect(fb, x - 60, y - 100 , 500,50, 0xFFFF00);
        
        draw_text(framebuffer, x + 100, y - 80 ,"     Invalid filename!   ", 0x000000);
        sleep(3000);
        return;
    }

    draw_rect(fb, x - 60, y - 100 , 500,50, 0xFFFF00);
    draw_text(framebuffer, x + 100, y - 80, "   Enter file content:   ", 0x000000);

    keyboard_read_string(content_file, 512);

    g_bpb = fat32_read_bpb();

    int result = fat32_create_file( g_bpb.root_cluster, filename, &g_bpb, (const uint8_t *)content_file, string_length(content_file));
    clear_screen(framebuffer, 0x808080);
    draw_rect(fb, x - 60, y - 100 , 500,50, 0xFFFF00);

    if (result == 0) {
        draw_text(framebuffer, x + 100, y - 80, " File was created! [Esc] ", 0x000000);
                  //sleep(3000);
                  return;
    } else {
        draw_text(framebuffer, x + 100, y - 80, "File was not created! [Esc]", 0x000000);
                  
                  return;
    }

    
}
