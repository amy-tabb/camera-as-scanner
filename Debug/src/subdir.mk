################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DirectoryFunctions.cpp \
../src/camera-as-scanner.cpp \
../src/camera_calibration.cpp 

OBJS += \
./src/DirectoryFunctions.o \
./src/camera-as-scanner.o \
./src/camera_calibration.o 

CPP_DEPS += \
./src/DirectoryFunctions.d \
./src/camera-as-scanner.d \
./src/camera_calibration.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/eigen3 -I/usr/local/include/opencv4 -I/usr/include/eigen3 -O0 -g3 -Wall -c -fopenmp -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


