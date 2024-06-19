[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api034.nas"]

		GLOBAL	_api_getconscury
[SECTION .text]

_api_getconscury:		; int api_getconscury(void);
		MOV		EDX,34
		INT		0x40
		RET