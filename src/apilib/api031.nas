[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api031.nas"]

		GLOBAL	_api_mouseleftchick
[SECTION .text]

_api_mouseleftchick:		; int api_mouseleftchick(void);
		MOV		EDX,31
		INT		0x40
		RET
