// Lobster Definitions
//
//	(C)2007 Censerve(R) Consulting, Inc.
//			All rights reserved.
//

far void show_lcd(void);
void clear_lcd(char mask);
void DisableSlaveSPI(void);
void EnableSlaveSPI(void);
char SEERead(unsigned int addr);
void SEEWrite(unsigned int addr, char data);
// C18 to XC8 Compiler Changes Start
//char save_prog(char progno, struct persprog* ppers);
char save_prog(signed char progno, struct persprog* ppers);
// C18 to XC8 Compiler Changes End

///////////////////////////////////////////////////////////////////
//	CAUTION!!
//		Dynamic selection mode ensures that the method chosen is
//		within limits (specified below) of the prior shot. However,
//		the first shot must assume some 'prior' shot. We use locn 0
//		and method 14 (50 mph in the current tables). If this is 
//		not acceptable, be sure to adjust DEFMETH and DEFLOCN to be
//		what you consider an acceptable starting value.
///////////////////////////////////////////////////////////////////
#define DEFLOCN  0							// default 'prior' location for first fully random selection
#define DEFMETH	14							// default 'prior' method for first fully random selection
// Support dynamic or static limits for FULLY RANDOM shots
#define FR_ABSCHGSPEED		1			// if defined, uses max and min speed; if undefined, uses INC and DEC
#define FR_MAXSPDABS			65U		//65U		// largest allowed speed - for absolute limits
#define FR_MINSPDABS			50U		// smallest allowed speed - for absolute limits
#define FR_MAXSPDDEC			10		// largest reduction in speed (from last shot) allowed
#define FR_MAXSPDINC			10		// largest increase in speed (from last shot) allowed
#define FR_ABSCHGSPIN			1			// if defined, uses max and min spin; if undefined, uses INC and DEC
#define FR_MAXSPNABS			2			// largest allowed spin - for absolute limits
#define FR_MINSPNABS		 -2			// smallest allowed spin - for absolute limits
#define FR_MAXSPNDEC			2			// largest reduction in spin (from last shot) allowed
#define FR_MAXSPNINC			2			// largest increase in spin (from last shot) allowed


// ELEVATION_ADJUST is the pulse count from when the home sensor is detected to where an elevation of zero is defined.
// ELEVATION_ADJUST is the value written to home_ofs[ELEV] in the slave code.
// In the slave code, when the elevation limit sensor is detected, position[ELEV] = 0 - home_ofs[ELEV].
//#define ELEVATION_ADJUST	0xbb	// mjz 10/24/08, commented
// v1p184 start
//#define	ELEVATION_ADJUST	0x50	// mjz 10/24/08, lowered offset from the home sensor to alleviate some machines from reaching this limit prematurely.
#define	ELEVATION_ADJUST	0x30	// decreased by 0x20
//#define	ELEVATION_ADJUST	0x00	// for debugging only
//#define ELEVATION_LIMIT		0x630	// mjz 10/27/08, commented
//#define ELEVATION_LIMIT		0x690	// mjz 10/27/08, increased to make up for the effect from decrease in the offset from the home sensor, ELEVATION_ADJUST.
#define ELEVATION_LIMIT		0x6b0	// increased by 0x20
// v1p184 end
// v1p185 start
// SWEEP_ADJUST is the pulse count from when the home sensor is detected to where the sweep limit is defined.
// SWEEP_ADJUST is the value written to home_ofs[SWEEP] in the slave code.
// In the slave code, when the sweep limit sensor is detected, position[SWEEP] = limit[SWEEP] + home_ofs[SWEEP].
#define SWEEP_ADJUST		0xf0	// Currently the same value as in the slave code.
//#define SWEEP_ADJUST_CLUB	0x438	// Exaggerated count, New value for horizontally centering the Club model.
// v1p191p0R start
//#define SWEEP_ADJUST_CLUB	0x120	// New value for horizontally centering the Club model.
#define SWEEP_ADJUST_CLUB	0x12a	// New value for horizontally centering the Club model.
// v1p185 end
// v1p191p0R end

// v1.196 start
//#define REMDELAY				30U		
#define REMDELAY				15U		
// v1.196 end
#define BATTSHOWDELAY			230U
#define TWOLINEDELAY			345U
#define ONESEC					115L
#define TWOSEC					230L		// mjz 11/06/08
#define THREESEC				345L		// mjz 11/06/08
#define FOURSEC					460L		// mjz 11/06/08
#define FIVESEC					575L
#define SIXSEC					690L		// mjz 11/06/08
#define SEVENSEC				805L		// mjz 11/06/08
#define EIGHTSEC				920L		// mjz 11/06/08
#define NINESEC					1035L		// mjz 11/06/08
#define TENSEC					1150L
// v1.221 start
#define TWENTYSEC				2300L
// v1.221 end

#define STATE_CHANGED 0x80 
#define extract_key(cmd)				(char)((cmd) & (uchar)(~(STATE_CHANGED)))
#define state_just_changed(cmd) ((cmd) & (STATE_CHANGED))

// Parameter Defaults
//
#define DEF_SPEED	50	// 50 displays as 60 right now, could use 40 which display as 50
#define DEF_SPIN	0
#define DEF_FEED	4
#define DEF_ELEV	10;
#define DEF_SWEEP	0;

// DEBUG - define allows operation without motors
//#define DBGSTATES 1	// enables diagnostic output for states
//#define DBGSTR	
//#define NOHOME		1
//#define NOSHOT		1
//#define NOMOTORS	1
//#define SHOW_BUILDCMD	1

// v1.186 start
#ifdef NOMACHINE
#define NOHOME
#endif
// v1.186 end

// Parameter Definitions
//#define P_SPEEDMIN	30			// mjz 11/04/08, commented
#define P_SPEEDMIN	25				// mjz 11/04/08
#define P_SPEEDMAX	70
#define P_SPEEDINC	 5
#define P_SPINMIN		-3
#define P_SPINMAX		 3
#define P_SPININC		 1
// v1p184 start
//#define P_ELEVMIN		 5
#define P_ELEVMIN		2
// v1p184 end
#define P_ELEVMAX		80
#define P_ELEVINC		 1
#define P_SWEEPMIN  -3
#define P_SWEEPMAX	 3
#define P_SWEEPINC	 1
// v1.194 & v1.195 start
// The current selectable feed rates are 2 (P_FEEDMIN) thru 9 (P_FEEDMAX), but they are translated into a feed rate table index range
// of 0 (P_FEEDMIN - 2) thru 7 (P_FEEDMAX - 2).
// v1.197 start
//#define P_FEED_RANDOM_MAX	4	// maximum feed rate selection when in random feed mode
#define P_FEED_RANDOM_MAX	5	// maximum feed rate selection when in random feed mode
// The index range into feed rate table is 0 thru P_FEED_RANDOM_MAX-2, so the following results in a randomly selected feed rate table
// index within that range.
//#define RANDOM_FEED (rand() % (P_FEED_RANDOM_MAX-1))
#define RANDOM_FEED ((rand() % (P_FEED_RANDOM_MAX-2)) + 1)	// random feed rate range changed from 2-4 to 3-5.
// v1.197 end
//#define P_FEEDMIN			2
#define P_FEEDMIN			1	// If P_FEEDMIN == 1, then random feed is selected
// v1.194 & v1.195 end
//#define P_FEEDMAX			12
#define P_FEEDMAX			9
#define P_FEEDINC			1
#define P_COLMIN		 0
#define P_COLMAX		 5
#define P_COLINC		 1
#define P_ROWMIN		 0
// v1.190 start
//#define P_ROWMAX		 2
#define P_ROWMAX		 3	// Row A, B, C, RANDOM, not necessarily in this order
// v1.190 end
#define P_ROWINC		 1
#define P_2LNMIN		 0
#define P_2LNMAX		 2
#define P_2LNINC		 1
// 07/07/09 start
#define P_TWOLNRWMIN    0
#define P_TWOLNRWMAX    3
#define P_TWOLNRWINC    1
// 07/07/09 end

// Parameter indicies
#define SPEED_IX		0U
#define ELEV_IX			1U
#define SPIN_IX			2U		// was 4
#define HORIZ_IX		3U		// was 2

// 07/03/09 start
//#define FEED_IX			4U		// was 3
////#define SPIN_IX			4
//#define COL_IX			5U
//#define ROW_IX			6U
//#define WID_IX			7U
#define TWOLNRW_IX		4U		// 2-Line mode row index
#define FEED_IX			5U		// was 3
#define COL_IX			6U
#define ROW_IX			7U
#define WID_IX			8U
// 07/03/09 end

// Personal Program States
#define PP_POWERON	0			// power on condition
#define PP_NONPP		1			// not a personal program state
#define PP_SCRN1		2			// screen #1
#define PP_SCRN2		3			// screen #2
#define PP_SCRN3		4			// screen #3
#define PP_CHANGE		5			// only priorState is set to this, to cause redraw

// Personal Program Edit Fields
#define PF_PROGNO		5			// Program #
#define PF_SHOTNO		6			// Shot #
#define PF_LOCNO		7			// Location #
#define PF_EDIT			8			// Edit Y/N
#define PF_TRANS		9			// Transition (not really a field)

// Elevation adjustment (for the test shot) equivalent to 6 inches
#define DIST_6IN		12
// v1.193 start
// v1.205 start
//#define ELEV_CORR_MIN	-40			// minimum allowable adjustment during test shot calibration
// v1.205 end
// v1.202 start
//#define ELEV_CORR_MAX	110			// maximum allowable adjustment during test shot calibration
#define ELEV_CORR_MAX	205			// maximum allowable adjustment during test shot calibration
// v1.202 end
// v1.193 end

#define SPEED_LIMITS	{ P_SPEEDMIN, P_SPEEDMAX, P_SPEEDINC }
#define SPIN_LIMITS		{ P_SPINMIN,	P_SPINMAX,	P_SPININC	 }
#define FEED_LIMITS		{ P_FEEDMIN,	P_FEEDMAX,	P_FEEDINC	 }
#define ELEV_LIMITS		{ P_ELEVMIN,	P_ELEVMAX,	P_ELEVINC	 }
#define SWEEP_LIMITS	{ P_SWEEPMIN,	P_SWEEPMAX,	P_SWEEPINC }
// 07/07/09 start
// 07/03/09 start
//#define TWOLNRW_LIMITS	{ P_ROWMIN,		P_ROWMAX,	P_ROWINC }		// 2-Line mode row limits
// 07/03/09 end
#define TWOLNRW_LIMITS	{ P_TWOLNRWMIN,		P_TWOLNRWMAX,	P_TWOLNRWINC }		// 2-Line mode row limits
// 07/07/09 end
#define COL_LIMITS		{ P_COLMIN,		P_COLMAX,		P_COLINC	 }
#define ROW_LIMITS		{ P_ROWMIN,		P_ROWMAX,		P_ROWINC	 }
#define TWOLN_LIMITS	{ P_2LNMIN,		P_2LNMAX,		P_2LNINC	 }
#define FULL_LIMITS		{ 0,	0, 	0 }
	
// Chartoa formatting
#define PRESET_FMT 0x10	// indicates value to print is not a # but an index into preset_sub[]
#define SPIN_FMT	 0x90 // indicates TOP#, 0 or BOT#
#define STR_FMT		 0x10 // indicates value is index into descriptive text array
#define string_format(mask) (((mask) & 0xf0) == ((uchar)STR_FMT))
#define SPN_FMT		 0x20 // indicates valud is index into spin descriptive array
#define spin_format(mask)		(((mask) & 0xf0) == ((uchar)SPN_FMT))
#define NEG_NUM		 0xc0 // indicates value is a negative # and not an index into a text array

#define PRE_LOWEST 0
#define PRE_HIGHEST 1
#define PRE_ELEVATN	0
#define PRE_SWEEP		2
#define PRE_LELEV		((PRE_LOWEST)|(PRE_ELEVATN))
#define PRE_LSWEEP	((PRE_LOWEST)|(PRE_SWEEP))
#define PRE_HELEV		((PRE_HIGHEST)|(PRE_ELEVATN))
#define PRE_HSWEEP	((PRE_HIGHEST)|(PRE_HIGHEST))

#define MAXSWEEPRATE	0x1ff

// Preset Types
// v1.213 start
//#define PRE_RANDHORZ	0U
//#define PRE_RANDVERT	1U
//#define PRE_TWOLINE	2U
#define PRE_RANDHORZ	0
#define PRE_RANDVERT	1U	// If made signed, then serve motor invalid data error, for some reason.
#define PRE_TWOLINE		2
// v1.213 end
#define PRE_FULLRAND	3
#define PRE_GRINDER		4
#define PRE_POWER		5
#define PRE_ALLCOURT	6
#define PRE_TRANSITION	7
#define PRE_ATTDEF		8
#define PRE_FOREONLY	9
// v1.208 start
//#define NUMPRESETS		10
#define PRE_BEGINNER		10
#define PRE_INTERMEDIATE	11
#define PRE_ADVANCED		12
#define PRE_MOONBALLER		13
#define PRE_SLICER			14
#define PRE_LEFTY			15
#define NUMPRESETS			16
// v1.208 end

// Preset Masks
#define PRE_SPEED		1U //(ushort)(1 << SPEED_IX)	//0x01
#define PRE_ELEV		(1U << ELEV_IX)		//0x02
#define PRE_HORIZ		(1U << HORIZ_IX)	//0x08 was 0x04
#define PRE_FEED		(1U << FEED_IX)		//0x10 was 0x08
#define PRE_SPIN		(1U << SPIN_IX)		//0x04 was 0x10

// 07/03/09 start
//#define PRE_RSWP		(1U << COL_IX)		//0x20
//#define PRE_RELEV		(1U << ROW_IX)		//0x40
//#define PRE_WIDTH		(1U << WID_IX)		//0x80
//#define PRE_FULL	 0x100
//#define PRE_GRND	 0x200
//#define PRE_PWR		 0x400
//#define PRE_ALLCRT 0x800
//#define PRE_TRNS	 0x1000
//#define PRE_ADEF	 0x2000
//#define PRE_FONLY  0x4000
#define PRE_TWOLNRW		(1U << TWOLNRW_IX)	//0x20, 2-Line mode row index
#define PRE_RSWP		(1U << COL_IX)		//0x40
#define PRE_RELEV		(1U << ROW_IX)		//0x80
//#define PRE_WIDTH		(1 << WID_IX)		//0x100
#define PRE_WIDTH	0x100					//0x100, Had to hard code, because the shift crossed over 8 bit boundary.
#define PRE_FULL	0x200
#define PRE_GRND	0x400
#define PRE_PWR		0x800
#define PRE_ALLCRT	0x1000
#define PRE_TRNS	0x2000
#define PRE_ADEF	0x4000
#define PRE_FONLY	0x8000
// 07/03/09 end
// v1.208 start
#define PRE_BGNNR	0x10000		// Beginner Drill
#define PRE_INTRMDT	0x20000		// Intermediate Drill
#define PRE_ADVNCD	0x40000		// Advanced Drill
#define PRE_MNBLLR	0x80000		// Moonballer Drill
#define PRE_SLCR	0x100000	// Slicer Drill
#define PRE_LFTY	0x200000	// Lefty Drill
// v1.208 end
#define PRE_RAND	 (PRE_RSWP|PRE_RELEV|PRE_WIDTH|PRE_FULL)
// v1.208 start
//#define PRE_DRILL	 (PRE_GRND|PRE_PWR|PRE_ALLCRT|PRE_TRNS|PRE_ADEF|PRE_FONLY)
#define PRE_DRILL	 (PRE_GRND|PRE_PWR|PRE_ALLCRT|PRE_TRNS|PRE_ADEF|PRE_FONLY|PRE_BGNNR|PRE_INTRMDT|PRE_ADVNCD|PRE_MNBLLR|PRE_SLCR|PRE_LFTY)
// v1.208 end
#define PRE_ANY		 (PRE_RAND|PRE_DRILL)

// Remote Values
#define RKY_PAUSE	0x31
#define	RKY_DRILLS	0x32
#define RKY_SWEEP	0x33
#define RKY_2LINE	0x34
#define RKY_SPDUP	0x35
#define RKY_FDUP	0x36
#define RKY_SPDDN	0x37
#define RKY_FDDN 	0x38
#define RKY_RANDOM	0x39

// v1.213 start
#define RKY_VERSION	0x3F	// "?", Lobster machine software version request.
// v1.213 end

#define RKY_SPNUP	0x41
#define RKY_SPNDN	0x42
#define RKY_ELEVUP	0x43
#define RKY_ELEVDN	0x44
#define RKY_DEPTH	0x45
#define RKY_TEST	0x46
#define RKY_MANUAL	0x47	// v1.203 added
#define RKY_DISPLAY	0x48	// Show display on remote control v1.209 added

// v1.219 start
#define RKY_UDRILLS		0x49	// Get programmable User Drills
#define RKY_SAVEDRL		0x4a	// Save User Drill
#define RKY_EDITDRL		0x4b	// Edit User Drill
#define RKY_PLAYDRL		0x4c	// User Drill Play/Pause
#define RKY_XUDRILLS	0x4d	// Exit User Drills
#define RKY_EDITSPEED	0x4e	// Edit speed value in User Drill.
#define RKY_EDITSPIN	0x4f	// Edit spin value in User Drill.
#define RKY_EDITFEED	0x50	// Edit feed value in User Drill.
#define RKY_EDITLOCN	0x51	// Edit location value in User Drill.
#define RKY_DELSHOT		0x52	// Delete shot in User Drill.
#define RKY_COMMERR		0x53	// Communication error.
#define RKY_GETCMD		0x54	// Get command from machine.
#define RKY_EDITSHOT	0x55	// Edit complete shot.
#define RKY_SHOTTABLES	0x56	// Upload shot table data.
// v1.220 start
#define RKY_DWNLDDRL	0x57	// Download drill from mobile device to machine.
// v1.220 end
#define RKY_RCQUAL		0x5a	// Qualifying Z byte, next byte is valid.

#define RKY_LOCN1	0x01	// Court location 1
#define RKY_LOCN2	0x02	// Court location 2
#define RKY_LOCN3	0x03	// Court location 3
#define RKY_LOCN4	0x04	// Court location 4
#define RKY_LOCN5	0x05	// Court location 5
#define RKY_LOCN6	0x06	// Court location 6
#define RKY_LOCN7	0x07	// Court location 7
#define RKY_LOCN8	0x08	// Court location 8
#define RKY_LOCN9	0x09	// Court location 9
#define RKY_LOCN10	0x0a	// Court location 10
#define RKY_LOCN11	0x0b	// Court location 11
#define RKY_LOCN12	0x0c	// Court location 12
#define RKY_LOCN13	0x0d	// Court location 13
#define RKY_LOCN14	0x0e	// Court location 14
#define RKY_LOCN15	0x0f	// Court location 15
#define RKY_LOCN16	0x10	// Court location 16
#define RKY_LOCN17	0x11	// Court location 17
#define RKY_LOCN18	0x12	// Court location 18

#define RKY_USRDRL1	0x61	// User drill 1
#define RKY_USRDRL2	0x62	// User drill 2
#define RKY_USRDRL3	0x63	// User drill 3
#define RKY_USRDRL4	0x64	// User drill 4
#define RKY_USRDRL5	0x65	// User drill 5
#define RKY_USRDRL6	0x66	// User drill 6
// v1.223 start
#define RKY_USRDRL7		0x67	// User drill 7
#define RKY_USRDRL8		0x68	// User drill 8
#define RKY_USRDRL9		0x69	// User drill 9
#define RKY_USRDRL10	0x6a	// User drill 10
#define RKY_USRDRL11	0x6b	// User drill 11
#define RKY_USRDRL12	0x6c	// User drill 12
// v1.223 end
#define RKY_SHOT1	0x71	// Drill shot# 1
#define RKY_SHOT2	0x72	// Drill shot# 2
#define RKY_SHOT3	0x73	// Drill shot# 3
#define RKY_SHOT4	0x74	// Drill shot# 4
#define RKY_SHOT5	0x75	// Drill shot# 5
#define RKY_SHOT6	0x76	// Drill shot# 6

// RemotePgmMode bits
//#define RPM_NONE		0x0000
//#define RPM_START		0x0001	// Entered "User Drills" view.
//#define RPM_SHOTNUM	0x0002	// Awaiting selected shot number.
//#define RPM_SHOTLOCN	0x0004	// Awaiting selected location number in an entire shot edit.
//#define RPM_SHOTSPEED	0x0008	// Awaiting selected shot speed in an entire shot edit.
//#define RPM_SHOTSPIN	0x0010	// Awaiting selected shot spin in an entire shot edit.
//#define RPM_SHOTFEED	0x0020	// Awaiting selected shot feed in an entire shot edit.
//#define RPM_EDITLOCN	0x0040	// Edit shot location only.
//#define RPM_EDITSPEED	0x0080	// Edit shot speed only.
//#define RPM_EDITSPIN	0x0100	// Edit shot spin only.
//#define RPM_EDITFEED	0x0200	// Edit shot feed only.

// RemotePgmMode (RPM) bits
static const unsigned int RPM_NONE = 0x0000;
static const unsigned int RPM_START = 0x0001;		// Entered "User Drills" view.
static const unsigned int RPM_SHOTNUM = 0x0002;		// Awaiting selected shot number.
static const unsigned int RPM_SHOTLOCN = 0x0004;	// Awaiting selected location number in an entire shot edit.
static const unsigned int RPM_SHOTSPEED = 0x0008;	// Awaiting selected shot speed in an entire shot edit.
static const unsigned int RPM_SHOTSPIN = 0x0010;	// Awaiting selected shot spin in an entire shot edit.
static const unsigned int RPM_SHOTFEED = 0x0020;	// Awaiting selected shot feed in an entire shot edit.
static const unsigned int RPM_EDITLOCN = 0x0040;	// Edit shot location only.
static const unsigned int RPM_EDITSPEED = 0x0080;	// Edit shot speed only.
static const unsigned int RPM_EDITSPIN = 0x0100;	// Edit shot spin only.
static const unsigned int RPM_EDITFEED = 0x0200;	// Edit shot feed only.
static const unsigned int RPM_SHOTTABLES = 0x0400;	// Upload shot tables.
// v1.219 end
// v1.220 start
static const unsigned int RPM_DWNLDDRL = 0x0800;	// Received command to download a drill from mobile device.
// v1.220 end
// Keypad Values
#define KY_MENUDN	0x01
#define KY_PRESET	0x02
#define KY_PLAY		0x03
#define KY_PLUS		0x04
#define KY_PROG		0x05
#define KY_MANUAL	0x06
#define KY_LOCN06	0x07
#define KY_LOCN18	0x08
#define KY_LOCN12	0x09
#define KY_LOCN24	0x0a
#define KY_LOCN30	0x0b
#define KY_LOCN36	0x0c
#define KY_LOCN05	0x0d
#define KY_LOCN17	0x0e
#define KY_LOCN11	0x0f
#define KY_LOCN23	0x10
#define KY_LOCN29	0x11
#define KY_LOCN35	0x12
#define KY_LOCN04	0x13
#define KY_LOCN16	0x14
#define KY_LOCN10	0x15
#define KY_LOCN22	0x16
#define KY_LOCN28	0x17
#define KY_LOCN34	0x18
#define KY_LOCN03	0x19
#define KY_LOCN15	0x1a
#define KY_LOCN09	0x1b
#define KY_LOCN21	0x1c
#define KY_LOCN27	0x1d
#define KY_LOCN33	0x1e
#define KY_LOCN02	0x1f
#define KY_LOCN14	0x20
#define KY_LOCN08	0x21
#define KY_LOCN20	0x22
#define KY_LOCN26	0x23
#define KY_LOCN32	0x24
#define KY_LOCN01	0x25
#define KY_LOCN13	0x26
#define KY_LOCN07	0x27
#define KY_LOCN19	0x28
#define KY_LOCN25	0x29
#define KY_LOCN31	0x2a
#define KY_MINUS	0x2b
#define KY_WRAP		0x2c	// fake value - same as KY_PLUS only with wrapping
#define KY_MENUUP	0x2d
#define KY_TEST		0x2e
#define KY_REMOTE	0x2f
#define LOBNUMBUT	0x30
#define KY_INVLOC	0x00
#define INV_SHOT	0	//-1

// ELO4CHG #6 (start) 
#define KY_TWOLINENARROW    31  //Loc 2
#define KY_TWOLINEMEDIUM    33  //Loc 8
#define KY_TWOLINEWIDE      32  //Loc 14
#define KY_GRINDER          7   //Loc 6
#define KY_POWER            9   //Loc 12
#define KY_ALLCOURTER       8   //Loc 18
#define KY_RNDSWEEP         19   //Loc 4
#define KY_RNDDEPTH         21   //Loc 10
#define KY_FULLRAND         20   //Loc 16
// ELO4CHG #6 (end)

// v1.186 start
#define KY_RANDOM	0x0c	// Loc 36
#define KY_2LINE	0x12	// Loc 35
#define KY_SWEEP	0x18	// Loc 34
#define KY_DEPTH	0x1e	// Loc 33
#define KY_DRILLS	0x24	// Loc 32
// v1.186 end

// 07/29/09 start
// 06/16/09 start
//#define KB_MAXLOC	6				// maximum location available on the keypad, currently not used
//#define KB_MAXLOC	18				// maximum location available on the keypad, currently not used
//#define PP_MAXLOC	6				// maximum location available for personal programs
//#define PP_MAXLOC	18				// maximum location available for personal programs
// 06/16/09 end
// 07/29/09 end
//#define PP_MAXSPD 70
//#define PP_MINSPD 30

#define AR_1STREPEAT	 50U	// # ticks for ? second initial delay before repeat
#define AR_AUTOREPEAT  25U	// # ticks for .? second repeat of key presses
#define AR_OFF					0U	// this value disables the auto repeat functionality
#define AR_REPEAT				1U	// this value is the single value at which we want to repeat
#define DEBOUNCE			 10U	// this is the number of ticks to wait for debounce

#define FMT_STD	0
#define FMT_HILITE 1

//struct parm_list {
//	const rom struct limit parm_limits[8];	// list of parameter limits and increments
//	const rom char* parm;										// pointer to parameter value list
//	const rom char* prompt[8][2];						// pointers to paramter prompt strings
//	const rom char field_size[8];						// parameter display size (- indicates +/-)
//	const rom char field_locn[8];						// parameter display location
//};

// C18 to XC8 Compiler Changes
//struct t_parm {
//	const rom struct limit limits;	// parameter limits and increments
//	const rom char** prompt;		// pointers to parameter prompt strings
//	const rom char field_size;		// parameter display size (- indicates +/-)
//	const rom char field_locn;		// parameter display location
//	const rom char text_size;		// parameter text size
//	const rom char text_start;		// parameter text start position (allows mult/line)
//	const rom char line;			// parameter line #
//};
struct t_parm {
	const struct limit limits;	// parameter limits and increments
	const char** prompt;		// pointers to parameter prompt strings
	const char field_size;		// parameter display size (- indicates +/-)
	const char field_locn;		// parameter display location
	const char text_size;		// parameter text size
	const char text_start;		// parameter text start position (allows mult/line)
	const char line;			// parameter line #
};
// C18 to XC8 Compiler Changes
struct el04_loc {
	uchar speed;
	ushort top;
	ushort bottom;
	ushort elev;
	signed short sweep;
	signed char spin;
};

#define PS_SYSTEM_ON		0		// normal operating state
#define PS_WAIT_ON			1		// waiting for home completion while powering on
#define PS_WAITHOME		 	2		// waiting for home completion while powering off
#define PS_DONE 				3		// initial battery connection state (after Setup())
#define PS_WAKEUP				4		// initial powering on state which begins homing
#define PS_OFFBUT 			5		// initial powering off state which begins homing

// Personal Program Selection Level Definitions
#define PP_RESET	0
#define PP_PROG		1
#define PP_SHOT		2
#define PP_PARMS	3

// Play Modes (bit mapped)
//	Mapped this way so we can use for the LED command
#define PM_NONE		0			// Not sure if this matters
#define PM_PLAYING	1			// Playing one of the primary modes
#define PM_PRESET	2			// Preset Program (primary mode)
#define PM_PERSONAL	4			// Personal Programs (primary mode)
#define PM_MANUAL	8			// Manual Program	(primary mode)
#define PM_REMOTE	0x10		// Remote Control is active 
#define PM_ELIMMODE	0x11		// eliminates mode while keeping playing and remote
#define PM_MODE	((PM_MANUAL)|(PM_PRESET)|(PM_PERSONAL))

#define EE_PROGSTART	0xb0

struct ee_hdr	{
	char	 vers[8];			// [00] version(s)
	ushort ops_size;			// [08] operational parameter size
	char	 npresets;			// [0A] number of preset types
	char	 parm_size;			// [0B] size of parameter structure
	char	 npersprog;			// [0C] number of personal programs
	char	 pers_size;			// [0D] size of personal program structure
	char	 num_methods;		// [0E] number of methods stored for each location
	char	 num_locs;			// [0F] number of locations stored
	char	 additional_locs;	// [10] number of additional locations
	char	 ndrills[3];		// grinder;	// [11] number of locations for grinder drill
//	char	 npower;			// [12] number of locations for power play drill
//	char	 nallcourt;			// [13] number of locations for all courter drill
	ushort elev_correction;		// [14] pwm correction value for elevation
	ushort sweep_correction;	// [16] pwm correction value for sweep
// v1.208 start
//	char	 rsrved[8];			// [18] reserved so additions wont have to move everything
	char	 rsrved[9];			// [18] reserved so additions wont have to move everything
//};							// [20] size of this header structure
};								// [21] size of this header structure
// v1.208 end

struct shot {
	ushort top;							// top serve motor param
	ushort bot;							// bot serve motor param
	ushort y;								// elevation motor param
	ushort x;								// sweep motor param
	ushort yLo;							// only used for random vertical
	ushort xLeft;						// only used for random horizontal
	ushort feedrate;
	ushort sweeprate;				// pwm for either bidirectional motor
};

// Parameters for location selection - full random
//
struct rand_info {
	char range;
	char incr;
	char start;
	char max;
};

#ifdef NOHOME
#define start_home()
#define homing()			(ticks < (150U + home_ticks))
#else // HOME
#define start_home()	CurShotState = SHOT_INIT; home_ticks = ticks
#define homing()			(CurShotState >= (ushort)SHOT_INIT)
#endif // NOHOME else HOME

#ifdef NOSHOT
#define start_shot()
#define STARTFEED(val)// SetFeedRate(val)
#define STARTFDCONT(val)// StartFeedCont(val)
#else // SHOT
#define start_shot()	CurShotState = SHOT_RUN
#define STARTFEED(val) SetFeedRate(val)
#define STARTFDCONT(val) StartFeedCont(val)
#endif // NOSHOT else SHOT

#ifdef DBGSTR
#define PUTRSDIAG(str)	putrsDIAG(str)
#define DUMPHEX(x)			dumphex(x)
#define DUMPLONG(x)			dumplong(x)
#define DUMPSHORT(x)		dumpshort(x)
#define DUMPADDR(x)			dumpaddr(x)
#else  // !DBGSTR
#define PUTRSDIAG(str)
#define DUMPHEX(x)
#define DUMPLONG(x)
#define DUMPSHORT(x)
#define DUMPADDR(x)
#endif //

#ifdef SHOW_BUILDCMD
#define DUMPHEX(x) dumphex(x)
#define CRLF() putrsDIAG(crlf)
#else // NOT SHOW_BUILDCMD
//#define DUMPHEX(x) dumphex(x) // fails if we don't do this
#ifndef DUMPHEX
#define DUMPHEX(x) 
#endif
#define CRLF()
#endif // SHOW_BUILDCMD else NOT

// shot states
#define SHOT_IDLE			0U
#define	SHOT_RUN			1
#define	SHOT_SNDY			2
#define	SHOT_STRTTOP		3
#define	SHOT_STRTBOT		4
#define	SHOT_SNDSTAT		5
#define	SHOT_WAITSTAT		6
#define	SHOT_DLYSTAT		7
#define	SHOT_FEED			8
#define	SHOT_DONE			9
#define SHOT_WAIT_SNDX	  	10
//#define SHOT_RAN_INIT		20
//#define	SHOT_RAN_STRTTOP	21
//#define	SHOT_RAN_STRTBOT	22
//#define	SHOT_RAN_SNDSTAT	23
//#define	SHOT_RAN_WAITSTAT	24
//#define	SHOT_RAN_DLYSTAT	25
//#define	SHOT_RAN_STARTFEED	26
//#define SHOT_RAN_LOOP		27

// v1.190 start
#define SHOT_RAN_INIT		17
#define	SHOT_RAN_STRTTOP	18
#define	SHOT_RAN_STRTBOT	19
#define	SHOT_RAN_SNDSTAT	20
#define	SHOT_RAN_WAITSTAT	21
#define	SHOT_RAN_DLYSTAT	22
#define	SHOT_RAN_SNDSTAT2	23
#define	SHOT_RAN_WAITSTAT2	24
#define	SHOT_RAN_DLYSTAT2	25
#define	SHOT_RAN_STARTFEED	26
#define SHOT_RAN_LOOP		27
// v1.190 end

#define SHOT_INIT			30U
#define SHOT_INIT1			31
#define	SHOT_HOME_Y			32
#define	SHOT_SLOWTOP		33
#define	SHOT_SLOWBOT		34
#define	SHOT_INITSTAT		35
#define	SHOT_INITWAIT		36
#define	SHOT_INITDLY		37

#define PLAY_BEGIN			0U
#define PLAY_WAIT			1U
#define PLAY_DONE			2U

// v1.221 start
//#define STABLE_TICKS		20		// Used to qualify power button presses, to power off and power on machine
#define STABLE_TICKS		30		// Used to qualify power button presses, to power off and power on machine
// v1.221 end
// v1.221 start
// Separated Power-Off button press stable ticks so the value can be increased without affecting Power-On.
// The value is increased for Power-Off so there is less chance of an accidental Power-Off when just checking 
// the battery level with a short press of the Power button.
//#define PWROFF_STABLE_TICKS 20
// v1.221 end

// Power Button Related Definitions

#define POWER_PRESSED	0
#define POWER_RELEASED	1

// Power Button - keeps debounce information all in one spot
struct power_button {
	uchar count;							// time (in ticks) since power button was last stable
	uchar previous;						// previous instantaneous state of the power button
	uchar current;						// current instantaneous state (read from PORTBbits.INT0)
	uchar previous_debounced;	// previous debounced state of the power button
	uchar debounced;					// most recent debounced state of the power button
};

// Serve Motor Speeds
#define SRV_00MPH 0x7000		// extrapolated - do NOT use
#define SRV_05MPH 0x7000		// extrapolated - do NOT use
#define SRV_10MPH 0x7000		// extrapolated - do NOT use
#define SRV_15MPH 0x7000		// extrapolated - do NOT use
#define SRV_20MPH 0x6000
#define SRV_25MPH 0x4500
#define SRV_30MPH 0x4000
#define SRV_35MPH 0x3600
#define SRV_40MPH 0x3200
#define SRV_42_5  0x3000
#define SRV_45MPH 0x2900
#define SRV_47_5  0x2800
#define SRV_50MPH 0x2700
#define SRV_55MPH 0x2500
#define SRV_60MPH 0x2200
#define SRV_65MPH 0x2100
#define SRV_70MPH 0x1a00
#define SRV_75MPH 0x1a00
// Minimum speed while waiting for user
#define SRV_MINIMUM SRV_20MPH
#define SRV_MAXIMUM SRV_75MPH

// SPI Device Selection
#define LOB_SLAVE		0
#define LOB_EEPROM	1
#define LOB_PGA			2
#define LOB_POT			3

struct moterr {
	unsigned LoBat:1;						//  0 - set if low battery
	unsigned Reserved1:1;				//  1 -			reserved
	unsigned Reserved2:1;				//  2 -			reserved
	unsigned TopPeriod:1;				//  3 - set if top motor period requested is invalid
	unsigned BotPeriod:1;				//  4 -	set if bot motor period requested is invalid
	unsigned TopCurSense:1;			//  5 - set if top serve motor over current
	unsigned BotCurSense:1;			//  6 - set if bottom serve motor over current
	unsigned FeedCurSense:1;		//  7 - set if feed motor over current
	unsigned SweepCurSense:1;		//  8 - set if sweep motor over current
	unsigned ElevCurSense:1;		//  9 - set if elevation motor over current
	unsigned TopCnt:1;					// 10 - set if top serve motor is not getting timer intr
	unsigned BotCnt:1;					// 11 - set if bot serve motor is not getting timer intr
	unsigned Reserved:1;				// 12 - set if low battery
	unsigned SweepCnt:1;				// 13 - set if sweep motor is not getting decoder counts
	unsigned ElevCnt:1;					// 14 - set if elevation motor is not getting decoder counts
};

// 07/29/09 start
// model_type
#define EL05	0U	// Elite Grand V
#define EL05LE	1U	// Elite Grand V Limited Edition
#define EL04	2U	// Elite Grand
// v1.199 start
//#define CLUB	3U	// Club Model
#define CLUB2	3U	// Club Model, Phenom 2
// v1.199 end
// v1.185 start
#define EL05LE2	4U	// Elite Grand V Limited Edition with new control panel
// v1.185 end
// v1.199 start
#define CLUB1	5U	// Phenom 1 (Club with EL04 panel)
// v1.199 end
// v1.221 start
#define BatLevelDisplayTmrPreload 0;
#define PlayLEDTmrPreload 170;
// v1.221 end
