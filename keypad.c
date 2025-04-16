#include "types.h"
#include "lob.h"
// C18 to XC8 Compiler Changes
//#include <p18f6722.h>				// Register definitions
#include <xc.h>				// Register definitions
// C18 to XC8 Compiler Changes
#include "diag.h"

//extern char dashes;				// when non-0 displays dashes in parameter indicated by bits 02/07/09 commented
extern unsigned long repeat;
extern ushort keyhold;
extern uchar lcd_updated;

// Legitimate values returns from keypad (membrane)
//	only one bit should be low at a time

// C18 to XC8 Compiler Changes
//const rom char rowbits[6] = { 0x3e, 0x3b ,0x3d, 0x37, 0x2f, 0x1f };
const char rowbits[6] = { 0x3e, 0x3b ,0x3d, 0x37, 0x2f, 0x1f };
// C18 to XC8 Compiler Changes

// Polls all columns for key pressed
//
char check_keypad(void)
{
	char rc = 0;
	char i, j;
	char col;
	char loop;
	static char odat=0;
	static ushort hold=0;
//	char odashes = dashes;								// 02/07/09 commented

	if (keyhold == 0U)	
		hold = 0;
	if (keyhold > hold) {															// only check keypad once per tick
		hold = keyhold;
		for (i=0,j=0,col=1; (col&0xff); col<<=1,i++) {	// cycle through all columns
			PORTE = col^0xff;															//	select column
			rc = PORTD & 0x3f;														//	read data from rows
			for (j=0; j<6; j++) {													//	if any single bit shows up
				if (rc == rowbits[j]) {
					break;																		//   quit looking
				}
			}
			if (j!=6) {																		// if found a key press
				rc = i*6 + j + 1;														//	turn row and column into key #
				break;																			//	and exit column loop
			} else if (rc != 0x3f) {											// else if more than one row on
				dumphex(col);																//	show because that is an error
				dumphex(rc);
				rc = 0;
				break;
			}
		}
		if (i == 8) {																		// otherwise no key pressed
			rc = 0;
			repeat = AR_OFF;															//	disable auto repeat functionality
//			dashes = 0;								// 02/07/09 commented
		}
		if (rc != odat) {																// if key pressed changed
			odat = rc;																		//	remember the last key pressed
			rc = 0;																				//	indicate nothing happened
			hold = 0;																			//  reset hold time
			keyhold = 0;
		} else {																				// else (same key pressed)
			if (hold < DEBOUNCE) {												//  if less than the minimum time
				rc = 0;																			//	 indicate nothing pressed yet
			} else if (hold == DEBOUNCE) {								//  if exactly the minimum time
				repeat = AR_1STREPEAT;											//	 set for intial 2 second repeat
			} else if (repeat == AR_REPEAT) {							//	if repeat count reached
				repeat = AR_AUTOREPEAT;											//	 use faster 1/2 second repeat
				rc = odat;																	//	 and send repetitive value 
			} else {																			//	if more than the minimum time
				rc = 0;																			//	 return no key detected yet (only return it once)
			}
		}
	} else {
		rc = 0;																					// do it here so odat is not affected
	}
// 02/07/09
// Commented the following line so now the dashes won't disappear until the flashing cursor requires a LCD update.
// This allows for the dashes to remain on the LCD for a longer time, depending on when the user stops pressing
// the + or - keys in relation to the cursor flashing.

//	if (odashes != dashes) lcd_updated = 1;

	return(rc);
}
