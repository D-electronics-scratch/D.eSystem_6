#include "video.h"
#include "d_e_shell.h"
#include "keyboard.h"
#include "keyboard_input.h"
#include "calculator_input.h"
#include "util.h"
#include "calculator.h"
#include "clock.h"
#include "main.h"
#include "D_e_System_version.h"
#include "desktop_ui.h"
#include "string.h"
#include <string.h>
#include "main_ui.h"

char line_1[51];
char line_2[51];
char line_3[51];
char line_4[51];
char line_5[51];
char line_6[51];
char line_7[51];
char line_8[51];
char line_9[51];
char line_10[51];
char awnswer[51];

char cmd[51];
int shell_open;
extern int wallpaper_num;
extern int ui_mode;
extern int setup_yes_no;

void open_d_e_shell();
void d_eShell_run();
void d_e_shell_ui();
void shell_input();
void if_else_shell();
void d_e_shell_loop();
void change_lines();
void render_lines();

void open_d_e_shell() {
    d_e_shell();
}

void d_e_shell_loop() {
    while(1) {
        if(shell_open == 1) {
            shell_input();

        } else if(shell_open == 2) {
            break;
        }
    } 
  
}

void shell_input() {
    keyboard_read_string(cmd, 51);
    if_else_shell();

}

void d_e_shell_ui() {

    clear_screen(framebuffer,  0x000000);
    print_main_ui();
    
    draw_text(framebuffer, 0, 50, "Welcome in D.eShell 1.4!!", 0xFFFFFF);
    
    draw_text(framebuffer, 0, 60, "Type cl to close D.eShell!!", 0xFFFFFF);
    draw_text(framebuffer, 0, 70, "Type help to show base commands!!", 0xFFFFFF);

}

void d_e_shell() {
    shell_open = 1;
    d_e_shell_ui();
    d_e_shell_loop();
    

    


    

}

void if_else_shell() {
    if (strcmp(cmd, "cl") == 0) {
        
        shell_open = 2;
        clear_screen(framebuffer,  0x000088);
        draw_text(framebuffer, 430, 50, "--------------------------------------------------------", 0xFF0000);
        draw_text(framebuffer, 430, 60, "|              Press anything to continue              |", 0xFF0000);
        draw_text(framebuffer, 430, 70, "--------------------------------------------------------", 0xFF0000);
        //ui_desktop();
        return;
        

    //} //else {
        //d_e_shell();

    } else if (strcmp(cmd, "help") == 0) {
        d_e_shell_ui();
        strcpy(awnswer, "You can type: calculator7open, clock7open, de7version, shell7version, gui7version, wallpaper7cube, wallpaper7base");
        change_lines();
        strcpy(line_1, awnswer);
        render_lines();

    } else if(strcmp(cmd, "calculator7open") == 0) {
        shell_open = 2;
        
        app_calculator_run();
        return;

    } else if(strcmp(cmd, "clock7open") == 0) {
        shell_open = 2;
        run_clock_d_elec();
    } else if(strcmp(cmd, "pc7shutdown") == 0) {
        return;

    } else if(strcmp(cmd, "de7version") == 0) {
        D_e_Sys_v_run();

    } else if(strcmp(cmd, "shell7version") == 0) {
        d_e_shell_ui();
        strcpy(awnswer, "D.eShell 1.4");
        change_lines();
        strcpy(line_1, awnswer);
        render_lines();


    } else if (strcmp(cmd, "gui7version") == 0) {
        d_e_shell_ui();
        strcpy(awnswer, "GUI version: D.UI 12");
        change_lines();
        strcpy(line_1, awnswer);
        render_lines();


        

    } else if(strcmp(cmd, "wallpaper7cube") == 0) {
        d_e_shell_ui();
        if(ui_mode > 1) {
            wallpaper_num = 0;

        } else {
            wallpaper_num = 2;
        }
        
        

        if(wallpaper_num == 2) {
            strcpy(awnswer, "Command was sucessfully fineshed!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();             

        } else if(wallpaper_num == 0){
            strcpy(awnswer, "ERROR!! wallpapers are not suported with ASCII UI!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();
            

        } else {
            strcpy(awnswer, "ERROR!! The command had a unknown error!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();

        }

        


        
    } else if(strcmp(cmd, "wallpaper7base") == 0) {
        d_e_shell_ui();
        if(ui_mode > 1) {
            wallpaper_num = 0;

        }else {
            wallpaper_num = 1;

        }
        


        if(wallpaper_num == 1) {
            strcpy(awnswer, "Command was sucessfully fineshed!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();             

        } else if(wallpaper_num == 0){
            strcpy(awnswer, "ERROR!! wallpapers are not suported with ASCII UI!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();
            

        } else {
            strcpy(awnswer, "ERROR!! The command had a unknown error!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();

        }

    } else if(strcmp(cmd, "setup7run") == 0) {
        d_e_shell_ui();
        setup_yes_no = 1;
        strcpy(awnswer, "The setup will start if you close D.eShell, type cl to close D.eShell.");
        change_lines();
        strcpy(line_1, awnswer);
        render_lines();

        
    } else if(strcmp(cmd, "use7gui") == 0) {
        d_e_shell_ui();
        ui_mode = 1;
if (ui_mode == 1) {
            strcpy(awnswer, "Command was sucesfully finished!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();


        } else {
            strcpy(awnswer, "ERROR!! The command had a unamed error!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();

        }

    } else if(strcmp(cmd, "use7ascii7ui") == 0) {
        d_e_shell_ui();
        ui_mode = 2;


        if (ui_mode == 2) {

            strcpy(awnswer, "Command was sucesfully finished!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();


        } else {
            strcpy(awnswer, "ERROR!! The command had a unamed error!!");
            change_lines();
            strcpy(line_1, awnswer);
            render_lines();

        }


    } else if(strcmp(cmd, "open7file7viewer7test")) {
        
    } else {
        strcpy(awnswer, "Command not found!!");
        change_lines();
        strcpy(line_1, awnswer);
        render_lines();
        
    }
    
} 






void change_lines() {


    strcpy(line_10, line_9);
    strcpy(line_9, line_8);
    strcpy(line_8, line_7);
    strcpy(line_7, line_6);
    strcpy(line_6, line_5);
    strcpy(line_5, line_4);
    strcpy(line_4, line_3);
    strcpy(line_3, line_2);
    strcpy(line_2, line_1);

}

void render_lines() {
    draw_text(framebuffer, 0, 90, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 100, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 110, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 120, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 130, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 140, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 150, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 160, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 170, "                                                                                                                             ", 0x000000);
    draw_text(framebuffer, 0, 180, "                                                                                                                             ", 0x000000);

    draw_text(framebuffer, 0, 90, line_1, 0xFFFFFF);
    draw_text(framebuffer, 0, 100, line_2, 0xFFFFFF);
    draw_text(framebuffer, 0, 110, line_3, 0xFFFFFF);
    draw_text(framebuffer, 0, 120, line_4, 0xFFFFFF);
    draw_text(framebuffer, 0, 130, line_5, 0xFFFFFF);
    draw_text(framebuffer, 0, 140, line_6, 0xFFFFFF);
    draw_text(framebuffer, 0, 150, line_7, 0xFFFFFF);
    draw_text(framebuffer, 0, 160, line_8, 0xFFFFFF);
    draw_text(framebuffer, 0, 170, line_9, 0xFFFFFF);
    draw_text(framebuffer, 0, 180, line_10, 0xFFFFFF);

}