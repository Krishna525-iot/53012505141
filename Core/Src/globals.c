#include "globals.h"




// Global variables
uint8_t gesture_enabled = 0;   // Gesture system enabled/disabled
uint8_t cursor_row = 1;        // Cursor row position
uint8_t cursor_col = 0;        // Cursor column position
uint8_t intensity_value = 1;   // Default intensity value
int8_t color_state = 0;       // Default color state (0: CW, 1: WW, 2: NW)
uint8_t sensor_state = 0;      // Sensor state (0: Disabled, 1: Enabled)
uint8_t lamp_state = 1;        // Lamp state (0: OFF, 1: ON)
uint8_t endo_state = 0;        // Endo state (0: OFF, 1: ON)
uint8_t depth_state = 0;       // Depth state (0: OFF, 1: ON)
uint8_t press_state = 0;

// Add global variables to track previous cursor position
uint8_t prev_cursor_row = 0;
uint8_t prev_cursor_col = 0;
uint8_t camera_enabled = 0;

// Initialize global variables
uint8_t screen_state = 0; // Start with Screen1
uint8_t focus_state = 0;  // Focus OFF by default
uint8_t cam_state = 0;  // Focus OFF by default
uint8_t option_states[3] = { 0, 0, 0 }; // All options OFF by default
uint32_t system_start_time = 0;
