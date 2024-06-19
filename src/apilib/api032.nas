[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api032.nas"]

		GLOBAL	_api_mouserightchick
[SECTION .text]

_api_mouserightchick:		; int api_mouserightchick(void);
		MOV		EDX,32
		INT		0x40
		RET
