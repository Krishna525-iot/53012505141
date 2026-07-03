#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

extern volatile uint32_t systemTimeSec;
extern uint8_t camera_state;

typedef struct {
	uint8_t row;
	uint8_t col;
	void (*on_action)(void);  // Pointer to function for "ON/Increment" action
	void (*off_action)(void); // Pointer to function for "OFF/Decrement" action
} CursorAction;

extern const CursorAction cursor_actions[];
extern const uint8_t num_positions;
extern const uint8_t cursor_positions[][2];
// Declare global variables
//extern ButtonState button_states[];
extern uint8_t gesture_enabled;
extern uint8_t intensity_value;
extern uint8_t cam_state;
extern int8_t color_state;
extern uint8_t sensor_state;
extern uint8_t lamp_state;
extern uint8_t endo_state;
extern uint8_t depth_state;

extern uint8_t cursor_row;
extern uint8_t cursor_col;

extern uint8_t prev_cursor_row;
extern uint8_t prev_cursor_col;

extern uint8_t current_screen;
extern uint8_t screen_state;
extern uint8_t focus_state;
extern uint8_t option_states[3];
extern uint8_t press_state;

//extern uint32_t system_start_time;

#define LONG_PRESS_THRESHOLD 4000 // Long press duration in milliseconds

extern uint8_t camera_enabled;

// Track button press state
//static uint8_t move_up_pressed = 0;       // 1 = pressed, 0 = released
//static uint32_t move_up_press_time = 0;  // Time when the button was pressed

#endif // GLOBALS_H
