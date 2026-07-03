#include "screen1.h"
#include "globals.h"        // Shared global variables and states
#include "button_manager.h" // Button-related utilities
#include <stdio.h>
#include <command_manager.h>
#include "screen2.h"
#include "main.h"

uint8_t current_position = 0; // Current cursor position index

void init_screen1(uint8_t start_position) {
    current_position = start_position; // Set the starting position dynamically
    cursor_row = cursor_positions[current_position][0];
    cursor_col = cursor_positions[current_position][1];
    update_screen1_display();
    update_screen1_with_current_values();
    update_cursor();
    if (HAL_GPIO_ReadPin(cam_enable_GPIO_Port, cam_enable_Pin) == GPIO_PIN_RESET)
    {
        camera_enabled = 1;  // Enable camera if pin is grounded (logic LOW)
    }
    else
    {
        camera_enabled = 0;  // Disable camera if pin is HIGH
    }
}

// Move cursor to a specific position index (screen1 only)
// Called from uart_command_handler to sync cursor after UART commands
void move_cursor_to_position(uint8_t pos) {
    if (screen_state != 0) return;  // Only apply when screen1 is active
    if (pos >= num_positions) return; // Guard against out-of-range index
    current_position = pos;
    cursor_row = cursor_positions[current_position][0];
    cursor_col = cursor_positions[current_position][1];
    update_cursor();
}

// Handle input actions specific to Screen1
void handle_first_screen_input(InputAction action) {
    switch (action) {
    case INPUT_MOVE_DOWN:
        // Move to the next cursor position
        current_position = (current_position + 1) % num_positions;
        if (current_position == 2)
        {
            current_position = 3;
        }
        // Transition to Screen2 if moving down from the last position
        if (current_position == 0) {
            switch_to_screen2();
            return;
        }
        break;

    case INPUT_MOVE_UP:
        // Move to the previous cursor position
        current_position =
                (current_position == 0) ?
                        num_positions - 1 : current_position - 1;
        // Transition to Screen2 if moving up from the first position
        if (current_position == num_positions - 1) {
            switch_to_screen2();
            return;
        }
        break;

    case INPUT_SELECT:
        // Perform the ON/Increment action for the current position
        if (cursor_actions[current_position].on_action) {
            cursor_actions[current_position].on_action();
        }
        break;

    case INPUT_BACK:
        // Perform the OFF/Decrement action for the current position
        if (cursor_actions[current_position].off_action) {
            cursor_actions[current_position].off_action();
        }
        break;

    case INPUT_DEPTH:
        // Handle depth state toggle or uptime display
        handle_depth_action();
        break;

    case INPUT_LONG_DEPTH:
        // Handle depth state toggle or uptime display
        systemup_time();
        break;

    case INPUT_LONG_MOVE_UP:
        // Long press UP: Toggle sensor ON/OFF
        toggle_sensor_state();
        break;

//  case INPUT_LONG_MOVE_DOWN:
//      toggle_cam_state();
//      init_second_screen();
//      break;

    default:
        return; // Ignore unhandled actions
    }

    // Update cursor position or refresh the screen
    if (action == INPUT_MOVE_UP || action == INPUT_MOVE_DOWN) {
        cursor_row = cursor_positions[current_position][0];
        cursor_col = cursor_positions[current_position][1];
        update_cursor();
    } else {
        update_screen1_with_current_values();
    }
}

// Update Screen1's static display labels
void update_screen1_display(void) {
    LCD_Clear();
    lcd_puts(0, 0,  (int8_t*) "______ COGNATE _____");
    lcd_puts(1, 1,  (int8_t*) "INTEN");
    lcd_puts(2, 1,  (int8_t*) "COLOR");
    lcd_puts(3, 1,  (int8_t*) "SENSR");
    lcd_puts(1, 11, (int8_t*) "LAMP");
    lcd_puts(2, 11, (int8_t*) "ENDO");
    lcd_puts(3, 11, (int8_t*) "DEPTH");
}

// Update Screen1's dynamic values
void update_screen1_with_current_values(void) {
    // Update intensity
    char intensity_str[4];
    snprintf(intensity_str, sizeof(intensity_str), "%02d", intensity_value);
    lcd_puts(1, 7, (int8_t*) intensity_str);

    // Update color mode
    const char *color_modes[] = { "WW", "NW", "CW" };
    uint8_t mapped_color_state = (color_state + 5) / 5;

    // Ensure mapped_color_state is valid
    if (mapped_color_state < 0 || mapped_color_state > 2) {
        lcd_puts(2, 7, (int8_t*) "ERR");
    } else {
        lcd_puts(2, 7, (int8_t*) color_modes[mapped_color_state]);
    }

    // Update sensor state
    lcd_puts(3, 7, sensor_state ? (int8_t*) "ON " : (int8_t*) "OFF");

    // Update lamp state
    lcd_puts(1, 17, lamp_state ? (int8_t*) "ON " : (int8_t*) "OFF");

    // Update ENDO state
    lcd_puts(2, 17, endo_state ? (int8_t*) "ON " : (int8_t*) "OFF");

    // Update depth state
    lcd_puts(3, 17, depth_state ? (int8_t*) "ON " : (int8_t*) "OFF");
}

// Toggle sensor state (ON/OFF)
void toggle_sensor_state(void) {
    if (sensor_state) {
        sensor_state = 0;
//      APDS9960_DisableGesture(&apds);
    } else {
        sensor_state = 1;
//      APDS9960_EnableGesture(&apds);
    }
    lcd_puts(3, 7, sensor_state ? (int8_t*) "ON " : (int8_t*) "OFF");
}

// Handle depth action (toggle or display uptime)
//-------------------------------------------------------------------------------------
void handle_depth_action(void) {
    endo_state = !endo_state;
    if (endo_state) {
        turn_on_endo();
        lcd_puts(2, 17, (int8_t*) "ON ");
    } else {
        turn_off_endo();
        lcd_puts(2, 17, (int8_t*) "OFF ");
    }
}

//-------------------------------------------------------------------------------------
// Transition to Screen2
void switch_to_screen2(void) {
    screen_state = 1;
    LCD_Clear();
    init_second_screen();
}

const CursorAction cursor_actions[] = {
    { 1, 7,  increment_intensity,  decrement_intensity  },  // INTEN  — pos 0
    { 2, 7,  toggle_color_mode,    decrement_color_mode },  // COLOR  — pos 1
    { 3, 7,  enable_sensor,        disable_sensor       },  // SENSR  — pos 2
    { 1, 17, turn_on_lamp,         turn_off_lamp        },  // LAMP   — pos 3
    { 2, 17, turn_on_endo,         turn_off_endo        },  // ENDO   — pos 4
    { 3, 17, turn_on_depth,        turn_off_depth       }   // DEPTH  — pos 5
};

const uint8_t num_positions = sizeof(cursor_actions) / sizeof(cursor_actions[0]);

const uint8_t cursor_positions[][2] = {
    { 1, 0 },   // INTEN  — pos 0
    { 2, 0 },   // COLOR  — pos 1
    { 3, 0 },   // SENSR  — pos 2
    { 1, 10 },  // LAMP   — pos 3
    { 2, 10 },  // ENDO   — pos 4
    { 3, 10 }   // DEPTH  — pos 5
};

// Action implementations
void increment_intensity(void) {
    intensity_value = (intensity_value < 10) ? intensity_value + 1 : 10;
    send_intensity_command(intensity_value);
}

void decrement_intensity(void) {
    intensity_value = (intensity_value > 1) ? intensity_value - 1 : 1;
    send_intensity_command(intensity_value);
}

void toggle_color_mode(void) {
    if (color_state == 5) {
        color_state = 5;
    } else {
        color_state = (color_state == -5) ? 0 : (color_state == 0) ? 5 : -5;
    }
    send_color_command(color_state);
}

void decrement_color_mode(void) {
    if (color_state == -5) {
        color_state = -5;
    } else {
        color_state = (color_state == 5) ? 0 : (color_state == 0) ? -5 : 5;
    }
    send_color_command(color_state);
}

void enable_sensor(void) {
    sensor_state = 1;
//  send_sensor_command(sensor_state);
}

void disable_sensor(void) {
    sensor_state = 0;
//  send_sensor_command(sensor_state);
}

void turn_on_lamp(void) {
    lamp_state = 1;
    send_lamp_command(lamp_state);
}

void turn_off_lamp(void) {
    lamp_state = 0;
    send_lamp_command(lamp_state);
}

void turn_on_endo(void) {
    endo_state = 1;
    send_endo_command(endo_state);
}

void turn_off_endo(void) {
    endo_state = 0;
    send_endo_command(endo_state);
}

void systemup_time(void) {
    display_uptime_and_return();
}

void turn_on_depth(void) {
    depth_state = 1;
    send_depth_command(depth_state);
}

void turn_off_depth(void) {
    depth_state = 0;
    send_depth_command(depth_state);
}
