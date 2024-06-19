[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api035.nas"]

		GLOBAL	_api_consbackspace
[SECTION .text]

_api_consbackspace:		; void api_consbackspace(void);
		MOV		EDX,35
		INT		0x40
		RET
