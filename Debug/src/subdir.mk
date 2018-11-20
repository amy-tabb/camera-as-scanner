################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Chessboard-camera-cali-project.cpp \
../src/DirectoryFunctions.cpp \
../src/camera_calibration.cpp 

OBJS += \
./src/Chessboard-camera-cali-project.o \
./src/DirectoryFunctions.o \
./src/camera_calibration.o 

CPP_DEPS += \
./src/Chessboard-camera-cali-project.d \
./src/DirectoryFunctions.d \
./src/camera_calibration.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/eigen3 -I/usr/include/eigen3 -O0 -g3 -Wall -c -fmessage-length=0 -fopenmp -Wall -pedantic -Wextra -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


