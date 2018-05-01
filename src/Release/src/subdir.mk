################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/font12x16_reduced.c \
../src/font5x7.c \
../src/irmp.c \
../src/ugui.c 

CPP_SRCS += \
../src/Timer.cpp \
../src/gpio.cpp \
../src/i2c.cpp \
../src/icons.cpp \
../src/ir.cpp \
../src/json.cpp \
../src/lcd64x32.cpp \
../src/main.cpp \
../src/rotenc.cpp \
../src/rtc.cpp \
../src/tim.cpp \
../src/usb.cpp 

OBJS += \
./src/Timer.o \
./src/_write.o \
./src/font12x16_reduced.o \
./src/font5x7.o \
./src/gpio.o \
./src/i2c.o \
./src/icons.o \
./src/ir.o \
./src/irmp.o \
./src/json.o \
./src/lcd64x32.o \
./src/main.o \
./src/rotenc.o \
./src/rtc.o \
./src/tim.o \
./src/ugui.o \
./src/usb.o 

C_DEPS += \
./src/_write.d \
./src/font12x16_reduced.d \
./src/font5x7.d \
./src/irmp.d \
./src/ugui.d 

CPP_DEPS += \
./src/Timer.d \
./src/gpio.d \
./src/i2c.d \
./src/icons.d \
./src/ir.d \
./src/json.d \
./src/lcd64x32.d \
./src/main.d \
./src/rotenc.d \
./src/rtc.d \
./src/tim.d \
./src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/STM32_USB-FS-Device_Driver/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/vcp/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/STM32_USB-FS-Device_Driver/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1/vcp/inc" -I"/home/ne23kaj2/btsync/work/cpp-neon/beamer1" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


