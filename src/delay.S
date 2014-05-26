#include <avr/io.h> 
#define	mpr	R16
.global W100us
.global W1ms
.global W10ms
.global W100ms
.global W1s
; 4Mhz -> 97 	8Mhz -> 197
W100us:		PUSH	mpr
			NOP

			LDI		mpr, 97
W100usL:	NOP
			DEC		mpr
			BRNE	W100usL

			POP		mpr
			RET


W1ms:		PUSH	mpr
			LDI		mpr, 9

W1msL:		RCALL	W100us
			DEC		mpr
			BRNE	W1msL

			RJMP	WEnd

W10ms:		PUSH	mpr
			LDI		mpr, 9

W10msL:		RCALL	W100us
			RCALL	W1ms
			DEC		mpr
			BRNE	W10msL

			RJMP	WEnd

W100ms:		PUSH	mpr
			LDI		mpr, 9

W100msL:	RCALL	W100us
			RCALL	W1ms
			RCALL	W10ms
			DEC		mpr
			BRNE	W100msL

			RJMP	WEnd

W1s:		PUSH	mpr
			LDI		mpr, 9

W1sL:		RCALL	W100us
			RCALL	W1ms
			RCALL	W10ms
			RCALL	W100ms
			DEC		mpr
			BRNE	W1sL

			RJMP	WEnd

WXms:		PUSH	mpr
			LDI		mpr, 40
			
			RCALL	WMPRms

			RJMP	WEnd


WEnd:		RCALL	Padding

			POP		mpr
			RET


WMPRms:		

WMPRmsL:	RCALL	W1ms
			DEC		mpr
			BRNE	WMPRmsL
			
			RET

; 4Mhz -> 87 	8Mhz -> 187
Padding:	PUSH	mpr
			NOP
			LDI		mpr, 87
			

PaddingL1:	NOP
			DEC		mpr
			BRNE	PaddingL1

			POP 	mpr
			RET
