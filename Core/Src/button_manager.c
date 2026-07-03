#include "button_manager.h"
#include "main.h"
#include "globals.h"

// Button pin definitions
static const uint16_t button_pins[] = { S_PRV_Pin, S_NEXT_Pin, CHANGE_P_Pin, CHANGE_N_Pin, DEPTH_Pin};

// Unique button IDs
static const uint8_t button_ids[] = { 0, 1, 2, 3, 4 };

// Button state tracking
ButtonState button_states[sizeof(button_pins) / sizeof(button_pins[0])];

// Debounce and long press thresholds
#define DEBOUNCE_THRESHOLD 50       // 50 ms debounce time

// Callback for button events
static ButtonEventCallback event_callback = NULL;

// Initialization function
void ButtonManager_Init(ButtonEventCallback callback) {
    event_callback = callback;
}

uint32_t lastInterruptTime = 0;
uint32_t debounceDelay = 5000;


//-----------------------------------------------------------------------------------------
// Handle button press or release
// Handle button press or release
void ButtonManager_HandleInterrupt(uint16_t GPIO_Pin) {
    uint32_t current_time = HAL_GetTick();

    // Find the button index for the given GPIO pin
    int button_index = -1;
    for (int i = 0; i < sizeof(button_pins) / sizeof(button_pins[0]); i++) {
        if (button_pins[i] == GPIO_Pin) {
            button_index = i;
            break;
        }
    }

    // If no matching button index found or callback is NULL, return
    if (button_index == -1 || event_callback == NULL) {
        return;
    }

    ButtonState *state = &button_states[button_index];

    // Debounce logic: Ignore events within DEBOUNCE_THRESHOLD
    if (current_time - state->last_interrupt_time < DEBOUNCE_THRESHOLD) {
        return;
    }
    state->last_interrupt_time = current_time;

    // Handle button press or release
    if (!state->is_pressed) {
        // Button pressed
        state->press_start_time = current_time;
        state->is_pressed = 1;
        state->long_press_detected = 0; // Reset long press flag
    } else {
        // Button released
        uint32_t press_duration = current_time - state->press_start_time;
        state->is_pressed = 0; // Mark as released

        // 👉 Only trigger short press if long press was NOT detected
        if (!state->long_press_detected) {
            ButtonEvent event = {
                .button_id = button_ids[button_index],
                .action = BUTTON_ACTION_SHORT_PRESS
            };
            event_callback(&event);
        }
    }
}

// Continuous check for long press while button is held
// Continuous check for long press while button is held
void ButtonManager_CheckLongPress(void) {
    uint32_t current_time = HAL_GetTick();

    for (int i = 0; i < sizeof(button_pins) / sizeof(button_pins[0]); i++) {
        ButtonState *state = &button_states[i];

        if (state->is_pressed && !state->long_press_detected) {
            uint32_t press_duration = current_time - state->press_start_time;

            if (press_duration >= LONG_PRESS_THRESHOLD) {
                // Long press detected
                state->long_press_detected = 1; // Mark long press as handled to prevent duplicate events

                ButtonEvent event = {
                    .button_id = button_ids[i],
                    .action = BUTTON_ACTION_LONG_PRESS
                };

                event_callback(&event);
            }
        }
    }
}

