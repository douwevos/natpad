################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/generated/abcparserconfig.c 

OBJS += \
./src/generated/abcparserconfig.o 

C_DEPS += \
./src/generated/abcparserconfig.d 


# Each subdirectory must supply rules for building sources it contributes
src/generated/%.o: ../src/generated/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/gtk-3.0/unix-print -I/usr/include/atk-1.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gtk-3.0 -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pixman-1 -I"/home/superman/cpp-workspace/trunk/natpad.ide/caterpillar/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


