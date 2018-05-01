################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vcp/src/hw_config.c \
../vcp/src/stm32_it.c \
../vcp/src/usb_desc.c \
../vcp/src/usb_endp.c \
../vcp/src/usb_istr.c \
../vcp/src/usb_prop.c \
../vcp/src/usb_pwr.c 

OBJS += \
./vcp/src/hw_config.o \
./vcp/src/stm32_it.o \
./vcp/src/usb_desc.o \
./vcp/src/usb_endp.o \
./vcp/src/usb_istr.o \
./vcp/src/usb_prop.o \
./vcp/src/usb_pwr.o 

C_DEPS += \
./vcp/src/hw_config.d \
./vcp/src/stm32_it.d \
./vcp/src/usb_desc.d \
./vcp/src/usb_endp.d \
./vcp/src/usb_istr.d \
./vcp/src/usb_prop.d \
./vcp/src/usb_pwr.d 


# Each subdirectory must supply rules for building sources it contributes
vcp/src/%.o: ../vcp/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/STM32_USB-FS-Device_Driver/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/vcp/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


