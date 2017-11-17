################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./FreeRTOS/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./FreeRTOS/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/ARM_CM4F/%.o: ../FreeRTOS/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F411VETx -DSTM32F4 -DSTM32 -DSTM32F411E_DISCO -DDEBUG -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -DUSE_FREERTOS -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/inc" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/HAL_Driver/Inc" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/HAL_Driver/Inc/Legacy" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/CMSIS/device" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/CMSIS/core" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/FreeRTOS/CMSIS_RTOS" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/FreeRTOS/include" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/azure/workspace1/TRACKING_DRIVER_v0.1/LITVE_Library/include" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


