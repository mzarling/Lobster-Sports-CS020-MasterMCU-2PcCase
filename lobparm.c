// Lobster Parameter processing 
//
// (C)2007 Censerve Consulting, Inc.
//				 All rights reserved.
//

// C18 to XC8 Compiler Changes
// Replaced all strcpypgm2ram with strcpy.
// Replaced all strncpypgm2ram with strncpy.
// Replaced all strlenpgm with strlen.
// C18 to XC8 Compiler Changes

#include "types.h"
#include <string.h>
#include "lob.h"
//#include "usart.h"
#include "diag.h"

// C18 to XC8 Compiler Changes
#include <delays.h>
// C18 to XC8 Compiler Changes


extern union parm preset[NUMPRESETS];	// where we store the values for each of the preset parameters
extern char man_parm;
// C18 to XC8 Compiler Changes Start
//extern char preset_type;								// sub mode for presets (e.g. random horiz, etc)
extern unsigned char preset_type;								// sub mode for presets (e.g. random horiz, etc)
// C18 to XC8 Compiler Changes End
extern uchar play_state;

// C18 to XC8 Compiler Changes
//extern const rom struct rand_info pre_rand[];
extern const struct rand_info pre_rand[];
extern void abort_shot(void);
// C18 to XC8 Compiler Changes

extern long play_mode;
extern uchar lcd_updated;// = 0;

// C18 to XC8 Compiler Changes
//extern char lcd_line[][17];
extern char lcd_line[][17];
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];
// C18 to XC8 Compiler Changes

// v1p195 start
extern char FirstShot;
// v1p195 end

// 07/29/09 start
extern unsigned char model_type;
// 07/29/09 end


// v1.208 start
// v1.219 start

// C18 to XC8 Compiler Changes
//#pragma idata parameter_text = 0x600
// C18 to XC8 Compiler Changes

//#pragma romdata parameter_text
// v1.219 end
// This table is now larger than 256 bytes.
// It can only be accessed through pointers, not directly.
// Required a change to the linker script to increase the size of the data bank.
// v1.208 end
// Parameter format strings
//  two for each parameter: normal and highlighted

// C18 to XC8 Compiler Changes
//const rom char *parm_fmt[][2] = {
const char *parm_fmt[][2] = {
// C18 to XC8 Compiler Changes

//   0123456789012345     0123456789012345
  { "SPEED:      MPH ",  "SPEED: >    MPH<" },
//  { "ELEV:       DEG ",  "ELEV:  >    DEG<" },
  { "ELEV:           ",  "ELEV:  >    <   " },
// 07/03/09 start
//  { "SPIN    ",  				 "SPIN>  <" },
//  { "ANGLE   ", 				 "ANGLE>  " },
  { "SPIN:   ",  				 "SPIN>  <" },
  { "ANGLE:  ", 				 "ANGLE>  " },
  { "        ",					 ">      <" },	// Row selection in 2-Line mode.
// 07/03/09 end
  { "FEED:       SEC ",  "FEED:  >    SEC<" },
//  { "SPIN    ",  				 "SPIN>  <" },
//  { "                ",  ">              <" },
// 07/03/09 start
//	{ "RND SWEEP       ",  "RND SWEEP>     <" },
//	{ "RND DEPTH       ",  "RND DEPTH>     <" },
//	{ "2-LINE          ",  "2-LINE >       <" },
	{ "RND SWEEP:      ",  "RND SWEEP>     <" },
	{ "RND DEPTH:      ",  "RND DEPTH>     <" },
	{ "2-LINE:         ",  "2-LINE:>       <" },
// 07/03/09 end
	{ "FULLY RANDOM    ",  "FULLY RANDOM  ><" },
	{ "GRINDER         ",	 "GRINDER       ><" },
	{ "POWER BASELINER ",  "POWER BASELINE><" },
	{ "ALL-COURTER     ",  "ALL-COURTER   ><" },
//	{ "TRANSITION      ",	 "TRANSITION    ><" }, //12			// mjz 11/03/08, commented
	{ "APPROACH        ",	 "APPROACH      ><" }, //12			// mjz 11/03/08
	{ "ATTACK/DEFEND   ",  "ATTACK/DEFEND ><" }, //13
//	{ "FOREHAND ONLY   ",  "FOREHAND ONLY ><" }, //14			// mjz 11/03/08, commented
	{ "FOREHAND PLUS   ",  "FOREHAND PLUS ><" }, //14			// mjz 11/03/08
// v1.208 start
	{ "BEGINNER        ",  "BEGINNER      ><" },
	{ "INTERMEDIATE    ",  "INTERMEDIATE  ><" },
	{ "ADVANCED        ",  "ADVANCED      ><" },
	{ "MOONBALLER      ",  "MOONBALLER    ><" },
	{ "SLICER          ",  "SLICER        ><" },
	{ "LEFTY           ",  "LEFTY         ><" },
// v1.208 end
	{ "INVALID         ",  "INVALID         " },
};

// C18 to XC8 Compiler Changes
//extern const rom char preparm1base[];
extern const char preparm1base[];
// C18 to XC8 Compiler Changes

//#ifndef	M4	// if EL04 not defined

// C18 to XC8 Compiler Changes
//const rom char* preset_sub[] = {
const char* preset_sub[] = {
// C18 to XC8 Compiler Changes

		"LINE 1","LINE 2","LINE 3","LINE 4","LINE 5","LINE 6",
// v1.182 start
//		"ROW A", "ROW B", "ROW C",
// v1.190 start
//		"ROW A", "ROW C", "ROW B",
		"ROW A", "RANDOM", "ROW C", "ROW B",
// V1.182 END
// v1.190 end
		" WIDE ","MEDIUM","NARROW",
		"   SPIN -3   ",	"   SPIN -2   ",	"   SPIN -1   ",	"   SPIN 0    ",	"   SPIN +1   ",	"   SPIN +2   ",	"   SPIN +3   ",
// 07/07/09 start
// 07/03/09 start
//		"A", "B", "C",
// 07/03/09 end
// v1.182 start
//		"ROW: A", "ROW: B", "ROW: C", "RANDOM",
		"ROW: A", "RANDOM", "ROW: C", "ROW: B",
// V1.182 END
// 07/07/09 end
};
//#endif

//#ifdef M4
// ELO4CHG #5 (start) 
//const rom char* preset_sub[] = {

// C18 to XC8 Compiler Changes
//const rom char* preset_sub2[] = {
const char* preset_sub2[] = {
// C18 to XC8 Compiler Changes

		//"LINE 1","LINE 2","LINE 3","LINE 4","LINE 5","LINE 6",
		" LEFT ","","CENTER","","","RIGHT ",
		//"ROW A", "ROW B", "ROW C",
// v1.182 start
//		"DEEP", "MID", "SHORT",
// v1.190 start
//		"DEEP", "SHORT", "MID",
		"DEEP", "RANDOM", "SHORT", "MID",
// v1.182 end
// v1.190 end
		" WIDE ","MEDIUM","NARROW",
		"   SPIN -3   ",	"   SPIN -2   ",	"   SPIN -1   ",	"   SPIN 0    ",	"   SPIN +1   ",	"   SPIN +2   ",	"   SPIN +3   ",
// v1.189 start
		"ROW: A", "RANDOM", "ROW: C", "ROW: B",
// v1.189 end
};
// ELO4CHG #5 (end) 
//#endif

//=============================================================================
// 07/30/09 start
// v1.219 start
//#pragma idata parameters = 0x500

// C18 to XC8 Compiler Changes
//#pragma idata parameters = 0x500
// C18 to XC8 Compiler Changes

// v1.219 end
// 07/30/09 end
//=============================================================================

struct t_parm parameters[] = {
	// limits/incr	display text			fmt pos  sz st ln 
	{ SPEED_LIMITS, &parm_fmt[SPEED_IX][0],  2,  9, 16, 0, 0 },	// limits, format strings, formatting, location
	{ ELEV_LIMITS,	&parm_fmt[ELEV_IX][0],   2,  9, 16, 0, 0 },
	{ SPIN_LIMITS,	&parm_fmt[SPIN_IX][0],  -1,  5,  8, 0, 2 },
	{ SWEEP_LIMITS, &parm_fmt[HORIZ_IX][0], -1, 14,  8, 8, 2 },	// manual mode angle selection
// 07/07/09 start
// 07/03/09 start
//	{ TWOLNRW_LIMITS, &parm_fmt[TWOLNRW_IX][0],STR_FMT, 14, 8, 8, 2 },
	{ TWOLNRW_LIMITS, &parm_fmt[TWOLNRW_IX][0],STR_FMT, 9, 8, 8, 2 },
// 07/03/09 end
// 07/07/09 end
	{ FEED_LIMITS,	&parm_fmt[FEED_IX][0], 2, 9, 16, 0, 0 },
//	{ SPIN_LIMITS,	&parm_fmt[4][0], -1, 5,  8, 0, 2 },
//	{ SPIN_LIMITS,	&parm_fmt[4][0], SPN_FMT, 1 },
	{ ROW_LIMITS,	&parm_fmt[ROW_IX][0],STR_FMT,10,16, 0, 0 },
	{ COL_LIMITS,	&parm_fmt[COL_IX][0],STR_FMT,10,16, 0, 0 },
	{ TWOLN_LIMITS,	&parm_fmt[WID_IX][0],STR_FMT, 9,16, 0, 0 },
	{ FULL_LIMITS,	&parm_fmt[8][0], 0, 9, 16, 0, 0 },	// From here,
	{ FULL_LIMITS,  &parm_fmt[9][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[10][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[11][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[12][0], 0, 9, 16, 0, 0 },	// I seems like parm_fmt should be increased by 1 to match parm_fmt above,
	{ FULL_LIMITS,	&parm_fmt[13][0], 0, 9, 16, 0, 0 },	// but appears that parm_fmt may not be used here for these lines.
	{ FULL_LIMITS,	&parm_fmt[14][0], 0, 9, 16, 0, 0 },	//
// v1.208 start
	{ FULL_LIMITS,	&parm_fmt[15][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[16][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[17][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[18][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[19][0], 0, 9, 16, 0, 0 },	//
	{ FULL_LIMITS,	&parm_fmt[20][0], 0, 9, 16, 0, 0 },	// to here.
// v1.208 end
};

// C18 to XC8 Compiler Changes/
//const rom char speed_trans[] = {
const char speed_trans[] = {
// C18 to XC8 Compiler Changes/

// Actual mph 0   5  10  15  20  25  30  35  40  45  50  55  60  65  70  75
///* Display */	0,	5, 10, 15, 20, 30, 35, 40, 50, 55, 60, 65, 70, 75, 80, 90	// mjz 11/04/08, commented
/* Display */	0,	15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 90	// mjz 11/04/08
};

// C18 to XC8 Compiler Changes
//char is_available(char locno, uchar spd, char spn);
//char isLocAvail(char locNo, uchar spd, char spn);
char is_available(char locno, uchar spd, signed char spn);
char isLocAvail(char locNo, uchar spd, signed char spn);
// C18 to XC8 Compiler Changes

char dashes = 0;
// C18 to XC8 Compiler Changes
//char ond = 0x8f;		// not used
// C18 to XC8 Compiler Changes

// Convert a signed character into an ascii string
//	c	- signed character to be converted
//	cp	- buffer in which to place ascii string
//	ndig- number of decimal digits to place in buffer (negative - add sign character)
//

// C18 to XC8 Compiler Changes
//void chartoa(const char c, char* cp, char ndig) 
void chartoa(const signed char c, char* cp, signed char ndig) 
// C18 to XC8 Compiler Changes

{

// C18 to XC8 Compiler Changes
//	char x = c;						// do not modify original value
	signed char x = c;				// do not modify original value
// C18 to XC8 Compiler Changes

	if ((ndig > 15) && !(ndig & NEG_NUM)) {		// if value indicates string needed
		if (spin_format(ndig))									//	if spin formatting is required
			x = c + 3;														//	 character is 
		else																		//	else (any other formatting)
// 07/03/09 start
//			x = c & 15;														//	 process lsdigit
			x = c;	// above line masked out new 2-Line row selection text in preset_sub[]
// 07/03/09 end
// v1.199 start
//		if (model_type != EL04)								// for all models except EL04
		if ((model_type != EL04) && (model_type != CLUB1))	// not for all models with EL04 control panel
// v1.199 end

// C18 to XC8 Compiler Changes
//			strncpypgm2ram(cp,(rom char*)preset_sub[x],strlenpgm((rom char*)preset_sub[x]));	
//		else					// for EL04 model only
//			strncpypgm2ram(cp,(rom char*)preset_sub2[x],strlenpgm((rom char*)preset_sub2[x]));	
			strncpy(cp,(char*)preset_sub[x],strlen((char*)preset_sub[x]));	
		else					// for EL04 model only
			strncpy(cp,(char*)preset_sub2[x],strlen((char*)preset_sub2[x]));	
// C18 to XC8 Compiler Changes

	} else {
		if (ndig < 0) {				// if negative number of digits (render as signed)
		  if (x < 0) { 				//	if character is negative
				*cp++ = '-';			//	 add minus sign to output
				x = -x;						//	 make character into a positive number
		  } else if (x) {			//	else if character is non-zero
				*cp++ = '+';			//	 add plus sign to output
	  	} else {						//	else (positive non-zero)
		  	*cp++ = ' ';			//	 add space
			}
		  ndig = -ndig;				//	make # digits desired into a positive number
		}
		if (ndig > 3) {				// allow a maximum of 3 digits
	  	*cp++ = '?';				//  rendering number as ? if too many specified
	 	} else {
 		  cp += ndig - 1;			// reposition to end of string
 	  	while (ndig--) {		// recursively add digits starting w/least significant
				*cp-- = (x % 10) + '0';	//  render as ascii decimal digit
				x = x / 10;				//	eliminate rendered digit
  	  }
		}
	}
}

// Process Lobster Parameter UI
//	cmd		- button pressed (bit 7 is the first pass flag for the current state)
//	pparm	- pointer to parameter list
//	pix		- pointer to current parameter index
//	mask	- mask of allowed parameter indices
//
//char lob_parm(char cmd, uchar *pix, char* pparm, ushort mask) {
// C18 to XC8 Compiler Changes
//char lob_parm(char cmd, uchar *pix, char* pparm, unsigned short long mask) {
char lob_parm(char cmd, uchar *pix, signed char* pparm, unsigned short long mask) {
// C18 to XC8 Compiler Changes
//	ushort i, cmsk, tmsk;
	ushort i;
	unsigned short long cmsk, tmsk;
	ushort ln,j, k;

// C18 to XC8 Compiler Changes
//	char newval;
	signed char newval;
// C18 to XC8 Compiler Changes

	ushort premode;
	char botMenu = (mask < PRE_RSWP) ? FEED_IX : (FEED_IX+1);	// invalid index - to allow bottom wrap
	
//	if (model_type != EL05LE)					// Currently, only EL05LE supports row selection in 2-Line mode,
//	if ((model_type != EL05LE) && (model_type != CLUB))		// Currently, only EL05LE and CLUB models support row selection in 2-Line mode,
//	if ((model_type != EL05LE) && (model_type != CLUB) && (model_type != EL05LE2)) // Currently, only EL05LE, CLUB, EL05LE2 models support row selection in 2-Line mode,
//	if (model_type == EL04) // all models except EL04 now support 2-Line row selection
//		mask = mask & ~(ushort)PRE_TWOLNRW;		// so mask it out for other models.
//	tmsk = mask & 0x7fffU;														// prevent menu up/dn from cycling to fully random line
// When 2-Line row selection was added, all mask bits were required. The above line prevented the last preset drill from displaying.
	tmsk = mask;
	cmd = cmd & 0x7f;																// extract button value if any
	if (cmd == KY_MENUDN) {       						   		// if MENU DOWN button pressed
		if ((mask & 255) == PRE_FEED)									//	if only the one item on the menu 
			return(0);																	//	 ignore attempts to select another item
		dashes = 0;
		lcd_updated = 1;															//	display has changed
		if (*pix >= botMenu) { // > FEED_IX) {				//	if past last parameter on page
			*pix = -1;																	//	 start with invalid parameter # (low)
			do {		
				(*pix)++;																	//	 find lowest valid parameter
//			} while (!(tmsk & (1 << *pix)));
			} while (!(tmsk & (1UL << *pix)));
			dumphex(*pix);
		} else if (tmsk >> (*pix + 1)) {							//	if any allowed higher index
			do {		
				(*pix)++;																	//	 increment index
//			} while (!(tmsk & (1 << *pix)));						//	 until hit next allowed index 
			} while (!(tmsk & (1UL << *pix)));						//	 until hit next allowed index 
			dumphex(*pix);
		}
	} else if (cmd == KY_MENUUP) {          				// else if MENU UP
		if ((mask & 255) == PRE_FEED)									//	if only the one item on the menu 
			return(0);																	//	 ignore attempts to select another item
		dashes = 0;
		lcd_updated = 1;															//  display has changed	
		if (*pix == SPEED_IX) {												//	if at lowest parameter
			*pix = WID_IX+1;															//	 wrap back to highest
			do {
				(*pix)--;																	//	 find highest valid parameter
//			} while (!(mask & (1<< *pix)));
			} while (!(mask & (1UL << *pix)));
			dumphex(*pix);
		} else if (*pix && (tmsk & ((1 << (*pix + 1)) - 1))) {//	if any allowed lower index
			do {
				(*pix)--;																	//	  decrement index
//			} while (!(tmsk & (1 << *pix)));						//	 until hit next allowed index
			} while (!(tmsk & (1UL << *pix)));						//	 until hit next allowed index
			dumphex(*pix);
		}
	} else if (cmd == KY_MINUS) {           				// else if SETTING DECREASE
		lcd_updated = 1;															//	display has changed
		if ((play_mode & PM_PRESET) && ((*pix == SPEED_IX) || (*pix == SPIN_IX))) {	//  if preset speed or spin 
			newval = pparm[*pix] - parameters[*pix].limits.incr;//	decrement value
 		  if (newval >= parameters[*pix].limits.minimum) {	//	if at least the allowed minimum
// Index into preset random tables.
				if (preset_type == PRE_RANDHORZ)				// Random Horizontal
					premode = 0;
				else if (preset_type == PRE_RANDVERT)			// Random Vertical
					premode = 7;
				else if (preset_type == PRE_TWOLINE) {			// 2-Line
					if (preset[PRE_TWOLINE].twolnrw == 0)
						premode = 13;							// index for row A, wide width.
					else if (preset[PRE_TWOLINE].twolnrw == 1)
						premode = 16;							// index for random row, wide width.
					else if (preset[PRE_TWOLINE].twolnrw == 2)
						premode = 19;							// index for row C, wide width
	              	else if (preset[PRE_TWOLINE].twolnrw == 3)
						premode = 22;                  			// index for row B, wide width
				}
				else if (preset_type == PRE_FULLRAND)			// Full Random
					premode = 25;								// Get beyond new 2-Line row indexes
				premode += ((preset_type == 3) ? 0 : preset[preset_type].value[preset_type+6]); // up to 6 preset sub selections
				if (is_available(pre_rand[premode].start, (*pix==SPEED_IX)?newval:pparm[SPEED_IX], (*pix==SPIN_IX)?newval:pparm[SPIN_IX])) {
					pparm[*pix] = newval;
					dashes = 0;
					if (preset_type <= PRE_RANDVERT) {
						abort_shot();
						play_state = PLAY_DONE;
					}
				} else {
					dashes = 1 << (*pix);										// set dashes for this parm until cleared
				}
			}

// v1.226 start
		} else if ((play_mode & PM_MANUAL) && (*pix == SPIN_IX) && (pparm[SPEED_IX] == P_SPEEDMAX)) {	// If decreasing Manual Mode spin and at maximum speed, don't allow
			dashes = 1 << (*pix);																		// because can't have any spin to achieve maximum speed. 
// v1.226 end

		} else {			
dec1:	// ELO4CHG #5.1 (additional)

			pparm[*pix] -= parameters[*pix].limits.incr;		//	decrement value
//			if (*pix <= FEED_IX) {								// if normal parameter
			if ((*pix <= FEED_IX) && (*pix != TWOLNRW_IX)) {	// If a common parameter, but not the new 2-Line row selection,

// C18 to XC8 Compiler Changes
//			if (pparm[*pix] < parameters[*pix].limits.minimum)	// if below the allowed minimum
//				pparm[*pix] = parameters[*pix].limits.minimum;	// limit to that minimum
//			putrsDIAG("\n\rpparm[*pix] = ");
//			dumplong (pparm[*pix]);
//			Delay1KTCYx(255);
//			putrsDIAG("\n\rparameters[*pix].limits.minimum = ");
//			dumplong (parameters[*pix].limits.minimum);
			if (pparm[*pix] < parameters[*pix].limits.minimum) {	// if below the allowed minimum
				pparm[*pix] = parameters[*pix].limits.minimum;	// limit to that minimum
			}
// C18 to XC8 Compiler Changes
  	  		} else {													// else (preset subtype parameter)
				if (pparm[*pix] < parameters[*pix].limits.minimum)	// if below the allowed minimum
				pparm[*pix] = parameters[*pix].limits.maximum;		// wrap to the maximum
			}

//        	if (*pix == ROW_IX && model_type == EL04) {  // RND Depth - code to Skip line 2, 4 and 5
        	if (*pix == ROW_IX && ((model_type == EL04) || (model_type == CLUB1))) {  // RND Depth - code to Skip line 2, 4 and 5
            	//putrsDIAG(" pparm[*pix]=");
            	//dumphex(pparm[*pix]);
            	if ((pparm[*pix] == 1) || (pparm[*pix] == 3) || (pparm[*pix] == 4)) {
                	//putrsDIAG(" skip");
                	goto dec1;
            	}
        	}
    	}
//		if (*pix >= COL_IX) {						// if changed LINE, ROW or WIDTH		// mjz 11/16/08, commented
//		if ((*pix >= COL_IX) && (*pix != WID_IX)) {	// if changed LINE, ROW, not WIDTH		// mjz 11/16/08, added
		if (((*pix >= COL_IX) && (*pix != WID_IX)) || (*pix == TWOLNRW_IX)) {	// if changed line or row (random horizontal or 2-line)
//			pparm[SPEED_IX] = DEF_SPEED;			//	reset parameters to defaults
//			pparm[SPIN_IX]	= DEF_SPIN;
//			pparm[FEED_IX]	= DEF_FEED;
// Index into preset random tables.
			if (preset_type == PRE_RANDHORZ)				// Random Horizontal
				premode = 0;
			else if (preset_type == PRE_RANDVERT)			// Random Vertical
				premode = 7;
			else if (preset_type == PRE_TWOLINE) {			// 2-Line
				if (preset[PRE_TWOLINE].twolnrw == 0)
					premode = 13;							// index for row A, wide width.
				else if (preset[PRE_TWOLINE].twolnrw == 1)
					premode = 16;							// index for random row, wide width.
				else if (preset[PRE_TWOLINE].twolnrw == 2)
					premode = 19;							// index for row C, wide width
                else if (preset[PRE_TWOLINE].twolnrw == 3)
					premode = 22;                  			// index for row B, wide width
			}
			premode += ((preset_type == 3) ? 0 : preset[preset_type].value[preset_type+6]);		// up to 6 preset sub selections
			if (!(is_available(pre_rand[premode].start, pparm[SPEED_IX], pparm[SPIN_IX]))) {
				// If the current speed/spin combo doesn't exist for the new index value (row/column/two line row),

#ifdef notdef	// Only decrement/increment once to find nearby valid speed/spin combo or revert to default values.
				newval = pparm[SPEED_IX] - parameters[SPEED_IX].limits.incr;					// If decreased elevation, first decrement speed.
				putrsDIAG("\n\rnewval=");
				dumplong(newval);
				if ((newval >= parameters[SPEED_IX].limits.minimum)								// If at least the allowed minimum speed,
				  && (is_available(pre_rand[premode].start, newval, pparm[SPIN_IX]))) {			// and it is a valid speed/spin combo,
					pparm[SPEED_IX] = newval;													// then make it the new speed.
					putrsDIAG("\n\rdecrement");
				} else {
					newval = pparm[SPEED_IX] + parameters[SPEED_IX].limits.incr;				// else increment speed instead.
					putrsDIAG("\n\rnewval=");
					dumplong(newval);
		 			if ((newval <= parameters[SPEED_IX].limits.maximum)							// If at most the allowed maximum,
					  && (is_available(pre_rand[premode].start, newval, pparm[SPIN_IX]))) {		// and it is a valid speed/spin combo,
							pparm[SPEED_IX] = newval;											// then make it the new speed.
							putrsDIAG("\n\rincrement");
					} else {																	// else no valid speed/spin combo
						pparm[SPEED_IX] = DEF_SPEED;											// found nearby,
						pparm[SPIN_IX]	= DEF_SPIN;												// reset speed/spin to defaults.
					}
				}
#endif
// At least for sweep mode, since we are decreasing (KY_MINUS) the elevation, first keep decrementing the speed until a valid
// speed/spin combo is found. If we go under the min allowed speed, a valid combo wasn't found, so then go back to original invalid
// speed and keep incrementing the speed until a valid speed/spin combo is found. 
				newval = pparm[SPEED_IX];														// Current invalid speed.
				while ((newval >= parameters[SPEED_IX].limits.minimum)							// While >= minimum allowed speed,
				  && (!(is_available(pre_rand[premode].start, newval, pparm[SPIN_IX])))) {		// and it is a invalid speed/spin combo,
					newval = newval - parameters[SPEED_IX].limits.incr;							// since decreased elevation, first decrement speed.
				}
				if (newval < parameters[SPEED_IX].limits.minimum) {								// If no valid speed/spin combo found,
					newval = pparm[SPEED_IX];													// Current invalid speed.
					while ((newval <= parameters[SPEED_IX].limits.maximum)						// while <= maximum allowed speed,
				  	  && (!(is_available(pre_rand[premode].start, newval, pparm[SPIN_IX])))) {	// and it is a invalid speed/spin combo,
						newval = newval + parameters[SPEED_IX].limits.incr;						// increment speed.
					}
				}
				pparm[SPEED_IX] = newval;
			}									// else allow new setting.
//			if (play_state == PLAY_WAIT)
			abort_shot();
			play_state = PLAY_DONE;
		}
 	} else if (cmd == KY_PLUS) {														// else if SETTING INCREASE
	 	lcd_updated = 1;																			//	display has changed
	 	if ((play_mode & PM_PRESET) && ((*pix == SPEED_IX) || (*pix == SPIN_IX))) { // if preset speed or spin
		 	newval = pparm[*pix] + parameters[*pix].limits.incr;
		 	if (newval <= parameters[*pix].limits.maximum) {
// Index into preset random tables.
				if (preset_type == PRE_RANDHORZ)				// Random Horizontal
					premode = 0;
				else if (preset_type == PRE_RANDVERT)			// Random Vertical
					premode = 7;
				else if (preset_type == PRE_TWOLINE) {			// 2-Line
					if (preset[PRE_TWOLINE].twolnrw == 0)
						premode = 13;							// index for row A, wide width.
					else if (preset[PRE_TWOLINE].twolnrw == 1)
						premode = 16;							// index for random row, wide width.
					else if (preset[PRE_TWOLINE].twolnrw == 2)
						premode = 19;							// index for row C, wide width
                	else if (preset[PRE_TWOLINE].twolnrw == 3)
						premode = 22;                  			// index for row B, wide width
				}
				else if (preset_type == PRE_FULLRAND)			// Full Random
					premode = 25;								// Get beyond new 2-Line row indexes
				premode += ((preset_type == 3) ? 0 : preset[preset_type].value[preset_type+6]); // up to 6 preset sub selections
				if (is_available(pre_rand[premode].start, (*pix==SPEED_IX)?newval:pparm[SPEED_IX], (*pix==SPIN_IX)?newval:pparm[SPIN_IX])) {
// If in Random Depth or 2-Line mode with Random row selected (twolnrw==1), and we increased to the maximum speed,
// then disallow because random elevation is not possible at the maximum speed, due to limited number of shots at that speed.
					if ((preset_type == PRE_RANDVERT || (preset_type == PRE_TWOLINE && preset[PRE_TWOLINE].twolnrw == 1))
						&& *pix==SPEED_IX && newval >= parameters[*pix].limits.maximum) {
						dashes = 1 << (*pix);
					}
					else {
						pparm[*pix] = newval;
						dashes = 0;
						if (preset_type <= PRE_RANDVERT) {
							abort_shot();
							play_state = PLAY_DONE;
						}
					}
				} else {
					dashes = 1 << (*pix);
				}
			}

// v1.226 start
		} else if ((play_mode & PM_MANUAL) && (*pix == SPIN_IX) && (pparm[SPEED_IX] == P_SPEEDMAX)) {	// If increasing Manual Mode spin and at maximum speed, don't allow
			dashes = 1 << (*pix);																		// because can't have any spin to achieve maximum speed. 
		// Increasing Manual Mode speed to maximum speed, when the current spin is not zero, is not allowed because both throwing motors must be running at maximum speed,
		// i.e. not spin. The first attempt to achieve this limitation was to display dashes when increasing speed to maximum speed and a non-zero spin was already present.
		// This may confuse the player making them not know why the maximum speed was not allowed. So the second attempt to achieve this limitation is to automatically change
		// the spin back to zero when the player increases the speed to maximum speed when a non-zero spin was already present. Then if the player tries to change the spin,
		// dashes will be displayed to show spin is not allowed when the maximum speed is already present.
		} else if ((play_mode & PM_MANUAL) && (*pix == SPEED_IX)										// If increasing Manual Mode speed to maximum speed and current spin is not
			 && (pparm[*pix] + parameters[*pix].limits.incr == P_SPEEDMAX) && (pparm[SPIN_IX] !=0)) {	// zero, 
//			dashes = 1 << (*pix);																		// don't allow because can't have any spin to achieve maximum speed.
	   		pparm[*pix] += parameters[*pix].limits.incr;												// then increment speed to maximum speed,
	   		pparm[SPIN_IX] = 0;																			// and force spin to 0, because spin not allowed at maximum speed.
// v1.226 end

		} else {			

inc1:	// ELO4CHG #5.2 (additional)

	   	pparm[*pix] += parameters[*pix].limits.incr;					//	increment value
//		if (*pix <= FEED_IX) {											// if normal parameter
		if ((*pix <= FEED_IX) && (*pix != TWOLNRW_IX)) {				// If a common parameter, but not the new 2-Line row selection,
	  	 	if (pparm[*pix] > parameters[*pix].limits.maximum)			// if above the allowed maximum
	 			pparm[*pix] = parameters[*pix].limits.maximum;			// limit to that maximum
  	 	} else {														// else (preset subtype parameter)
	  	 	if (pparm[*pix] > parameters[*pix].limits.maximum)			// if above the allowed maximum
  	 			pparm[*pix] = parameters[*pix].limits.minimum;			// wrap to the minimum
	  	}

//		if (*pix == ROW_IX && model_type == EL04) {   								// RND Depth - code to Skip line 2, 4 and 5
		if (*pix == ROW_IX && ((model_type == EL04) || (model_type == CLUB1))){		// RND Depth - code to Skip line 2, 4 and 5
            //putrsDIAG(" pparm[*pix]=");
            //dumphex(pparm[*pix]);
            if ((pparm[*pix] == 1) || (pparm[*pix] == 3) || (pparm[*pix] == 4)) {
                //putrsDIAG(" skip");
                goto inc1;
            }
        }
   	}

//		if (*pix >= COL_IX) {						// if changed LINE, ROW or WIDTH
// Changed 2-Line so that if the 2-Line width was changed, the previously selected 2-Line motor parameters would
// be maintained. It would not revert back to the defaults. This requires that court locations 1 thru 6 all have
// the same selectable speed/spin shots, so that the same stored shot actually exists within all of the 2-Line
// widths, Narrow, Medium and Wide, locations 3/4, 2/5, 1/6 respectively. Shots are added to locations 1, 2, 5,
// & 6 in the shot tables, to achieve this requirement.
// 07/03/09 start
//		if ((*pix >= COL_IX) && (*pix != WID_IX)) {	// if changed LINE, ROW, not WIDTH
// mjz 11/16/08, end
		if (((*pix >= COL_IX) && (*pix != WID_IX)) || (*pix == TWOLNRW_IX)) {	// if changed line or row (random horizontal or 2-line)
//			pparm[SPEED_IX] = DEF_SPEED;					//	reset parameters to defaults
//			pparm[SPIN_IX]	= DEF_SPIN;
//			pparm[FEED_IX]	= DEF_FEED;
// Index into preset random tables.
			if (preset_type == PRE_RANDHORZ)				// Random Horizontal
				premode = 0;
			else if (preset_type == PRE_RANDVERT)			// Random Vertical
				premode = 7;
			else if (preset_type == PRE_TWOLINE) {			// 2-Line
				if (preset[PRE_TWOLINE].twolnrw == 0)
					premode = 13;							// index for row A, wide width.
				else if (preset[PRE_TWOLINE].twolnrw == 1)
					premode = 16;							// index for random row, wide width.
				else if (preset[PRE_TWOLINE].twolnrw == 2)
					premode = 19;							// index for row C, wide width
                else if (preset[PRE_TWOLINE].twolnrw == 3)
					premode = 22;                  			// index for row B, wide width
			}
			premode += ((preset_type == 3) ? 0 : preset[preset_type].value[preset_type+6]);		// up to 6 preset sub selections
			if (!(is_available(pre_rand[premode].start, pparm[SPEED_IX], pparm[SPIN_IX]))) {
				// If the current speed/spin combo doesn't exist for the new index value (row/column/two line row),

#ifdef notdef	// Only increment/decrement once to find nearby valid speed/spin combo or revert to default values.
				newval = pparm[SPEED_IX] + parameters[SPEED_IX].limits.incr;					// If increased elevation, first increment speed.
				putrsDIAG("\n\rnewval=");
				dumplong(newval);
		 		if ((newval <= parameters[SPEED_IX].limits.maximum)								// If at most the allowed maximum,
				  && (is_available(pre_rand[premode].start, newval, pparm[SPIN_IX]))) {			// and it is a valid speed/spin combo,
					pparm[SPEED_IX] = newval;													// then make it the new speed.
					putrsDIAG("\n\rdecrement");
				} else {
					newval = pparm[SPEED_IX] - parameters[SPEED_IX].limits.incr;				// else decrement speed instead.
					putrsDIAG("\n\rnewval=");
					dumplong(newval);
					if ((newval >= parameters[SPEED_IX].limits.minimum)							// If at least the allowed minimum speed,
					  && (is_available(pre_rand[premode].start, newval, pparm[SPIN_IX]))) {		// and it is a valid speed/spin combo,
							pparm[SPEED_IX] = newval;											// then make it the new speed.
							putrsDIAG("\n\rincrement");
					} else {																	// else no valid speed/spin combo
						pparm[SPEED_IX] = DEF_SPEED;											// found nearby,
						pparm[SPIN_IX]	= DEF_SPIN;												// reset speed/spin to defaults.
					}
				}
#endif
// At least for sweep mode, since we are increasing (KY_PLUS) the elevation, first keep incrementing the speed until a valid
// speed/spin combo. If we go over the max allowed speed, a valid combo wasn't found, so then go back to original invalid speed
// and keep decrementing the speed until a valid speed/spin combo is found. 
				newval = pparm[SPEED_IX];														// Current invalid speed.
				while ((newval <= parameters[SPEED_IX].limits.maximum)							// While <= maximum allowed speed,
				  && (!(is_available(pre_rand[premode].start, newval, pparm[SPIN_IX])))) {		// and it is a invalid speed/spin combo,
					newval = newval + parameters[SPEED_IX].limits.incr;							// since increased elevation, first increment speed.
				}
				if (newval > parameters[SPEED_IX].limits.maximum) {								// If no valid speed/spin combo found,
					newval = pparm[SPEED_IX];													// Current invalid speed.
					while ((newval >= parameters[SPEED_IX].limits.minimum)						// while >= minimum allowed speed,
				  	  && (!(is_available(pre_rand[premode].start, newval, pparm[SPIN_IX])))) {	// and it is a invalid speed/spin combo,
						newval = newval - parameters[SPEED_IX].limits.incr;						// decrement speed.
					}
				}
				pparm[SPEED_IX] = newval;
			}									// else allow new setting.
			abort_shot();
			play_state = PLAY_DONE;
		}
  }
	if (mask & PRE_ANY) {	// if is preset
		ln = 1;
		if ((mask & (PRE_FULL|PRE_DRILL)) && lcd_updated) {
			clear_lcd(0xf);
		}
	} else {
		ln = 0;
	}
//	for (i=0,cmsk=1; i<=(ushort)(NUMPRESETS+4); i++) {		// process all parameters
	for (i=0,cmsk=1; i<=(ushort)(NUMPRESETS+5); i++) {		// process all parameters, added 2-Line row selection
		if (tmsk & cmsk) {												//	if parameter is active
		 	if (cmsk & (PRE_FULL|PRE_DRILL)) {			//	 if no adjustable parameter
			  ln = 0;
			}

//			putrsDIAG("\n\rtmsk = ");
//			dumplong(tmsk);

//			if (play_mode & PM_MANUAL) {
			if ((play_mode & PM_MANUAL) || ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE))) {
				if (i == FEED_IX)
					k = 3;
				else
					k = (parameters[i].line != 0) ? parameters[i].line : ln & 3;
				if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {
					if (i == WID_IX)
						k = 0;
				}
			} else {
				k = ln & 3;
			}
			strncpy(&lcd_line[k][parameters[i].text_start],			//	 copy in base text (hilited or normal)

// C18 to XC8 Compiler Changes
//	  		(rom char*)parm_fmt[i][(*pix == i) ? FMT_HILITE : FMT_STD],parameters[i].text_size);
	  		(char*)parm_fmt[i][(*pix == i) ? FMT_HILITE : FMT_STD],parameters[i].text_size);
// C18 to XC8 Compiler Changes

	  	ln++;
			if (!(play_mode & PM_MANUAL) && (i == SPIN_IX)) {
				strcpy(&lcd_line[k][8],"        ");					
			}
		if (i == TWOLNRW_IX) {
			  chartoa(preparm1base[11]+pparm[i],&lcd_line[k][parameters[i].field_locn],parameters[i].field_size);	// hardcoded index of base indexes
		} else if (i > FEED_IX) {
//		  if (i > FEED_IX) {											//	 if preset parameter (OLD:or spin)
			  chartoa(preparm1base[i-FEED_IX]+pparm[i],&lcd_line[k][parameters[i].field_locn],parameters[i].field_size);
		} else if (parameters[i].field_size) {	//	 else (standard parameter)
				if (dashes & (1<<i)) {
					strncpy(&lcd_line[k][parameters[i].field_locn],"--",2);
				} else {
					if (i == SPEED_IX) {
//						putrsDIAG("Spd: ");
//						dumphex(pparm[i]);
//						dumphex(speed_trans[pparm[i]/5]);
			  		chartoa(speed_trans[pparm[i]/5],&lcd_line[k][parameters[i].field_locn],parameters[i].field_size);
			  	} else {
// v1.194 & v1.195 start
//			  		chartoa(pparm[i],&lcd_line[k][parameters[i].field_locn],parameters[i].field_size);
					if ((i == FEED_IX) && (pparm[i] == P_FEEDMIN)) {	// the min value is now defined as random feed
						strncpy(&lcd_line[k][parameters[i].field_locn-1],"Random ",7);
					}
					else
// v1.194 & v1.195 end
					{
			  			chartoa(pparm[i],&lcd_line[k][parameters[i].field_locn],parameters[i].field_size);
					}
			  	}
		  	}
		  }		  
		}
	  cmsk <<= 1;
  }
	return(0);
}				// end of lob_parm

// Determines if the specified speed and spin combination is valid for the
// specified starting location. It may search multiple locations within the
// same line or row depending on the mode we are in.
// NOTE: for now only Random Depth has special handling
//

// C18 to XC8 Compiler Changes
//char is_available(char locno, uchar spd, char spn)
char is_available(char locno, uchar spd, signed char spn)
// C18 to XC8 Compiler Changes

{
	char rc = 0;
	
	rc = isLocAvail(locno, spd, spn);						// always check for specified location
	if (preset_type == PRE_RANDVERT) {						// if  random depth
		if (!rc) 											// if not in farthest row
			rc = isLocAvail(locno+6, spd, spn);				// see if it is in same line middle row
		if (!rc) 											// if not in farthest two rows
			rc = isLocAvail(locno+12, spd, spn);			// see if it is in same line nearest row
	}
	return rc;												// return ultimate result
}

// Checks the given location to determine if there is a method (set of parameters)
// for that location which matches the specifed speed and spin
// 

// C18 to XC8 Compiler Changes
//char isLocAvail(char locNo, uchar spd, char spn)
char isLocAvail(char locNo, uchar spd, signed char spn)
// C18 to XC8 Compiler Changes
{

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* locp;
	const struct el04_loc* locp;
// C18 to XC8 Compiler Changes

	signed char method;
	
	for (method=0; method<NUMMETH; method++) {				// search all methods of achieving this location
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//			locp = &court_locs[locNo][method];				// method under consideration
//		else
//			locp = &court_locs_CLUB[locNo][method];			// CLUB model only
#ifndef PHENOMS
			locp = &court_locs[locNo][method];				// method under consideration
#else
			locp = &court_locs_CLUB[locNo][method];			// CLUB model only
#endif
// v1.219 end
// 07/03/09 start
//        putrsDIAG("\n\rlocnNo = ");
//        dumphex(locNo);
//        putrsDIAG("method = ");
//        dumphex(method);
//        putrsDIAG("locp->spin = ");
//        dumphex(locp->spin);
//        putrsDIAG("locp->speed = ");
//        dumphex(locp->speed);
// 07/03/09 end
		if ((spn == locp->spin) && (spd == locp->speed)) {	// and speed has increased
			break;											// the search is over
		}
	}
// 07/03/09 start
//	putrsDIAG("\n\rmethod = ");
//	dumphex(method);
// 07/03/09 end
	return ((method < NUMMETH) ? 1 : 0);
}

// Checks the given location to determine if there is a method (set of parameters)
// for that location which matched the specified speed and spin.
// if such a match is available, the method value pointed to by pmeth is changed
// to indicate the matching method
// Currently used in Program mode only.

// C18 to XC8 Compiler Changes
//char getLocAvail(char locNo, uchar spd, char spn, char* pmeth)
char getLocAvail(char locNo, uchar spd, signed char spn, char* pmeth)
// C18 to XC8 Compiler Changes

{

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* locp;
	const struct el04_loc* locp;
// C18 to XC8 Compiler Changes

	signed char method;
	
	for (method=0; method<NUMMETH; method++) {				// search all methods of achieving this location
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//			locp = &court_locs[locNo][method];				// method under consideration
//		else
//			locp = &court_locs_CLUB[locNo][method];			// CLUB model only
#ifndef PHENOMS
			locp = &court_locs[locNo][method];				// method under consideration
#else
			locp = &court_locs_CLUB[locNo][method];			// CLUB model only
#endif
// v1.219 end
		if ((spn == locp->spin) && (spd == locp->speed)) {	// and speed has increased
			*pmeth = method;
			break;											// the search is over
		}
	}
	return ((method < NUMMETH) ? 1 : 0);
}
