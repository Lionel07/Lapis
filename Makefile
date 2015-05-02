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
CFLAGS	:= -ffreestanding -std=gnu99 -nostartfiles -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Os -s
LD		:= ${CC}
LD_FLAGS:= -ffreestanding -O2 -nostdlib -lgcc -s

GENISO 	:= genisoimage
GENISOF	:= -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table
#----------------------------------------------------------
# Precalculated variables
LD_LINKER_SCRIPT := arch/${ARCH}/${BOARD}/link.ld
#----------------------------------------------------------
# All Source Files
FILES_KERNEL	:= $(patsubst %.c,%.o,$(wildcard kernel/*.c))
FILES_INIT  	:= $(patsubst %.c,%.o,$(wildcard init/*.c))
FILES_LIB   	:= $(patsubst %.c,%.o,$(wildcard lib/*.c))
FILES_LOG   	:= $(patsubst %.c,%.o,$(wildcard log/*.c))
FILES_DRIVERS	:= $(patsubst %.c,%.o,$(wildcard drivers/*.c))

FILES_ARCH  	:= $(patsubst %.c,%.o,$(wildcard arch/${ARCH}/*.c)) $(patsubst %.s,%.o,$(wildcard arch/${ARCH}/*.s))
FILES_BOARD 	:= $(patsubst %.c,%.o,$(wildcard arch/${ARCH}/${BOARD}/*.c)) $(patsubst %.s,%.o,$(wildcard arch/${ARCH}/${BOARD}/*.s))

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
	@${CC} -c ${CFLAGS} -Iincludes/  -o $@ $<

arch/${ARCH}/${BOARD}/crti.o: arch/${ARCH}/${BOARD}/crti.s
	@echo "GAS " $@
	@${GAS} -o $@ $<
arch/${ARCH}/${BOARD}/crtn.o: arch/${ARCH}/${BOARD}/crtn.s
	@echo "GAS " $@
	@${GAS} -o $@ $<


all: lapis

lapis: ${FILES_COMPILE}
	@echo "Building Lapis..."
	@${LD} -T ${LD_LINKER_SCRIPT} -o ${KERNEL_OUTPUT} ${LD_FLAGS} ${FILES_COMPILE}
	@echo "Done"
clean:
	-@find . -name "*.o" -type f -delete

geniso:
	@cp ${KERNEL_OUTPUT} media/iso/lapis
	@${GENISO} ${GENISOF} -o cdrom.iso media/iso
	@rm media/iso/lapis