#ifndef LCD_H
#define LCD_H

#include <stdint.h> // For standard integer types

// User must define these macros or include them in their code
#define LCD_D4_Pin GPIO_PIN_0
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_1
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_5
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_4
#define LCD_D7_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_4
#define LCD_EN_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_5
#define LCD_RS_GPIO_Port GPIOA

// Public API
void LCD_Init(void);                      // Initialize the LCD
void LCD_Clear(void);                     // Clear the LCD screen
void LCD_SetCursor(uint8_t row, uint8_t col); // Set cursor position
void LCD_WriteChar(char c);               // Write a single character
void LCD_WriteString(const char *str);    // Write a string
void lcd_puts(int8_t row, int8_t col, int8_t *str); // Write a string at specific position

#endif // LCD_H
