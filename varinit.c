// Variable Initialization
//
//	(C)2007 Censerve(R) Consulting, Inc.
//	All rights reserved.
//
#include "types.h"
#include "state_mach.h"
#include "lob.h"
#include "string.h"

// C18 to XC8 Compiler Changes
//#include "stdlib.h"
// C18 to XC8 Compiler Changes

extern char dashes;
extern char abort_key;
extern char aborted_mode;

// C18 to XC8 Compiler Changes
//extern ram uchar num_methods[];
extern uchar num_methods[];
// C18 to XC8 Compiler Changes

extern uchar loaded_progno;
extern uchar cur_st;
extern uchar prev_st;
extern signed char program_ix;
extern signed char ppshot_ix;
extern signed char ppparm_ix;
extern char lcd_changed;
extern uchar play_state;
extern uchar lcd_updated;
extern long play_mode;
extern char remote_enabled;
extern char lob_on;
extern union parm manual; 		// parameters for manual program
extern char man_parm;          // currently selected parameter for change
// C18 to XC8 Compiler Changes Start
//extern char preset_type;
extern unsigned char preset_type;
// C18 to XC8 Compiler Changes End
extern char preset_ix[NUMPRESETS];
extern union parm preset[NUMPRESETS];
extern char idle_fpass;
extern char man1shot;
extern unsigned short dpb_thrash;
extern volatile struct power_button power;
extern int main_ostate;

// C18 to XC8 Compiler Changes
//extern volatile	unsigned long home_ticks;
extern unsigned long home_ticks;
// C18 to XC8 Compiler Changes

extern unsigned long batt_ticks;
extern unsigned char batt_cnt;
extern unsigned char BatAvgCnt;
extern char homeshown;
extern unsigned long loop_delay;
extern unsigned long repeat;
extern unsigned short keyhold;

// C18 to XC8 Compiler Changes
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
//extern unsigned long ee_progstart;
//extern far rom struct ee_hdr flash_header;
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];
extern unsigned long ee_progstart;
extern const struct ee_hdr flash_header;
// C18 to XC8 Compiler Changes

extern char rem_feed_chg;
// v1.199 start
extern unsigned char model_type;
// v1.199 end
// v1.219 start
// C18 to XC8 Compiler Changes
//extern unsigned char RemotePgmMode;
extern unsigned int RemotePgmMode;
// C18 to XC8 Compiler Changes
// v1.219 end

// v1.221 start
extern char BatLevelDisplay;
extern unsigned char BatLevelDisplayTmr;
extern int BatPercent;
extern unsigned char PlayLEDPass;
// v1.221 end

// This routine must be called at power on and wake up from sleep mode
//
void varinit(void)
{
	int i = 0;
	char j;

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc *locp;
	const struct el04_loc *locp;
// C18 to XC8 Compiler Changes

	
//	srand(1);							// make sure seeds the same random sequence for now
	dashes = 0;
	cur_st  = ST_IDLE;
	abort_key = KY_INVLOC;
	aborted_mode = PM_NONE;
	rem_feed_chg = 0;
	prev_st = -1;
	program_ix = 0;
	ppshot_ix = 0;
	ppparm_ix = 0;
	lcd_changed = 0;
	play_state = PLAY_DONE;
	lcd_updated = 0;
	play_mode = PM_NONE;
	remote_enabled = 0;
	lob_on=0;
// v1.199 start
// Moved the following initialization so it only occurs at battery connect, not wakeup.
//	manual.speed = DEF_SPEED; // defaults parameters for manual program
//	manual.elev =  DEF_ELEV;
//	manual.horiz = DEF_SWEEP;
//	manual.feed =  DEF_FEED;
//	manual.spin =  DEF_SPIN;
// v1.199 end
	man_parm = 0;                        // currently selected parameter for change
// v1.199 start
//	preset_type = PRE_RANDHORZ;
	if (model_type == EL05 || model_type == EL05LE)	// older models with Presets key on control panel
		preset_type = PRE_RANDHORZ;	// Keep this so models with Presets key on control panel still default to Sweep mode first.
// v1.218 start
//	else preset_type = 0xff;		// This prevents Sweep row getting incremented when Sweep mode is entered right after power-on.
	else preset_type = NUMPRESETS;	// One more than max preset_type number, which is invalid. preset_type is signed, so 0xff was actually -1.
// v1.218 end
// v1.199 end
//	memset((char*)&preset_ix[0],0,sizeof(preset_ix));
// 07/03/09 start
//	preset_ix[0] = 5;	// Start Random Horizontal with columns
//	preset_ix[1] = 6; // Start Random Vertical with rows
//	preset_ix[2] = 7; // Start Two-Line with widths
//	preset_ix[3] = 0;	// Start Fully Random with speeds
	preset_ix[0] = 6;	// Start Random Horizontal with columns
	preset_ix[1] = 7; // Start Random Vertical with rows
	preset_ix[2] = 8; // Start Two-Line with widths
	preset_ix[3] = 0;	// Start Fully Random with speeds
// 07/03/09 end

// v1.199 start
// Moved the following initialization so it only occurs at battery connect, not wakeup.
//	for (i=0; i<9; i++) {									// init defined presets		// mjz 10/29/08, commented, too small count
//	for (i=0; i<10; i++) {									// init defined presets		// mjz 10/29/08, 0 thru 9 presets
//		preset[i].speed = DEF_SPEED;
//		preset[i].elev	= DEF_ELEV;					// WAS 35
//		preset[i].horiz = DEF_SWEEP;
// 07/03/09 start
//		preset[i].twolnrw = 0;
// 07/03/09 end
//		preset[i].feed	= DEF_FEED;
//		preset[i].spin	=  DEF_SPIN;
//		preset[i].col	  =  0;
//		preset[i].row		=  0;
//		preset[i].width =  0;
//	}
// v1.199 end

//	memset((char*)&preset[5],0,sizeof(union parm)*3);	// clear the 3 preset spares
// mjz 10/29/08, commented the above line because it caused preset[5] thru preset[7] to have default feed rate of 0,
// which showed up in some drills when they were selected with the new Drills button on the remote control.

	idle_fpass = 1;
	man1shot = 0;
	dpb_thrash = 0;
	memset((char*)&power,1,sizeof(power));	// all 1's
	power.count = 0;												// except the count
	main_ostate = -1;
	loaded_progno = AVAILPRG;								// start out invalid
	home_ticks = 0;													// clear for now
	batt_ticks = 0;
	batt_cnt = 0;
	BatAvgCnt = 0;
	homeshown = 0;
	loop_delay = 0;


#ifndef PHENOMS							// v1.219 added
	for (i=0; i<NUMLOCS; i++) {
		locp = &court_locs[i][0];
		for (j=0; j<NUMMETH; j++) {
			if (locp->speed == 0U) {
				num_methods[i] = j;
				break;
			}
			locp++;
		}
	}
#else									// v1.219 added
	for (i=0; i<NUMLOCS; i++) {
		locp = &court_locs_CLUB[i][0];
		for (j=0; j<NUMMETH; j++) {
			if (locp->speed == 0U) {
				num_methods[i] = j;
				break;
			}
			locp++;
		}
	}
#endif									// v1.219 added

	keyhold = 0;
	repeat = 0;
	ee_progstart = sizeof(flash_header) + flash_header.ops_size;
	ee_progstart += flash_header.npresets * flash_header.parm_size;

// v1.219 start
	RemotePgmMode = 0;
// v1.219 end
// v1.221 start
	BatLevelDisplay = 0;
	BatLevelDisplayTmr = BatLevelDisplayTmrPreload;
	BatPercent = 0;
	PlayLEDPass = 0;
// v1.221 end
}
