[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api033.nas"]

		GLOBAL	_api_getconscurx
[SECTION .text]

_api_getconscurx:		; int api_getconscurx(void);
		MOV		EDX,33
		INT		0x40
		RET
