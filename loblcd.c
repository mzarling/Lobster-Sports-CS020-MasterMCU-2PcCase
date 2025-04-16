// C18 to XC8 Compiler Changes
// Replaced all strcpypgm2ram with strcpy.
// C18 to XC8 Compiler Changes

// v1.209 start
//#pragma code STUFF
// v1.209 end

// Lobster LCD Routines
//
#include "types.h"
// C18 to XC8 Compiler Changes
//#include "p18f6722.h"
#include <xc.h>
// C18 to XC8 Compiler Changes
//#include "state_mach.h"
//#include "usart.h"
#include <string.h>
// C18 to XC8 Compiler Changes
//#include "lcd.h"
//#include "c:\mcc18\h\spi.h"
// C18 to XC8 Compiler Changes
#include "lob.h"


// C18 to XC8 Compiler Changes
//rom char *null_str =         "                ";
const char *null_str =         "                ";
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//char lcd_line[4][17] = {  
unsigned char lcd_line[4][17] = {  
// C18 to XC8 Compiler Changes
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0, 
};

extern uchar lcd_updated;
extern char *state_name[];
void ChipSelect(int chan);
void ChipDeSelect(int chan);

void set_intensity(void)
{
//		OpenSPI(SPI_FOSC_16, MODE_00, SMPEND);			
		ChipSelect(LOB_POT);
		WriteSPI(0x11);
		WriteSPI(0xe4);
		ChipDeSelect(LOB_POT);
//		CloseSPI();
}

// Clears to spaces all lines requested
//  mask - bitmap of lines to clear (Bit 0 set = clear line 0, Bit 1 ... line 1, etc.)
//
void clear_lcd(char mask)
{
  char i, line;
  
  for (i=0, line=1; i<4; i++,line<<=1) {  // check all four lcd lines
    if (line & mask) {                    // if bit set
      strcpy(&lcd_line[i][0],null_str);   //  clear that line
    }
  }
  lcd_updated = 1;
}

//#ifdef notdef
//const rom char line_start[4] = { 0x00, 0x40, 0x10, 0x50 };
//
//// Show LCD Lines
//void show_lcd(void)
//{
//  int i;
//  
//  for (i=0; i<4; i++) {
//		while (BusyLCD());
//		SetDDRamAddr(line_start[i]);
//		while (BusyLCD());
//	  putsLCD(&lcd_line[i][0]);
//  }
//  lcd_updated = 0;
//}
//#endif // notdef
