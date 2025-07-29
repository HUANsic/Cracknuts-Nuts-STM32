################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/USER/user.c 

OBJS += \
./Core/USER/user.o 

C_DEPS += \
./Core/USER/user.d 


# Each subdirectory must supply rules for building sources it contributes
Core/USER/user.o: D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/USER/user.c Core/USER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F439xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/USER" -I"D:/Tsinghua/Cracknuts/Nuts/STM32Code/nuts/USER/mbedtls" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-USER

clean-Core-2f-USER:
	-$(RM) ./Core/USER/user.cyclo ./Core/USER/user.d ./Core/USER/user.o ./Core/USER/user.su

.PHONY: clean-Core-2f-USER

