[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api029.nas"]

		GLOBAL	_api_getmousex

[SECTION .text]

_api_getmousex:		; int api_getmousex(void);
		MOV		EDX,29
		INT		0x40
		RET
