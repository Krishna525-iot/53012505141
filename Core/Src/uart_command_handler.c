#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "command_manager.h"
#include "screen2.h"
#include "uart_command_handler.h"
#include "inactivity_manager.h"
#include "screen1.h"
#include "globals.h"

char temp_buff[10];
char intensity_str[4];

void process_uart_command(const char *command) {
    strncpy(temp_buff, command, sizeof(temp_buff) - 1);

    if (strncmp(command, "@RST#", 5) == 0)
    {
        focus_state     = 0;
        depth_state     = 0;
        lamp_state      = 1;
        endo_state      = 0;
        color_state     = 1;
        intensity_value = 1;
        gesture_enabled = 0;
        lcd_puts(0, 10, (int8_t*)"OFF");
        send_focus_command(0);
        HAL_Delay(50);
        init_screen1(0);
        update_screen1_with_current_values();
        lcd_puts(1, 7,  (int8_t*)"01");
        lcd_puts(1, 17, (int8_t*)"ON ");
        lcd_puts(2, 17, (int8_t*)"OFF");
        lcd_puts(3, 17, (int8_t*)"OFF");
        lcd_puts(2, 7,  (int8_t*)"NW");
        lcd_puts(3, 7,  (int8_t*)"OFF");
        send_intensity_command(1);
        send_endo_command(0);
        send_depth_command(0);
        send_color_command(0);
        send_lamp_command(1);
        return;
    }

    if (strncmp(command, "@RSF#", 5) == 0)
    {
        focus_state = 0;
        lcd_puts(0, 10, (int8_t*)"OFF");
        send_focus_command(0);
        HAL_Delay(50);
        init_screen1(0);
        update_screen1_with_current_values();
        if (depth_state == 1)
        {
            depth_state     = 0;
            intensity_value = 10;
            color_state     = 1;
            lamp_state      = 1;
            endo_state      = 0;
            gesture_enabled = 0;
            lcd_puts(1, 7,  (int8_t*)"10");
            lcd_puts(3, 17, (int8_t*)"OFF");
            lcd_puts(2, 7,  (int8_t*)"NW");
            lcd_puts(1, 17, (int8_t*)"ON ");
            lcd_puts(2, 17, (int8_t*)"OFF");
            lcd_puts(3, 7,  (int8_t*)"OFF");
            send_intensity_command(10);
            send_depth_command(0);
            send_color_command(0);
            send_endo_command(0);
            send_lamp_command(1);
        }
        return;
    }

    if (focus_state == 1) {
        if (command[0] == '@' &&
           (command[1] == 'I' || command[1] == 'C' ||
            command[1] == 'E' || command[1] == 'D' || command[1] == 'L')) {
            return;
        }
    }

    if (command[0] == '@') {
        InactivityManager_NotifyActivity();
    }

    /* ================================================================
     *  INTENSITY  @I01# – @I09# / @I0:#
     * ================================================================ */
    if (strncmp(command, "@I", 2) == 0) {
        uint8_t intensity = atoi(&command[2]);
        if (intensity == 0) {
            intensity_value = 10;
            snprintf(intensity_str, sizeof(intensity_str), "%02d", 10);
            lcd_puts(1, 7, (int8_t*)intensity_str);
            send_intensity_command(10);
        } else if (intensity > 0 && intensity <= 9) {
            intensity_value = intensity;
            snprintf(intensity_str, sizeof(intensity_str), "%02d", intensity);
            lcd_puts(1, 7, (int8_t*)intensity_str);
            send_intensity_command(intensity);
        } else {
            printf("Error: Invalid intensity value.\n");
        }
        move_cursor_to_position(0);

    /* ================================================================
     *  COLOR  @C-5# / @C05# / @C+5#
     * ================================================================ */
    } else if (strncmp(command, "@C", 2) == 0) {
        int color = atoi(&command[2]);
        const char *color_modes[] = { "WW", "NW", "CW" };
        if (command[2] == '-' || command[2] == '0' || command[2] == '+') {
            if (command[2] == '-') {
                color_state = -5;
                lcd_puts(2, 7, (int8_t*)color_modes[0]);
                color = -5;
            } else if (command[2] == '0') {
                color_state = 0;
                lcd_puts(2, 7, (int8_t*)color_modes[1]);
                color = 0;
            } else if (command[2] == '+') {
                color_state = +5;
                lcd_puts(2, 7, (int8_t*)color_modes[2]);
                color = +5;
            }
            send_color_command(color);
        } else {
            printf("Error: Invalid color value.\n");
        }
        move_cursor_to_position(1);

    /* ================================================================
     *  CAMERA  @M_0# / @M_1# / @M_2# / @M_3#
     * ================================================================ */
    } else if (strncmp(command, "@M_", 3) == 0) {
        int cam = 0;
        if (command[3] == '0' || command[3] == '1' ||
            command[3] == '2' || command[3] == '3') {
            if (command[3] == '0') {
                camera_state = 0;
                cam = 0;
                if (camera_enabled) {
                    lcd_puts(1, 10, (int8_t*)"OFF");
                    init_second_screen();
                }
            } else if (command[3] == '1') {
                camera_state = 1;
                cam = 1;
                if (camera_enabled) {
                    init_second_screen();
                    lcd_puts(1, 10, (int8_t*)"ON ");
                }
            } else if (command[3] == '2') {
                camera_state = 1;
                cam = 2;
            } else if (command[3] == '3') {
                camera_state = 1;
                cam = 3;
            }
            send_cam_command(cam);
        } else {
            init_screen1(0);
        }

    /* ================================================================
     *  LAMP  @L_0# / @L_1#
     * ================================================================ */
    } else if (strncmp(command, "@L_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        lamp_state = state;
        lcd_puts(1, 17, lamp_state ? (int8_t*)"ON " : (int8_t*)"OFF");
        send_lamp_command(state);
        move_cursor_to_position(3);

    /* ================================================================
     *  ENDO  @E_0# / @E_1#
     * ================================================================ */
    } else if (strncmp(command, "@E_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        endo_state = state;
        lcd_puts(2, 17, endo_state ? (int8_t*)"ON " : (int8_t*)"OFF");
        send_endo_command(state);
        move_cursor_to_position(4);

    /* ================================================================
     *  DEPTH  @D_0# / @D_1#
     * ================================================================ */
    } else if (strncmp(command, "@D_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        depth_state = state;
        lcd_puts(3, 17, depth_state ? (int8_t*)"ON " : (int8_t*)"OFF");
        send_depth_command(state);
        move_cursor_to_position(5);

    /* ================================================================
     *  GREEN LED — on/off   @G_0# / @G_1#
     * ================================================================ */
    } else if (strncmp(command, "@G_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        send_Green_command(state);

    /* ================================================================
     *  GREEN LED — intensity  @G01# – @G09# / @G0:#
     *  Third byte is '0', fourth byte is digit '1'–'9' or ':'
     * ================================================================ */
    } else if (strncmp(command, "@G0", 3) == 0) {
        uint8_t level;
        if (command[3] == ':') {
            level = 10;
        } else if (command[3] >= '1' && command[3] <= '9') {
            level = command[3] - '0';
        } else {
            printf("Error: Invalid green intensity value.\n");
            return;
        }
        send_Green_intensity_command(level);

    /* ================================================================
     *  RED LED — on/off   @R_0# / @R_1#
     * ================================================================ */
    } else if (strncmp(command, "@R_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        send_Red_command(state);

    /* ================================================================
     *  RED LED — intensity  @R01# – @R09# / @R0:#
     *  Third byte is '0', fourth byte is digit '1'–'9' or ':'
     * ================================================================ */
    } else if (strncmp(command, "@R0", 3) == 0) {
        uint8_t level;
        if (command[3] == ':') {
            level = 10;
        } else if (command[3] >= '1' && command[3] <= '9') {
            level = command[3] - '0';
        } else {
            printf("Error: Invalid red intensity value.\n");
            return;
        }
        send_Red_intensity_command(level);

    /* ================================================================
     *  FOCUS  @F_0# / @F_1#
     * ================================================================ */
    } else if (strncmp(command, "@F_", 3) == 0) {
        uint8_t state = (command[3] == '1') ? 1 : 0;
        focus_state = state;
        if (focus_state) {
            init_second_screen();
            load_initial_focus();
            lcd_puts(0, 10, (int8_t*)"ON ");
        } else {
            lcd_puts(0, 10, (int8_t*)"OFF");
            init_screen1(0);
        }
        send_focus_command(0);

    /* ================================================================
     *  FOCUS OPTIONS  @F01# / @F02# / @F03#
     * ================================================================ */
    } else if (strncmp(command, "@F01", 4) == 0) {
        reset_focus_options();
        option_states[0] = command[3];
        send_focus_command(0);
        load_focus_options();

    } else if (strncmp(command, "@F02", 4) == 0) {
        reset_focus_options();
        option_states[1] = command[3];
        send_focus_command(1);
        load_focus_options();

    } else if (strncmp(command, "@F03", 4) == 0) {
        reset_focus_options();
        option_states[2] = command[3];
        send_focus_command(2);
        load_focus_options();

    /* ================================================================
     *  RELAY PA  @PA1# / @PA0#
     * ================================================================ */
    } else if (strncmp(command, "@PA1#", 5) == 0) {
        send_PA_command(1);

    } else if (strncmp(command, "@PA0#", 5) == 0) {
        send_PA_command(0);

    } else {
        printf("Error: Unrecognized command: %s\n", command);
    }
}
