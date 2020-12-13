################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Map.cc \
../Player.cc \
../RayCastingWindow.cc \
../Window.cc \
../main.cc 

CC_DEPS += \
./Map.d \
./Player.d \
./RayCastingWindow.d \
./Window.d \
./main.d 

OBJS += \
./Map.o \
./Player.o \
./RayCastingWindow.o \
./Window.o \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


