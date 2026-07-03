//#ifndef SCREEN1_H
//#define SCREEN1_H
//
//#include "input_manager.h"
//
//// Function Prototypes
//void init_screen1(void);
//void handle_first_screen_input(InputAction action);
//void update_screen1_with_current_values(void);
//
//#endif // SCREEN1_H

#ifndef SCREEN1_H
#define SCREEN1_H

#include <stdint.h>
#include "input_manager.h"// Include shared InputEvent and InputAction definitions

void init_screen1(uint8_t start_position);
void toggle_sensor_state(void);
void handle_depth_action(void);
void increment_intensity(void);
void decrement_intensity(void);
void toggle_color_mode(void);
void decrement_color_mode(void);
void enable_sensor(void);
void disable_sensor(void);
void turn_on_lamp(void);
void turn_off_lamp(void);
void turn_on_endo(void);
void turn_off_endo(void);
void turn_on_depth(void);
void turn_off_depth(void);
void systemup_time(void);

void update_screen1_display(void);
void display_uptime_and_return();
void update_screen1_with_current_values(void);
void switch_to_screen2(void);
void init_second_screen(void);

#endif // SCREEN1_H
