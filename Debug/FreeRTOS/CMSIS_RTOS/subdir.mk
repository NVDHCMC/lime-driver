################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./FreeRTOS/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./FreeRTOS/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/CMSIS_RTOS/%.o: ../FreeRTOS/CMSIS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F411VETx -DSTM32F4 -DSTM32 -DSTM32F411E_DISCO -DDEBUG -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/inc" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/HAL_Driver/Inc" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/HAL_Driver/Inc/Legacy" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/CMSIS/device" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/CMSIS/core" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/FreeRTOS/CMSIS_RTOS" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/FreeRTOS/include" -I"/home/azure/workspace1/LIME_DRIVER_0.2.0/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


