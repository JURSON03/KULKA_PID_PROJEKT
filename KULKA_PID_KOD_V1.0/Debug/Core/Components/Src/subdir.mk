################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Components/Src/HCSR04.c \
../Core/Components/Src/PID.c \
../Core/Components/Src/SG90.c 

C_DEPS += \
./Core/Components/Src/HCSR04.d \
./Core/Components/Src/PID.d \
./Core/Components/Src/SG90.d 

OBJS += \
./Core/Components/Src/HCSR04.o \
./Core/Components/Src/PID.o \
./Core/Components/Src/SG90.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Components/Src/%.o Core/Components/Src/%.su Core/Components/Src/%.cyclo: ../Core/Components/Src/%.c Core/Components/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Core/Components/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Components-2f-Src

clean-Core-2f-Components-2f-Src:
	-$(RM) ./Core/Components/Src/HCSR04.cyclo ./Core/Components/Src/HCSR04.d ./Core/Components/Src/HCSR04.o ./Core/Components/Src/HCSR04.su ./Core/Components/Src/PID.cyclo ./Core/Components/Src/PID.d ./Core/Components/Src/PID.o ./Core/Components/Src/PID.su ./Core/Components/Src/SG90.cyclo ./Core/Components/Src/SG90.d ./Core/Components/Src/SG90.o ./Core/Components/Src/SG90.su

.PHONY: clean-Core-2f-Components-2f-Src

