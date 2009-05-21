################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Torrent/Vista/vistaTorrent.cpp 

CPP_DEPS += \
./Torrent/Vista/vistaTorrent.d 

OBJS += \
./Torrent/Vista/vistaTorrent.o 


# Each subdirectory must supply rules for building sources it contributes
Torrent/Vista/%.o: ../Torrent/Vista/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtkmm-2.4` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


