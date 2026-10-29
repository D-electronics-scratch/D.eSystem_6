#include <limine.h>
#include "keyboard.h"
//#include "limine.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "video.h"
#include "calculator.h"
#include "main.h"
#include "D_e_System_version.h"
#include <string.h>
#include "fs.h"
#include "xhci.h"
#include "usb_core.h"
#include "paging.h"
#include "introduction.h"
#include "d_e_shell.h"
#include "file_viewer.h"
#include "fat32_fs.h"
#include "bpb.h"
#include "sleep.h"
#include "ui_desktop.h"
#include "main_ui.h"
#include "desktop_ui.h"
#include "thundercalculator.h"
#include "debug_fw.h"


struct limine_framebuffer *framebuffer;
struct superblock superblock;
struct inode inodes [128];
uint32_t *fb;
int setup_yes_no;
int ui_mode;
int ui_mode_2;











// Set the base revision to 6, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.

//__attribute__((used, section(".limine_requests")))
//static volatile struct limine_disk_request disk_request = {
//    .id = LIMINE_DISK_REQUEST,
//    .revision = 0
//};


__attribute__((used, section(".limine_requests")))
volatile struct limine_firmware_type_request firmware_type_request = {
    .id = LIMINE_FIRMWARE_TYPE_REQUEST_ID,
    .revision = 0
};


__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 0
};










void ASCII_desktop_home(struct limine_framebuffer *framebuffer);
void shutdown_computer();
void app_calculator_run();
void kernel_loop();
void ASCII_desktop_ui();
void ui_panic();
void boot_mode_unsuported();
void boot_logo();








void kmain(void) {

    

    //if (disk_request.response == NULL || disk_request.response->disk_count < 1) {
    //    hcf();
   //}

    
    
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    framebuffer = framebuffer_request.response->framebuffers[0];
    //volatile uint32_t *fb = framebuffer->address;
    fb = (uint32_t*)framebuffer->address; 

    //uint64_t firmware_check = get_boot_mode();

    //if (firmware_check == BOOT_MODE_UEFI) {

    //} else {
    //    boot_mode_unsuported();

    //}

boot_logo(framebuffer);

if (module_request.response == NULL ||
module_request.response->module_count < 1) {
draw_text(framebuffer, 0, 80,
"No Limine module found!", 0xFF0000);
hcf();
}

struct limine_file *mod =
module_request.response->modules[0];

if (mod == NULL || mod->address == NULL || mod->size < 512) {
draw_text(framebuffer, 0, 100,
"Invalid FAT32 module!", 0xFF0000);
hcf();
}

fs_image = mod->address;
fs_image_size = mod->size;


g_bpb = fat32_read_bpb();

if (g_bpb.bytes_per_sector != 512) {
draw_text(framebuffer, 0, 120,
"FAT32 BPB INIT FAILED!", 0xFF0000);
hcf();
}

sleep(3000);


    if (firmware_type_request.response->firmware_type == LIMINE_FIRMWARE_TYPE_EFI64) {
        setup_yes_no = 1;
        kernel_loop();

        // UEFI
    } else if (firmware_type_request.response->firmware_type == LIMINE_FIRMWARE_TYPE_X86BIOS) {

        draw_text(framebuffer, 0, 40, "Runing in legacy Mode", 0xFF0000);
        draw_text(framebuffer, 0, 50, "You are runing D.eSystem with Legacy BIOS!! All the new features which require the UEFI firmware will be disabled!!", 0xFFFFFF);
        draw_text(framebuffer, 0, 60, "To use UEFI turn CSM/Legacy_mode in your BIOS settings off.", 0xFFFFFF);
        sleep(5000);

        
        setup_yes_no = 1;
        kernel_loop();
        //ui_mode = 2;
        //boot_mode_unsuported();
    // Legacy BIOS
    }


    //setup_yes_no = 1;
    //Skernel_loop();


}


void kernel_loop() {

    while (1) {
        
        for (size_t y = 0; y < framebuffer->height; y++) {
            for (size_t x = 0; x < framebuffer->width; x++) {
                fb[y * (framebuffer->pitch / 4) + x] = 0x202020;
            }
        }

        if (setup_yes_no == 1) {
            run_setup_start();
        } else {
            if (ui_mode == 1) {
                ui_mode_2 = ui_mode;
                ui_desktop();
            } else if (ui_mode == 2) {
                ASCII_desktop_ui();
            } else {
                ui_panic();

            }

            print_main_ui();

            char key = 0;
            while (key == 0) {
                key = ps2_read_key();
            }

            if (key == '4') {
                D_e_Sys_v_run();
            } else if (key == '2') {
                app_calculator_run();
            } else if (key == '3') {
                run_clock_d_elec();
            } else if (key == '5') {
                open_d_e_shell();
            } else if (key == '6') {
                file_viewer_open();
            } else if (key == '7') {
                open_debug();
            }
        }
    
}

}

 

void ASCII_desktop_home(struct limine_framebuffer *framebuffer) {


    

            

    //ui_desktop();
    //xhci_full_init();
    
    
    

    char key = 0;

    //main menu input
    while(key == 0) {
        key = ps2_read_key();
    }

    if (key == '1') {
        draw_text(framebuffer, 500, 130, "Shutdown selected", 0xFFFFFF);
        shutdown_computer();

    } else if (key == '2') {
        app_calculator_run();
        
    } else if (key == '3') {

        run_clock_d_elec();

    } else if (key == '4') {
        D_e_Sys_v_run();
    } else if (key == '5') {
        int shell_open = 0;
        shell_open = 1;
        d_e_shell_ui();


        //sd_e_shell();

    } else if (key == '6') {
        run_thunder_calc();
    }
    
}

//void shutdown_computer() {
//    acpi_shutdown();

//}

//fs mount

int fs_mount(void* image) {

    if(!image) return -1;
    uint8_t *img = (uint8_t *)image;
    
    //read the superblock
    memcpy(&superblock, img, sizeof(struct superblock));

    if (superblock.magic != 0xDE5006) {
        return -2;
    }


    //read inode table
    uint8_t * inode_table_addr = img + superblock.inode_table_start * FS_BLOCK_SIZE;
    memcpy(inodes, inode_table_addr, sizeof(inodes));

    return 0;
}


void ASCII_desktop_ui(){
    //ASCII interface fr homescreen
    clear_screen(framebuffer, 0x000000);
    draw_text(framebuffer, 500,100, "-----------------------------------------", 0xFFFFFF);
    draw_text(framebuffer, 500, 110, "| hold powerswitch to shutdown         |", 0xFFFFFF);
    draw_text(framebuffer, 500, 120, "| 2: calculator                        |", 0xFFFFFF);
    draw_text(framebuffer, 500, 130, "| 3: clock                             |", 0xFFFFFF);
    draw_text(framebuffer, 500, 140, "| 4: D.eSystem version                 |", 0xFFFFFF);
    draw_text(framebuffer, 500, 150, "| 5: D.eShell                          |", 0xFFFFFF);
    draw_text(framebuffer, 500, 160, "----------------------------------------", 0xFFFFFF);

}


void ui_panic() {
    clear_screen(framebuffer, 0xFF0000);
    draw_text(framebuffer, 500, 150, "An UI errror hapended,press 1 to start text interface!!", 0xFFFFFF);
    char key = 0;
    while (key == 0) {
        key = ps2_read_key();
    }
    if(key == '1') {
        ui_mode = 2;
        ASCII_desktop_ui();
    }
}


void boot_mode_unsuported() {
    clear_screen(framebuffer, 0x000000);
    draw_text(framebuffer, 500, 50, "Legacy BIOS not supported!! Please use UEFI firmware!!", 0xFFFFFF);
}



void boot_logo(struct limine_framebuffer *fb) {
    uint32_t W = fb->width;
    uint32_t H = fb->height;

    uint32_t icon_w = 100;
    uint32_t icon_h = 100;
    uint32_t x = (W / 2) - (icon_w / 2) - 500;
    uint32_t y = (H * 1) / 4 - (icon_h / 2);

    draw_rect(fb, x + 500, y, 20, 200, 0x1A7FFF);
    draw_rect(fb, x + 520, y, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 520, y + 180, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 540, y + 20, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 540, y + 160, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 560, y + 40, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 560, y + 140, 20, 20, 0x1A7FFF);
    draw_rect(fb, x + 560, y + 60, 20, 100, 0x1A7FFF);

    draw_rect(fb, x + 600, y + 180, 20, 20, 0x1A7FFF);

    
}










