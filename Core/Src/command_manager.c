#include "command_manager.h"
#include "uart.h"
#include <stdio.h>
#include "globals.h"
#include <string.h>


void CommandManager_Init(void) {
    UART_Init();
}

void send_command(const char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0) {
        return;
    }
    UART_Send(cmd, strlen(cmd));
}

void send_intensity_command(uint8_t intensity) {
    char cmd[8];
    if (intensity == 10) {
        snprintf(cmd, sizeof(cmd), "@I0:#");
    } else {
        snprintf(cmd, sizeof(cmd), "@I%02d#", intensity);
    }
    send_command(cmd);
}

void send_color_command(int8_t color) {
    char cmd[8];
    if (color == -5) {
        snprintf(cmd, sizeof(cmd), "@C-5#");
    } else if (color == 0) {
        snprintf(cmd, sizeof(cmd), "@C05#");
    } else if (color == +5) {
        snprintf(cmd, sizeof(cmd), "@C+5#");
    }
    send_command(cmd);
}

void send_lamp_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@L_%d#", state);
    send_command(cmd);
}

void send_endo_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@E_%d#", state);
    send_command(cmd);
}

void send_cam_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@M0%d#", state);
    send_command(cmd);
}

void send_depth_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@D_%d#", state);
    send_command(cmd);
}

void send_PA_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@PA%d#", state);
    send_command(cmd);
}

/* ---------------------------------------------------------------
 *  RED LED — on/off
 *  @R_0#  → off
 *  @R_1#  → on (full brightness)
 * --------------------------------------------------------------- */
void send_Red_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@R_%d#", state);
    send_command(cmd);
}

/* ---------------------------------------------------------------
 *  RED LED — intensity levels 1–10
 *  level 1–9  → @R01# … @R09#
 *  level 10   → @R0:#
 * --------------------------------------------------------------- */
void send_Red_intensity_command(uint8_t level) {
    char cmd[8];
    if (level < 1 || level > 10) return;   /* guard: ignore out-of-range */

    if (level == 10) {
        snprintf(cmd, sizeof(cmd), "@R0:#");
    } else {
        snprintf(cmd, sizeof(cmd), "@R0%d#", level);
    }
    send_command(cmd);
}

/* ---------------------------------------------------------------
 *  GREEN LED — on/off
 *  @G_0#  → off
 *  @G_1#  → on (full brightness)
 * --------------------------------------------------------------- */
void send_Green_command(uint8_t state) {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), "@G_%d#", state);
    send_command(cmd);
}

/* ---------------------------------------------------------------
 *  GREEN LED — intensity levels 1–10
 *  level 1–9  → @G01# … @G09#
 *  level 10   → @G0:#
 * --------------------------------------------------------------- */
void send_Green_intensity_command(uint8_t level) {
    char cmd[8];
    if (level < 1 || level > 10) return;   /* guard: ignore out-of-range */

    if (level == 10) {
        snprintf(cmd, sizeof(cmd), "@G0:#");
    } else {
        snprintf(cmd, sizeof(cmd), "@G0%d#", level);
    }
    send_command(cmd);
}

/* ---------------------------------------------------------------
 *  FOCUS / FIELD
 * --------------------------------------------------------------- */
void send_focus_command(uint8_t focus_option) {
    if (focus_state == 1) {
        char cmd[8];
        snprintf(cmd, sizeof(cmd), "@F_1#");
        send_command(cmd);

        if (focus_option < 3) {
            char cmd2[8];
            snprintf(cmd2, sizeof(cmd2), "@F0%d#", focus_option + 1);
            send_command(cmd2);
        }
    } else if (focus_state == 0) {
        char cmd[8];
        snprintf(cmd, sizeof(cmd), "@F_0#");
        send_command(cmd);
    }
}
