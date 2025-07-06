################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/process/user.c 

OBJS += \
./Core/process/user.o 

C_DEPS += \
./Core/process/user.d 


# Each subdirectory must supply rules for building sources it contributes
Core/process/user.o: D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/process/user.c Core/process/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/process" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-process

clean-Core-2f-process:
	-$(RM) ./Core/process/user.cyclo ./Core/process/user.d ./Core/process/user.o ./Core/process/user.su

.PHONY: clean-Core-2f-process

