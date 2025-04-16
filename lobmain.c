// Lobster Main Loop
//
// C18 to XC8 Compiler Changes
// Replaced all strcpypgm2ram with strcpy.
// C18 to XC8 Compiler Changes

#include "types.h"
#include "state_mach.h"
#include <string.h>
#include "lob.h"
#include "slave.h"
//#include "usart.h"
// C18 to XC8 Compiler Changes
//#include "lcd.h"
//#include "p18f6722.h"
#include <xc.h>
//#include "stdlib.h"
#include <stdlib.h>
// C18 to XC8 Compiler Changes
#include "diag.h"
#include "a2d.h"

// ROM Variable References

// C18 to XC8 Compiler Changes
//extern far rom struct ee_hdr flash_header;
//extern far rom struct el04_loc test_loc;
//extern far rom struct el04_loc test_loc_CLUB;
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
//extern far rom struct el04_loc grinder_locs[];
//extern far rom struct el04_loc grinder_locs_CLUB[];
//extern far rom struct el04_loc power_locs[];
//extern far rom struct el04_loc power_locs_CLUB[];
//extern far rom struct el04_loc allcourt_locs[];
//extern far rom struct el04_loc allcourt_locs_CLUB[];
//extern far rom struct el04_loc transition_locs[];
//extern far rom struct el04_loc transition_locs_CLUB[];
//extern far rom struct el04_loc attdef_locs[];
//extern far rom struct el04_loc attdef_locs_CLUB[];
//extern far rom struct el04_loc foreonly_locs[];
//extern far rom struct el04_loc foreonly_locs_CLUB[];
//extern far rom ushort base_speeds[][2];
//extern const rom struct state_mach lobmach;
//extern const rom struct state_mach lobmach_EL04;
//extern const rom struct state_mach lobmach_EL05LE2;	// v1.186
//extern const rom char*	state_name[];
//extern const rom struct rand_info pre_rand[];
//
//// v1.208 start
////extern const rom ushort preset_mask[];
//extern const rom unsigned short long preset_mask[];
//extern far rom struct el04_loc Beginner_locs[];
//extern far rom struct el04_loc Beginner_locs_CLUB[];
//extern far rom struct el04_loc Intermediate_locs[];
//extern far rom struct el04_loc Intermediate_locs_CLUB[];
//extern far rom struct el04_loc Advanced_locs[];
//extern far rom struct el04_loc Advanced_locs_CLUB[];
//extern far rom struct el04_loc Moonballer_locs[];
//extern far rom struct el04_loc Moonballer_locs_CLUB[];
//extern far rom struct el04_loc Slicer_locs[];
//extern far rom struct el04_loc Slicer_locs_CLUB[];
//extern far rom struct el04_loc Lefty_locs[];
//extern far rom struct el04_loc Lefty_locs_CLUB[];
//// v1.208 end

extern const struct ee_hdr flash_header;
extern const struct el04_loc test_loc;
extern const struct el04_loc test_loc_CLUB;
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];
extern const struct el04_loc grinder_locs[];
extern const struct el04_loc grinder_locs_CLUB[];
extern const struct el04_loc power_locs[];
extern const struct el04_loc power_locs_CLUB[];
extern const struct el04_loc allcourt_locs[];
extern const struct el04_loc allcourt_locs_CLUB[];
extern const struct el04_loc transition_locs[];
extern const struct el04_loc transition_locs_CLUB[];
extern const struct el04_loc attdef_locs[];
extern const struct el04_loc attdef_locs_CLUB[];
extern const struct el04_loc foreonly_locs[];
extern const struct el04_loc foreonly_locs_CLUB[];
extern const ushort base_speeds[][2];
extern const struct state_mach lobmach;
extern const struct state_mach lobmach_EL04;
extern const struct state_mach lobmach_EL05LE2;	// v1.186
extern const char*	state_name[];
extern const struct rand_info pre_rand[];

extern const unsigned short long preset_mask[];
extern const struct el04_loc Beginner_locs[];
extern const struct el04_loc Beginner_locs_CLUB[];
extern const struct el04_loc Intermediate_locs[];
extern const struct el04_loc Intermediate_locs_CLUB[];
extern const struct el04_loc Advanced_locs[];
extern const struct el04_loc Advanced_locs_CLUB[];
extern const struct el04_loc Moonballer_locs[];
extern const struct el04_loc Moonballer_locs_CLUB[];
extern const struct el04_loc Slicer_locs[];
extern const struct el04_loc Slicer_locs_CLUB[];
extern const struct el04_loc Lefty_locs[];
extern const struct el04_loc Lefty_locs_CLUB[];
// C18 to XC8 Compiler Changes

// RAM Variable References 
extern struct moterr ErrorStat;
extern mini_variant elev_corr; 
extern uchar num_methods[];
extern struct shot CurShot,NextShot;
extern struct button_def button[];
// C18 to XC8 Compiler Changes
//extern char lcd_line[][17];
extern char lcd_line[][17];
// C18 to XC8 Compiler Changes
extern uchar prev_st;
extern uchar cur_st;
extern ushort CurShotState;
extern signed char program_ix;
extern signed char ppshot_ix;
extern signed char ppparm_ix;
extern struct persprog act_program;
extern char power_state;
extern char idle_fpass;
// C18 to XC8 Compiler Changes Start
//extern char preset_type;
extern unsigned char preset_type;
// C18 to XC8 Compiler Changes End
extern char preset_ix[];
extern union parm preset[];
extern int a2dValue[NumA2dChan];
extern char power_on;
extern unsigned	d_top_per;
extern unsigned d_bot_per;
extern long d_feed_per;
extern char rem_feed_chg;
extern char ErrorShutDown;
extern unsigned char model_type;

extern char cursor_state;								// blinking cursor

// v1.194 & v1.195 start
extern char LargeSpdDrop;
//extern char BallTimerOn;
//unsigned short rndm_feed;
unsigned short rndm_base_feeds_index;	// name changed for clarity
// v1.194 & v1.195 end
// v1.197 start
unsigned short rndm_base_feeds_index_old;	// previous
// v1.197 end

// v1.205 start
extern int ELEV_CORR_MIN;
// v1.205 end
// v1.219 start
extern unsigned int RemotePgmMode;
// v1.219 end
// v1.221 start
extern char BatLevelDisplay;
extern int BatPercent;
long PlayModeBlink;
// v1.221 end

// RAM Variables 	
signed short eladj = 0;
// v1.192.0U start
signed short elev_test_adjust = 0;	// ifdef elev_test, this gets added to shots in Program mode
// v1.192.0U end
signed short elev_corr_tmp;		// v1.193 added

char timflag;
uchar play_state;
uchar lcd_updated;
long play_mode;
char remote_enabled;
char lob_on;
union parm manual;	// default parameters for manual program
uchar man_parm;     // currently selected parameter for change
char idle_fpass;
char man1shot;
char abort_key;		// key which sent us to abort state
char aborted_mode;	// mode during which we aborted (actually LED)
ulong loop_delay;	// tick count at which to restart loop
ulong start_delay;	// tick count to delay start of 2-line shots
int batval;
int lastbatval;
ulong lastbata2d;
ulong LastBatAvg;
	
// ROM Variables

// C18 to XC8 Compiler Changes
////						 rom ushort sweepcols[7] = { 0-0x140, 0-0x00c0, 0-0x3c, 0, 0x3c, 0xc0, 0x140 };
//						 rom ushort sweepcols[7] = { 0xfec0, 0xff40, 0xffc4, 0, 0x003c, 0x00c0, 0x00140 };
//						 rom char* lob_model;
//                         rom char* lob_version;
//						 rom char* homingmsg		= " Calibrating... ";
//						 rom char* battmsg			= " Battery:     % ";
//						 rom char* shutdnmsg		= " Shutting down! ";
//						 rom char* waitmsg			= "  Please wait.  ";
//
////						 rom char* manualmsg 		= "Press MANUAL or ";
////						 rom char* presetmsg 		= "      PRESETS or";
////						 rom char* programmsg		= "      PROGRAM   ";
//						 rom char* manualmsg 		= "2. Calibrate    ";
//						 rom char* presetmsg 		= "3. Select Mode  ";
//						 rom char* programmsg		= "4. Press PLAY   ";
//
//						 rom char* test_main		= " TEST CALIBRATE ";
//						 rom char* test_lr			= "Move unit lft/rt";
//// v1.193 start
////						 rom char* test_elev		= "+/- Distance    ";
//						 rom char* test_elev		= "+/- Distance  or";
//// v1.193 end
//						 rom char* test_shoot		= "Push TEST to try";
//						 ram char	 keepfeedon;
//
//// Conversion Table from feed rate (in seconds/ball) to pwm values for the feed motor.
////	0x3ff is the maximum PWM limit,	0x0a0 is the minimum PWM limit (slowest feed rate)
//						rom unsigned short base_feeds[16] = {
//							 0x3ff,0x2e0,0x250,0x1e0,	//  2sec -  5sec
//							 0x198,0x160,0x136,0x120,	//  6sec -  9sec
//							 0x110,0x0f8,0x0f0,0x0d8,	// 10sec - 13sec
//							 0x0d8,0x0d8				// 14sec - 15sec
//						};
//						rom unsigned short base_feeds_cont[16] = {
//							 0x385,0x280,0x1e0,0x198,	//  2sec -  5sec
//							 0x160,0x150,0x130,0x120,	//  6sec -  9sec
//							 0x108,0x0f8,0x0ec,0x0d8,	// 10sec - 13sec
//							 0x0d8,0x0d8				// 14sec - 15sec
//						};
//
//const rom char line_start[4] = { 0x00, 0x40, 0x10, 0x50 };

						 const ushort sweepcols[7] = { 0xfec0, 0xff40, 0xffc4, 0, 0x003c, 0x00c0, 0x00140 };
						 const char* lob_model;
                         const char* lob_version;
						 const char* homingmsg		= " Calibrating... ";
						 const char* battmsg			= " Battery:     % ";
						 const char* shutdnmsg		= " Shutting down! ";
						 const char* waitmsg			= "  Please wait.  ";
						 const char* manualmsg 		= "2. Calibrate    ";
						 const char* presetmsg 		= "3. Select Mode  ";
						 const char* programmsg		= "4. Press PLAY   ";
						 const char* test_main		= " TEST CALIBRATE ";
// C18 to XC8 Compiler Changes
//						 const char* test_lr			= "Move unit lft/rt";		// not used
// C18 to XC8 Compiler Changes
						 const char* test_elev		= "+/- Distance  or";
						 const char* test_shoot		= "Push TEST to try";
						 char	 keepfeedon;

// Conversion Table from feed rate (in seconds/ball) to pwm values for the feed motor.
//	0x3ff is the maximum PWM limit,	0x0a0 is the minimum PWM limit (slowest feed rate)
						const unsigned short base_feeds[16] = {
							 0x3ff,0x2e0,0x250,0x1e0,	//  2sec -  5sec
							 0x198,0x160,0x136,0x120,	//  6sec -  9sec
							 0x110,0x0f8,0x0f0,0x0d8,	// 10sec - 13sec
							 0x0d8,0x0d8				// 14sec - 15sec
						};
						const unsigned short base_feeds_cont[16] = {
							 0x385,0x280,0x1e0,0x198,	//  2sec -  5sec
							 0x160,0x150,0x130,0x120,	//  6sec -  9sec
							 0x108,0x0f8,0x0ec,0x0d8,	// 10sec - 13sec
							 0x0d8,0x0d8				// 14sec - 15sec
						};

const char line_start[4] = { 0x00, 0x40, 0x10, 0x50 };
// C18 to XC8 Compiler Changes

void build_curshot(union parm* pp);
void lob_error(void);
void chartoa(const char c, char* cp, char ndig); 
void abort_shot(void);
void stop_all(int code, long *args);
char lob_program(char cmd);
char lob_preset(char cmd);
//void StartFeed(long feedpwm);
void SetFeedRate(long feedpwm);
void SetFeedImm(long feedpwm);
void StartFeedCont(long feedpwm);
void StopFeedMtr(void);
void sm_service(ushort wParam);
void sm_service_EL04(ushort wParam);
void sm_service_EL05LE2(ushort wParam);	// v1.186
void set_serve(union parm* pp);
extern void BuildCmd(int code, int cnt, long* args);
// v1.208 start
//extern char lob_parm(char cmd, uchar *pix, char* pparm, ushort mask);

// C18 to XC8 Compiler Changes
//extern char lob_parm(char cmd, uchar *pix, char* pparm, unsigned short long mask);
extern char lob_parm(char cmd, uchar *pix, signed char* pparm, unsigned short long mask);
// C18 to XC8 Compiler Changes

// v1.208 end
char check_limits(const struct el04_loc* locp, char ix);

// C18 to XC8 Compiler Changes
extern uchar show_prog1(unsigned char field);
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//#pragma code MYCODE
// C18 to XC8 Compiler Changes

char lob_sleep(char cmd)
{
	if (state_just_changed(cmd)) {
		PUTRSDIAG("\n\rSLEEP ");
	}
	return(0);
}

extern volatile unsigned long ticks;
extern unsigned long home_ticks;
extern unsigned long batt_ticks;
extern char FirstShot;
char homeshown = 0;

char lob_idle(char cmd)
{
	char ky;

	if (!homing()) {															// Until we are done homing, leave display alone
		ky = extract_key(abort_key);								//	unpack key only
		if (ky >= LOBNUMBUT) { 											//	if invalid key
			ky = KY_MANUAL;
			abort_key = KY_MANUAL;										//	 pretend pressed MANUAL
		}
		if (aborted_mode & ~(PM_ELIMMODE|PM_MODE)) {//  if any invalid bits are on
			aborted_mode = PM_MANUAL;									//	 assume aborted from manual mode
		}
		SDUMPHEX(abort_key);
		SDUMPHEX(aborted_mode);
		play_mode &= PM_REMOTE;											//	turn off play and mode LEDs
		BuildCmd('3',1,&play_mode);									//  via slave command
		StopFeedMtr();															//	turn off feed motors 
		loop_delay = 0;
// v1.221 start
		start_delay = 0;
// v1.221 end
		if (power_on && (batt_ticks && (batt_ticks < ticks))) {//	if not homing (normal operation)
// v1.186 start
// v1.199 start
//			if (model_type == EL04){
			if (model_type == EL04 || model_type == CLUB1){	// models with EL04 control panel
// v1.199 end
			    if (extract_key(abort_key) != KY_PLAY) {	//  if abort NOT due to pressing PLAY
				    sm_service_EL04(ky);													//	 mode indicated by key
			    } else {																	//	else (pressed PLAY)
				    switch(aborted_mode & PM_MODE) {				//	 mode indicated by aborted mode
					    case PM_MANUAL: sm_service_EL04(ky=KY_MANUAL); break;
					    case PM_PRESET: sm_service_EL04(ky=KY_PRESET); break;
					    case PM_PERSONAL: sm_service_EL04(ky=KY_PROG); break;
					    case PM_NONE:								break;
					    default:									break;
				    }
			    }
            }
// v1.191.0R start
//			else if (model_type == EL05LE2){
// v1.199 start
//			else if (model_type == EL05LE2 || model_type == CLUB){
			else if (model_type == EL05LE2 || model_type == CLUB2){	// models with new EL05LE2 panel
// v1.199 end
// v1.191.0R end
			    if (extract_key(abort_key) != KY_PLAY) {	//  if abort NOT due to pressing PLAY
				    sm_service_EL05LE2(ky);													//	 mode indicated by key
			    } else {																	//	else (pressed PLAY)
				    switch(aborted_mode & PM_MODE) {				//	 mode indicated by aborted mode
					    case PM_MANUAL: sm_service_EL05LE2(ky=KY_MANUAL); break;
					    case PM_PRESET: sm_service_EL05LE2(ky=KY_PRESET); break;
					    case PM_PERSONAL: sm_service_EL05LE2(ky=KY_PROG); break;
					    case PM_NONE:															break;
					    default:																	break;
				    }
			    }
            }
            else {    // all other models
			    if (extract_key(abort_key) != KY_PLAY) {	//  if abort NOT due to pressing PLAY
				    sm_service(ky);													//	 mode indicated by key
			    } else {																	//	else (pressed PLAY)
				    switch(aborted_mode & PM_MODE) {				//	 mode indicated by aborted mode
					    case PM_MANUAL: sm_service(ky=KY_MANUAL); break;
					    case PM_PRESET: sm_service(ky=KY_PRESET); break;
					    case PM_PERSONAL: sm_service(ky=KY_PROG); break;
					    case PM_NONE:															break;
					    default:																	break;
				    }
			    }
			}
// v1.186 end
		}

		if (homeshown && (batt_ticks && (batt_ticks < ticks))) {
			lcd_updated = 1;
			homeshown = 0;														//	indicate homing not showing
//			strcpypgm2ram(&lcd_line[0][0], lob_model);
			strcpy(&lcd_line[0][0], "1. Press TEST   ");
			strcpy(&lcd_line[1][0], manualmsg);//	Show Idle message
			strcpy(&lcd_line[2][0], presetmsg);
//			strcpypgm2ram(&lcd_line[2][0], presetmsg);	// 07/03/09 write it again, first didn't take
			strcpy(&lcd_line[3][0], programmsg);
		}
	}
	return(0);
}

// Manual Mode State Handler
//
char lob_manual(char cmd)
{
// C18 to XC8 Compiler Changes start
//	char* pparm = (char *)&manual;
	signed char * pparm = (signed char *)&manual;
// C18 to XC8 Compiler Changes end
	long arg, ang;
	if (state_just_changed(cmd)) {										// if just transitioned to manual mode
		clear_lcd(0xf);
		eladj = 0;
		SPUTRSDIAG("\n\rManual - ");
		play_mode = (play_mode & PM_ELIMMODE) | PM_MANUAL;	//	turn off preset & program turn on manual
		BuildCmd('3',1,&play_mode);
	}
	lob_parm(cmd,&man_parm,pparm,PRE_SPEED|PRE_ELEV|PRE_HORIZ|PRE_FEED|PRE_SPIN);		// Random Horizontal
//	lob_parm(cmd,&man_parm,pparm,PRE_SPEED|PRE_ELEV|PRE_FEED|PRE_SPIN);		// Random Horizontal
	arg = (ushort)20 * (ushort)manual.elev;
	if ((ushort)arg != CurShot.y) {
		CurShot.y = (ushort)arg;
		arg += elev_corr.s;
		BuildCmd(SC_Y1, 1, &arg);
	}
	if (sweepcols[(ushort)(manual.horiz + 3)] != CurShot.x) {
		CurShot.x = sweepcols[(ushort)(manual.horiz + 3)];
		putrsDIAG("\n\rMswp:");
		dumphex(manual.horiz);
		dumpshort(sweepcols[(ushort)(manual.horiz + 3)]);
		arg = CurShot.x;
// v1.204 start
// The following BuildCmd was not getting executed, when the debug messages were turned off to allow for the Wi-Fi remote control to
// use the serial port. The following delay was the minimum+1 that caused the following BuildCmd to start working again.
		Delay1KTCYx(5);
// v1.204 end
		BuildCmd(SC_X1, 1, &arg); 
	}
	set_serve(&manual);
	return(0);
}

// Sets "new" values for the serve motors
//	inputs:
//		pp - pointer to parameters union
//  WARNING:
//		requires that the server motors already be turned on or nothing will happen
//
void set_serve(union parm* pp)
{
	unsigned short topval, botval, tmp;
	short stmp;
	unsigned char ixa;

	if (ErrorStat.LoBat) return;

		ixa = pp->speed/5; 											// convert speed to table index
		stmp = (pp->spin > -1) ? pp->spin : 0 - pp->spin;			// absolute value of spin
	
		if (stmp == 3) {											// if max spin (+ or -)
			topval = base_speeds[ixa][1];							//	use alternate speed table
			botval = 0x6000;										//	and absolute minimum speed
		} else {													// else (0, 1 or 2 spin (+ or -)
			topval = base_speeds[ixa+stmp][0];						//	std speed plus spin offset
			botval = base_speeds[(stmp==0) ? ixa : (ixa - 1)][0];	// all but 0 spin offset by 1
		}
	
	//	if (manual.spin < 0) {	
		if (pp->spin < 0) {											// if spin < 0 (bottom?)
			tmp = topval;											//	reverse the top and bottom
			topval = botval;										//	speed values
			botval = tmp;
		}
	
		if ((topval > SRV_MINIMUM) || (topval < SRV_MAXIMUM)) {
			stop_all(0,(long*)0);
			ErrorStat.TopPeriod = 1;
			ErrorShutDown = 1;
		} else if ((botval > SRV_MINIMUM) || (botval < SRV_MAXIMUM)) {
			stop_all(0,(long*)0);
			ErrorStat.BotPeriod = 1;
			ErrorShutDown = 1;
		}else {
			d_top_per = topval;										// change the wheels in motion
			d_bot_per = botval;
		}
}

void build_curshot(union parm* pp)
{
	unsigned short topval, botval, tmp;
	short stmp;
	unsigned char ixa;

	stmp = (short)pp->horiz;									//	pick up horizontal value
//	CurShot.x = stmp * 27; //pp->horiz * 27; 					//	convert sweep angle to motor value
	CurShot.x = sweepcols[stmp+3];
//	putrsDIAG("bld_curshot ");
//	dumpshort(CurShot.x);
//	dumphex(stmp);
//	dumphex(stmp+3);
//	dumpshort(sweepcols[stmp+3]);
	DUMPSHORT((unsigned long)CurShot.x);
	tmp = pp->elev;												//	convert elevation to motor value
	CurShot.y = 20 * tmp; //20 * pp->elev;
	DUMPSHORT((unsigned long)CurShot.y);
	ixa = pp->speed/5; 											//	convert speed to table index
	PUTRSDIAG("<");
	DUMPSHORT((unsigned long)ixa);
	stmp = (pp->spin > -1) ? pp->spin : 0 - pp->spin;			//	absolute value of spin
	DUMPSHORT((unsigned long)stmp);
	PUTRSDIAG(">");
	if (stmp == 3) {											// if max spin (+ or -)
		topval = base_speeds[ixa][1];							//	use alternate speed table
		botval = 0x6000;										//	and absolute minimum speed
	} else {													// else (0, 1 or 2 spin (+ or -)
		topval = base_speeds[ixa+stmp][0];						//	std speed plus spin offset
		botval = base_speeds[(stmp==0) ? ixa : (ixa - 1)][0];	//	all but 0 spin offset by 1
	}
//	if (manual.spin < 0) {
	if (pp->spin < 0) { 										// if spin < 0 (bottom?)
		tmp = topval;											//	reverse the top and bottom
		topval = botval;										//	speed values
		botval = tmp;
	}
	CurShot.top = topval;										// top motor
	DUMPSHORT((unsigned long)topval);
	CurShot.bot = botval;										// bottom motor
	DUMPSHORT((unsigned long)botval);
	start_shot();												// invoke shot logic
	play_state = PLAY_BEGIN;									// initial play state
}



//far rom struct el04_loc* lastLoc = &court_locs[DEFLOCN][DEFMETH]; // pointer to most recent shot

// C18 to XC8 Compiler Changes
//far rom struct el04_loc* lastLoc;
const struct el04_loc* lastLoc;
// C18 to XC8 Compiler Changes

// Checks methods for specified shot
//	if ix is 0xff, it returns a count of the the number which match the limits
//	else it returns the ixth method which matches the limits
//







#ifdef notdef
// The following code for the routine "check_limits" stopped working when the compiler
// was changed from C18 to XC8.
// Nothing was passing the "if ((locp->speed >= FR_MINSPDABS) && (locp->speed <= FR_MAXSPDABS)) {"
// so the routine would fall through and method 0 was always being selected as the method for the
// randomly selected location. This resulted in every shot having a -3 spin at the slowest
// available speed with that spin for that location.
// locp->speed was always looking larger than FR_MINSPDABS and FR_MAXSPDABS.
// In the replacement routine that follows, I added variables to equal locp->speed and locp->spin.

// C18 to XC8 Compiler Changes
//char check_limits(far rom struct el04_loc* locp, char ix)
char check_limits(const struct el04_loc* locp, char ix)
// C18 to XC8 Compiler Changes

{
	char i, j;
	char absSpeed, absSpin, dynSpeed, dynSpin;

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* tmploc = locp;
//	const struct el04_loc* tmploc = locp;
	const struct el04_loc* //tmploc = locp;
// C18 to XC8 Compiler Changes

// v1.219 start
//	if ((model_type != CLUB1) && (model_type != CLUB2)) // all models except CLUBs
//		lastLoc = &court_locs[DEFLOCN][DEFMETH]; 		// pointer to most recent shot
//	else
//		lastLoc = &court_locs_CLUB[DEFLOCN][DEFMETH];	// pointer to most recent shot, CLUB model only
#ifndef PHENOMS
        lastLoc = &court_locs[DEFLOCN][DEFMETH]; 		// pointer to most recent shot
#else
		lastLoc = &court_locs_CLUB[DEFLOCN][DEFMETH];	// pointer to most recent shot, CLUB model only
#endif
// v1.219 end

	putrsDIAG("\n\r");
	putrsDIAG("\n\rlocp = ");
	dumplong(locp);
	putrsDIAG("\n\rlocp->speed = ");
	dumplong(locp->speed);
	putrsDIAG("\n\rlocp->spin = ");
	dumplong(locp->spin);
	putrsDIAG("\n\r");
	
	for (i=0,j=0; i<NUMMETH; i++, locp++) {							// search all methods
//		absSpeed = ((locp->speed >= FR_MINSPDABS) && (locp->speed <= FR_MAXSPDABS)) ? 1 : 0;
//		dumphex(absSpeed);
//		dynSpeed = ((locp->speed >= (lastLoc->speed - FR_MAXSPDDEC)) && (locp->speed <= (lastLoc->speed + FR_MAXSPDINC))) ? 1 : 0;
//		dumphex(dynSpeed);
//		absSpin	 = ((locp->spin  >= FR_MINSPNABS) && (locp->spin  <= FR_MAXSPNABS)) ? 1 : 0;
//		dumphex(absSpin);
//		dynSpin	 = ((locp->spin >= (lastLoc->spin - FR_MAXSPNDEC)) && (locp->spin <= (lastLoc->spin + FR_MAXSPNINC))) ? 1 : 0;
//		dumphex(dynSpin);
#ifdef FR_ABSCHGSPEED		// use absolute limits to range of speed for shots
//		putrsDIAG("AbsSpeed");
		if ((locp->speed >= FR_MINSPDABS) && (locp->speed <= FR_MAXSPDABS)) {	//	if 50-65 mph
#else									// use dynamic limits to range of speed for shots
//Z		putrsDIAG("DynSpeed");
		if ((locp->speed >= (lastLoc->speed - FR_MAXSPDDEC))
		 && (locp->speed <= (lastLoc->speed + FR_MAXSPDINC))) { // if within dynamic limits 
#endif								// end of speed limiting ifdef... else... endif
#ifdef FR_ABSCHGSPIN		// use absolute limits to range of speed for shots
//			putrsDIAG("AbsSpin");
			if ((locp->spin >= FR_MINSPNABS) && (locp->spin <= FR_MAXSPNABS)) {		//	 and +2 to -2 spin
#else									// use dynamic limits to range of spin for shots
//Z			putrsDIAG("DynSpin");
		if ((locp->spin >= (lastLoc->spin - FR_MAXSPNDEC))
		 && (locp->spin <= (lastLoc->spin + FR_MAXSPNINC))) { // if within dynamic limits 
#endif								// end of spin limiting ifdef... else... endif
//Z				putrsDIAG("#");
//Z				dumphex(i);
				j++;																				//	  count of matches
				if (j == (ix+1)) {													//	if count equals randomly chosen value
					lastLoc = locp;														//	 remember for next pass what if dynamic
					break;																		//	 stop searching
				}
			}
		} 
	}
	return ((j-1) == ix)? i : j;											// report either the selected method or the number of matches
}

#endif


char check_limits(const struct el04_loc* locp, char ix)
{
	char i, j;
	unsigned char absSpeed, absSpin, dynSpeed, dynSpin;
	
	unsigned char locpSpeed;
	signed char locpSpin;

#ifndef PHENOMS
        lastLoc = &court_locs[DEFLOCN][DEFMETH]; 		// pointer to most recent shot
#else
		lastLoc = &court_locs_CLUB[DEFLOCN][DEFMETH];	// pointer to most recent shot, CLUB model only
#endif
	
	for (i=0,j=0; i<NUMMETH; i++, locp++) {							// search all methods
//		putrsDIAG("AbsSpeed");
		putrsDIAG("\n\rlocp->speed = ");
		dumphex(locp->speed);
		dumphex(FR_MINSPDABS);
		dumphex(FR_MAXSPDABS);

		locpSpeed = locp->speed;
		locpSpin = locp->spin;

//		if ((locp->speed >= FR_MINSPDABS) && (locp->speed <= FR_MAXSPDABS)) {	//	if 50-65 mph
		if ((locpSpeed >= FR_MINSPDABS) && (locpSpeed <= FR_MAXSPDABS)) {	//	if 50-65 mph
			putrsDIAG("AbsSpin");
//			if ((locp->spin >= FR_MINSPNABS) && (locp->spin <= FR_MAXSPNABS)) {		//	 and +2 to -2 spin
			if ((locpSpin >= FR_MINSPNABS) && (locpSpin <= FR_MAXSPNABS)) {		//	 and +2 to -2 spin
				j++;																				//	  count of matches
				if (j == (ix+1)) {													//	if count equals randomly chosen value
					lastLoc = locp;														//	 remember for next pass what if dynamic
					break;																		//	 stop searching
				}
			}
		} 
	}
//	putrsDIAG("\n\ri = ");
//	dumphex(i);
//	putrsDIAG("\n\rj = ");
//	dumphex(j);
//	putrsDIAG("\n\rix = ");
//	dumphex(ix);
//	putrsDIAG("\n\r");
	return ((j-1) == ix)? i : j;											// report either the selected method or the number of matches
}



// Limit Method to only allow "random" shots to be those which can be achieved
//	within the feed rate.w
//	Original - limit to be 50-60mph and +2 to -2 spin
//	03/08/08 - limit to +/- 5 mph and +2 to -2 spin
//		locp - pointer to the court table data for the desired shot - 1st method
//		ix	 - randomly chosen default method - which is to be preferred
//


// C18 to XC8 Compiler Changes
//char limit_method(far rom struct el04_loc* locp, ushort ix)
char limit_method(const struct el04_loc* locp, ushort ix)
// C18 to XC8 Compiler Changes

{
	char i,	j = NUMMETH+2;

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* tmpLoc = locp;
//	const struct el04_loc* tmpLoc = locp;
// C18 to XC8 Compiler Changes

	i = check_limits(locp, NUMMETH+2);		// find out how many methods match
	if (i < NUMMETH) {
		j = check_limits(locp, (char)(ix % i));
		i=j;
	} else {
		i = NUMMETH;
	}
	return (i);
}

// Locates the method for the specified location which uses the speed and spin chosen
//	locp - pointer to first method for location desired
//	pp	 - preset parameters (two of which are speed and spin)
//	min	 - 0: if a match is not found, use the smallest elevation
//				 1: if a match is not found, use the largest elevation
//				 2: if a match is not found, use the smallest sweep
//				 3: if a match is not found, use the largest sweep
//				

// C18 to XC8 Compiler Changes
//char serve_method(far rom struct el04_loc* locp, union parm* pp, char flags)
char serve_method(const struct el04_loc* locp, union parm* pp, char flags)
// C18 to XC8 Compiler Changes

{
	char i;
	char hix = NUMMETH;
	char lox = NUMMETH;
	
	for (i=0; i<NUMMETH; i++, locp++) {		// search all methods for given location
		if ((pp->speed == locp->speed) 			//	if speed and spin both match
		 && (pp->spin == locp->spin)) {
			break;														//	 we are done (i is the desired method)
		} else if (!(flags & PRE_HIGHEST)) {//	else if looking for a small value
			if (lox == NUMMETH) {							//	 if no valid minimum yet
				lox = i;												//		start with the first one
			} else if (flags & PRE_ELEVATN) {	//	 else if need minimum elevation
				if (locp[i].elev < locp[lox].elev)
					lox = i;
			} else {													//	 else if need minimum sweep
				if (locp[i].sweep < locp[lox].sweep)
					lox = i;
			}
		} else {														//	else (we are looking for a big value)
			if (hix == NUMMETH)	{							//	 if no valid maxinimum yet
				hix = i;												//		start with the first one
			} else if (flags & PRE_ELEVATN) {	//	 else if need minimum elevation
				if (locp[i].elev < locp[hix].elev)
					hix = i;
			} else {													//	 else if need minimum sweep
				if (locp[i].sweep < locp[hix].sweep)
					hix = i;
			}
		}
	}
	if (i >= NUMMETH) {													// if exact match was not found
		if (flags) {															//	if alternate choice was requested
			i = (flags & PRE_HIGHEST) ? hix : lox;	//	 use min or max as requested 
		}																					//	else (just want to know no such method)
	}																						// else (just use exact match)
	return (i);
}

unsigned char bias[3] = { 0,0,0 };						// track
unsigned char lastrow = 0;										// remember row we are "on"

char lob_play(char cmd)
{
//	unsigned short topval, botval, tmp;
	static signed char loc;
	static unsigned char line = 0;
	char premode;
	static char preshot_ix;
	static char preposition = 0;
	union parm* pp;

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* shotp;
	const struct el04_loc* shotp;
// C18 to XC8 Compiler Changes

	uchar method;
	ushort rnd;
    static char pp_prev_loc;                                 // 02/08/09
	
//	putrsDIAG("\n\rlob_play");
	if (state_just_changed(cmd)) {							// if just changed to play state
		eladj = 0;
		man1shot = 0;
		FirstShot = 1;
		preposition = 0;
        pp_prev_loc = 0;                                    // 02/08/09
		SPUTRSDIAG("\n\rPlay ");
		play_mode |= PM_PLAYING;							// play led ON in addition to others
		BuildCmd('3',1,&play_mode);							// and tell slave to do it
// v1.221 start
//#ifdef notdef
//		if (!remote_enabled) {								// if not "using" the remote
//			start_delay = ticks + TENSEC;					// allow time for user to get into position
//			start_delay = ticks + TWENTYSEC;				// allow time for user to get into position
//			PlayModeBlink = play_mode;						// Copy LEDs for blinking control without affecting original LED setting.
//		} 
//		else {												// else we are "using" the remote
//			start_delay = 0;								// play begins as soon as lobster iosready
//		}
//#endif
//		start_delay = 0;
// v1.221 end
		if (play_mode & PM_MANUAL) {								//	if manual mode
			pp = &manual;															//	 use manual parameters
// v1.194 & v1.195 start
			LargeSpdDrop = 0;									// Initialize for random feed rate option.
			putrsDIAG("\n\rLargeSpdDrop = 0");
			srand(ticks);										// Now need it for manual mode with random feed rate.
// v1.194 & v1.195 end
			if (!start_delay) {
// v1.194 & v1.195 start
				if (pp->feed == P_FEEDMIN) {					// P_FEEDMIN is used for random feed selection
// v1.197 start
//					rndm_base_feeds_index = RANDOM_FEED;		// do it this way, so randomly chosen feed can be sent to diag port
					while (rndm_base_feeds_index == rndm_base_feeds_index_old) {
						rndm_base_feeds_index = RANDOM_FEED;	// do it this way, so randomly chosen feed can be sent to diag port
					}
					rndm_base_feeds_index_old = rndm_base_feeds_index;
//  v1.197 end
					putrsDIAG("\n\rrndm_base_feeds_index = ");
					dumphex(rndm_base_feeds_index);
					STARTFDCONT(base_feeds_cont[rndm_base_feeds_index]);
				}
				else
// v1.194 & v1.195 end
				STARTFDCONT(base_feeds_cont[pp->feed - 2]);		//	start feed motor (continuous)
				PUTRSDIAG("\n\rafter manual feed ");
			}
			FirstShot = 0;
		} else if (play_mode & PM_PRESET) {			 		//	else if preset mode
			srand(ticks);
			line = 1;
// C18 to XC8 Compiler Changes Start						// preset_type is no longer signed
//			if  (preset_type >= 0) {						// if preset method chosen
				pp = &preset[preset_type];					// set up to select among submodes
				preshot_ix = 0;
//			}
// C18 to XC8 Compiler Changes End
			FirstShot = 1;
		} else {																		//	else (eventually program mode)
// The following lines caused the 2nd shot to be written as an invalid shot if the user went
//to Program mode and wanted to play the saved sequence rather than start making new changes.
//			if (ppshot_ix++ < NUMSHOT) {							//	 if program not full
//				act_program.shot[ppshot_ix] = INV_SHOT;	//		next shot is invalid (terminate)
//			}
			ppshot_ix = -1;
			FirstShot = 1;
			show_prog1(0);														// show shot list while playing
// v1.192.0U start
			elev_test_adjust = 0;	// ifdef elev_test, this gets added to shots in Program mode, start with 0 if Play just started
// v1.192.0U end
		}																						//  arg=0 to prevent > < on any field
//		putrsDIAG("sd:");
//		dumphex(start_delay);
	}
	if (start_delay) {														// if initial delay required
		if (start_delay > ticks) {									//  if insufficient delay has occurred
// v1.221 start
//			if (remote_enabled) {											//	 if user wants to cancel delay
//				start_delay = 0;												//		do so
//				if (play_mode & PM_MANUAL) { 						//		if manual mode
//					STARTFDCONT(base_feeds_cont[pp->feed - 2]);//start feed motor (continuous)
//					PUTRSDIAG("\n\rafter manual feed ");
//				}
//			} else {																	//	 else (still waiting)
				return(0);															// 		exit and try later
//			}
		} else {																		//	else (done waiting)
// v1.221 start
			play_mode = play_mode | PM_PLAYING;					// Guarantee LED is on after flashing.
			BuildCmd('3',1,&play_mode);							// Send LED command.
// v1.221 end
			start_delay = 0;													//	 indicate no delay
			if (play_mode & PM_MANUAL) {								//	if manual mode
				STARTFDCONT(base_feeds_cont[pp->feed - 2]);		//	start feed motor (continuous)
				PUTRSDIAG("\n\rafter manual feed ");
			}
		}
	}
	if (play_mode & PM_MANUAL) {
// C18 to XC8 Compiler Changes start
//		lob_parm(cmd,&man_parm,(char*)&manual,PRE_SPEED|PRE_ELEV|PRE_HORIZ|PRE_FEED|PRE_SPIN);
		lob_parm(cmd,&man_parm,(signed char*)&manual,PRE_SPEED|PRE_ELEV|PRE_HORIZ|PRE_FEED|PRE_SPIN);
// C18 to XC8 Compiler Changes end
		pp = &manual;
		FirstShot = 0;
		if (lcd_updated) {
			keepfeedon = 1;
// v1.194 & v1.195 start
//			STARTFDCONT(base_feeds_cont[pp->feed - 2]);		//	start feed motor (continuous)
			if (pp->feed == P_FEEDMIN) {						// P_FEEDMIN is used for random feed selection
				if (LargeSpdDrop) {								// If a ball was thrown, then choose another feed rate.
					putrsDIAG("\n\rLargeSpdDrop");
//					BallTimerOn = 1;
// v1.197 start
//					rndm_base_feeds_index = RANDOM_FEED;		// do it this way, so randomly chosen feed can be sent to diag port
					while (rndm_base_feeds_index == rndm_base_feeds_index_old) {
						rndm_base_feeds_index = RANDOM_FEED;	// do it this way, so randomly chosen feed can be sent to diag port
					}
					rndm_base_feeds_index_old = rndm_base_feeds_index;
//  v1.197 end
					putrsDIAG("\n\rrndm_base_feeds_index = ");
					dumphex(rndm_base_feeds_index);
					STARTFDCONT(base_feeds_cont[rndm_base_feeds_index]);
					LargeSpdDrop = 0;
					putrsDIAG("\n\rLargeSpdDrop = 0");
				}
			}
			else {
				if (LargeSpdDrop) {
					LargeSpdDrop = 0;							// Only used in random feed rate selection above.
					putrsDIAG("\n\rLargeSpdDrop = 0");
				}
				// The following line is necessary to allow for continuous checking of feed rate selection change.
				STARTFDCONT(base_feeds_cont[pp->feed - 2]);		// start feed motor (continuous)
			}
// v1.194 & v1.195 end
			build_curshot(pp);
			man1shot = 1;
			PUTRSDIAG("\n\rRecalc MANUAL\n\r");
		}
	} else if ((play_mode & PM_PRESET)) {
// v1.194 & v1.195 start
		pp = &preset[preset_type];
// v1.194 & v1.195 end
		lob_parm(cmd,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[SPEED_IX]),preset_mask[preset_type]);
			if ((preset_type <= PRE_RANDVERT) || (preset_type == PRE_TWOLINE)) {
				set_serve(&preset[preset_type]);
// v1.194 & v1.195 start
				if ((preset_type <= PRE_RANDVERT) && (play_mode & PM_PLAYING)) {	// LargeSpdDrop is already qualified with random
					if (LargeSpdDrop) {												// feed rate selection.
						play_state = PLAY_DONE;
					}						
				}
// v1.194 & v1.195 end
			}
			if (!((play_mode & PM_PRESET) && loop_delay && (preshot_ix==0) && (preset_type > PRE_FULLRAND))) {
				if ((preset_type == PRE_TWOLINE) 								// if doing the two line drill
							&& (preset[preset_type].feed == 2)				//	and 2 sec/ball is specified
// v1.201 start
//							&& (preset[preset_type].width == 0))			//	for the wide drill
							&& ((preset[preset_type].width == 0)			//	for wide width 2-Line
							|| (preset[preset_type].width == 1)))
// v1.201 end
					STARTFEED(base_feeds_cont[1]);								//	 then use 3 sec/ball
// v1.194 & v1.195 start
//				else
//					STARTFEED(base_feeds_cont[preset[preset_type].feed - 2]);// if feed motor started, change feed rate immediately,
				else {
					if (preset[preset_type].feed == P_FEEDMIN) {
					// If random feed (P_FEEDMIN is now random feed), don't change the feed rate here. Don't change immediately,
					// because if the feed rate is randomly generated here, it will continuously change.
					}
// v1.208 start
//					else
//						STARTFEED(base_feeds_cont[preset[preset_type].feed - 2]);// if feed motor started, change feed rate immediately,
					else {
						STARTFEED(base_feeds_cont[preset[preset_type].feed - 2]);// if feed motor started, change feed rate immediately,
					}
// v1.208 end
				}
// v1.194 & v1.195 end

			rem_feed_chg = 0;
		}
	} 
	if (play_state == PLAY_DONE) {								// if last shot was completed
//		putrsDIAG("\n\rPLAY_DONE");
		if ((play_mode & PM_MANUAL) && !man1shot) {
			man1shot = 1;
			PUTRSDIAG("\n\rplay_done build\n\r");
			build_curshot(pp);
		} else if (play_mode & PM_PRESET) {					// else if one of the presets
			if (loop_delay && (preshot_ix == 0) && (preset_type > PRE_FULLRAND)) {		//	if timeout active & set's shot 1
				if (loop_delay > ticks){								//	 if timeout is not yet complete
					if (!preposition && (preset_type != PRE_TWOLINE)) {
						return (0);														//	 wait until it is done
					}
				} else {
					loop_delay = 0;							//	 prevent retrigger
					putrsDIAG("\n\rgo");
				}
			}
			pp = &preset[preset_type];						//	parameters for preset submode

	#ifdef	notdef	// v1.190 start
// 07/03/09 start
//			premode = (preset_type * 6);					// index into preset random tables
			if (preset_type <= PRE_TWOLINE)					// Random Horizontal, Random Vertical, or 2-Line Row A
			premode = (preset_type * 6);					// index into preset random tables
// 07/07/09 start
//			if (preset_type == PRE_FULLRAND)
//				premode = ((preset_type * 6) + 3);			// added 3 to get beyond new 2-Line row indexes
			if (preset_type == PRE_FULLRAND)
				premode = ((preset_type * 6) + 6);			// added 6 to get beyond new 2-Line row indexes
// 07/07/09 end
			if (preset_type == PRE_TWOLINE) {
				if (preset[PRE_TWOLINE].twolnrw == 1)		// If the 2-Line row selected is row B,
					premode = premode + 3;					// index for row B, wide width.
				if (preset[PRE_TWOLINE].twolnrw == 2)
					premode = premode + 6;					// row C, wide width
// 07/07/09 start
                if (preset[PRE_TWOLINE].twolnrw == 3)
					premode = premode + 9;                  // random row, wide width
// 07/07/09 end
			}
// 07/03/09 end
// 07/03/09 start
//			premode += ((preset_type == 3) ? 0 : pp->value[preset_type+5]);
			premode += ((preset_type == 3) ? 0 : pp->value[preset_type+6]); // up to 6 preset sub selections
//			putrsDIAG("\n\rpremode = ");
//			dumphex(premode);
// 07/03/09 end
	#endif	// v1.190 end
// v1.190 start
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
			premode += ((preset_type == 3) ? 0 : pp->value[preset_type+6]); // up to 6 preset sub selections
// v1.190 end

			loc = pre_rand[premode].start;							// first location for preset submode
// v1.219 start
//			if ((model_type != CLUB1) && (model_type != CLUB2)) 	// all models except CLUBs
//			    shotp = &court_locs[loc][0];						// first method for selected location
//			else
//				shotp = &court_locs_CLUB[loc][0];  					// CLUB model only
#ifndef PHENOMS
			    shotp = &court_locs[loc][0];						// first method for selected location
#else
				shotp = &court_locs_CLUB[loc][0];  					// CLUB model only
#endif
// v1.219 end
			switch (preset_type) {									// submodes are handled differently
			case PRE_RANDHORZ:										// Random Horizontal Submode
				putrsDIAG("\n\r");
				putrsDIAG("\n\rRandom Horizontal (Sweep) Mode");
				keepfeedon = 1;										// always continuous shooting
				method = serve_method(shotp,pp,PRE_LSWEEP);			// convert speed/spin to method
				shotp += method;									// point to that exact shot
// v1.190 start
				if (preset[PRE_RANDHORZ].col !=1) {					// if selected row A, B, or C, not random
//				CurShot.xLeft = shotp->sweep;						// use leftmost as range limit
				CurShot.xLeft = shotp->sweep;						// use leftmost location on the row as sweep range limit
// v1.190 end
// v1.190 start
//					loc = pre_rand[premode].max;					// leftmost on row
					loc = pre_rand[premode].max;					// use rightmost location on the row for elevation and sweep values
// v1.190 end
					putrsDIAG("\n\rloc = ");
					dumphex (loc);
// v1.219 start
//                	if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//				    	shotp = & court_locs[loc][0];
//					else
//						shotp = & court_locs_CLUB[loc][0];			// CLUB model only
#ifndef PHENOMS
				    	shotp = & court_locs[loc][0];
#else
						shotp = & court_locs_CLUB[loc][0];			// CLUB model only
#endif
// v1.219 end
					method = serve_method(shotp,pp,PRE_HSWEEP);
					shotp += method;
					CurShot.yLo	= shotp->elev;						//	 assume elevation is standard
// v1.190 start
					putrsDIAG("\n\rRH");
				} else {											// else if random elevation selected
// For Random Horizontal (sweep) mode, random elevation cannot be done as start/stop method, as it is done in 2-Line mode with random
// elevation selected, because in Random Horizontal mode, a ball thrown is not detected (shot done), so the elevation is never moved
// to a new position for the next shot. A continuous elevation oscillation must be used, instead.

// Searching the left court column/line of shot locations from nearest to farthest locations, get the left sweep angle from the
// first shot that is found with the correct speed and spin values. Since the left sweep angle is taken from the nearest found shot,
// and the nearer shots have wider angles than farther shots, the farther shots may be too wide. The alternative would be to take the
// left sweep angle from the farthest found shot, in which case the nearer shots may be too narrow.
				    method = NUMMETH;
					for (loc = pre_rand[premode].range;								// start with nearest shot on left line
				        	(loc >= pre_rand[premode].start) && (method >= NUMMETH);// until no more or found
				          		loc -= 6) {		                        			// moving up one row each time
					        putrsDIAG("\n\rNearest loc for CurShot.xLeft= ");
					        dumphex(loc);
// v1.219 start
//                			if ((model_type != CLUB1) && (model_type != CLUB2))		// all models except CLUBs
//								shotp = & court_locs[loc][0];						// point to first method for location
//							else
//								shotp = & court_locs_CLUB[loc][0]; 					// CLUB model only
#ifndef PHENOMS
    					        shotp = & court_locs[loc][0];						// point to first method for location
#else
								shotp = & court_locs_CLUB[loc][0]; 					// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);						// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
					}
				    shotp += method;												// parameters for that method
				    CurShot.xLeft = shotp->sweep;									// set left sweep angle

// Searching the left court column/line of shot locations from farthest to nearest locations, get the elevation for the deepest
// shot that is found with the correct speed and spin values.
			    	method = NUMMETH;
			        for (loc = pre_rand[premode].start;								// start with farthest shot on left line
							(loc <= pre_rand[premode].range) && (method >= NUMMETH);// until no more or found
								loc += 6) {		                       				// moving down one row each time
				    	    putrsDIAG("\n\rFarthest loc for CurShot.yLo = ");
					        dumphex(loc);
// v1.219 start
//                			if ((model_type != CLUB1) && (model_type != CLUB2))		// all models except CLUBs
//								shotp = &court_locs[loc][0];						// point to first method for location
//							else
//								shotp = &court_locs_CLUB[loc][0]; 	 				// CLUB model only
#ifndef PHENOMS
								shotp = &court_locs[loc][0];						// point to first method for location
#else
								shotp = &court_locs_CLUB[loc][0]; 	 				// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);						// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
					}
				    shotp += method;												// parameters for that method
				    CurShot.yLo	= shotp->elev;										// set elevation for deepest shot

// Searching the right court column/line of shot locations from nearest to farthest locations, get the elevation and sweep angle
// from the first shot that is found with the correct speed and spin values. Since the right sweep angle is taken from the nearest
// found shot, and the nearer shots have wider angles than farther shots, the farther shots may be too wide. The alternative would
// be to take the right sweep angle from the farthest found shot, in which case the nearer shots may be too narrow.
				    method = NUMMETH;
					for (loc = (pre_rand[premode].range + pre_rand[premode].incr);	// start with nearest shot on rightmost line
				        	(loc >= pre_rand[premode].max) && (method >= NUMMETH);	// until no more or found
				          		loc -= 6) {		                        			// moving up one row each time
					        putrsDIAG("\n\rNearest loc for Curshot.x and Curshot.y = ");
					        dumphex(loc);
// v1.219 start
//                			if ((model_type != CLUB1) && (model_type != CLUB2))		// all models except CLUBs
//								shotp = & court_locs[loc][0];						// point to first method for location
//							else
//								shotp = & court_locs_CLUB[loc][0]; 					// CLUB model only
#ifndef PHENOMS
								shotp = & court_locs[loc][0];						// point to first method for location
#else
								shotp = & court_locs_CLUB[loc][0]; 					// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);						// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
					}
				    shotp += method;												// get nearest elevation and right sweep angle
				    putrsDIAG("\n\rRH_RV ");
				}
// v1.190 end
				CurShot.feedrate = base_feeds[pp->feed - 2];
// v1.190 start
//				putrsDIAG("\n\rRH");
// v1.190 end
				CurShot.sweeprate = MAXSWEEPRATE;
				break;
			case PRE_RANDVERT:												// 	Random Vertical Submode
// v1.190 start
				putrsDIAG("\n\r");
				putrsDIAG("\n\rRandom Vertical (Depth) Mode");
// v1.190 end
				keepfeedon = 1;													//	 always continuous shooting
				method = NUMMETH;
				for (loc = pre_rand[premode].start;			//	 start with farthest shot on line
				 (loc <= pre_rand[premode].max) && (method >= NUMMETH);//	until no more or found
				 loc += pre_rand[premode].incr) {				//	 	 moving down one row each time
					 putrsDIAG("\n\rFarL = ");
					 dumphex(loc);
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//						shotp = &court_locs[loc][0];					// point to first method for location
//					else
//						shotp = &court_locs_CLUB[loc][0];  				// CLUB model only
#ifndef PHENOMS
						shotp = &court_locs[loc][0];					// point to first method for location
#else
						shotp = &court_locs_CLUB[loc][0];  				// CLUB model only
#endif
// v1.219 end
					method = serve_method(shotp,pp,0);					// convert speed/spin to method
					 putrsDIAG("\n\rM");
					 dumphex(method);
				}
				shotp += method;												//	 parameters for that method
				CurShot.yLo	= shotp->elev;							//	 get farthest elevation
				method = NUMMETH;
				for (loc = pre_rand[premode].max;				//	 start with nearest shot on line
				 (loc >= pre_rand[premode].start) && (method >= NUMMETH);	// until no more or found
				 loc -= pre_rand[premode].incr) {				//		 moving up one row each time
					 putrsDIAG("\n\rNearL = ");
					 dumphex(loc);
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//						shotp = & court_locs[loc][0];					//	 point to first method for location
//					else
//						shotp = & court_locs_CLUB[loc][0]; 				// CLUB model only
#ifndef PHENOMS
						shotp = & court_locs[loc][0];					//	 point to first method for location
#else
						shotp = & court_locs_CLUB[loc][0]; 				// CLUB model only
#endif
// v1.219 end
					method = serve_method(shotp,pp,0);		//	 convert speed/spin to method
					 putrsDIAG("\n\rM");
					 dumphex(method);
				}
				shotp += method;
				CurShot.xLeft = shotp->sweep;						//	 get nearest elevation
				CurShot.feedrate = base_feeds[pp->feed - 2];
				putrsDIAG("\n\rRV");
				CurShot.sweeprate = MAXSWEEPRATE;
				break;
			case PRE_TWOLINE:														// 	Two Line
				putrsDIAG("\n\r");				// v1.214 commented
				putrsDIAG("\n\r2-Line Mode");	// v1.214 commented
				if (preset[preset_type].width != 2) 
					keepfeedon = (pp->feed == 2U) ? 1 : 0;	//	 only for 2 sec/ball and wide
				line = (line + 1) & 1;										//	 toggles between left and right
// 07/07/09 start
// There are several ways to add random elevation to 2-Line mode.
// 1) Continuous elevation oscillation, like Random Depth mode, where a check to performed for the oscillation range, based on the selected
//    speed and spin values.
// 2) Randomly select the row that the next line shot will use, but it must be a row that is achievable with the current speed/spin selections.
// 3) Randomly select the location on the line, but again, it must be achievable with the current speed/spin selections.
// 4) For each shot, randomly select the elevation value that falls within the range that is determined, based on the speed/spin selections.
// v1.182 start
//              if (preset[PRE_TWOLINE].twolnrw < 3) {
                if (preset[PRE_TWOLINE].twolnrw != 1) {	// Random row selection changed from 3 to 1
// v1.182 end
// 07/07/09 end
				loc +=  line * pre_rand[premode].incr;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &court_locs[loc][0];
//				else
//					shotp = &court_locs_CLUB[loc][0];  			// CLUB model only
#ifndef PHENOMS
					shotp = &court_locs[loc][0];
#else
					shotp = &court_locs_CLUB[loc][0];  			// CLUB model only
#endif
// v1.219 end
				method = serve_method(shotp,pp,PRE_LELEV);			// choose method matching parms
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &court_locs[loc][method];				// point to that exact shot
//				else
//					shotp = &court_locs_CLUB[loc][method];  		// CLUB model only
#ifndef PHENOMS
					shotp = &court_locs[loc][method];				// point to that exact shot
#else
					shotp = &court_locs_CLUB[loc][method];  		// CLUB model only
#endif
// v1.219 end
					PUTRSDIAG("\n\rLOC#");
					DUMPHEX(loc);
// 07/03/09 start
//				putrsDIAG("\n\rline = ");
//				dumphex(line);
//				putrsDIAG("\n\rloc = ");
//				dumphex(loc);
//				putrsDIAG("\n\rmethod = ");
//				dumphex(method);
//				putrsDIAG("\n\rshotp = ");
//				dumphex(shotp);
// 07/03/09 end

// 07/07/09 start
// The following elevation range checking routines below are very similar to that used in Random Vertical (Depth) mode.
// pre_rand[premode].incr is required for 2-Line mode to increment to the next line, but an increment is also needed for the elevation range
// checking that is performed below. There is only one increment field in lobpre.c preset selection tables. Since the increment for the
// elevation range checking routines is always 6, it will be hardcoded into the routines. A separate max field is also required for the 2-Line
// horizontal increment and for the elevation range checking routines. There is another field, range, that is only used in Fully Random mode,
// so it will be used for the max field in the elevation range checking routines below.
                } else {                                        // very similar to Random Depth mode
//				    keepfeedon = 1;								//	 always continuous shooting
                    putrsDIAG("\n\rline = ");
                    dumphex(line);
			    	method = NUMMETH;
                    if (line) {
			        	for (loc = pre_rand[premode].start;			//	 start with farthest shot on line
//				          (loc <= pre_rand[premode].max) && (method >= NUMMETH);// until no more or found
				          (loc <= pre_rand[premode].range) && (method >= NUMMETH);// until no more or found
//				          loc += pre_rand[premode].incr) {			// moving down one row each time
				          loc += 6) {		                       	// moving down one row each time
				    	    putrsDIAG("\n\rFar loc = ");
					        dumphex(loc);
// v1.219 start
//							if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//								shotp = &court_locs[loc][0];					// point to first method for location
//							else
//								shotp = &court_locs_CLUB[loc][0];  			// CLUB model only
#ifndef PHENOMS
								shotp = &court_locs[loc][0];					// point to first method for location
#else
								shotp = &court_locs_CLUB[loc][0];  			// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);					// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
			       	    }
                    } else {    // other line, add the 2-Line increment
			        	for (loc = pre_rand[premode].start + pre_rand[premode].incr;			//	 start with farthest shot on line
//				          (loc <= pre_rand[premode].max) && (method >= NUMMETH);// until no more or found
				          (loc <= pre_rand[premode].range + pre_rand[premode].incr) && (method >= NUMMETH);// until no more or found
//				          loc += pre_rand[premode].incr) {			// moving down one row each time
				          loc += 6) {		                       	// moving down one row each time
				    	    putrsDIAG("\n\rFar loc = ");
					        dumphex(loc);
// v1.219 start
//							if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//								shotp	 = &court_locs[loc][0];					// point to first method for location
//							else
//								shotp	 = &court_locs_CLUB[loc][0];    		// CLUB model only
#ifndef PHENOMS
								shotp	 = &court_locs[loc][0];					// point to first method for location
#else
								shotp	 = &court_locs_CLUB[loc][0];    		// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);					// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
			       	    }
                    }
				    shotp += method;							// parameters for that method
				    CurShot.yLo	= shotp->elev;					// get farthest elevation
				    method = NUMMETH;
                    if (line) {
//  				    for (loc = pre_rand[premode].max;			// start with nearest shot on line
	    			    for (loc = pre_rand[premode].range;			// start with nearest shot on line
				        (loc >= pre_rand[premode].start) && (method >= NUMMETH);// until no more or found
//				          loc -= pre_rand[premode].incr) {			// moving up one row each time
				          loc -= 6) {		                        	// moving up one row each time
					        putrsDIAG("\n\rNear loc = ");
					        dumphex(loc);
// v1.219 start
//							if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//								shotp = & court_locs[loc][0];					// point to first method for location
//							else
//								shotp = & court_locs_CLUB[loc][0]; 			// CLUB model only
#ifndef PHENOMS
								shotp = & court_locs[loc][0];					// point to first method for location
#else
								shotp = & court_locs_CLUB[loc][0]; 			// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);					// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
				        }
                    } else {    // other line
//  				    for (loc = pre_rand[premode].max;			// start with nearest shot on line
	    			    for (loc = pre_rand[premode].range + pre_rand[premode].incr;			// start with nearest shot on line
				        (loc >= (pre_rand[premode].start + pre_rand[premode].incr)) && (method >= NUMMETH);// until no more or found
//				          loc -= pre_rand[premode].incr) {			// moving up one row each time
				          loc -= 6) {		                        	// moving up one row each time
					        putrsDIAG("\n\rNear loc = ");
					        dumphex(loc);
// v1.219 start
//							if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//								shotp = & court_locs[loc][0];					// point to first method for location
//							else
//								shotp = & court_locs_CLUB[loc][0]; 				// CLUB model only
#ifndef PHENOMS
								shotp = & court_locs[loc][0];					// point to first method for location
#else
								shotp = & court_locs_CLUB[loc][0]; 				// CLUB model only
#endif
// v1.219 end
					        method = serve_method(shotp,pp,0);					// convert speed/spin to method
					        putrsDIAG("\n\rmethod = ");
					        dumphex(method);
				        }
                    }
				    shotp += method;				// get nearest elevation
				    CurShot.xLeft = shotp->sweep;
// v1.194 & v1.195 start
// Moved the following line a few lines down. Here the followwing line was only executed for random row.
//				    CurShot.feedrate = base_feeds[pp->feed - 2];
// v1.194 & v1.195 end
				    putrsDIAG("\n\r2L_RV ");
				    CurShot.sweeprate = MAXSWEEPRATE;
                    }
// v1.194 & v1.195 start
					CurShot.feedrate = base_feeds[pp->feed - 2];	// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
// 07/07/09 end
				break;
			case PRE_FULLRAND:													// 	Fully Random
				putrsDIAG("\n\r");
				putrsDIAG("\n\rFull Random Mode");
				keepfeedon = 1;														//	 always continuous shooting
				do {
					rnd = rand() / 7;												//   kluge to prevent odd/even alt
					loc = rnd % pre_rand[premode].range;		//	 randomly chooses acceptable value
// 07/03/09 start
//					putrsDIAG("\n\rpre_rand[premode].range = ");
//					dumphex(pre_rand[premode].range);
//					putrsDIAG("\n\rpremode = ");
//					dumphex(premode);
// 07/03/09 end
#ifndef FR_ABSCHGSPEED
					if (bias[lastrow] < bias[1]) {					//	 if current row happening less often than center
						if ((loc/6) == 1)	{										//		and if selecting center row
							method = NUMMETH;										//		force another selection - could be row 0 or 2
							continue;														//		by skipping to end of while
						}
					}
#endif // FR_ABSCHGSPEED
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					    method = limit_method(&court_locs[loc][0],(rnd / pre_rand[premode].range));
//					else
//						method = limit_method(&court_locs_CLUB[loc][0],(rnd / pre_rand[premode].range));   // CLUB model only
#ifndef PHENOMS
					    method = limit_method(&court_locs[loc][0],(rnd / pre_rand[premode].range));
#else
						method = limit_method(&court_locs_CLUB[loc][0],(rnd / pre_rand[premode].range));   // CLUB model only
#endif
// v1.219 end
				} while (method >= NUMMETH);							// try new shots until a matching method is found
// 07/03/09 start
//				putrsDIAG("LIM:");
//				dumphex(loc);
//				dumphex(method);
				putrsDIAG("\n\rLIM: ");
				putrsDIAG("\n\rloc = ");
				dumphex(loc);
				putrsDIAG("\n\rmethod = ");
				dumphex(method);
// 07/03/09 end
				if (bias[loc/6] < 255)										//	 prevent individual overflow
					bias[loc/6]++;													//		# times row has been selected
				if ((bias[0]>0) && (bias[1]>0) && (bias[2]>0)) {	// normalize bias values
					bias[0]--;															//		by reducing all values
					bias[1]--;															//		whenever possible
					bias[2]--;
				}
				lastrow = loc / 6;												//	 remember last row
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &court_locs[loc][method];
//				else
//					shotp = &court_locs_CLUB[loc][method]; 			// CLUB model only
#ifndef PHENOMS
					shotp = &court_locs[loc][method];
#else
					shotp = &court_locs_CLUB[loc][method]; 			// CLUB model only
#endif
// v1.219 end
					PUTRSDIAG("\n\rLOC#");
					DUMPHEX(loc);
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_GRINDER:										//	Grinder Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rGrinder Drill");
					PUTRSDIAG("\n\rSeq#");
					DUMPHEX(preshot_ix);
					keepfeedon = 1;
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					    shotp = &grinder_locs[preshot_ix++];
//					else
//						shotp = &grinder_locs_CLUB[preshot_ix++];   	// CLUB model only
#ifndef PHENOMS
					    shotp = &grinder_locs[preshot_ix++];
#else
						shotp = &grinder_locs_CLUB[preshot_ix++];   	// CLUB model only
#endif
// v1.219 end
					DUMPHEX(preshot_ix);
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];			// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_POWER:												//	Power Player Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rPower Baseliner Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &power_locs[preshot_ix++];
//				else
//					shotp = &power_locs_CLUB[preshot_ix++];    	// CLUB model only
#ifndef PHENOMS
					shotp = &power_locs[preshot_ix++];
#else
					shotp = &power_locs_CLUB[preshot_ix++];    	// CLUB model only
#endif
// v1.219 end
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_ALLCOURT:										//	All Court Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rAll-Courter Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &allcourt_locs[preshot_ix++];
//				else
//					shotp = &allcourt_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &allcourt_locs[preshot_ix++];
#else
					shotp = &allcourt_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_TRANSITION:									// Transition Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rApproach Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &transition_locs[preshot_ix++];	
//				else
//					shotp = &transition_locs_CLUB[preshot_ix++];   // CLUB model only
#ifndef PHENOMS
					shotp = &transition_locs[preshot_ix++];	
#else
					shotp = &transition_locs_CLUB[preshot_ix++];   // CLUB model only
#endif
// v1.219 end
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_ATTDEF:										// Attack/Defend Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rAttack/Defend Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &attdef_locs[preshot_ix++];
//				else
//					shotp = &attdef_locs_CLUB[preshot_ix++];   	// CLUB model only
#ifndef PHENOMS
					shotp = &attdef_locs[preshot_ix++];
#else
					shotp = &attdef_locs_CLUB[preshot_ix++];   	// CLUB model only
#endif
// v1.219 end
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
			case PRE_FOREONLY:										// Forehand Only Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rForehand Plus Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &foreonly_locs[preshot_ix++];
//				else
//					shotp = &foreonly_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &foreonly_locs[preshot_ix++];
#else
					shotp = &foreonly_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
// v1.194 & v1.195 start
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
// v1.194 & v1.195 end
				break;
// v1.208 start
			case PRE_BEGINNER:										// New Drill #1 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rBeginner Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Beginner_locs[preshot_ix++];
//				else
//					shotp = &Beginner_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &Beginner_locs[preshot_ix++];
#else
					shotp = &Beginner_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
			case PRE_INTERMEDIATE:										// New Drill #2 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rIntermediate Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Intermediate_locs[preshot_ix++];
//				else
//					shotp = &Intermediate_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &Intermediate_locs[preshot_ix++];
#else
					shotp = &Intermediate_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
			case PRE_ADVANCED:										// New Drill #3 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rAdvanced Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Advanced_locs[preshot_ix++];
//				else
//					shotp = &Advanced_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &Advanced_locs[preshot_ix++];
#else
					shotp = &Advanced_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
			case PRE_MOONBALLER:									// New Drill #4 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rMoonballer Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Moonballer_locs[preshot_ix++];
//				else
//					shotp = &Moonballer_locs_CLUB[preshot_ix++]; 	// CLUB model only
#ifndef PHENOMS
					shotp = &Moonballer_locs[preshot_ix++];
#else
					shotp = &Moonballer_locs_CLUB[preshot_ix++]; 	// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
			case PRE_SLICER:										// New Drill #5 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rSlicer Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Slicer_locs[preshot_ix++];
//				else
//					shotp = &Slicer_locs_CLUB[preshot_ix++]; 		// CLUB model only
#ifndef PHENOMS
					shotp = &Slicer_locs[preshot_ix++];
#else
					shotp = &Slicer_locs_CLUB[preshot_ix++]; 		// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
			case PRE_LEFTY:											// New Drill #6 Drill
				putrsDIAG("\n\r");
				putrsDIAG("\n\rLefty Drill");
				keepfeedon = 1;
// v1.219 start
//				if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//					shotp = &Lefty_locs[preshot_ix++];
//				else
//					shotp = &Lefty_locs_CLUB[preshot_ix++]; 		// CLUB model only
#ifndef PHENOMS
					shotp = &Lefty_locs[preshot_ix++];
#else
					shotp = &Lefty_locs_CLUB[preshot_ix++]; 		// CLUB model only
#endif
// v1.219 end
				CurShot.feedrate = base_feeds[pp->feed - 2];		// not necessary, just so value can be sent to diag port
				break;
// v1.208 end
			}
			CurShot.x = shotp->sweep;
			putrsDIAG(" !");
//			dumpshort((unsigned long)CurShot.x);
			CurShot.y = shotp->elev;
//			dumpshort((unsigned long)CurShot.y);
			CurShot.top = shotp->top;
			dumpshort((unsigned long)CurShot.top); 
			CurShot.bot = shotp->bottom;
			dumpshort((unsigned long)CurShot.bot);
			putrsDIAG("!\r\n");

// v1.190 start
			putrsDIAG("\n\rCurShot.xLeft = ");	// v1.214 commented
			dumplong(CurShot.xLeft);			// v1.214 commented
			putrsDIAG("\n\rCurShot.x = ");		// v1.214 commented
			dumplong(CurShot.x);				// v1.214 commented
// v1.190 end
// 07/07/09 start
			putrsDIAG("\n\rCurShot.y = ");		// v1.214 commented
			dumplong(CurShot.y);				// v1.214 commented
			putrsDIAG("\n\rCurShot.yLo = ");	// v1.214 commented
			dumplong(CurShot.yLo);				// v1.214 commented
// v1.182 start
//          if ((preset_type == (signed)PRE_TWOLINE) && (preset[PRE_TWOLINE].twolnrw == 3U)) {
// C18 to XC8 Compiler Changes start
//          if ((preset_type == (signed)PRE_TWOLINE) && (preset[PRE_TWOLINE].twolnrw == 1U)) {	// random row selection changed from 3 to 1
            if ((preset_type == (signed char)PRE_TWOLINE) && (preset[PRE_TWOLINE].twolnrw == 1U)) {	// random row selection changed from 3 to 1
// C18 to XC8 Compiler Changes end
// v1.182 end
                if (CurShot.yLo > CurShot.y)    // prevent divide by 0 in next line
                    CurShot.y = ((rand() % (CurShot.yLo - CurShot.y)) + CurShot.y);                
                putrsDIAG("\n\rRandom Elevation = ");
                dumplong(CurShot.y);
//                if (line)
//                    CurShot.x = 0-CurShot.x;    // sweep to other line
            }
// 07/07/09 end

// v1.190 start
// For Random Horizontal (sweep) mode, random elevation cannot be done as start/stop method, as it is done in 2-Line mode with random
// elevation selected, because in Random Horizontal mode, a ball thrown is not detected (shot done), so the elevation is never moved
// to a new position for the next shot. A continuous elevation oscillation must be used, instead and the following lines are commented.
//			if ((preset_type == (signed)PRE_RANDHORZ) && (preset[PRE_RANDHORZ].col == 1U)) {	// random elevation selected
//				StartElevRandom(CurShot.sweeprate, CurShot.y, CurShot.yLo);			// continuous elevation, rather than start/stop
//				putrsDIAG("\n\rOscillate Elevation");
//			}
//				if (CurShot.yLo > CurShot.y)    												// prevent divide by 0 in next line
//					CurShot.y = ((rand() % (CurShot.yLo - CurShot.y)) + CurShot.y);                
//				putrsDIAG("Random Elevation = ");
//				dumplong(CurShot.y);
//				CurShot.yLo = CurShot.y;
//			}
// v1.190 end

// 07/07/09 start
// C18 to XC8 Compiler Changes start
//			if (preset_type >= (signed)PRE_TWOLINE) {
			if (preset_type >= (signed char)PRE_TWOLINE) {
// C18 to XC8 Compiler Changes end
//			if ((preset_type > (signed)PRE_TWOLINE) || ((preset_type == (signed)PRE_TWOLINE) && (preset[PRE_TWOLINE].twolnrw < 3))) {
// 07/07/09 end
//				if (preset_type == PRE_TWOLINE) {										// if doing the two line drill
//					if (!start_delay) {	
//						if ((preset[preset_type].feed == 2)							//  and 2 sec/ball is specified
//									&& (preset[preset_type].width == 0)) {		//	for the wide drill
//							STARTFEED(base_feeds[1]);											//	 then use 3 sec/ball
//							putrsDIAG("Wide Two Line\n\r");
//						} else {																				//	else (no problem keeping up)
//							STARTFEED(base_feeds[pp->feed - 2]);					//	 start feed motor (continuous)
//						}
//					} else STARTFEED(0);
//				} else


				if ((pp->speed) || (preset_type > PRE_FULLRAND)) {
					if (!preposition){
//						dumphex(preset[preset_type].width);
//						dumphex(preset[preset_type].feed);
						if ((preset_type == PRE_TWOLINE) 					// if doing the two line drill
						&& (preset[preset_type].feed == 2)					// and 2 sec/ball is specified
// v1.201 start
//						&& (preset[preset_type].width == 0)) {				// for the wide drill
						&& ((preset[preset_type].width == 0)				// for wide width 2-Line
						|| (preset[preset_type].width == 1))) {				// or medium width 2-Line
// v1.201 end
							STARTFEED(base_feeds[1]);						// then use 3 sec/ball 
							putrsDIAG("\n\rWide Two Line\n\r");				// v1.214 commented
						} else {											// else (no problem keeping up)						
// v1.194 & v1.195 start
//							STARTFEED(base_feeds[pp->feed - 2]);			//	start feed motor (continuous)			
//							if (preset[preset_type].feed == P_FEEDMIN) {
							if (pp->feed == P_FEEDMIN) {
								// If random feed (P_FEEDMIN is now random feed), randomly select a new rate.
// v1.197 start
//								rndm_base_feeds_index = RANDOM_FEED;		// do it this way, so randomly chosen feed can be sent to diag port
								while (rndm_base_feeds_index == rndm_base_feeds_index_old) {	// do not repeat feed rate
									rndm_base_feeds_index = RANDOM_FEED;	// do it this way, so randomly chosen feed can be sent to diag port
// v1.198 start
// If medium or wide 2-Line mode, then add 1 to feed rate, to alleviate early balls being thrown while still sweeping to new line.
									if ((preset_type == PRE_TWOLINE)
										&& ((preset[preset_type].width == 0) || (preset[preset_type].width == 1)))
									rndm_base_feeds_index = rndm_base_feeds_index + 1;
// v1.198 end
								}
								rndm_base_feeds_index_old = rndm_base_feeds_index;
//  v1.197 end
								putrsDIAG("\n\rrndm_base_feeds_index = ");
								dumphex(rndm_base_feeds_index);
								STARTFEED(base_feeds[rndm_base_feeds_index]);
								// If random feed (P_FEEDMIN is now random feed), CurShot.feedrate will be invalid when set in the
								// switch [preset_type] cases, so correct this now.
								CurShot.feedrate = base_feeds[rndm_base_feeds_index];
							}
							else {
// v1.194 & v1.195 end
								STARTFEED(base_feeds[pp->feed - 2]);		// start feed motor (continuous)
							}			
						}
					} else {
						STARTFEED(0);
					}
				}
				if (preset_type >= PRE_GRINDER) {
					ushort tmpix;
//					preshot_ix++;
//					if (preshot_ix >= flash_header.ndrills[preset_type-PRE_GRINDER]) {
					if (preset_type > PRE_ALLCOURT) {						// if not one of the first 3 drills
						tmpix = flash_header.rsrved[preset_type - PRE_TRANSITION];
					}	else {																		// else - one of the first 3 drills
						tmpix = flash_header.ndrills[preset_type-PRE_GRINDER];
					}
// mjz 11/06/08, start
// Use the following statements to cause a wait until all motors are correct for first shot in each loop
// through the drill shot sequence.
					if (preshot_ix == 1)
						FirstShot = 1;
// mjz 11/06/08, end
					if (preshot_ix >= tmpix) {
						keepfeedon = 0;												// halt feed between sets
						preshot_ix = 0;
						preposition = 1;
//						loop_delay = ticks + ((long)115 * ((long)pp->feed + (long)10));			//			"10 sec" loop delay
//						PUTRSDIAG("\n\rSet delay to ");
//						DUMPLONG(loop_delay);
//						DUMPLONG(ticks);
//						DUMPLONG(pp->feed);
					}
					PUTRSDIAG("\n\rupdt");
					DUMPHEX(preshot_ix);
				}
// 07/07/09 start
//                if ((preset_type == (signed)PRE_TWOLINE) && (preset[PRE_TWOLINE].twolnrw == 3)) {
//                    StartElevRandom(CurShot.sweeprate, CurShot.y, CurShot.yLo);
//                }
// 07/07/09 end
				start_shot();
				if (preposition == 2) {
					preposition = 0;
					preshot_ix = 0;
				} else {
					if (preposition == 1) ++preposition;
//					play_state = PLAY_BEGIN;	// mjz 11/10/08, commented
				}

				play_state = PLAY_BEGIN;		// mjz 11/10/08

			} else {							// else random horizontal or random vertical (depth)				
// v1.194 & v1.195 start
//				CurShotState = SHOT_RAN_INIT;
// Previously random horizontal and vertical only got here when they were first started, but now with random feed rate
// we get here after every ball thrown (PLAY_DONE), so qualify the initial shot state.
				if (CurShotState != SHOT_RAN_LOOP)	// If not running random horizontal or vertical loop,
					CurShotState = SHOT_RAN_INIT;	// then initialize, else stay in the SHOT_RAN_LOOP shot state.
				LargeSpdDrop = 0;
				putrsDIAG("\n\rLargeSpdDrop = 0");
				putrsDIAG("\n\rFirstShot = ");
				dumphex(FirstShot);
//				if (preset[preset_type].feed == P_FEEDMIN) {
				if (pp->feed == P_FEEDMIN) {
					// If random feed (P_FEEDMIN is now random feed), randomly select a new rate.
// v1.197 start
//					rndm_base_feeds_index = RANDOM_FEED;		// do it this way, so randomly chosen feed can be sent to diag port
					while (rndm_base_feeds_index == rndm_base_feeds_index_old) {	// do not repeat feed rate
						rndm_base_feeds_index = RANDOM_FEED;	// do it this way, so randomly chosen feed can be sent to diag port
// v1.198 start
// If medium or wide 2-Line mode, then add 1 to feed rate, to alleviate early balls being thrown while still sweeping to new line.
						if ((preset_type == PRE_TWOLINE)
							&& ((preset[preset_type].width == 0) || (preset[preset_type].width == 1)))
						rndm_base_feeds_index = rndm_base_feeds_index + 1;
// v1.198 end
					}
					rndm_base_feeds_index_old = rndm_base_feeds_index;
//  v1.197 end
					putrsDIAG("\n\rrndm_base_feeds_index = ");
					dumphex(rndm_base_feeds_index);
					STARTFEED(base_feeds[rndm_base_feeds_index]);
					// If random feed (P_FEEDMIN is now random feed), CurShot.feedrate will be invalid when set in the
					// switch [preset_type] for random horizontal and random vertical cases, so correct this now.
					CurShot.feedrate = base_feeds[rndm_base_feeds_index];
				}
// v1.194 & v1.195 end
				play_state = PLAY_BEGIN;
			}
// v1.194 & v1.195 start
			putrsDIAG("\n\rCurShot.feedrate = ");	// v1.214 commented
			dumplong(CurShot.feedrate);				// v1.214 commented
// v1.194 & v1.195 end
		} else if (play_mode & PM_PERSONAL) {						// else if personal program
// v1.200 start
//			keepfeedon = (act_program.feed == 2) ? 1 : 0;
			keepfeedon = (act_program.feed[ppshot_ix] == 2) ? 1 : 0;
// v1.200 end
			if (loop_delay) {										// if timeout is active
				if (((ppshot_ix+1) == 0)							// if first shot in program 
				 || (act_program.shot[ppshot_ix+1] == INV_SHOT)) {
// mjz 11/06/08, start
// Use the following statement to cause a wait until all motors are correct for first shot in each loop
// through the programmed shot sequence.
					FirstShot = 1;
// mjz 11/06/08, end
					if (loop_delay > ticks){						// if timeout not yet complete
						if (!preposition) {
							return (0);			
						}		
					}												// wait until it is done	// mjz 11/10/08, commented
//					} else loop_delay = 0;							// mjz 11/10/08
				}
				loop_delay = 0;										// prevent retrigger	// mjz 11/10/08, commented
//				FirstShot = 1;
			}
			ppshot_ix++;											// go to next shot in program
			if ((act_program.shot[ppshot_ix] == INV_SHOT) ||
			 (ppshot_ix == NUMSHOT)) {
				ppshot_ix = 0;
				keepfeedon = 0;
			}
			loc = act_program.shot[ppshot_ix] - 1;	
// v1.219 start
//			if ((model_type != CLUB1) && (model_type != CLUB2))		// all models except CLUBs
//				shotp = &court_locs[loc][act_program.method[ppshot_ix]];
//			else
//				shotp = &court_locs_CLUB[loc][act_program.method[ppshot_ix]];  // CLUB model only
#ifndef PHENOMS
				shotp = &court_locs[loc][act_program.method[ppshot_ix]];
#else
				shotp = &court_locs_CLUB[loc][act_program.method[ppshot_ix]];  // CLUB model only
#endif
// v1.219 end
			PUTRSDIAG("\n\rSHOT#");
			DUMPHEX(ppshot_ix);
			PUTRSDIAG("\n\rLOC#");
			DUMPHEX(loc);
			PUTRSDIAG("\n\r!");
			CurShot.x = shotp->sweep;
			DUMPSHORT((unsigned long)CurShot.x);
			CurShot.y = shotp->elev;
// v1.192.0U start
#ifdef elev_test
			CurShot.y = CurShot.y + elev_test_adjust;	// ifdef elev_test, this gets added to shots in Program mode
#endif
// v1.192.0U end
			DUMPSHORT((unsigned long)CurShot.y);
			CurShot.top = shotp->top;
			DUMPSHORT((unsigned long)CurShot.top);
			CurShot.bot = shotp->bottom;
			DUMPSHORT((unsigned long)CurShot.bot);
			if (shotp->speed) {
				if (!preposition) {
// 02/08/09 start
// v1.200 start
//					putrsDIAG("\n\ract_program.feed: ");
//					dumphex(act_program.feed);
					putrsDIAG("\n\ract_program.feed[ppshot_ix]: ");
					dumphex(act_program.feed[ppshot_ix]);
// v1.200 end
                    putrsDIAG("\n\rpp_prev_loc: ");
                    dumphex(pp_prev_loc);
                    putrsDIAG("\n\ract_program.shot: ");
                    dumphex(act_program.shot[ppshot_ix]);
// v1.200 start
//					if (act_program.feed == 2)  {							// If feed rate is set for 2,
					if (act_program.feed[ppshot_ix] == 2)  {				// If feed rate is set for 2,
// v1.200 end
                        if (((act_program.shot[ppshot_ix] == 1) && (pp_prev_loc == 6))
                            || ((act_program.shot[ppshot_ix] == 6) && (pp_prev_loc == 1)))
					        STARTFEED(base_feeds[1]);                      // then use 3 sec/ball,
// v1.200 start
//						else STARTFEED(base_feeds[act_program.feed - 2]);				// start feed motor
						else STARTFEED(base_feeds[act_program.feed[ppshot_ix] - 2]);	// start feed motor
// v1.200 end
                    }
					else                                                    // else start feed motor at selected speed.
// 02/08/09 end
// v1.200 start
//					STARTFEED(base_feeds[act_program.feed - 2]);				// start feed motor
					STARTFEED(base_feeds[act_program.feed[ppshot_ix] - 2]);		// start feed motor
// v1.200 end
				} else 
					STARTFEED(0);
			}
            pp_prev_loc = act_program.shot[ppshot_ix];                      // 02/08/09
// v1.200 start
//			DUMPHEX(act_program.feed);
//			DUMPSHORT(base_feeds[act_program.feed - 2]);
			DUMPHEX(act_program.feed[ppshot_ix]);
			DUMPSHORT(base_feeds[act_program.feed[ppshot_ix] - 2]);
// v1.200 end
			PUTRSDIAG("\n\r!\r\n");
			if (!preposition && ((act_program.shot[ppshot_ix+1] == INV_SHOT)	//loop around
				|| ((ppshot_ix + 1) == NUMSHOT))) {
					keepfeedon = 0;
					ppshot_ix = -1;		// preposition 1st shot
					preposition = 1;	// before loop delay
			}
			start_shot();
			if (preposition == 2) { 
				preposition = 0;
				ppshot_ix = -1;
			} else {
				if (preposition == 1) ++preposition;
//				play_state = PLAY_BEGIN;
			}
			play_state = PLAY_BEGIN;
//			if ((act_program.shot[ppshot_ix+1] == INV_SHOT) ||
//			 ((ppshot_ix+1) == NUMSHOT)) {
//				loop_delay = ticks + ((long)115 * ((long)act_program.feed + (long)10));			//			"10 sec" loop delay
//			}
		}
		PUTRSDIAG("\n\rEoPlay");
		DUMPHEX(play_state);
		DUMPHEX(CurShotState);
	}
	return(0);
}

// Comes here from PLAYing modes so we can abort the shot and then transition
//	to idle mode - or now to MANUAL, PRESET or PROGRAM mode
//
char lob_abort(char cmd)
{
	SPUTRSDIAG("\n\rAbort: ");
	if (state_just_changed(cmd)) {
		abort_shot();
		play_state = PLAY_DONE;
		abort_key  = cmd;
		aborted_mode = play_mode;		// extract currently playing mode
// v1.221 start
//		start_delay = 0;				// Stop blinking Play LED if applicable.
// v1.221 end
		SDUMPHEX(abort_key);
		SDUMPHEX(aborted_mode);
// v1.186 start
//      if (model_type != EL04)         // all models except EL04
//    		sm_service(0);
//      else sm_service_EL04(0);        // EL04 model only
// v1.199 start
//		if (model_type == EL04)
		if (model_type == EL04 || model_type == CLUB1)	// models with EL04 control panel
// v1.199 end
			sm_service_EL04(0);
// v1.191.0R start
//		else if (model_type == EL05LE2)
// v1.199 start
//		else if (model_type == EL05LE2 || model_type == CLUB)
		else if (model_type == EL05LE2 || model_type == CLUB2)	// models with new EL05LE2 control panel
// v1.199 end
// v1.191.0R end
			sm_service_EL05LE2(0);
		else
			sm_service(0);			// all other models
// v1.186 end
	}
	return 0;
}


char lob_test(char cmd)
{
//	far	rom struct el04_loc* shotp = &test_loc;

// C18 to XC8 Compiler Changes
//	far	rom struct el04_loc* shotp;
	const struct el04_loc* shotp;
// C18 to XC8 Compiler Changes

	unsigned short ft;
	int changed = 0;


// v1.221 start
		start_delay = 0;				// Stop blinking Play LED if applicable.
// v1.221 end

// v1.219 start
//	if ((model_type != CLUB1) && (model_type != CLUB2)) // all models except CLUB
//		shotp = &test_loc;
//	else
//		shotp = &test_loc_CLUB; // CLUB model only
#ifndef PHENOMS
		shotp = &test_loc;
#else
		shotp = &test_loc_CLUB; // CLUB model only
#endif
// v1.219 end

	if (state_just_changed(cmd) || (extract_key(cmd) == KY_TEST)) {
		save_prog(program_ix,&act_program);									//  save pers. program
		play_mode = play_mode & PM_ELIMMODE;								//	eliminate mode LEDs
		BuildCmd('3',1,&play_mode);													//	show LEDs
//			putrsDIAG("Sv eladj ");
//			putrsDIAG("\n\relev_corr.s = ");	// v1.193 added
//			dumplong(elev_corr.s);				// v1.193 uncommented
//			putrsDIAG("\n\relev_corr_tmp = ");	// v1.193 added
//			dumplong(elev_corr_tmp);			// v1.193 added
//			putrsDIAG("\n\reladj = ");			// v1.193 added
//			dumplong(eladj);					// v1.193 uncommented
		if (eladj >= 0)
			elev_corr.s += (eladj * DIST_6IN); 
		else {
//			dumplong(0-eladj);
//			dumplong(((ushort)(0-eladj))*DIST_6IN);
			elev_corr.s -= ((ushort)(0-eladj))*DIST_6IN;
		}
			elev_corr_tmp = elev_corr.s;		// v1.193 added
			putrsDIAG("\n\reladj = ");			// v1.193 added
			dumplong(eladj);					// v1.193 added
			putrsDIAG("\n\relev_corr.s = ");	// v1.193 added
			dumplong(elev_corr.s);

			DisableSlaveSPI();															// keep slave from interfering
			SEEWrite((long)&flash_header.elev_correction - (long)&flash_header, elev_corr.c[0]&0xff);
			SEEWrite(((long)&flash_header.elev_correction - (long)&flash_header) + 1, elev_corr.c[1]&0xff);		
			EnableSlaveSPI();																// resume normal slave operation
			eladj = 0;												// clear it once elev_correction has been updatd
    strcpy(&lcd_line[0][0], test_main);					//	show three unchanging lines
// v1.193 start
//	strcpypgm2ram(&lcd_line[2][0], test_elev);
	if (elev_corr.s + DIST_6IN > ELEV_CORR_MAX) {
		strcpy(&lcd_line[1][0], "(+) Not Allowed ");
		strcpy(&lcd_line[2][0], "(-) Distance  or");
	} else if (elev_corr.s - DIST_6IN < ELEV_CORR_MIN) {
		strcpy(&lcd_line[1][0], "(-) Not Allowed ");
		strcpy(&lcd_line[2][0], "(+) Distance  or");
	} else {
		strcpy(&lcd_line[2][0], test_elev);
		clear_lcd(0x2);																			//	clear the other line
	}
	strcpy(&lcd_line[3][0], test_shoot);
//	clear_lcd(0x2);																			//	clear the other line
// v1.193 end
    lcd_updated = 1;
		SPUTRSDIAG("\n\rTest Shot ");
		CurShot.x = shotp->sweep;
		DUMPSHORT((unsigned long)CurShot.x);
		CurShot.y = shotp->elev;
		DUMPSHORT((unsigned long)CurShot.y);
		CurShot.top = shotp->top;
		DUMPSHORT((unsigned long)CurShot.top); 
		CurShot.bot = shotp->bottom;
		DUMPSHORT((unsigned long)CurShot.bot);
		PUTRSDIAG("\n\r!\r\n");
		STARTFEED(base_feeds[0]);//	start feed motor (continuous)			
		start_shot();
		play_state = PLAY_BEGIN;
		changed = 1;
	}
	if (CurShotState == SHOT_IDLE) {
// v1.186 start
//      if (model_type != EL04) // all models except EL04
//    		sm_service(0);
//      else sm_service_EL04(0); // EL04 model only
// v1.199 start
//		if (model_type == EL04)
		if (model_type == EL04 || model_type == CLUB1)			// models with EL04 control panel
// v1.199 end
			sm_service_EL04(0);
// v1.191.0R start
//		else if (model_type == EL05LE2)
// v1.199 start
//		else if (model_type == EL05LE2 || model_type == CLUB)
		else if (model_type == EL05LE2 || model_type == CLUB2)	// models with new EL05LE2 control panel
// v1.199 end
// v1.191.0R end
			sm_service_EL05LE2(0);
		else
			sm_service(0);										// all other models

// v1.186 end
	}
// v1.193 start
		if (extract_key(cmd) == KY_PLUS) {
			if (elev_corr_tmp + DIST_6IN < ELEV_CORR_MAX) {
				elev_corr_tmp = elev_corr_tmp + DIST_6IN;
				eladj++;
				changed = 1;
				strcpy(&lcd_line[2][0], test_elev);
			}
			else {
				strcpy(&lcd_line[1][0], "(+) Not Allowed ");
				strcpy(&lcd_line[2][0], "(-) Distance  or");
			}
		} else if (extract_key(cmd) == KY_MINUS) {
			if (elev_corr_tmp - DIST_6IN > ELEV_CORR_MIN) {
				elev_corr_tmp = elev_corr_tmp - DIST_6IN;
				eladj--;
				changed = 1;
				strcpy(&lcd_line[2][0], test_elev);
			}
			else {
				strcpy(&lcd_line[1][0], "(-) Not Allowed ");
				strcpy(&lcd_line[2][0], "(+) Distance  or");
			}
		}
// v1.193 end
		if (!changed) {
		} else 
		 if (eladj < 0) {
  	 	strcpy(&lcd_line[1][0], "  .  FT. NEARER ");					//	show two unchanging lines
    	ft = (0 - eladj);
    	lcd_line[1][3] = (ft & 1) ? '5': '0';
    	ft = ft >> 1;
    	lcd_line[1][1] = '0' + (ft % 10);
    	if ((ft / 10) != 0U) lcd_line[1][0] = '0' + ft/10;
	    lcd_updated = 1;
		} else if (eladj != 0) {
    	strcpy(&lcd_line[1][0], "  .  FT. FARTHER");					//	show two unchanging lines			
    	ft = eladj;
    	lcd_line[1][3] = (eladj & 1) ? '5': '0';
    	ft = ft >> 1;
    	lcd_line[1][1] = '0' + (ft % 10);
    	if ((ft / 10) != 0U) lcd_line[1][0] = '0' + ft/10;
	    lcd_updated = 1;
		} else {
// v1.193 start
//    	strcpypgm2ram(&lcd_line[1][0], "                ");					//	show two unchanging lines			
		if (elev_corr.s + DIST_6IN > ELEV_CORR_MAX) {
			strcpy(&lcd_line[1][0], "(+) Not Allowed ");
			strcpy(&lcd_line[2][0], "(-) Distance  or");
		} else if (elev_corr.s - DIST_6IN < ELEV_CORR_MIN) {
			strcpy(&lcd_line[1][0], "(-) Not Allowed ");
			strcpy(&lcd_line[2][0], "(+) Distance  or");
		} else {
			strcpy(&lcd_line[2][0], test_elev);
    		strcpy(&lcd_line[1][0], "                ");
		}
// v1.193 end
	    lcd_updated = 1;
		}
		// TODO: recalculate test shot
	return 0;
}

// C18 to XC8 Compiler Changes
//rom char (*lobfuncs[ST_MAX])(char cmd) = {
char (*lobfuncs[ST_MAX])(char cmd) = {
// C18 to XC8 Compiler Changes

  lob_sleep, lob_idle, lob_manual, lob_preset, lob_program, lob_play, lob_abort, lob_test
};

// Main Processing for Lobster
//  
// C18 to XC8 Compiler Changes start
//far void lobmain(char key)
void lobmain(char key)
// C18 to XC8 Compiler Changes end
{ 
  int flg;
  int i;

// v1.221 start
	char BatValASCII[4];		// 3 digits for percent plus room for null terminator.
// v1.221 end

	if (STKPTRbits.STKFUL) {
		SetDDRamAddr(line_start[3]);

// C18 to XC8 Compiler Changes
//		putrsLCD((rom char*)"STACK OVERFLOW  ");
		putrsXLCD((const char*)"STACK OVERFLOW  ");
//////////////////

	}
	if (key == KY_REMOTE) {                					// if Remote
    remote_enabled = !remote_enabled;     					// toggle remote enable
    if (remote_enabled) {									// reflect remote in mode
	    play_mode |= PM_REMOTE;						
// v1.221 start
		PlayModeBlink |= PM_REMOTE;							// In case Remote button is pressed while blinking Play LED. Do it here to keep current state of blinking LED.
// v1.221 end
	} else {
		play_mode &= ~PM_REMOTE;
// v1.221 start
		PlayModeBlink &= ~PM_REMOTE;						// In case Remote button is pressed while blinking Play LED. Do it here to keep current state of blinking LED.
// v1.221 end
	}
	BuildCmd('3',1,&play_mode);								// and turn on/off LED	
  }
// v1.219 start
	if (key) {												// if button just pressed
//	if ((key && !RemotePgmMode) || (key == KY_PROG)) {		// If button just pressed and not in remote programming mode (User Drills),
// v1.219 end
		DUMPHEX(key);
		if ((power_state == PS_SYSTEM_ON) &&
				(batt_ticks && (batt_ticks < ticks))) {		// once system is fully powered on
// v1.186 start
//          if (model_type != EL04)
//    		    sm_service((ushort)key);           			// process any state change, all model except EL04
//          else sm_service_EL04((ushort)key);				// process any state change, EL04 model only
// v1.199 start
//			if (model_type == EL04)
			if (model_type == EL04 || model_type == CLUB1)	// models with EL04 control panel
// v1.199 end
				sm_service_EL04((ushort)key);
// v1.191.0R start
//			else if (model_type == EL05LE2)
// v1.199 start
//			else if (model_type == EL05LE2 || model_type == CLUB)
			else if (model_type == EL05LE2 || model_type == CLUB2)	// models with new EL05LE2 control panel
// v1.199 end
// v1.191.0R end
				sm_service_EL05LE2((ushort)key);
			else
				sm_service((ushort)key);
// v1.186 end
		}
	}
	if (prev_st != cur_st) {									// if button caused a change of state
		flg = STATE_CHANGED | key;							//	indicate with flag
		prev_st = cur_st;
	} else {																	// else (did not result in change of state)
		flg = key;															//	indicate by absence of flag
	}
	if (!homing() && (power_state != PS_WAITHOME) && (batt_ticks && (batt_ticks < ticks)))
		lobfuncs[cur_st](flg);										// call current state function
	switch (play_state) {											// process play substate
	case PLAY_BEGIN:													// BEGIN
// mjz 11/11/08 start
// The loop delay setup was moved here from case PLAY_WAIT so that the loop delay is started right away after the
// last shot in the loop. That way, the delay is already active while positioning for the next shot. Previously,
// the delay did not start until all motors were correct for the first shot in the loop, which added significant
// time to the delay.
			if (play_mode & PM_PERSONAL) {
// v1.200 start
//				if (act_program.feed <= 4)
//					loop_delay = ticks + (FIVESEC - (ONESEC * (act_program.feed)));// delay = 5 sec total
				if (act_program.feed[ppshot_ix] <= 4)
					loop_delay = ticks + (FIVESEC - (ONESEC * (act_program.feed[ppshot_ix])));// delay = 5 sec total
// v1.200 end
				else loop_delay = ticks + ONESEC;				// delay = 1 second
			}
			else if (play_mode & PM_PRESET) {
				if (preset[preset_type].feed <= 4) {
					loop_delay = ticks + (FIVESEC - (ONESEC * (preset[preset_type].feed)));	// delay = 5 sec total
				}
				else loop_delay = ticks + ONESEC;				// delay = 1 second
			}
			else loop_delay = 0;
// mjz 11/11/08 end, moved from case PLAY_WAIT
	  	play_state = PLAY_WAIT;								//	transition to wait for shot completion
	  	break;
	case PLAY_WAIT:														// WAIT
	  	if (CurShotState != SHOT_IDLE) break;//	break until shot is complete
#ifdef	notdef	// mjz 11/11/08
//			loop_delay = ticks + TENSEC;	// (long)1150;			//			"10 sec" loop delay
// mjz 11/06/08, start
//			loop_delay = ticks + FIVESEC;	// (long)575, "5 sec" loop delay
// The loop delays above, caused the feed motor to stop for the indicated delay and then
// start back up at the selected feed rate. This resulted in a total loop delay of the initial loop delay plus
// the delay time for the feed rate to drop the next ball.

// The loop delay below is also added on top of feed rate for final ball delay
// between loops, between the last shot of the loop and the first shot of the loop.
//			loop_delay = ticks + (ONESEC * preset[preset_type].feed);
// The loop delay below sets the total delay (feed rate + loop delay) to 5 seconds for feed rates 2 thru 4,
// and sets the total delay to the feed rate only (no added loop delay) for feed rates greater than 4.
			if (play_mode & PM_PERSONAL) {
// v1.200 start
//				if (act_program.feed <= 4)
//					loop_delay = ticks + (FIVESEC - (ONESEC * (act_program.feed)));// delay = 5 sec total
				if (act_program.feed[ppshot_ix] <= 4)
					loop_delay = ticks + (FIVESEC - (ONESEC * (act_program.feed[ppshot_ix])));// delay = 5 sec total
// v1.200 end
//				else loop_delay = 0;							// delay = feed rate time only
				else loop_delay = ticks + ONESEC;				// delay = 1 second
			}
			else if (play_mode & PM_PRESET) {
				if (preset[preset_type].feed <= 4)
					loop_delay = ticks + (FIVESEC - (ONESEC * (preset[preset_type].feed)));	// delay = 5 sec total
//				else loop_delay = 0;							// delay = feed rate time only
				else loop_delay = ticks + ONESEC;				// delay = 1 second
			}
			else loop_delay = 0;
// mjz 11/06/08, end
#endif	// mjz 11/11/08
	  	SPUTRSDIAG("\n\rShotIdle->PLAY_DONE\n\r");
	  	play_state = PLAY_DONE;										//	then transition to done
	  	break;
	case PLAY_DONE:													// DONE
	  	break;
	}
	lob_error();



// v1.221 start
//	if (lcd_updated) {				 								// if lcd has changed
	if (lcd_updated && !BatLevelDisplay) {							// If LCD has changed and not showing battery level.
// v1.221 end
		for (i=0; i<4; i++) {										//	redraw all lines

// C18 to XC8 Compiler Changes
//			while (BusyLCD());										//	 wait until ready
			while (BusyXLCD());										//	 wait until ready
// C18 to XC8 Compiler Changes

			SetDDRamAddr(line_start[i]);							//	 position at start of line

// C18 to XC8 Compiler Changes
//			while (BusyLCD());										//	 wait until ready again
			while (BusyXLCD());										//	 wait until ready again
// C18 to XC8 Compiler Changes

			if ((i!=3) || !STKPTRbits.STKFUL) {						// don't change 4th line if Stack error

				if (cursor_state == 0){ //mjz
					char cursor_chk[17];
					char *cursor1_ptr;
					char *cursor2_ptr;
					int cursor1_pos;
					int cursor2_pos;
					strcpy(cursor_chk,&lcd_line[i][0]);
					cursor1_ptr=strchr(cursor_chk,'>');
					cursor2_ptr=strchr(cursor_chk,'<');
					if((cursor1_ptr != NULL)||(cursor2_ptr != NULL)) {
						if(cursor1_ptr != NULL){
							cursor1_pos=cursor1_ptr-cursor_chk;
							cursor_chk[cursor1_pos]=' ';
						}
						if(cursor2_ptr != NULL){
							cursor2_pos=cursor2_ptr-cursor_chk;
							cursor_chk[cursor2_pos]=' ';
						}

// C18 to XC8 Compiler Changes
//						putsLCD(cursor_chk);
//						}
//						else putsLCD(&lcd_line[i][0]);				//	 and splat out the line's contents
// 
//					}
//						else putsLCD(&lcd_line[i][0]);				//	 and splat out the line's contents

						putsXLCD(cursor_chk);
					}
						else
							putsXLCD(&lcd_line[i][0]);				//	 and splat out the line's contents
 
				}
				else
					putsXLCD(&lcd_line[i][0]);						//	 and splat out the line's contents
// C18 to XC8 Compiler Changes

			}
		}


  		lcd_updated = 0;											//	indicate display is current (for now)
	}
// v1.221 start
	else if (lcd_updated && BatLevelDisplay) {						// If LCD has changed and not showing battery level.
//	else if (lcd_updated && BatLevelDisplay == 2) {						// If LCD has changed and not showing battery level.
//		itoa(BatValASCII,batval,10);								// Convert battery value.
		itoa(BatValASCII,BatPercent,10);							// Convert battery value.
		while (BusyXLCD());
		SetDDRamAddr(line_start[0]);								// position at start of line
		while (BusyXLCD());
//		putrsXLCD("                ");
		putrsXLCD("----------------");
		while (BusyXLCD());
		SetDDRamAddr(line_start[1]);								// position at start of line
		while (BusyXLCD());
		putrsXLCD(" Battery Level  ");
		while (BusyXLCD());
		SetDDRamAddr(line_start[2]);								// position at start of line
		while (BusyXLCD());
//		if (BatPercent >= 100)										// 3 digits
		if (strlen(BatValASCII) == 3)								// 3 digits
			putrsXLCD("     ");										// + 5 spaces
//		else if (BatPercent > 9 && BatPercent < 100)				// 2 digits
		else if (strlen(BatValASCII) == 2)							// 2 digits
			putrsXLCD("      ");									// + 6 spaces
		else														// 1 digit
			putrsXLCD("       ");									// + 7 spaces
		while (BusyXLCD());
		putrsXLCD(BatValASCII);
		while (BusyXLCD());
		putrsXLCD(" %      ");										// + 8 more = 16
		while (BusyXLCD());
		SetDDRamAddr(line_start[3]);								// position at start of line
		while (BusyXLCD());
//		putrsXLCD("                ");
		putrsXLCD("----------------");
	}
// v1.221 end
}
