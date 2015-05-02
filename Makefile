#==========================================================
# Build Data
ARCH	:= x86
BOARD	:= i386
KERNEL_OUTPUT 	:= lapis
#----------------------------------------------------------
# Tools
AS		:= nasm -felf32# NASM or YASM is required for x86
GAS		:= as
CC		:= gcc
CPP		:= g++
CFLAGS	:= -ffreestanding -std=gnu99 -nostartfiles -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -O2 -g
CPFLAGS := -ffreestanding -std=c++11 -nostartfiles -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -O2 -g
LD		:= ${CPP}
LD_FLAGS:= -ffreestanding -O2 -nostdlib -lgcc -g -Wall -Wextra -Wno-unused-function -Wno-unused-parameter

GENISO 	:= genisoimage
GENISOF	:= -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table
#----------------------------------------------------------
# Precalculated variables
LD_LINKER_SCRIPT := arch/${ARCH}/${BOARD}/link.ld
#----------------------------------------------------------
# All Source Files
FILES_KERNEL	:= $(patsubst %.cpp,%.o,$(wildcard kernel/*.cpp))
FILES_INIT  	:= $(patsubst %.cpp,%.o,$(wildcard init/*.cpp))
FILES_LIB   	:= $(patsubst %.cpp,%.o,$(wildcard lib/*.cpp))
FILES_LOG   	:= $(patsubst %.cpp,%.o,$(wildcard log/*.cpp))
FILES_DRIVERS	:= $(patsubst %.cpp,%.o,$(wildcard drivers/*.cpp))

FILES_ARCH  	:= $(patsubst %.c,%.o,$(wildcard arch/${ARCH}/*.cpp)) $(patsubst %.s,%.o,$(wildcard arch/${ARCH}/*.s))
FILES_BOARD 	:= $(patsubst %.cpp,%.o,$(wildcard arch/${ARCH}/${BOARD}/*.cpp)) $(patsubst %.s,%.o,$(wildcard arch/${ARCH}/${BOARD}/*.s))

FILES_CRTI 		:=arch/${ARCH}/${BOARD}/crti.o
FILES_CRTBEGIN	:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
FILES_CRTEND	:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
FILES_CRTN 		:=arch/${ARCH}/${BOARD}/crtn.o

FILES_ALL		:= ${FILES_INIT} ${FILES_DRIVERS} ${FILES_KERNEL} ${FILES_LIB} ${FILES_LOG} ${FILES_ARCH} ${FILES_BOARD} 
FILES_COMPILE	:= ${FILES_ALL}
#==========================================================

%.o: %.s
	@echo "AS  " $@
	@${AS} -o $@ $<

%.o: %.c
	@echo "CC  " $@
	@${CC} -c ${CFLAGS} -I includes  -o $@ $<

%.o: %.cpp
	@echo "CPP " $@
	@${CPP} -c ${CPFLAGS} -I includes  -o $@ $<

arch/${ARCH}/${BOARD}/crti.o: arch/${ARCH}/${BOARD}/crti.s
	@echo "GAS " $@
	@${GAS} -o $@ $<
arch/${ARCH}/${BOARD}/crtn.o: arch/${ARCH}/${BOARD}/crtn.s
	@echo "GAS " $@
	@${GAS} -o $@ $<


all: lapis

lapis: ${FILES_COMPILE}
	@echo "Building Lapis..."
	@${LD} -T ${LD_LINKER_SCRIPT} -Iincludes/ -o ${KERNEL_OUTPUT} ${LD_FLAGS} ${FILES_COMPILE}
	@echo "Done"
clean:
	-@find . -name "*.o" -type f -delete

geniso:
	@cp ${KERNEL_OUTPUT} media/iso/lapis
	@${GENISO} ${GENISOF} -o cdrom.iso media/iso
	@rm media/iso/lapis