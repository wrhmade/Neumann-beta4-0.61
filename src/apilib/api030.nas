[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api030.nas"]

		GLOBAL	_api_getmousey

[SECTION .text]

_api_getmousey:		; int api_getmousey(void);
		MOV		EDX,30
		INT		0x40
		RET
