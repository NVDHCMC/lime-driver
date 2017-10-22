################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f411xe.s 

OBJS += \
./startup/startup_stm32f411xe.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/inc" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/HAL_Driver/Inc" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/HAL_Driver/Inc/Legacy" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/CMSIS/core" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/CMSIS/device" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


