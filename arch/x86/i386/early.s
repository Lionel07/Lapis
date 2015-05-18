;==========================================================
; DATA
;----------------------------------------------------------
; Multiboot header

section .multiboot
MBALIGN 		equ  1<<0
MEMINFO 		equ  1<<1
FLAGS   		equ  MBALIGN | MEMINFO
MAGIC   		equ  0x1BADB002
CHECKSUM 		equ -(MAGIC + FLAGS)
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

;----------------------------------------------------------
; Stack

section .early_stack
align 4
kernel_earlyStackBottom:
times 8192 db 0
kernel_earlyStackTop:

;==========================================================

section .text

extern early_kernel_main
extern _init

global _start
_start:
	mov esp, kernel_earlyStackTop ; Setup stack

	push eax
	push ebx

	; Call Kernel Startup Parameters

	call early_kernel_main

kernel_forcequit:
	; Trigger kernel debugger here.
.kernel_forcequit_haltloop:
	cli
	hlt
	jmp .kernel_forcequit_haltloop