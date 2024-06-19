[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_updatemouse

[SECTION .text]

_api_updatemouse:		; int api_updatemouse(int status);
		MOV		EDX,28
		MOV		EAX,[ESP+4]	; status
		INT		0x40
		RET
