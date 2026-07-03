#include "lcd.h"
#include "stm32f0xx_hal.h" // Include the HAL library for STM32F0 (adjust as needed)

// Define helper macros for GPIO operations
#define LCD_GPIO_WritePin(port, pin, value) HAL_GPIO_WritePin((port), (pin), (value))
#define LCD_Delay(ms) HAL_Delay((ms)) // Replace with your platform's delay function

// Internal function prototypes
static void LCD_Send4Bits(uint8_t data);
static void LCD_WriteCommand(uint8_t cmd);
static void LCD_WriteData(uint8_t data);

void LCD_Init(void) {
	// Initialize LCD in 4-bit mode
	LCD_Delay(20);                       // Wait for power stabilization
	LCD_WriteCommand(0x03);              // Initialize
	LCD_Delay(5);
	LCD_WriteCommand(0x03);              // Initialize
	LCD_Delay(1);
	LCD_WriteCommand(0x03);              // Initialize
	LCD_WriteCommand(0x02);              // Switch to 4-bit mode

	// Function set: 2 lines, 5x8 font
	LCD_WriteCommand(0x28);

	// Display ON, Cursor OFF, Blink OFF
	LCD_WriteCommand(0x0C);

	// Entry mode: Increment cursor
	LCD_WriteCommand(0x06);

	// Clear display
	LCD_Clear();
}

void LCD_Clear(void) {
	LCD_WriteCommand(0x01); // Clear display
	LCD_Delay(2);           // Delay for command execution
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
	uint8_t address = 0x80; // Start address for line 1
	if (row == 1)
		address = 0xC0; // Start address for line 2
	if (row == 2)
		address = 0x94; // Start address for line 3
	if (row == 3)
		address = 0xD4; // Start address for line 4
	address += col;
	LCD_WriteCommand(address);
}

void LCD_WriteChar(char c) {
	LCD_WriteData(c);
}

void LCD_WriteString(const char *str) {
	while (*str) {
		LCD_WriteChar(*str++);
	}
}

void lcd_puts(int8_t row, int8_t col, int8_t *str) {
	LCD_SetCursor(row, col);
	LCD_WriteString((char*) str);
}

// Internal functions
static void LCD_Send4Bits(uint8_t data) {
	// Write data to D4-D7 pins
	LCD_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin,
			(data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCD_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin,
			(data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCD_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin,
			(data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCD_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin,
			(data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Toggle the Enable pin
	LCD_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
	LCD_Delay(1); // Short delay
	LCD_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
	LCD_Delay(1); // Short delay
}

static void LCD_WriteCommand(uint8_t cmd) {
	// RS = 0 for command
	LCD_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

	// Send higher nibble
	LCD_Send4Bits(cmd >> 4);

	// Send lower nibble
	LCD_Send4Bits(cmd & 0x0F);
}

static void LCD_WriteData(uint8_t data) {
	// RS = 1 for data
	LCD_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	// Send higher nibble
	LCD_Send4Bits(data >> 4);

	// Send lower nibble
	LCD_Send4Bits(data & 0x0F);
}
