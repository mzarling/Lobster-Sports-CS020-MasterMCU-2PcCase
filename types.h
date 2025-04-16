// Typedefs
//
//	©2003 Censerve® Consulting, Inc.
//				All rights reserved.
//

#ifndef __TYPES_H
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
// C18 to XC8 Compiler Changes
#ifdef notdef
// C18 to XC8 Compiler Changes
typedef volatile unsigned long vulong;
typedef unsigned char packettype[8];

#ifdef DANWIN
#define far
#define rom
#define ram
#define auto
#define near
#define far
typedef unsigned long uslong;
typedef signed long sslong;
typedef signed long sslint;
typedef unsigned long uslint;
#define strcpypgm2ram strcpy
#define strncpypgm2ram strncpy
#define strlenpgm strlen
#else // !DANWIN
typedef unsigned short long uslong;
typedef signed short long sslong;
typedef signed short long int sslint;
typedef unsigned short long int uslint;
typedef unsigned bool;
#define TRUE	1U
#define FALSE 0U
#endif
// C18 to XC8 Compiler Changes
#endif
// C18 to XC8 Compiler Changes

typedef union sc {
	char c[2];
	uchar uc[2];
	short s;
	ushort us;
} mini_variant;

// Union allows non-cast conversion between long and pointer types
//
typedef union ulp {
	char c[4];
	uchar uc[4];
	short s[2];
	ushort us[2];
	long l;
	unsigned long ul;
	void *vp;
	const void *cvp;
	volatile void *vvp;
	char *cp;
	const char *ccp;
	uchar *ucp;
	const uchar *cucp;
	short *sp;
	const short *csp;
	ushort *usp;
	const ushort *cusp;
	long *lp;
	const long *clp;
	ulong *unlp;
	const ulong *culp;
	void  (*func)();
} variant;

// C18 to XC8 Compiler Changes
#ifdef notdef
// C18 to XC8 Compiler Changes

typedef struct stmach stmach;

typedef char t_device;

// C18 to XC8 Compiler Changes
#endif
// C18 to XC8 Compiler Changes

union parm {
// 07/03/09 start
//	char value[8];	

// C18 to XC8 Compiler Changes
//	char value[9];
	signed char value[9];
// C18 to XC8 Compiler Changes

// 07/03/09 end
	struct {
  	uchar speed; 	  // +30 to +95 miles per hour
  	uchar elev;  	  // +15 to +60 degrees
// C18 to XC8 Compiler Changes
//  char spin;	    //  +3 to  -3 unitless						WAS fifth item see below
//  char horiz; 	  // +45 to -45 degrees
  	signed char spin;	    //  +3 to  -3 unitless						WAS fifth item see below
   	signed char horiz; 	  // +45 to -45 degrees
// C18 to XC8 Compiler Changes
// 07/03/09 start
	uchar twolnrw;		// 2-Line row selection
// 07/03/09 end
  	uchar feed;  	  // +00 to +20 seconds/ball
//  	char spin;	    //  +3 to  -3 unitless
// v1.190 start
// 	uchar col;			// 0 to 5 random sweep columns
// 	uchar row;			// 0 to	2 random elevation rows
// The functions of col and row are reversed from what they imply.
// col(umn) actually indicates the row selected in Random Horizontal mode.
// row actually indicates the column selected in Random Depth mode.
  	uchar col;			// 0 to	3 random sweep rows; A, B, C or RANDOM, not necessarily in that order
  	uchar row;			// 0 to	5 random elevation columns
// v1.190 end
  	uchar width;		// 0 to	2 two line drill spacing
  };
};
	
// There should be one of these for each parameter of each mode and submode that want
//	to call lob_parm to process the menu arrow and +/- buttons.
//	e.g. 4 for manual mode, 16 for preset modes
//struct parm_def {
//	char parm_no:4;		// index into generic parameter tables for formatting, limits, etc.
//	char parm_ln:4;		// line number on which parameter is to be displayed
//	char parm_val;		// value of this parameter
//};

// C18 to XC8 Compiler Changes
//struct limit {
//  char minimum;	// minimum value for parameter
//  char maximum;	// maximum value for parameter
//  char incr;	// increment
//};
struct limit {
  signed char minimum;	// minimum value for parameter
  signed char maximum;	// maximum value for parameter
  signed char incr;	// increment
};
// C18 to XC8 Compiler Changes


struct motor_status {
	unsigned cap1:1;				// top motor - capture module 1 interrupt occurred
	unsigned cap2:1;				// bot motor - capture module 2 interrupt occurred
	unsigned HaveCmd:1;				// diagnostic command received
	unsigned InCmd:1;				// processing diagnostic command
	unsigned CmdErr:1;				// command error - command string too long
	unsigned TickInt:1;				// tick interrupt had occurred (timer 2)
	unsigned WaitResp:1;			// waiting for response from slave
	unsigned a2dStarted:1;			// started next a2d operation
};


#define NUMPROG		30	// number of personal programs
// v1.223 start
//#define AVAILPRG	6	// number of usable (available) personal programs
#define AVAILPRG	12	// number of usable (available) personal programs
// v1.223 end
#define NUMSHOT		30	// number of shots per pesrsonal program
#define AVAILSHT	6	// number of usable (available) shots 
#define NUMMETH 	50	// number of methods per shot
#define NUMLOCS		18	// number of location buttons (choices)
#define MAXFEED 	12	// maximum feed value 
#define MINFEED 	3	// minimum feed value

struct persprog {
// C18 to XC8 Compiler Changes Start
//char shot[NUMSHOT]; 		// basic location
//char method[NUMSHOT];			// combination of parameters to achieve this shot
  signed char shot[NUMSHOT]; 	// basic location
  signed char method[NUMSHOT];	// combination of parameters to achieve this shot
// C18 to XC8 Compiler Changes End
// v1.200 start
//char feed;				// feed between this and the next shot
  char feed[NUMSHOT];		// feed between this and the next shot
// v1.200 end
  char reserved;			// 64
};
// C18 to XC8 Compiler Changes
#ifdef notdef
// C18 to XC8 Compiler Changes
#define __TYPES_H
#endif //__TYPES_H
// C18 to XC8 Compiler Changes
#endif
// C18 to XC8 Compiler Changes

