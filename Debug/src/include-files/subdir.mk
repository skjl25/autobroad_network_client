################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/root/workspace/include-files/TCPClientSocket.cpp \
/root/workspace/include-files/TCPServerSocket.cpp \
/root/workspace/include-files/camera.cpp \
/root/workspace/include-files/cvideo.cpp \
/root/workspace/include-files/image_processing.cpp \
/root/workspace/include-files/m3dvc.cpp \
/root/workspace/include-files/utility.cpp \
/root/workspace/include-files/wlessTools.cpp 

C_SRCS += \
/root/workspace/include-files/iwlib.c 

CC_SRCS += \
/root/workspace/include-files/load_image.cc \
/root/workspace/include-files/viewer.cc 

OBJS += \
./src/include-files/TCPClientSocket.o \
./src/include-files/TCPServerSocket.o \
./src/include-files/camera.o \
./src/include-files/cvideo.o \
./src/include-files/image_processing.o \
./src/include-files/iwlib.o \
./src/include-files/load_image.o \
./src/include-files/m3dvc.o \
./src/include-files/utility.o \
./src/include-files/viewer.o \
./src/include-files/wlessTools.o 

C_DEPS += \
./src/include-files/iwlib.d 

CC_DEPS += \
./src/include-files/load_image.d \
./src/include-files/viewer.d 

CPP_DEPS += \
./src/include-files/TCPClientSocket.d \
./src/include-files/TCPServerSocket.d \
./src/include-files/camera.d \
./src/include-files/cvideo.d \
./src/include-files/image_processing.d \
./src/include-files/m3dvc.d \
./src/include-files/utility.d \
./src/include-files/wlessTools.d 


# Each subdirectory must supply rules for building sources it contributes
src/include-files/TCPClientSocket.o: /root/workspace/include-files/TCPClientSocket.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/TCPServerSocket.o: /root/workspace/include-files/TCPServerSocket.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/camera.o: /root/workspace/include-files/camera.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/cvideo.o: /root/workspace/include-files/cvideo.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/image_processing.o: /root/workspace/include-files/image_processing.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/iwlib.o: /root/workspace/include-files/iwlib.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/load_image.o: /root/workspace/include-files/load_image.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/m3dvc.o: /root/workspace/include-files/m3dvc.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/utility.o: /root/workspace/include-files/utility.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/viewer.o: /root/workspace/include-files/viewer.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/include-files/wlessTools.o: /root/workspace/include-files/wlessTools.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/GL -I/home/sklee25/temp/libfreenect/include/ -I/usr/include -I/usr/local/include/libfreenect -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


