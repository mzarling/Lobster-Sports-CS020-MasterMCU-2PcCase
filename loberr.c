// Error handling routines
//
//	(C)2007 Censerve(R) Consulting, Inc.
//					All rights reserved.
//

// C18 to XC8 Compiler Changes
// Replaced all strcpypgm2ram with strcpy.
// C18 to XC8 Compiler Changes


#include "types.h"
#include "state_mach.h"
#include <string.h>
#include "slave.h"
//#include "usart.h"
// C18 to XC8 Compiler Changes
//#include "lcd.h"
//#include "p18f6722.h"
#include <xc.h>
//#include "stdlib.h"
// C18 to XC8 Compiler Changes
#include "diag.h"
#include "lob.h"


void stop_all(int code, long *args);

extern char lcd_updated;

// C18 to XC8 Compiler Changes
//extern char lcd_line[][17];
extern char lcd_line[][17];
// C18 to XC8 Compiler Changes

extern struct moterr ErrorStat;
extern ushort CurShotState;

// C18 to XC8 Compiler Changes
// 11/04/09 start
//		rom char* lowbatt_msg = "Battery Depleted";
//		rom char* lowbatt_msg;
// 11/04/09 end
//		rom char* turnoff_msg = "Please Turn Off ";
//		rom char* machine_msg = "    Machine     ";
// 
//		rom char* error_msg		= "**** Error **** ";
//		rom char* overcur_msg	= "  Over-Current  ";
//		rom char* feedbk_msg	= "    Feedback    ";
//		rom char* period_msg	= "  Invalid Data  ";
//
//rom char* motormsg[] = {
//	"Top Serve Motor ","Bot Serve Motor ","   Feed Motor   ","  Sweep Motor   ",
//	"Elevation Motor ",0,0,0,
//	0,0,0,0,
//	0,0,0,0
//};

//  C18 to XC8 Compiler Changes
//		char* lowbatt_msg;
//		char* turnoff_msg = "Please Turn Off ";
//		char* machine_msg = "    Machine     ";
 
//		char* error_msg		= "**** Error **** ";
//		char* overcur_msg	= "  Over-Current  ";
//		char* feedbk_msg	= "    Feedback    ";
//		char* period_msg	= "  Invalid Data  ";
		const char* lowbatt_msg;
		const char* turnoff_msg = "Please Turn Off ";
		const char* machine_msg = "    Machine     ";
 
		const char* error_msg		= "**** Error **** ";
		const char* overcur_msg	= "  Over-Current  ";
		const char* feedbk_msg	= "    Feedback    ";
		const char* period_msg	= "  Invalid Data  ";
//  C18 to XC8 Compiler Changes

//  C18 to XC8 Compiler Changes
//char* motormsg[] = {
const char* motormsg[] = {
//  C18 to XC8 Compiler Changes
	"Top Serve Motor ","Bot Serve Motor ","   Feed Motor   ","  Sweep Motor   ",
	"Elevation Motor ",0,0,0,
	0,0,0,0,
	0,0,0,0
};

// C18 to XC8 Compiler Changes

// Display error messages
//
void lob_error(void)
{
	signed char code = -1;
	
	if (ErrorStat.LoBat) {
		code = 0;
		stop_all(0,(long*)0);
	} else if (ErrorStat.TopPeriod) {				//	3 - set if invalid top serve motor period
		code = 0x30;
	} else if (ErrorStat.BotPeriod) {				//	4	- set if invalid bottom serve motor period
		code = 0x31;
	} else if (ErrorStat.TopCurSense) {			//  5 - set if top serve motor over current
		code = 0x10;
	} else if (ErrorStat.BotCurSense) {			//  6 - set if bottom serve motor over current
		code = 0x11;
	} else if (ErrorStat.FeedCurSense) {		//  7 - set if feed motor over current
		code = 0x12;
	} else if (ErrorStat.SweepCurSense) {		//  8 - set if sweep motor over current
		code = 0x13;
	} else if (ErrorStat.ElevCurSense) {		//  9 - set if elevation motor over current
		code = 0x14;
	} else if (ErrorStat.TopCnt) {					// 10 - set if top serve motor is not getting timer intr
		code = 0x20;
	} else if (ErrorStat.BotCnt) {					// 11 - set if bot serve motor is not getting timer intr
		code = 0x21;
	} else if (ErrorStat.SweepCnt) {				// 13 - set if sweep motor is not getting decoder counts
		code = 0x23;
	} else if (ErrorStat.ElevCnt) {					// 14 - set if elevation motor is not getting decoder counts
		code = 0x24;
	}
	if (code >= 0) {
		PUTRSDIAG("loberr:");
		DUMPHEX(code);
		CurShotState = SHOT_IDLE;
	}
	switch ((code >> 4) & 15) { 
	case 0:		// low battery
		strcpy(&lcd_line[0][0], lowbatt_msg);
		strcpy(&lcd_line[2][0],&turnoff_msg[0]);
		strcpy(&lcd_line[3][0],&machine_msg[0]);
		clear_lcd(0x2);
		lcd_updated = 1;
		break;
	case 1:	 // overcurrent
		strcpy(&lcd_line[0][0],error_msg);
		strcpy(&lcd_line[2][0],overcur_msg);
		strcpy(&lcd_line[3][0],motormsg[code&15]);
		clear_lcd(0x2);
		lcd_updated = 1;
		break;
	case 2:	// feedback
		strcpy(&lcd_line[0][0],error_msg);
		strcpy(&lcd_line[2][0],feedbk_msg);
		strcpy(&lcd_line[3][0],motormsg[code&15]);
		clear_lcd(0x2);
		lcd_updated = 1;
		break;
	case 3: //feedback
		strcpy(&lcd_line[0][0],error_msg);
		strcpy(&lcd_line[2][0],period_msg);
		strcpy(&lcd_line[3][0],motormsg[code&15]);
		clear_lcd(0x2);
		lcd_updated = 1;
		break;
	}
}
