################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_USB-FS-Device_Driver/src/usb_core.c \
../STM32_USB-FS-Device_Driver/src/usb_init.c \
../STM32_USB-FS-Device_Driver/src/usb_int.c \
../STM32_USB-FS-Device_Driver/src/usb_mem.c \
../STM32_USB-FS-Device_Driver/src/usb_regs.c \
../STM32_USB-FS-Device_Driver/src/usb_sil.c 

OBJS += \
./STM32_USB-FS-Device_Driver/src/usb_core.o \
./STM32_USB-FS-Device_Driver/src/usb_init.o \
./STM32_USB-FS-Device_Driver/src/usb_int.o \
./STM32_USB-FS-Device_Driver/src/usb_mem.o \
./STM32_USB-FS-Device_Driver/src/usb_regs.o \
./STM32_USB-FS-Device_Driver/src/usb_sil.o 

C_DEPS += \
./STM32_USB-FS-Device_Driver/src/usb_core.d \
./STM32_USB-FS-Device_Driver/src/usb_init.d \
./STM32_USB-FS-Device_Driver/src/usb_int.d \
./STM32_USB-FS-Device_Driver/src/usb_mem.d \
./STM32_USB-FS-Device_Driver/src/usb_regs.d \
./STM32_USB-FS-Device_Driver/src/usb_sil.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_USB-FS-Device_Driver/src/%.o: ../STM32_USB-FS-Device_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/STM32_USB-FS-Device_Driver/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/vcp/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


