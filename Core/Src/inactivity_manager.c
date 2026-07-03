#include "inactivity_manager.h"
#include "command_manager.h"  // for send_lamp_command
#include "globals.h"           // for lamp_state
#include "screen1.h"            // for init_screen1, update_screen1_with_current_values
#include "screen2.h"             // for screen_state etc.
#include "lcd.h"
#include "main.h"

#define INACTIVITY_MS   (30UL * 60UL * 1000UL)  // 1 minute

static uint32_t s_lastActivityMs = 0;
static uint8_t s_autoOffDone = 0;

void InactivityManager_Init(void)
{
//    s_lastActivityMs = HAL_GetTick();
//    s_autoOffDone = 0;
}

void InactivityManager_NotifyActivity(void)
{
//    s_lastActivityMs = HAL_GetTick();
//    s_autoOffDone = 0;   // allow re-trigger
}

void InactivityManager_CheckAndAutoOff(void)
{
//    uint32_t now = HAL_GetTick();
//
//    if (!s_autoOffDone && lamp_state == 1 && (now - s_lastActivityMs >= INACTIVITY_MS)) {
//        send_lamp_command(0);
//        lamp_state = 0;
//
//        screen_state = 0;
//        LCD_Clear();
//        init_screen1(0);
//        update_screen1_with_current_values();
//
//        s_autoOffDone = 1;
//        if (focus_state == 1) {
//                    focus_state = 0;
//                    send_command("@F_0#");
//                }
//    }
}
