################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_stm32f10x.c \
../system/src/cmsis/vectors_stm32f10x.c 

OBJS += \
./system/src/cmsis/system_stm32f10x.o \
./system/src/cmsis/vectors_stm32f10x.o 

C_DEPS += \
./system/src/cmsis/system_stm32f10x.d \
./system/src/cmsis/vectors_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/STM32_USB-FS-Device_Driver/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/vcp/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


