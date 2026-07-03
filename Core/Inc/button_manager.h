#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <stdint.h>

// Button action types
typedef enum {
	BUTTON_ACTION_SHORT_PRESS, BUTTON_ACTION_LONG_PRESS
} ButtonActionType;

// Button event structure
typedef struct {
	uint8_t button_id;          // Unique button ID
	ButtonActionType action;    // Action type (short/long press)
} ButtonEvent;

// Callback type for button events
typedef void (*ButtonEventCallback)(ButtonEvent *event);

// Button state structure
typedef struct {
	uint32_t last_interrupt_time; // Last interrupt time for debounce
	uint32_t press_start_time;    // Press start time for long press
	uint8_t is_pressed;           // Button state: 1 = pressed, 0 = released
	uint8_t long_press_detected
} ButtonState;

extern ButtonState button_states[];

// Public API
void ButtonManager_Init(ButtonEventCallback callback);
void ButtonManager_HandleInterrupt(uint16_t GPIO_Pin);

#endif // BUTTON_MANAGER_H
