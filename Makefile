SYSROOTS ?= /home/simon/sysroots
CC = ${SYSROOTS}/x86_64-angstromsdk-linux/usr/bin/arm-angstrom-linux-gnueabi/arm-angstrom-linux-gnueabi-g++
INCLUDES = -I${SYSROOTS}/armv7at2hf-vfp-neon-angstrom-linux-gnueabi/usr/include
LIB_PATH = -L${SYSROOTS}/armv7at2hf-vfp-neon-angstrom-linux-gnueabi/lib
LIBS = -lpthread -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect -lm
CFLAGS = -O2 -g -Wall -mfloat-abi=hard --sysroot=${SYSROOTS}/armv7at2hf-vfp-neon-angstrom-linux-gnueabi

all:
	${CC} ${CFLAGS} ${INCLUDES} ${LIB_PATH} ${LIBS} facedetect.cpp -o facedetect

clean:
	rm -rf facedetect
