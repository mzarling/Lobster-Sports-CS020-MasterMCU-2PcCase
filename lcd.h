#ifndef __LCD_H
#define __LCD_H

// PIC18 LCD Routines.
//
//   Notes:
//      - These libraries routines are written to support the
//        Hitachi HD44780 LCD controller.
//      - The user must define the following items:
//          - The LCD interface type (4- or 8-bits)
//          - If 4-bit mode
//              - whether using the upper or lower nibble
//          - The data port
//              - The tris register for data port
//              - The control signal ports and pins
//              - The control signal port tris and pins
//          - The user must provide three delay routines:
//              - DelayFor18TCY() provides a 18 Tcy delay
//              - DelayPORXLCD() provides at least 15ms delay
//              - DelayXLCD() provides at least 5ms delay
//

// LCD_RW
#define LCD_READ 	1
#define LCD_WRITE	0

// LCD_RS
#define LCD_CMD		1
#define LCD_DATA	0

//LCD_E
#define LCD_ENA		1
#define LCD_DIS		0

// DATA_PORT defines the port to which the LCD data lines are connected
#define DATA_PORT      PORTF
#define TRIS_DATA_PORT TRISF

// CTRL_PORT defines the port where the control lines are connected.
// These are just samples, change to match your application.
//
#define RW_PIN   PORTAbits.RA2   /* PORT for RW */
#define TRIS_RW  DDRAbits.RA2    /* TRIS for RW */
#define RS_PIN   PORTAbits.RA1   /* PORT for RS */
#define TRIS_RS  DDRAbits.RA1    /* TRIS for RS */
#define E_PIN    PORTAbits.RA3   /* PORT for E  */
#define TRIS_E   DDRAbits.RA3    /* TRIS for E  */

/* Display ON/OFF Control defines */
#define DON         0b00001111  /* Display on      */
#define DOFF        0b00001011  /* Display off     */
#define CURSOR_ON   0b00001111  /* Cursor on       */
#define CURSOR_OFF  0b00001101  /* Cursor off      */
#define BLINK_ON    0b00001111  /* Cursor Blink    */
#define BLINK_OFF   0b00001110  /* Cursor No Blink */

/* Cursor or Display Shift defines */
#define SHIFT_CUR_LEFT    0b00010011  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT   0b00010111  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT   0b00011011  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT  0b00011111  /* Display shifts to the right */

/* Function Set defines */
#define EIGHT_BIT  0b00111111  /* 8-bit Interface               */
#define LINE_5X7   0b00110011  /* 5x7 characters, single line   */
#define LINE_5X10  0b00110111  /* 5x10 characters               */
#define LINES_5X7  0b00111011  /* 5x7 characters, multiple line */

#define PARAM_SCLASS auto
#define MEM_MODEL far  /* Change this to near for small memory model */

// OpenLCD
// Configures I/O pins for LCD
//
void OpenLCD(PARAM_SCLASS unsigned char);

// SetCGRamAddr
// Sets the character generator address
//
void SetCGRamAddr(PARAM_SCLASS unsigned char);

// SetDDRamAddr
// Sets the display data address
//
void SetDDRamAddr(PARAM_SCLASS unsigned char);

// BusyLCD
// Returns the busy status of the LCD
//
unsigned char BusyLCD(void);

// ReadAddrLCD
// Reads the current address
//
unsigned char ReadAddrLCD(void);

// ReadDataLCD
// Reads a byte of data
//
char ReadDataLCD(void);

// WriteCmdLCD
// Writes a command to the LCD
//
void WriteCmdLCD(PARAM_SCLASS unsigned char);

// WriteDataLCD
// Writes a data byte to the LCD
//
void WriteDataLCD(PARAM_SCLASS char);

// putcLCD
// A putc is a write
//
#define putcLCD WriteDataLCD

// putsLCD
// Writes a string of characters to the LCD
//
void putsLCD(PARAM_SCLASS char *);

// putrsLCD
// Writes a string of characters in ROM to the LCD
//
void putrsLCD(PARAM_SCLASS const MEM_MODEL rom char *);

// User defines these routines according to the oscillator frequency */
extern void DelayFor18TCY(void);
extern void DelayPORXLCD(void);
extern void DelayXLCD(void);
extern void set_intensity(void);
#endif //__LCD_H
