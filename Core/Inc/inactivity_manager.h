#ifndef INACTIVITY_MANAGER_H
#define INACTIVITY_MANAGER_H

#include <stdint.h>

void InactivityManager_Init(void);
void InactivityManager_NotifyActivity(void);
void InactivityManager_CheckAndAutoOff(void);

#endif
