#include "screen2.h"
#include "screen1.h"
#include "lcd.h"
#include "globals.h"
#include "input_manager.h"
#include "main.h"

// Cursor positions
#define FOCUS_INDEX     0
#define CAMERA_INDEX    1

// Zoom indices commented out — not used anymore
// #define ZOOM_IN_INDEX   2
// #define ZOOM_OUT_INDEX  3

#define SMALL_INDEX     1
#define MEDIUM_INDEX    2
#define WIDE_INDEX      3

#define TOTAL_FOCUS_POSITIONS 4

static uint8_t current_position = 0;
uint8_t camera_state = 0;
extern uint8_t camera_enabled;  // Must be defined in globals.c

// Get how many items to show
// Zoom in/out removed — menu only shows FOCUS and CAMERA (max 2 items when camera enabled)
uint8_t get_total_menu_items(void) {
    if (focus_state) return TOTAL_FOCUS_POSITIONS;
    if (!camera_enabled) return 1;   // Only FOCUS
    return 2;                         // FOCUS + CAMERA only (zoom removed)
    // Previously: return camera_state ? 4 : 2; — zoom items removed
}

void init_second_screen(void) {
    LCD_Clear();

    lcd_puts(0, 1, (int8_t*)"FOCUS  :");
    lcd_puts(0, 10, focus_state ? (int8_t*)"ON " : (int8_t*)"OFF");

    if (camera_enabled) {
        lcd_puts(1, 1, (int8_t*)"CAMERA :");
        lcd_puts(1, 10, camera_state ? (int8_t*)"ON " : (int8_t*)"OFF");
    } else {
        lcd_puts(1, 0, (int8_t*)"                ");  // Clear line
    }

    // Lines 2 and 3 always cleared — zoom rows removed
    lcd_puts(2, 0, (int8_t*)"                ");
    lcd_puts(3, 0, (int8_t*)"                ");

    // Zoom in/out display removed
    // if (camera_enabled && camera_state && !focus_state) {
    //     lcd_puts(2, 1, (int8_t*)" ZOOMIN  :");
    //     lcd_puts(2, 12, (int8_t*)"++");
    //     lcd_puts(3, 1, (int8_t*)" ZOOMOUT :");
    //     lcd_puts(3, 12, (int8_t*)"--");
    // }

    if (focus_state) {
        load_focus_options();
    }

    current_position = FOCUS_INDEX;
    cursor_row = current_position;
    cursor_col = 0;
    update_cursor();
}

void handle_second_screen_input(InputAction action) {
    uint8_t max_pos = get_total_menu_items();  // Now max 2 when camera enabled (no zoom)

    switch (action) {
        case INPUT_MOVE_DOWN:
            if (!focus_state && current_position == (max_pos - 1)) {
                // At last item (CAMERA) — wrap around to screen1
                screen_state = 0;
                LCD_Clear();
                init_screen1(0);
                update_screen1_with_current_values();
                return;
            }
            current_position = (current_position + 1) % max_pos;
            break;

        case INPUT_MOVE_UP:
            if (!focus_state && current_position == 0) {
                // At first item (FOCUS) — wrap around to screen1
                screen_state = 0;
                LCD_Clear();
                init_screen1(5);
                update_screen1_with_current_values();
                return;
            }
            current_position = (current_position == 0) ? max_pos - 1 : current_position - 1;
            break;

        case INPUT_SELECT:
            if (current_position == FOCUS_INDEX) {
                if (!focus_state) {
                    toggle_focus_state();
                    current_position = SMALL_INDEX;
                }
            } else if (focus_state && current_position >= SMALL_INDEX && current_position <= WIDE_INDEX) {
                toggle_focus_option(current_position - 1);
            } else if (!focus_state && camera_enabled) {
                if (current_position == CAMERA_INDEX) {
                    toggle_camera_state();
                    init_second_screen();
                }
                // Zoom in/out select removed
                // else if (camera_state) {
                //     if (current_position == ZOOM_IN_INDEX) {
                //         send_command("@M_2#");
                //         lcd_puts(2, 12, (int8_t*)">>");
                //         HAL_Delay(300);
                //         lcd_puts(2, 12, (int8_t*)"++");
                //     }
                //     else if (current_position == ZOOM_OUT_INDEX) {
                //         send_command("@M_3#");
                //         lcd_puts(3, 12, (int8_t*)"<<");
                //         HAL_Delay(300);
                //         lcd_puts(3, 12, (int8_t*)"--");
                //     }
                // }
            }
            break;

        case INPUT_BACK:
            if (current_position == FOCUS_INDEX) {
                focus_state = 0;
                send_command("@F_0#");
                init_second_screen();
            } else if (focus_state && current_position >= SMALL_INDEX && current_position <= WIDE_INDEX) {
                toggle_focus_option(current_position - 1);
            } else if (!focus_state && camera_enabled) {
                if (current_position == CAMERA_INDEX) {
                    camera_state = 0;
                    send_command("@M_0#");
                    init_second_screen();
                }
                // Zoom out back action removed
                // else if (camera_state && current_position == ZOOM_OUT_INDEX) {
                //     send_command("@M_3#");
                //     lcd_puts(3, 12, (int8_t*)"<<");
                //     HAL_Delay(300);
                //     lcd_puts(3, 12, (int8_t*)"--");
                // }
            }
            break;

        default:
            return;
    }

    cursor_row = current_position;
    cursor_col = 0;
    update_cursor();
}

void toggle_focus_state(void) {
    focus_state = !focus_state;
    lcd_puts(0, 10, focus_state ? (int8_t*)"ON " : (int8_t*)"OFF");
    send_command(focus_state ? "@F_1#" : "@F_0#");

    if (focus_state) {
        lcd_puts(1, 0, (int8_t*)"                ");
        lcd_puts(2, 0, (int8_t*)"                ");
        lcd_puts(3, 0, (int8_t*)"                ");

        load_initial_focus();
        current_position = SMALL_INDEX;
        option_states[0] = 1;
    } else {
        current_position = FOCUS_INDEX;
        init_second_screen();
    }
}

void toggle_camera_state(void) {
    camera_state = !camera_state;
    lcd_puts(1, 10, camera_state ? (int8_t*)"ON " : (int8_t*)"OFF");
    send_command(camera_state ? "@M_1#" : "@M_0#");
    // Zoom rows not shown even when camera is ON — lines 2 and 3 stay blank
}

void load_focus_options(void) {
    const int8_t* labels[3] = { "SMALL  :", "MEDIUM :", "WIDE   :" };
    for (uint8_t i = 0; i < 3; i++) {
        lcd_puts(i + 1, 1, (int8_t*) labels[i]);
        lcd_puts(i + 1, 10, option_states[i] ? (int8_t*)"ON " : (int8_t*)"OFF");
    }
}

void load_initial_focus(void) {
    lcd_puts(1, 1, (int8_t*) "SMALL  :");
    lcd_puts(2, 1, (int8_t*) "MEDIUM :");
    lcd_puts(3, 1, (int8_t*) "WIDE   :");

    lcd_puts(1, 10, (int8_t*) "ON ");
    lcd_puts(2, 10, (int8_t*) "OFF");
    lcd_puts(3, 10, (int8_t*) "OFF");
}

void reset_focus_options(void) {
    for (uint8_t i = 0; i < 3; i++) {
        option_states[i] = 0;
    }
}

void toggle_focus_option(uint8_t option_index) {
    if (option_index < 3) {
        for (uint8_t i = 0; i < 3; i++) {
            option_states[i] = (i == option_index);
        }
        send_focus_command(option_index);
        load_focus_options();
    }
}

// Call this from other modules to enable/disable camera features
void set_camera_enabled(uint8_t state) {
    camera_enabled = state;
    init_second_screen();  // Refresh UI
}
