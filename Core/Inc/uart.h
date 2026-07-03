#ifndef UART_H
#define UART_H

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>


extern UART_HandleTypeDef huart1;
//extern volatile uint8_t dataReady;
// UART Initialization
//void UART_Init(void);

// UART Data Transmission
void UART_Send(const char *data, size_t length);
// UART Data Reception (optional if needed for incoming data)
//void UART_Receive(char *buffer, size_t length);
// UART Receive Callback for interrupt handling
//void UART_ReceiveCallback(void);

uint8_t isDataReady(void);
void resetDataReady(void);
void setDataReady(void);
uint8_t* getRxBuffer(void);


#define MAX_PACKET_SIZE 10  // Maximum packet size (adjust as needed)

// Function prototypes
void UART_Packet_Init(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t* UART_GetPacketBuffer(void);
uint8_t UART_GetPacketLength(void);

#endif // UART_H
