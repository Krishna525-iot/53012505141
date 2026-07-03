#ifndef SCREEN2_H
#define SCREEN2_H


#include <stdint.h>
#include "input_manager.h"

// Function Prototypes
void init_second_screen(void);
void handle_second_screen_input(InputAction action);
void load_focus_options(void);
void toggle_camera_state(void);
void load_initial_focus(void);
void toggle_focus_option(uint8_t option_index);
void reset_focus_options(void);

void toggle_focus_state(void);
void toggle_cam_state(void);
void set_small_focus(void);
void set_medium_focus(void);
void set_wide_focus(void);
//init_second_screen(void);
#endif // SCREEN2_H
