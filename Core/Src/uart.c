#include "uart.h"


#include <string.h>  // For memcpy or other string operations if needed
#include <stdbool.h>

// Private variables
static uint8_t UART2_rxBuffer[1];  // Buffer to store the incoming byte
uint8_t packetBuffer[MAX_PACKET_SIZE];  // Buffer to store the valid packet (from @ to \r)
static uint8_t packetIndex = 0;  // Index to track the current position in the buffer
static bool packetStarted = false;  // Flag to indicate if the packet has started
volatile uint8_t dataReady = 0;  // Flag to indicate if new data is ready
static UART_HandleTypeDef *huart_instance;  // UART instance for DMA

// Initialize the UART packet handler
void UART_Packet_Init(UART_HandleTypeDef *huart)
{
    huart_instance = huart;  // Store the UART instance
    HAL_UART_Receive_DMA(huart_instance, UART2_rxBuffer, 1);  // Start DMA reception
}

// UART receive complete callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t receivedChar;

    // Read the received character
    receivedChar = UART2_rxBuffer[0];  // Assuming the received byte is stored in UART2_rxBuffer[0]

    // Check if the received character is the start character '@'
    if (receivedChar == '@')
    {
        packetStarted = true;  // Set the flag to start capturing the packet
        packetIndex = 0;       // Reset the buffer index
    }

    // If the packet has started, process the received character
    if (packetStarted)
    {
        // If the character is '\r', end the packet
        if (receivedChar == '\r')
        {
            // Packet is complete, process it here
            // Null-terminate the packet buffer (optional, if treating it as a string)
            packetBuffer[packetIndex] = '\0';

            // Set the dataReady flag to indicate new data is available
            dataReady = 1;

            // Reset the flag and index for the next packet
            packetStarted = false;
            packetIndex = 0;
        }
        // If the character is '\n', ignore it
        else if (receivedChar == '\n')
        {
            // Do nothing, just ignore '\n'
        }
        // Otherwise, store the character in the buffer
        else
        {
            if (packetIndex < MAX_PACKET_SIZE - 1)  // Ensure we don't overflow the buffer
            {
                packetBuffer[packetIndex] = receivedChar;
                packetIndex++;
            }
            else
            {
                // Buffer overflow, reset the packet
                packetStarted = false;
                packetIndex = 0;
            }
        }
    }

    // Restart DMA reception for the next byte
    HAL_UART_Receive_DMA(huart_instance, UART2_rxBuffer, 1);
}


// Get the packet buffer (valid data from @ to \r)
uint8_t* UART_GetPacketBuffer(void)
{
    return packetBuffer;
}

// Get the length of the valid packet
uint8_t UART_GetPacketLength(void)
{
    return packetIndex;
}
//#...................................................


uint8_t* getRxBuffer(void) {
//	return rxBuffer;
    return packetBuffer;
}

uint8_t isDataReady(void) {
	return dataReady;
}

void resetDataReady(void) {
	dataReady = 0;
}

void setDataReady(void) {
	dataReady = 0;
}

// UART Data Transmission
void UART_Send(const char *data, size_t length) {
	if (data != NULL && length > 0) {
		HAL_UART_Transmit(&huart1, (uint8_t*) data, length, HAL_MAX_DELAY);
	}
}

// UART Data Reception
//void UART_Receive(char *buffer, size_t length) {
//	if (buffer != NULL && length > 0) {
//		HAL_UART_Receive(&huart1, (uint8_t*) buffer, length, HAL_MAX_DELAY);
//	}
//}

// UART Receive Callback for interrupt-based handling
