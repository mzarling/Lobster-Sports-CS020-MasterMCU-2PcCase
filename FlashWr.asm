///////////////////
//#include "p18f6722.inc"
#include <xc.inc>
///////////////////

;    EXTERN  CodeAddrU,CodeAddrH,CodeAddrL
;	EXTERN	DataAddrH,DataAddrL
;	EXTERN	Counter

;;;;;;;;;;;;;;;;;;;
;FLASHWRCODE CODE
FLASHWRCODE CODE
;;;;;;;;;;;;;;;;;;;

;/********************************************************************
;*   Function Name: FlashRd                                          *
;*   Return Value:  void                                             *
;*   Parameters:    unit:                                            *
;*   Description:   This routine read 64 bytes of data from flash    *
;*                  address CodeAddrU:CodeAddrH:CodeAddrL to memory  *
;*                  location DataAddrH:DataAddrL to                  *
;********************************************************************/

FlashRd

	bcf		INTCON,GIE						; disable interrupts

	movf	TBLPTRU, W						; save old TBLPTR
	movwf	OldCAU
	movf	TBLPTRH, W						; save old TBLPTR
	movwf	OldCAH
	movf	TBLPTRL, W						; save old TBLPTR
	movwf	OldCAL

	movf	CodeAddrU, W					; load TBLPTR with the base
	movwf	TBLPTRU							; address of the memory block
	movf	CodeAddrH, W
	movwf	TBLPTRH
	movf	CodeAddrL, W
	movwf	TBLPTRL

;load holding registers with data
	movf	DataAddrH,W						; point to ram data to write
	movwf	FSR0H
	movf	DataAddrL,W
	movwf	FSR0L
	movlw	0x40							; number of bytes in holding register
	movwf	Counter
ReadBlock
	tblrd*+									; read into TABLAT, and inc
	movf	TABLAT, W						; get data
	movwf	POSTINC0						; store data
	decfsz	Counter							; done?
	bra		ReadBlock

	movf	OldCAU,W						; restore TBLPTR
	movwf	TBLPTRU
	movf	OldCAH,W
	movwf	TBLPTRH
	movf	OldCAL,W
	movwf	TBLPTRL

	bsf		INTCON, GIE						; re-enable interrupts
    return



;/********************************************************************
;*   Function Name: FlashWr                                          *
;*   Return Value:  void                                             *
;*   Parameters:    unit:                                            *
;*   Description:   This routine writes 64 bytes of data from memory *
;*                  location DataAddrH:DataAddrL to                  *
;*                    CodeAddrU:CodeAddrH:CodeAddrL                  *
;********************************************************************/

FlashWr

	bcf		INTCON, GIE						; disable interrupts
	movf	TBLPTRU, W						; save old TBLPTR
	movwf	OldCAU
	movf	TBLPTRH, W						; save old TBLPTR
	movwf	OldCAH
	movf	TBLPTRL, W						; save old TBLPTR
	movwf	OldCAL

	movf	CodeAddrU,W						; load TBLPTR with the base
	movwf	TBLPTRU							; address of the memory block
	movf	CodeAddrH,W
	movwf	TBLPTRH
	movf	CodeAddrL,W
	movwf	TBLPTRL

;load holding registers with data
	tblrd*-									; dummy read decrement, point to byte before flash code addr
	movf	DataAddrH,W						; point to ram data to write
	movwf	FSR0H
	movf	DataAddrL,W
	movwf	FSR0L
	movlw	0x40							; number of bytes in holding register
	movwf	Counter
Write2Hregs
	movff	POSTINC0, WREG					; get low byte of buffer data
	movwf	TABLAT							; present data to table latch
	tblwt+*									; pre-incr pointer, write data, perform a short write
											; to internal TBLWT holding register.
	DECFSZ	Counter							; loop until buffers are full
	bra		Write2Hregs

;write data to flash
	bsf		EECON1, EEPGD					; point to Flash Program Memory
	bcf		EECON1, CFGS					; access Flash program memory
	bsf		EECON1, WREN					; enable write to memory
;	bcf		INTCON, GIE						; disable interrupts
	; start required sequence for write
	movlw	0x55
	movwf	EECON2							; write 55h
	movlw	0xaa	
	movwf	EECON2							; write 0aah
	bsf		EECON1, WR						; start program (CPU stall)
	; end required sequence for write	
	bsf		INTCON, GIE						; re-enable interrupts

	movf	OldCAU,W						; restore TBLPTR
	movwf	TBLPTRU
	movf	OldCAH,W
	movwf	TBLPTRH
	movf	OldCAL,W
	movwf	TBLPTRL

	bcf		EECON1, WREN					; disable write to memory
    return


;/********************************************************************
;*   Function Name: FlashEr                                          *
;*   Return Value:  void                                             *
;*   Parameters:    unit:                                            *
;*   Description:   This routine erase a block of 64 bytes of flash  *
;*                  program memory, then writes 64 bytes of data     *
;*                  from memory.                                     *
;********************************************************************/

FlashEr

	bcf		INTCON,GIE						; disable interrupts
	movf	TBLPTRU, W						; save old TBLPTR
	movwf	OldCAU
	movf	TBLPTRH, W						; save old TBLPTR
	movwf	OldCAH
	movf	TBLPTRL, W						; save old TBLPTR
	movwf	OldCAL

;erase 1 block of flash
	movf	CodeAddrU,W						; load TBLPTR with the base
	movwf	TBLPTRU							; address of the memory block
	movf	CodeAddrH,W
	movwf	TBLPTRH
	movf	CodeAddrL,W
	movwf	TBLPTRL
	bsf		EECON1,EEPGD					; point to flash program memory
	bcf		EECON1,CFGS						; access Flash program memory
	bsf		EECON1,WREN						; enable write to memory
	bsf		EECON1,FREE						; enable Row Erase operation
;	bcf		INTCON,GIE						; disable interrupts
	; start required sequence for erase
	movlw	0x55
	movwf	EECON2							; write 55h
	movlw	0xaa
	movwf	EECON2							; write AAh
	bsf		EECON1,WR						; start erase (CPU stall)
	; end required sequence for erase
	bcf		EECON1,WREN						; disable write to memory

	movf	OldCAU,W						; restore TBLPTR
	movwf	TBLPTRU
	movf	OldCAH,W
	movwf	TBLPTRH
	movf	OldCAL,W
	movwf	TBLPTRL

	bsf		INTCON,GIE						; re-enable interrupts

	return

    GLOBAL  FlashEr
    GLOBAL  FlashWr
	GLOBAL	FlashRd

FLASHDAT1      UDATA_ACS

CodeAddrU	RES 1
CodeAddrH	RES 1
CodeAddrL	RES 1
DataAddrH	RES 1
DataAddrL	RES	1
Counter		RES	1
OldCAU		RES	1
OldCAH		RES	1
OldCAL		RES	1

    GLOBAL  CodeAddrU,CodeAddrH,CodeAddrL
	GLOBAL	DataAddrH,DataAddrL
	GLOBAL	Counter
	GLOBAL	OldCAU,OldCAH,OldCAL

    END

