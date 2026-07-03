#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include "globals.h"
#include "lcd.h"
#include "command_manager.h"

#define INPUT_QUEUE_SIZE 2

typedef enum {
	INPUT_SOURCE_BUTTON, INPUT_SOURCE_GESTURE, INPUT_SOURCE_UART
} InputSource;

typedef enum {
    INPUT_MOVE_UP,
    INPUT_MOVE_DOWN,
    INPUT_SELECT,
    INPUT_BACK,
    INPUT_DEPTH,

    // Long presses
    INPUT_LONG_MOVE_UP,
    INPUT_LONG_MOVE_DOWN,
    INPUT_LONG_SELECT,
    INPUT_LONG_BACK,
    INPUT_LONG_DEPTH,

    // 🔹 New ones for CHANGE_P button
    INPUT_CHANGE_P,        // Short press on CHANGE_P
    INPUT_LONG_CHANGE_P,   // Long press on CHANGE_P (→ Standby toggle)

    INPUT_UNKNOWN = -1,
} InputAction;

typedef struct {
	InputSource source; // INPUT_SOURCE_BUTTON, INPUT_SOURCE_GESTURE, etc.
	InputAction action; // INPUT_MOVE_UP, INPUT_MOVE_DOWN, INPUT_SELECT, etc.
} InputEvent;

typedef struct {
	InputEvent events[INPUT_QUEUE_SIZE];
	uint8_t head;
	uint8_t tail;
} InputQueue;

// Function Prototypes
void enqueue_event(InputEvent event);
int dequeue_event(InputEvent *event);
void handle_input(InputAction action);
void process_input_events(void);
void handle_button_input(void);
void update_cursor(void);

void handle_first_screen_input(InputAction action);
void handle_second_screen_input(InputAction action);

#endif // INPUT_MANAGER_H
