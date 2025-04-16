// Lobster Preset Programs
//
//	(C)2007 Censerve(R) Consulting, Inc.
//					All rights reserved.
//
#include "types.h"
#include "lob.h"
#include "string.h"
#include "diag.h"

// C18 to XC8 Compiler Changes
#include <usart.h>
// C18 to XC8 Compiler Changes

extern unsigned char model_type;extern signed short eladj;
extern long play_mode;
// v1.213 start
//extern char preset_type;		// declared below as char preset_type
// v1.213 end
extern char lcd_updated;
extern char lcd_changed;

// C18 to XC8 Compiler Changes
//extern const rom char* parm_fmt[][2];
//extern const rom char* preset_sub[];
//extern const rom char* preset_sub2[];
extern const char* parm_fmt[][2];
extern const char* preset_sub[];
extern const char* preset_sub2[];
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//extern char lcd_line[][17];
extern char lcd_line[][17];
// C18 to XC8 Compiler Changes

extern struct t_parm parameters[];
extern signed char program_ix;
extern struct persprog act_program;

extern void BuildCmd(int code, int cnt, long* args);
extern void chartoa(const char c, char* cp, char ndig);

// v1.219 start
extern char RemoteString[68];
extern unsigned int RemotePgmMode;
// v1.219 end


// v1.208 start
//extern char lob_parm(char cmd, uchar *pix, char* pparm, ushort mask);

// C18 to XC8 Compiler Changes
//extern char lob_parm(char cmd, uchar *pix, char* pparm, unsigned short long mask);
extern char lob_parm(char cmd, uchar *pix, signed char* pparm, unsigned short long mask);
// C18 to XC8 Compiler Changes

// v1.208 end
extern void set_serve(union parm* pp);
// v1.208 start
//const rom ushort preset_mask[] = {

// C18 to XC8 Compiler Changes
//const rom unsigned short long preset_mask[] = {
const unsigned short long preset_mask[] = {
// C18 to XC8 Compiler Changes

// v1.208 end
	PRE_SPEED|PRE_RSWP	|PRE_FEED|PRE_SPIN,		// Random Horizontal
	PRE_SPEED|PRE_RELEV |PRE_FEED|PRE_SPIN,		// Random Vertical
// 07/03/09 start
//	PRE_SPEED|PRE_WIDTH	|PRE_FEED|PRE_SPIN,							// Two Line Drill
	PRE_SPEED|PRE_WIDTH	|PRE_FEED|PRE_SPIN|PRE_TWOLNRW,				// Two Line Drill with row selection
// 07/03/09 end
						PRE_FULL	|PRE_FEED,						// Fully Random
						PRE_GRND	|PRE_FEED,						// Grinder
						PRE_PWR		|PRE_FEED,						// Power Player
						PRE_ALLCRT	|PRE_FEED,						// All courter
						PRE_TRNS	|PRE_FEED,						// Transition
						PRE_ADEF	|PRE_FEED,						// Attack/Defend
						PRE_FONLY	|PRE_FEED,						// Forehand Only
// v1.208 start
						PRE_BGNNR	|PRE_FEED,						// Beginner Drill
						PRE_INTRMDT	|PRE_FEED,						// Intermediate Drill
						PRE_ADVNCD	|PRE_FEED,						// Advanced Drill
						PRE_MNBLLR	|PRE_FEED,						// Moonballer Drill
						PRE_SLCR	|PRE_FEED,						// Slicer Drill
						PRE_LFTY	|PRE_FEED,						// Lefty Drill
// v1.208 end
};

// C18 to XC8 Compiler Changes
//const rom char preparm1base[] = {
const char preparm1base[] = {
// C18 to XC8 Compiler Changes

// v1.190 start
//	12,	// Spin					- use spin descriptions
	13,	// Spin					- use spin descriptions
// v1.190 end
	6,	// Random Horizontal	- use general sweep descriptions
	0,	// Random Vertical		- use general elevation descriptions
// v1.190 start
//	9,	// Two-Line Drill		- use Two-Line width descriptions
	10,	// Two-Line Drill		- use Two-Line width descriptions
// v1.190 end
// 07/03/09 start
// Increased the following indexes because 2-Line row selection
// text was added to preset_sub[] in lobparm.c. It wasn't necessary
// to add the base index for the new 2-Line row selection text,
// because the base is hardcoded in the lob_parm routing for when
// the parameter is the 2-Line row selection, TWOLNRW_IX.
//	19,	// Fully Random			- use none
//	19,	// Grinder				- use none
//	19,	// Power Player			- use none
//	19,	// All courter			- use none
//	19, // Transition			- use none
//	19, // Attack/Defend		- use none
//	19, // Forehand Only		- use none
// v1.190 start
//	22,	// Fully Random			- use none
//	22,	// Grinder				- use none
//	22,	// Power Player			- use none
//	22,	// All courter			- use none
//	22, // Transition			- use none
//	22, // Attack/Defend		- use none
//	22, // Forehand Only		- use none
//	19, // Two Line row selection text
	23,	// Fully Random			- use none
	23,	// Grinder				- use none
	23,	// Power Player			- use none
	23,	// All courter			- use none
	23, // Transition			- use none
	23, // Attack/Defend		- use none
	23, // Forehand Only		- use none
	20, // Two Line row selection text
// 07/03/09 end
// v1.190 end
// v1.208 start
	23, // Beginner Drill		- use none
	23, // Intermediate Drill	- use none
	23, // Advanced Drill		- use none
	23, // Moonballer Drill		- use none
	23, // Slicer Drill			- use none
	23, // Lefty Drill			- use none
// v1.208 end
	0,
};

// C18 to XC8 Compiler Changes Start
//char preset_type;				// sub mode for presets (e.g. random horiz, etc)
unsigned char preset_type;				// sub mode for presets (e.g. random horiz, etc)
// C18 to XC8 Compiler Changes End
char preset_ix[NUMPRESETS];		// parameter index for each preset submode
union parm preset[NUMPRESETS];	// where we store the values for each of the preset parameters

// Preset Random Selection Tables
// 07/03/09 start
//const rom struct rand_info pre_rand[19] = {
// 07/07/09 start
//const rom struct rand_info pre_rand[22] = {
// 07/03/09 end
// v1.190 start
//const rom struct rand_info pre_rand[25] = {

// C18 to XC8 Compiler Changes
//const rom struct rand_info pre_rand[26] = {
const struct rand_info pre_rand[26] = {
// C18 to XC8 Compiler Changes

// v1.190 end
// 07/07/09 end
//	Rng	Inc	1st	Max
//	range, incr, start, max from struct rand_info in lob.h
// v1.182 start
// Changes so KY_PLUS actually increases elevation, row A, C, B in sequence, rather than row A, B, C, which actually decreases elevation.
// But we want to start with deep row, then wrap to shallow, then center, and back to deep row.
//	{	6,	0,	0,	5, }, 	// Random Horiz Top, ROW A
//	{	6,	0,	6, 11, }, 	// Random Horiz Center, ROW B
//	{	6,	0, 12, 17, }, 	// Random Horiz Bottom, ROW C
	{	6,	0,	0,	5, }, 	// Random Horiz Top, ROW A
// v1.190 start
//	{	6,	0,	0, 17, },	// Random Horiz, Random row selection (triple oscillation)
//	{	12,	0,	0,  5, },	// Random Horiz, Random row selection (triple oscillation)
//	{	17,	0,	0,  5, },	// Random Horiz, Random row selection (triple oscillation)
	{	12,	5,	0,  5, },	// Random Horiz, Random row selection (triple oscillation)
// v1.190 end
	{	6,	0, 12, 17, }, 	// Random Horiz Bottom, ROW C
	{	6,	0,	6, 11, }, 	// Random Horiz Center, ROW B
// v1.182 end
	{	0,	0,	0,	0, },	// Reserved for 4th row
	{	0,	0,	0,	0, },	// Reserved for 5th row
	{	0,	0,	0,	0, },	// Reserved for 6th row
	{	12,	6,	0,	12 },	// Random Vertical Far Left
	{	12,	6,	1,	13 },	//				   Mid Left
	{	12,	6,	2,	14 },	//				   Center Left
	{	12,	6,	3,	15 },	//				   Center Right			
	{	12,	6,	4,	16 },	//				   Mid Right			
	{	12,	6,	5,	17 },	//				   Far Right			
// Changed 2-Line to use the deep row, instead of the middle row.		
//	{	2,	5,	6,	11 },	// Two-Line Wide
//	{	2,  3,	7,	10 },	//			Medium
//	{	2,	1,	8,	 9 },	//			Narrow
//	{	2,	5,	0,	 5 },	// Two-Line Wide
//	{	2,  3,	1,	 4 },	//			Medium
//	{	2,	1,	2,	 3 },	//			Narrow
	{	2,	5,	0,	 5 },	// Two-Line Row A, Wide
	{	2,  3,	1,	 4 },	//          Row A, Medium
	{	2,	1,	2,	 3 },	//          Row A, Narrow
// v1.182 start
	{	12,	5,	0,	 5 },	// Two-Line Random Row, Wide
	{	13, 3,	1,	 4 },	//          Random Row, Medium
	{	14,	1,	2,	 3 },	//          Random Row, Narrow
	{	2,	5,	12,	17 },	// Two-Line Row C, Wide
	{	2,  3,	13,	16 },	//			Row C, Medium
	{	2,	1,	14,	15 },	//			Row C, Narrow
	{	2,	5,	6,	11 },	// Two-Line Row B, Wide
	{	2,  3,	7,	10 },	//			Row B, Medium
	{	2,	1,	8,	 9 },	//			Row B, Narrow
// v1.182 end
//	{	0,	0,	0,	0, },	// Reserved for Additional Two-Line
//	{	0,	0,	0,	0, },	// Reserved for Additional Two-Line
//	{	0,	0,	0,	0, },	// Reserved for Additional Two-Line
	{	18,	1,	0, 17, },	// Fully Random
};

// Preset Selection
char lob_preset(char cmd)
{
	char keep_motors = 0;		// mjz 11/16/08, added	ELO4CHG #4.1
	uchar* pix;
    char* pparmx;

// v1.219 start
	int i;
// v1.219 end


// v1.186 start
// v1.199 start
//	if (model_type == EL04){
	if ((model_type == EL04) || (model_type == CLUB1)){			// models with EL04 control panel
// v1.199 end

    	if (state_just_changed(cmd)) {							// if just got to Presets
    		eladj = 0;
    		save_prog(program_ix,&act_program);
    		play_mode = (play_mode & PM_ELIMMODE) | PM_PRESET;	//	ensure is preset mode
    		BuildCmd('3',1,&play_mode);							//	show LEDs
    		lcd_updated = 1;			// EL04CHG #7  uncomment fixed remote 2 button press issue from manual or test shot mode.
    		SPUTRSDIAG("\n\rPRESET#");
    		SDUMPHEX(preset_type);
    	} else {
            //if (((cmd & 0x7F) > 6) && ((cmd & 0x7F) < 0x40)) {  // caused minimum feed rate of 3 due to resetting preset[preset_type].feed  = 4 when we shouldn't have
            if (((cmd & 0x7F) > 6) && ((cmd & 0x7F) < 34)) {
    #ifdef notdef
                if ((cmd & 0x7F)!= KY_GRINDER ) {
                preset[preset_type].speed = 50;
    		    preset[preset_type].spin = 0;
    		    preset[preset_type].feed  = 4;
                }
    #endif
                switch (cmd & 0x7F) {
                case KY_TWOLINENARROW:     // 2-Line Narrow
// mjz 11/16/08, added the following line, ELO4CHG #4.1
//				    if (preset_type == PRE_TWOLINE)	keep_motors = 1;	// if already in 2-Line mode, then keep speed, spin, feed paramaters. 02/09/09 mjz commented
    				keep_motors = 1;	// Keep last used speed, spin, feed parameters for this mode. 02/09/09 mjz added
// C18 to XC8 Compiler Changes start
//                  pparmx = &(preset[preset_type].value[0]);
                    pparmx = (char *)&(preset[preset_type].value[0]);
// C18 to XC8 Compiler Changes end
                    pparmx[8] = parameters[8].limits.maximum;
                    break;
                case KY_TWOLINEMEDIUM:     // 2-Line Medium
// mjz 11/16/08, added the following line, ELO4CHG #4.1
//				    if (preset_type == PRE_TWOLINE)	keep_motors = 1;	// if already in 2-Line mode, then keep speed, spin, feed paramaters. 02/09/09 mjz commented
    				keep_motors = 1;	// Keep last used speed, spin, feed parameters for this mode. 02/09/09 mjz added
// C18 to XC8 Compiler Changes start
//                  pparmx = &(preset[preset_type].value[0]);
                    pparmx = (char *)&(preset[preset_type].value[0]);
// C18 to XC8 Compiler Changes end
                    pparmx[8] = parameters[8].limits.minimum;
                    pparmx[8] += parameters[8].limits.incr;
                       break;
                case KY_TWOLINEWIDE:       // 2-Line Wide
// mjz 11/16/08, added the following line, ELO4CHG #4.1
//			    	if (preset_type == PRE_TWOLINE)	keep_motors = 1;	// if already in 2-Line mode, then keep speed, spin, feed paramaters. 02/09/09 mjz commented
    				keep_motors = 1;	// Keep last used speed, spin, feed parameters for this mode. 02/09/09 mjz added
// C18 to XC8 Compiler Changes start
//                  pparmx = &(preset[preset_type].value[0]);
                    pparmx = (char*)&(preset[preset_type].value[0]);
// C18 to XC8 Compiler Changes end
                    pparmx[8] = parameters[8].limits.minimum;
                    break;
                case KY_GRINDER:            // Grinder
//                  preset_type=PRE_GRINDER;
                    break;
                case KY_POWER:              // Power Baseliner
//                  preset_type=PRE_POWER;
                    break;
                case KY_ALLCOURTER:         // All Courter
//                  preset_type=PRE_ALLCOURT;
                    break;
                case KY_RNDSWEEP:           // Random Sweep
//                  preset_type=PRE_RANDHORZ;
                    break;
                case KY_RNDDEPTH:           // Random Depth
//                  preset_type=PRE_RANDVERT;
                    break;
                case KY_FULLRAND:           // Fully Random
//                  preset_type=PRE_FULLRAND;
                    break;
                }
    
// v1.186 start    
//    		if (!keep_motors) {									// mjz 11/16/08, added, ELO4CHG #4.1
// v1.199 start
//    		if	(preset_type != PRE_TWOLINE) {				// Only reload defaults, if not in 2-Line mode.
// v1.186 end
//            	preset[preset_type].speed = DEF_SPEED;   //50;
//    			preset[preset_type].spin = DEF_SPIN;    //0;
//    			preset[preset_type].feed  = DEF_FEED;    //4;
//    		}													// mjz 11/16/08 added, ELO4CHG #4.1
// v1.199 end
    		lcd_updated = 1;  //yep we need this one
            }
    		PUTRSDIAG("\n\rPRESET#");
    		DUMPHEX(preset_type);
    	}
    	pix = (uchar *)&preset_ix[preset_type];

// v1.218 start
//    	if (preset_type >= PRE_FULLRAND) {
// preset_type is initialized to NUMPRESETS, which is currently 0x10. The previous statement would be true and the preset_type index would wrap to 0.
// This would cause problems with horizontal mode which is preset_type 0. Now must qualify with < NUMPRESETS also.
    	if ((preset_type >= PRE_FULLRAND) && (preset_type < NUMPRESETS)) { 
// v1.218 end
    		//*pix = 3U;
            *pix = FEED_IX;
    	}

// v1.218 start
// For some models, preset_type is purposely set to invalid value in varinit, so lob_parm was getting executed on an invalid preset_type and preset_mask
// for the first time here after power-on. Now only execute lob_parm here, if the preset_type is a valid one.
//    	lob_parm(cmd,pix,&(preset[preset_type].value[0]),preset_mask[preset_type]);
//    	if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
//    		set_serve(&preset[preset_type]);
//    	}
// C18 to XC8 Compiler Changes Start
//		if ((preset_type >= 0) && (preset_type < NUMPRESETS)) {
		if (preset_type < NUMPRESETS) {			// preset_type is no longer signed
// C18 to XC8 Compiler Changes End
    		lob_parm(cmd,pix,&(preset[preset_type].value[0]),preset_mask[preset_type]);
			if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
				set_serve(&preset[preset_type]);
			}
		}
// v1.218 end

    	return 0;
    }

// v1.191.0R start
//	else if (model_type == EL05LE2){
// v1.199 start
//	else if (model_type == EL05LE2 || model_type == CLUB){
	else if (model_type == EL05LE2 || model_type == CLUB2){		// models with new EL05LE2 control panel
// v1.199 end
// v1.191.0R end
    	if (state_just_changed(cmd)) {							// if just got to Presets
    		eladj = 0;
    		save_prog(program_ix,&act_program);
// v1.219 start
			if (RemotePgmMode) {
				for (i=0; i<4; i++) {
					RemoteString[i] = 0xff;
				}
				RemoteString[4] = 0x00;
				puts2USART (RemoteString);
				RemotePgmMode = RPM_START;
			}
// v1.219 end
    		play_mode = (play_mode & PM_ELIMMODE) | PM_PRESET;	//	ensure is preset mode
    		BuildCmd('3',1,&play_mode);							//	show LEDs
    		lcd_updated = 1;			// EL04CHG #7  uncomment fixed remote 2 button press issue from manual or test shot mode.
    		SPUTRSDIAG("\n\rPRESET#");
    		SDUMPHEX(preset_type);
    	} else {
            //if (((cmd & 0x7F) > 6) && ((cmd & 0x7F) < 0x40)) {  // caused minimum feed rate of 3 due to resetting preset[preset_type].feed  = 4 when we shouldn't have
//          if (((cmd & 0x7F) > 6) && ((cmd & 0x7F) < 34)) {
            if (((cmd & 0x7F) > 6) && ((cmd & 0x7F) <= 0x24)) {
                switch (cmd & 0x7F) {
                case KY_2LINE:
    				keep_motors = 1;	// Keep last used speed, spin, feed parameters for this mode. 02/09/09 mjz added
//					preset_type=PRE_TWOLINE;
                    break;
                case KY_DRILLS:
//					preset_type=PRE_GRINDER;
                    break;
                case KY_SWEEP:
//					preset_type=PRE_RANDHORZ;
                    break;
                case KY_DEPTH:
//					preset_type=PRE_RANDVERT;
                    break;
                case KY_RANDOM:
//					preset_type=PRE_FULLRAND;
                    break;
                }

// v1.186 start    
//    		if (!keep_motors) {									// mjz 11/16/08, added, ELO4CHG #4.1
// v1.199 start
//    		if	(preset_type != PRE_TWOLINE) {				// Only reload defaults, if not in 2-Line mode.
// v1.186 end
//           	preset[preset_type].speed = DEF_SPEED;   //50;
//    			preset[preset_type].spin = DEF_SPIN;    //0;
//    			preset[preset_type].feed  = DEF_FEED;    //4;
//    		}													// mjz 11/16/08 added, ELO4CHG #4.1
// v1.199 end
    		lcd_updated = 1;  //yep we need this one
            }
    		PUTRSDIAG("\n\rPRESET#");
    		DUMPHEX(preset_type);
    	}
    	pix = (uchar *)&preset_ix[preset_type];

// v1.218 start
//    	if (preset_type >= PRE_FULLRAND) {
// preset_type is initialized to NUMPRESETS, which is currently 0x10. The previous statement would be true and the preset_type index would wrap to 0.
// This would cause problems with horizontal mode which is preset_type 0. Now must qualify with < NUMPRESETS also.
    	if ((preset_type >= PRE_FULLRAND) && (preset_type < NUMPRESETS)) { 
// v1.218 end
    		//*pix = 3U;
            *pix = FEED_IX;
    	}

// v1.218 start
// For some models, preset_type is purposely set to invalid value in varinit, so lob_parm was getting executed on an invalid preset_type and preset_mask
// for the first time here after power-on. Now only execute lob_parm here, if the preset_type is a valid one.
//    	lob_parm(cmd,pix,&(preset[preset_type].value[0]),preset_mask[preset_type]);
//    	if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
//    		set_serve(&preset[preset_type]);
//    	}
// C18 to XC8 Compiler Changes Start
//		if ((preset_type >= 0) && (preset_type < NUMPRESETS)) {
		if (preset_type < NUMPRESETS) {		// preset_type is no longer signed
// C18 to XC8 Compiler Changes End
    		lob_parm(cmd,pix,&(preset[preset_type].value[0]),preset_mask[preset_type]);
			if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
				set_serve(&preset[preset_type]);
			}
		}
// v1.218 end

    	return 0;
    }

    else {   // all models except EL04
    	if (state_just_changed(cmd)) {												// if just got to Presets
    		eladj = 0;
    		save_prog(program_ix,&act_program);
// v1.219 start
			if (RemotePgmMode) {
				for (i=0; i<4; i++) {
					RemoteString[i] = 0xff;
				}
				RemoteString[4] = 0x00;
				puts2USART (RemoteString);
			}
// v1.219 end
    		play_mode = (play_mode & PM_ELIMMODE) | PM_PRESET;	//	ensure is preset mode
    		BuildCmd('3',1,&play_mode);													//	show LEDs
    		lcd_updated = 1;
    		putrsDIAG("\n\rPRESET#");
    		dumphex(preset_type);
    	} else if (cmd == KY_PRESET) {												// else if pressed again
    		if (++preset_type >= (NUMPRESETS)) {									//	if next type exceeds limit
    			preset_type = PRE_RANDHORZ;												//	 start type over
    		}
// v1.199 start
//    		if	(preset_type != PRE_TWOLINE) {				// Only reload defaults, if not in 2-Line mode. 12/31/08 added
//    			preset[preset_type].speed = DEF_SPEED;
//    			preset[preset_type].spin	= DEF_SPIN;
//    			preset[preset_type].feed  = DEF_FEED;
//    		}												// 12/31/08 added
// v1.199 end
    		lcd_updated = 1;
    		putrsDIAG("\n\rPRESET#");
    		dumphex(preset_type);
    	}
    	pix = (uchar *)&preset_ix[preset_type];
    	if (preset_type >= PRE_FULLRAND) {
    		*pix = FEED_IX;
    	}
    	lob_parm(cmd,pix,&(preset[preset_type].value[SPEED_IX]),preset_mask[preset_type]);
    	if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
    		set_serve(&preset[preset_type]);
    	}
    	return 0;
    }
// v1.186 end
}