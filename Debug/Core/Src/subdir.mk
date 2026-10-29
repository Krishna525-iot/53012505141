################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/button_manager.c \
../Core/Src/command_manager.c \
../Core/Src/eeprom.c \
../Core/Src/freertos.c \
../Core/Src/gesture.c \
../Core/Src/globals.c \
../Core/Src/inactivity_manager.c \
../Core/Src/input_manager.c \
../Core/Src/lcd.c \
../Core/Src/main.c \
../Core/Src/screen1.c \
../Core/Src/screen2.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f0xx.c \
../Core/Src/uart.c \
../Core/Src/uart_command_handler.c 

OBJS += \
./Core/Src/button_manager.o \
./Core/Src/command_manager.o \
./Core/Src/eeprom.o \
./Core/Src/freertos.o \
./Core/Src/gesture.o \
./Core/Src/globals.o \
./Core/Src/inactivity_manager.o \
./Core/Src/input_manager.o \
./Core/Src/lcd.o \
./Core/Src/main.o \
./Core/Src/screen1.o \
./Core/Src/screen2.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f0xx.o \
./Core/Src/uart.o \
./Core/Src/uart_command_handler.o 

C_DEPS += \
./Core/Src/button_manager.d \
./Core/Src/command_manager.d \
./Core/Src/eeprom.d \
./Core/Src/freertos.d \
./Core/Src/gesture.d \
./Core/Src/globals.d \
./Core/Src/inactivity_manager.d \
./Core/Src/input_manager.d \
./Core/Src/lcd.d \
./Core/Src/main.d \
./Core/Src/screen1.d \
./Core/Src/screen2.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f0xx.d \
./Core/Src/uart.d \
./Core/Src/uart_command_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/button_manager.cyclo ./Core/Src/button_manager.d ./Core/Src/button_manager.o ./Core/Src/button_manager.su ./Core/Src/command_manager.cyclo ./Core/Src/command_manager.d ./Core/Src/command_manager.o ./Core/Src/command_manager.su ./Core/Src/eeprom.cyclo ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gesture.cyclo ./Core/Src/gesture.d ./Core/Src/gesture.o ./Core/Src/gesture.su ./Core/Src/globals.cyclo ./Core/Src/globals.d ./Core/Src/globals.o ./Core/Src/globals.su ./Core/Src/inactivity_manager.cyclo ./Core/Src/inactivity_manager.d ./Core/Src/inactivity_manager.o ./Core/Src/inactivity_manager.su ./Core/Src/input_manager.cyclo ./Core/Src/input_manager.d ./Core/Src/input_manager.o ./Core/Src/input_manager.su ./Core/Src/lcd.cyclo ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/screen1.cyclo ./Core/Src/screen1.d ./Core/Src/screen1.o ./Core/Src/screen1.su ./Core/Src/screen2.cyclo ./Core/Src/screen2.d ./Core/Src/screen2.o ./Core/Src/screen2.su ./Core/Src/stm32f0xx_hal_msp.cyclo ./Core/Src/stm32f0xx_hal_msp.d ./Core/Src/stm32f0xx_hal_msp.o ./Core/Src/stm32f0xx_hal_msp.su ./Core/Src/stm32f0xx_it.cyclo ./Core/Src/stm32f0xx_it.d ./Core/Src/stm32f0xx_it.o ./Core/Src/stm32f0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f0xx.cyclo ./Core/Src/system_stm32f0xx.d ./Core/Src/system_stm32f0xx.o ./Core/Src/system_stm32f0xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su ./Core/Src/uart_command_handler.cyclo ./Core/Src/uart_command_handler.d ./Core/Src/uart_command_handler.o ./Core/Src/uart_command_handler.su

.PHONY: clean-Core-2f-Src

