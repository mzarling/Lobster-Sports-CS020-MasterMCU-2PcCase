// Lobster EL04 Personal Program Routines
//	

// C18 to XC8 Compiler Changes
// Replaced all strcpypgm2ram with strcpy.
// Replaced all strncpypgm2ram with strncpy.
// C18 to XC8 Compiler Changes

#include "types.h"
#include "lob.h"
// C18 to XC8 Compiler Changes
//#include "lcd.h"
// C18 to XC8 Compiler Changes
#include <string.h>
#include "diag.h"

// C18 to XC8 Compiler Changes
#include <delays.h>
// C18 to XC8 Compiler Changes


// C18 to XC8 Compiler Changes Start
//char load_prog(char progno, struct persprog* ppers);
//char save_prog(char progno, struct persprog* ppers);
char load_prog(signed char progno, struct persprog* ppers);
char save_prog(signed char progno, struct persprog* ppers);
// C18 to XC8 Compiler Changes End

// C18 to XC8 Compiler Changes
//void chartoa(const char c, char* cp, char ndig); 
void chartoa(const signed char c, char* cp, signed char ndig); 
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//char getLocAvail(char locNo, uchar spd, char spn, char* pmeth);
char getLocAvail(char locNo, uchar spd, signed char spn, char* pmeth);
// C18 to XC8 Compiler Changes

char pp_dashes = 0;         // 02/07/09
char pp_odashes = 0;        // 02/07/09
char lcd_update_wait = 0;   // 02/07/09

// v1.219 start
void remote_programming (void);
// v1.219 end

extern void BuildCmd(int code, int cnt, long* args);

// C18 to XC8 Compiler Changes
//extern const rom char preparm1base[];
extern const char preparm1base[];
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//extern char lcd_line[][17];
extern char lcd_line[][17];
// C18 to XC8 Compiler Changes

extern struct shot CurShot,NextShot;
// 07/03/09 start
//extern 								ushort CurShotState;
extern ushort CurShotState;
// 07/03/09 end
extern long play_mode;
extern uchar lcd_updated;

// C18 to XC8 Compiler Changes
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
//extern const rom char *parm_fmt[6][2];
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];

//extern const char *parm_fmt[6][2];
extern const char *parm_fmt[7][2];
// C18 to XC8 Compiler Changes

extern struct t_parm parameters[];
extern signed short eladj;

// C18 to XC8 Compiler Changes
//extern const rom char speed_trans[];
extern const char speed_trans[];
// C18 to XC8 Compiler Changes

// 07/29/09 start
extern unsigned char model_type;
// 07/29/09 end
// v1.219 start
extern unsigned int RemotePgmMode;
// v1.219 end

char prog_chgd=0;			// did anyyhing change
signed char program_ix;				// program # currently under consideration
signed char ppshot_ix;				// shot within program under consideration
signed char ppparm_ix;				// parameter currently under consideration
struct persprog act_program;	// ram copy of current program
char lcd_changed;
char priorState = PP_POWERON;
char curState = PP_POWERON;

// Key Press to Location Number (0 indicates not a location button)

// C18 to XC8 Compiler Changes
//const rom char key2loc[] = {
const char key2loc[] = {
// C18 to XC8 Compiler Changes

//	0, 0, 0, 0, 0, 0, 0, 6,	// KY 0-07		36 selectable shot locations
// 18,12,24,30,36, 5,17,11, // KY 8-0F		36 selectable shot locations
// 23,29,35, 4,16,10,22,28, // KY 10-17		36 selectable shot locations
// 34, 3,15, 9,21,27,33, 2,	// KY 18-1F		36 selectable shot locations
// 14, 8,20,26,32, 1,13, 7,	// KY 20-27		36 selectable shot locations
// 19,25,31, 0,	0, 0, 0, 0, // KY 28-2F		36 selectable shot locations

// 07/29/09 start
// EL05 table
// 06/16/09 start
  0, 0, 0, 0, 0, 0, 0, 6,	// KY 0-07		6 selectable shot locations
  0, 0, 0, 0, 0, 5, 0, 0,	// KY 8-0F		6 selectable shot locations
  0, 0, 0, 4, 0, 0, 0, 0,	// KY 10-17		6 selectable shot locations
  0, 3, 0, 0, 0, 0, 0, 2,	// KY 18-1F		6 selectable shot locations
  0, 0, 0, 0, 0, 1, 0, 0,	// KY 20-27		6 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 28-2F		6 selectable shot locations

// EL05LE table
// 07/29/09 end
  0, 0, 0, 0, 0, 0, 0, 6,	// KY 0-07		18 selectable shot locations
 18,12, 0, 0, 0, 5,17,11,   // KY 8-0F		18 selectable shot locations
  0, 0, 0, 4,16,10, 0, 0,   // KY 10-17		18 selectable shot locations
  0, 3,15, 9, 0, 0, 0, 2,	// KY 18-1F		18 selectable shot locations
 14, 8, 0, 0, 0, 1,13, 7,	// KY 20-27		18 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 28-2F		18 selectable shot locations
// 06/16/09 end

// 11/04/09 start
// EL04 table
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 0-07		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 8-0F		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 10-17		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 18-1F		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 20-27		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 28-2F		0 selectable shot locations

// CLUB MODEL table
// CLUB2 (Phenom 2) table	v1.199
  0, 0, 0, 0, 0, 0, 0, 6,	// KY 0-07		18 selectable shot locations
 18,12, 0, 0, 0, 5,17,11,   // KY 8-0F		18 selectable shot locations
  0, 0, 0, 4,16,10, 0, 0,   // KY 10-17		18 selectable shot locations
  0, 3,15, 9, 0, 0, 0, 2,	// KY 18-1F		18 selectable shot locations
 14, 8, 0, 0, 0, 1,13, 7,	// KY 20-27		18 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 28-2F		18 selectable shot locations
// 11/04/09 end

// v1.186 start
// EL05LE2 table, new control panel
  0, 0, 0, 0, 0, 0, 0, 6,	// KY 0-07		18 selectable shot locations
 18,12, 0, 0, 0, 5,17,11,   // KY 8-0F		18 selectable shot locations
  0, 0, 0, 4,16,10, 0, 0,   // KY 10-17		18 selectable shot locations
  0, 3,15, 9, 0, 0, 0, 2,	// KY 18-1F		18 selectable shot locations
 14, 8, 0, 0, 0, 1,13, 7,	// KY 20-27		18 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 28-2F		18 selectable shot locations
// v1.186 end

// v1.199 start
// CLUB1 (Phenom 1) table
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 0-07		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 8-0F		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 10-17		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 18-1F		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,	// KY 20-27		0 selectable shot locations
  0, 0, 0, 0, 0, 0, 0, 0,   // KY 28-2F		0 selectable shot locations
// v1.199 end
};

// Parameter format strings for Personal Program
//  two for each parameter: normal and highlighted

// C18 to XC8 Compiler Changes
//const rom char *pp_fmt[][2] = {
const char *pp_fmt[][2] = {
// C18 to XC8 Compiler Changes

//   0123456789012345     0123456789012345
  { "SPEED:      MPH ", "SPEED: >    MPH<" },
  { "SPIN:   ", "SPIN>  <" },
  { "FEED:   ", "FEED>  <" },
	{ "","" },
	{ "","" },
// 07/13/09 start
// 06/16/09 start
//	{ "PROGRAM #:      ", "PROGRAM #:> <   " },
//	{ "PROG:    or PLAY", "PROG:> < or PLAY" },
	{ "P#   PLAY TO RUN", "P#> <PLAY TO RUN" },
// 06/16/09 end
// 07/13/09 end
	{ "SHOT:   ", "SHOT> < " },
	{ "P#  SHOT#  LOC ?", "P#  SHOT#  LOC# " },
	{ "EDIT:NO ", "EDIT>NO<" },
	{ " Next + Back -  ", ">NEXT + BACK - <" },
};

struct t_parm ppparams[] = {
	// limits/incr	display text fmt pos  sz st ln 
	{ SPEED_LIMITS, &parm_fmt[SPEED_IX][0], 2, 9, 16, 0, 0 },	// limits, format strings, formatting, location
	{ SPIN_LIMITS, &parm_fmt[SPIN_IX][0], -1, 5, 8, 0, 2 },
	{ FEED_LIMITS, &parm_fmt[FEED_IX][0], 2, 9, 16, 0, 0 },
	{ ROW_LIMITS, &parm_fmt[ROW_IX][0],STR_FMT,10,16, 0, 0 },
};
// Finds the method for a given location in the shot tables which corresponds
//	to 50 mph with no spin (this is the default parameter set for personal
//	programs and presets
//
char get50mph0spin(char loc)
{
	char i;
	
// v1.219 start
//	if ((model_type != CLUB1) && (model_type != CLUB2)){		// all models except CLUBs
//		for (i=0; court_locs[loc][i].speed != 0U; i++) {		// search all methods
//			if (court_locs[loc][i].speed != 50U) {
//			} else if (court_locs[loc][i].spin != 0) {
//			} else {
//				break;											// quit because we found it
//			}
//		}
//		return ((court_locs[loc][i].speed) ? i : 0);			// return method 0 or 50/0
//	}
//	else {	// CLUB model only
//		for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {		// search all methods
//			if (court_locs_CLUB[loc][i].speed != 50U) {
//			} else if (court_locs_CLUB[loc][i].spin != 0) {
//			} else {
//				break;											// quit because we found it
//			}
//		}
//		return ((court_locs_CLUB[loc][i].speed) ? i : 0);			// return method 0 or 50/0
//	}
#ifndef PHENOMS
		for (i=0; court_locs[loc][i].speed != 0U; i++) {		// search all methods
			if (court_locs[loc][i].speed != 50U) {
			} else if (court_locs[loc][i].spin != 0) {
			} else {
				break;											// quit because we found it
			}
		}
		return ((court_locs[loc][i].speed) ? i : 0);			// return method 0 or 50/0
#else
		for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {		// search all methods
			if (court_locs_CLUB[loc][i].speed != 50U) {
			} else if (court_locs_CLUB[loc][i].spin != 0) {
			} else {
				break;											// quit because we found it
			}
		}
		return ((court_locs_CLUB[loc][i].speed) ? i : 0);			// return method 0 or 50/0
#endif
// v1.219 end
}
// 12/31/08 start
char get60mph0spin(char loc)	// All valid selectable locations must have a 60mph, 0 spin shot for this to work (70mph displayed).
{								// 60mph, 0 spin is currently a valid shot for locations 1 thru 12, but not 13 thru 18.
	char i;						// However, the current EL05 model only supports location 1 thru 6 in Program Mode, so all is good.
	
// v1.219 start
//	if ((model_type != CLUB1) && (model_type != CLUB2)){		// all models except CLUBs
//		for (i=0; court_locs[loc][i].speed != 0U; i++) {		// search all methods
//			if (court_locs[loc][i].speed != 60U) {
//			} else if (court_locs[loc][i].spin != 0) {
//			} else {
//				break;											// quit because we found it
//			}
//		}
//// 06/16/09 start
////	return ((court_locs[loc][i].speed) ? i : 0);			// return method 0 or 60/0
//		if (!(court_locs[loc][i].speed)) {						// If a 60mph/0 spin location did not exist,
//			for (i=0; court_locs[loc][i].speed != 0U; i++) {	// then repeat search for 50mph/0 spin shot.
//				if (court_locs[loc][i].speed != 50U) {
//				} else if (court_locs[loc][i].spin != 0) {
//				} else {
//					break;										// quit because we found it
//				}
//			}
//		}
//		return ((court_locs[loc][i].speed) ? i : 0);			//	return 60mph/0 spin, 50mph/0 spin method or method 0
//// 06/16/09 end
//	}
//	else {	// CLUB model only
//		for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {		// search all methods
//			if (court_locs_CLUB[loc][i].speed != 60U) {
//			} else if (court_locs_CLUB[loc][i].spin != 0) {
//			} else {
//				break;											// quit because we found it
//			}
//		}
//		if (!(court_locs_CLUB[loc][i].speed)) {						// If a 60mph/0 spin location did not exist,
//			for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {	// then repeat search for 50mph/0 spin shot.
//				if (court_locs_CLUB[loc][i].speed != 50U) {
//				} else if (court_locs_CLUB[loc][i].spin != 0) {
//				} else {
//					break;										// quit because we found it
//				}
//			}
//		}
//		return ((court_locs_CLUB[loc][i].speed) ? i : 0);			//	return 60mph/0 spin, 50mph/0 spin method or method 0
//	}
// 12/31/08 end
#ifndef PHENOMS
		for (i=0; court_locs[loc][i].speed != 0U; i++) {		// search all methods
			if (court_locs[loc][i].speed != 60U) {
			} else if (court_locs[loc][i].spin != 0) {
			} else {
				break;											// quit because we found it
			}
		}
		if (!(court_locs[loc][i].speed)) {						// If a 60mph/0 spin location did not exist,
			for (i=0; court_locs[loc][i].speed != 0U; i++) {	// then repeat search for 50mph/0 spin shot.
				if (court_locs[loc][i].speed != 50U) {
				} else if (court_locs[loc][i].spin != 0) {
				} else {
					break;										// quit because we found it
				}
			}
		}
		return ((court_locs[loc][i].speed) ? i : 0);			//	return 60mph/0 spin, 50mph/0 spin method or method 0
#else
		for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {		// search all methods
			if (court_locs_CLUB[loc][i].speed != 60U) {
			} else if (court_locs_CLUB[loc][i].spin != 0) {
			} else {
				break;											// quit because we found it
			}
		}
		if (!(court_locs_CLUB[loc][i].speed)) {						// If a 60mph/0 spin location did not exist,
			for (i=0; court_locs_CLUB[loc][i].speed != 0U; i++) {	// then repeat search for 50mph/0 spin shot.
				if (court_locs_CLUB[loc][i].speed != 50U) {
				} else if (court_locs_CLUB[loc][i].spin != 0) {
				} else {
					break;										// quit because we found it
				}
			}
		}
		return ((court_locs_CLUB[loc][i].speed) ? i : 0);			//	return 60mph/0 spin, 50mph/0 spin method or method 0
#endif
// v1.219 end
}

uchar show_prog1(unsigned char field)
{
	unsigned char i;
	unsigned char nused;
	
 	putrsDIAG("\n\rSCRN1: P");
	dumphex(program_ix+1);

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[0][0],pp_fmt[PF_PROGNO][(field == PF_PROGNO)?1:0]);	// not working in XC8
	if (field == PF_PROGNO) strcpy(&lcd_line[0][0],pp_fmt[PF_PROGNO][1]);
	else strcpy(&lcd_line[0][0],pp_fmt[PF_PROGNO][0]);
// C18 to XC8 Compiler Changes

// 07/13/09 start
// 06/16/09 start
//	chartoa(program_ix+1,&lcd_line[0][11],1);				// add program #
//	chartoa(program_ix+1,&lcd_line[0][6],1);				// add program #
// v1.223 start
//	chartoa(program_ix+1,&lcd_line[0][3],1);				// add program #
	if (program_ix+1 < 10)									// If program number is 1 digit,
		chartoa(program_ix+1,&lcd_line[0][3],1);			// add program #
	else													// else 2 digits.
		chartoa(program_ix+1,&lcd_line[0][2],2);			// add program #
// v1.223 end
// 07/13/09 end
//	strcpypgm2ram(&lcd_line[1][0],"LOCS            ");			
	strcpy(&lcd_line[2][0],"LOCS            ");		// clear 3rd line			
	strcpy(&lcd_line[3][0],"                ");		// clear 4th line			
// 06/16/09 end
	for (i=0,nused=AVAILSHT-1; i<AVAILSHT; i++) {		// add shot(s)
		if (act_program.shot[i] != INV_SHOT) {
// 06/16/09 start
//			chartoa(act_program.shot[i],&lcd_line[1][5 + 2*i],1);	// add loc #s
			if (i<4){	// display the first 4 shots on the 3rd line
				if (act_program.shot[i]<10)
					chartoa(act_program.shot[i],&lcd_line[2][5 + 3*i],1);	// add loc #s
				else chartoa(act_program.shot[i],&lcd_line[2][5 + 3*i],2);	// add loc #s
			}
			else {		// display remaining shots on the 4th line
				if (act_program.shot[i]<10)
					chartoa(act_program.shot[i],&lcd_line[3][5+(3*(i-4))],1);	// add loc #s
				else chartoa(act_program.shot[i],&lcd_line[3][5+(3*(i-4))],2);	// add loc #s
			}
// 06/16/09 end
		} else {
			nused = i;
			break;
		}
	}
// 	if (field == PF_PROGNO)													//	if changed program
// 		ppshot_ix = (nused < AVAILSHT) ? nused : nused-1;	//reset current shot #
 	putrsDIAG("S");
 	dumphex(ppshot_ix+1);
	putrsDIAG("#");
 	dumphex(nused);
// 06/16/09 start
//	strcpypgm2ram(&lcd_line[2][0],pp_fmt[PF_SHOTNO][(field == PF_SHOTNO)?1:0]);

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[1][0],pp_fmt[PF_SHOTNO][(field == PF_SHOTNO)?1:0]);	// not working in XC8
	if (field == PF_SHOTNO) strcpy(&lcd_line[1][0],pp_fmt[PF_SHOTNO][1]);
	else strcpy(&lcd_line[1][0],pp_fmt[PF_SHOTNO][0]);
// C18 to XC8 Compiler Changes

//	chartoa(ppshot_ix+1,&lcd_line[2][5],1);
	chartoa(ppshot_ix+1,&lcd_line[1][5],1);
//	strcpypgm2ram(&lcd_line[2][8],pp_fmt[PF_EDIT][(field == PF_EDIT)?1:0]);

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[1][8],pp_fmt[PF_EDIT][(field == PF_EDIT)?1:0]);
	if (field == PF_EDIT) strcpy(&lcd_line[1][8],pp_fmt[PF_EDIT][1]);
	else strcpy(&lcd_line[1][8],pp_fmt[PF_EDIT][0]);
// C18 to XC8 Compiler Changes

//	strcpypgm2ram(&lcd_line[3][0],"PUSH PLAY TO RUN");
// 06/16/09 end
 	lcd_updated = 1;
 	priorState = curState;
 	return(nused);
}

void show_prog2()
{
// 06/16/09 start
//	strcpypgm2ram(&lcd_line[0][0],	"P#  SHOT#  LOC ?");
	strcpy(&lcd_line[0][0],	"P#  Shot#  Loc ?");
// 06/16/09 end
// v1.223 start
//	chartoa(program_ix+1,&lcd_line[0][2],1);
	if (program_ix+1 < 10)
		chartoa(program_ix+1,&lcd_line[0][2],1);										// Need 2 digits now since changed from 6 to 12 max.
	else
		chartoa(program_ix+1,&lcd_line[0][2],2);									// Need 2 digits now since changed from 6 to 12 max.
// v1.223 end
	chartoa(ppshot_ix+1, &lcd_line[0][9],1);
  if (act_program.shot[ppshot_ix] != INV_SHOT) {
// 06/16/09 start
//		chartoa(act_program.shot[ppshot_ix], &lcd_line[0][15],1);					// add shot #s
		if (act_program.shot[ppshot_ix]<10)
			chartoa(act_program.shot[ppshot_ix], &lcd_line[0][15],1);				// add shot #s
		else chartoa(act_program.shot[ppshot_ix], &lcd_line[0][14],2);				// add shot #s
// 06/16/09 end
	}
	strcpy(&lcd_line[1][0],  "Press a location");
	strcpy(&lcd_line[2][0],	"to Prgm shot, or");
	strcpy(&lcd_line[3][0],	"Press - to erase");
	lcd_updated = 1;
 	priorState = curState;
}

void show_prog3()
{
	unsigned char loc;

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* locp;
	const struct el04_loc* locp;
// C18 to XC8 Compiler Changes
	
	putrsDIAG("\n\rSCRN3: L#");
	loc = act_program.shot[ppshot_ix];	
// v1.219 start
//	if ((model_type != CLUB1) && (model_type != CLUB2))					// all models except CLUBs
//		locp = &court_locs[loc-1][act_program.method[ppshot_ix]];
//	else
//		locp = &court_locs_CLUB[loc-1][act_program.method[ppshot_ix]];	// CLUB model only
#ifndef PHENOMS
		locp = &court_locs[loc-1][act_program.method[ppshot_ix]];
#else
		locp = &court_locs_CLUB[loc-1][act_program.method[ppshot_ix]];	// CLUB model only
#endif
// v1.219 end
	dumphex(loc);
	putrsDIAG(" M#");
	dumphex(act_program.method[ppshot_ix]);
// 06/19/09 start
//	strcpypgm2ram(&lcd_line[0][0],	"P#  SHOT#  LOC ?");
	strcpy(&lcd_line[0][0],	"P#  Shot#  Loc ?");
// 06/19/09 end
	chartoa(program_ix+1,&lcd_line[0][2],1);
	chartoa(ppshot_ix+1, &lcd_line[0][9],1);
	if (loc != INV_SHOT) {
// 06/16/09 start
//		chartoa(loc, &lcd_line[0][15],1);
		if (loc<10)
			chartoa(loc, &lcd_line[0][15],1);
		else chartoa(loc, &lcd_line[0][14],2);
// 06/16/09 end
	}

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[1][0],pp_fmt[SPEED_IX][(ppparm_ix == SPEED_IX)?1:0]);		// doesn't work in XC8
	if (ppparm_ix == SPEED_IX) strcpy(&lcd_line[1][0],pp_fmt[SPEED_IX][1]);
	else strcpy(&lcd_line[1][0],pp_fmt[SPEED_IX][0]);
// C18 to XC8 Compiler Changes

    if (pp_dashes == 1) {                                                           // If speed value to be replaced with dashes, 02/07/09
	strncpy(&lcd_line[1][9],"--",2);                                         // then replace with dashes in lcd data, 02/07/09
    }
    else chartoa(speed_trans[locp->speed / 5],&lcd_line[1][9],2);                   // else display normal speed value. 02/07/09
//	chartoa(speed_trans[locp->speed / 5],&lcd_line[1][9],2);                        // 02/07/09 commented

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[2][0],pp_fmt[SPIN_IX/2][(ppparm_ix == SPIN_IX)?1:0]);		// doesn't work in XC8
	if (ppparm_ix == SPIN_IX) strcpy(&lcd_line[2][0],pp_fmt[SPIN_IX/2][1]);
	else strcpy(&lcd_line[2][0],pp_fmt[SPIN_IX/2][0]);
// C18 to XC8 Compiler Changes

    if (pp_dashes == 2)                                                             // If spin value to be replaced with dashes, 02/07/09
	strncpy(&lcd_line[2][5],"--",2);                                         // then replace with dashes in lcd data, 02/07/09
    else chartoa(locp->spin,&lcd_line[2][5],-1);                                    // else display normal spin value. 02/07/09
//	chartoa(locp->spin,&lcd_line[2][5],-1);                                         // 02/07/09 commented

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[2][8],pp_fmt[FEED_IX/2][(ppparm_ix == FEED_IX)?1:0]);		// doesn't work in XC8
	if (ppparm_ix == FEED_IX) strcpy(&lcd_line[2][8],pp_fmt[FEED_IX/2][1]);
	else strcpy(&lcd_line[2][8],pp_fmt[FEED_IX/2][0]);
// C18 to XC8 Compiler Changes

// v1.200 start
//	chartoa(act_program.feed, &lcd_line[2][14],1);
	chartoa(act_program.feed[ppshot_ix], &lcd_line[2][14],1);
// v1.200 end

// C18 to XC8 Compiler Changes
//	strcpy(&lcd_line[3][0],pp_fmt[PF_TRANS][(ppparm_ix == (FEED_IX+2))?1:0]);	// doesn't work in XC8
	if (ppparm_ix == (FEED_IX+2)) strcpy(&lcd_line[3][0],pp_fmt[PF_TRANS][1]);
	else strcpy(&lcd_line[3][0],pp_fmt[PF_TRANS][0]);
// C18 to XC8 Compiler Changes

	priorState = curState;
    if (!(pp_odashes && !pp_dashes))    // If we are not here because the dashes flag has just been cleared due to dashes timer expiration, 02/07/09
	lcd_updated = 1;                    // then update the lcd asap, 02/07/09
}                                       // 02/07/09


// v1.219. start
uchar show_progRPM(unsigned char field) {								// Display for Remote Program Mode.
	unsigned char i;
	unsigned char nused;
	
	strcpy(&lcd_line[0][0],"* APP CONTROL * ");
// v1.223 start
//	strcpy(&lcd_line[1][0],"Program Drill # ");
	chartoa(program_ix+1,&lcd_line[1][15],1);							// Add program #.
	strcpy(&lcd_line[1][0],"Program Drill#  ");							// Needed another space for 2 digit drill numbers, 10, 11, 12.
//	strcpy(&lcd_line[1][0],"Custom Drill #  ");							// Needed another space for 2 digit drill numbers, 10, 11, 12.
	if (program_ix+1 < 10)
		chartoa(program_ix+1,&lcd_line[1][14],1);						// Add program #.
	else
		chartoa(program_ix+1,&lcd_line[1][14],2);						// Add program #.
// v1.223 end
	strcpy(&lcd_line[2][0],"Locations:      ");
	strcpy(&lcd_line[3][0],"                ");							// Clear the 4th line.
	for (i=0,nused=AVAILSHT-1; i<AVAILSHT; i++) {						// Add shot(s).
		if (act_program.shot[i] != INV_SHOT) {
			if (i<2){	// display the first 2 shots on the 3rd line
				if (act_program.shot[i]<10)
					chartoa(act_program.shot[i],&lcd_line[2][11 + 3*i],1);	// add loc #s
				else chartoa(act_program.shot[i],&lcd_line[2][11 + 3*i],2);	// add loc #s
			}
			else {		// display remaining shots on the 4th line
				if (act_program.shot[i]<10)
					chartoa(act_program.shot[i],&lcd_line[3][5+(3*(i-2))],1);	// add loc #s
				else chartoa(act_program.shot[i],&lcd_line[3][5+(3*(i-2))],2);	// add loc #s
			}
		} else {
			nused = i;
			break;
		}
	}
 	lcd_updated = 1;
 	priorState = curState;
 	return(nused);
}
// v1.219 end


// Personal Program 
//
char lob_program(char cmd)
{

// C18 to XC8 Compiler Changes
//	far rom struct el04_loc* locp;
//	far rom struct el04_loc* origp;
//	const struct el04_loc* locp;
//	const struct el04_loc* origp;
// C18 to XC8 Compiler Changes

					signed char origix;												// use for comparison when traversing the court tables
					char val, locno;

// C18 to XC8 Compiler Changes
//					signed char method;
					unsigned char method;
// C18 to XC8 Compiler Changes
					signed char val2;
					static char incr;
					static char parmena = 0;
					unsigned char i;
					static unsigned char nused;
					static unsigned char field;
	
//    char pp_speed_odashes;                                              // 02/07/09

	if (state_just_changed(cmd)) { 										// if first pass
		eladj = 0;																			//	clear any unsaved elevation adjustmet
		putrsDIAG("\n\rPers Prog ");
		play_mode = (play_mode & PM_ELIMMODE) | PM_PERSONAL;//	change to personal program
		BuildCmd('3',1,&play_mode);											//	change LED accordingly
		incr = 0;												//	new program, dont incr
  	ppparm_ix = SPEED_IX;										//	 reset to speed parameter
		ppshot_ix = 0;		
		load_prog(program_ix,&act_program);						// load saved program
 		parmena = 0;											// disable parameter change
 		if (priorState == PP_POWERON) {
	 		field = PF_PROGNO;
	 		curState = PP_SCRN1;
	 		priorState = PP_CHANGE;
	 	} else {
		 	field = PF_PROGNO;
	 		curState = PP_SCRN1;
	 		priorState = PP_CHANGE;
		}
  }
  switch (curState) {
	  case PP_SCRN1:
	  	if (curState != priorState) {							// if changed screen - redraw
// v1.219 start
//		  	nused = show_prog1(field);							//	show the easy way and get # used
			if (!RemotePgmMode)
		  		nused = show_prog1(field);						//	show the easy way and get # used
			else nused = show_progRPM(field);					// Show display for Remote Program Mode.
// v1.219 end
// v1.219 start
//		  	if (nused == 0) {
		  	if (nused == 0 && !RemotePgmMode) {					// If not remotely working with user drills
// v1.219 end
			  	curState = PP_SCRN2;
			  	break;
			  }
			}
// v1.219 start
			if (!RemotePgmMode) {								// Don't allow control panel button presses if in Remote Program Mode.
// v1.219 end
			if ((cmd & 0x7f) == KY_MENUUP) {					// Screen 1 change item to edit
	 			putrsDIAG("MENUUP");
		  	dumphex(field);
	 			if (--field < PF_PROGNO) field = PF_EDIT;		// if lowest field allowed, wrap
	 			else if (field == PF_LOCNO) field = PF_SHOTNO;	// if invalid field, skip it
	 			dumphex(field);
	 			priorState = PP_CHANGE;
	 		} else if ((cmd & 0x7f) == KY_MENUDN) {				// Screen 1 change item to edit
		 		putrsDIAG("MENUDN");
		  	dumphex(field);
		 		if (++field > PF_EDIT)	 field = PF_PROGNO;		// if highest field allowed, wrap
		 		else if (field == PF_LOCNO) field = PF_EDIT;	// if invalid field, skip it
		 		dumphex(field);
		 		priorState = PP_CHANGE;
		 	} else if ((cmd & 0x7f) == KY_PLUS) {				// Screen 1 raise item value
			 	putrsDIAG("PLUS");
			 	if (field == PF_PROGNO) {						//	if raising program #
				 	if (++program_ix >= AVAILPRG) program_ix = 0;	//	 limit by wrapping
				 	ppshot_ix = 0;									//	 always reset to shot 1
//				 	if (field == PF_PROGNO)							//	if changed program
// 						ppshot_ix = (nused < AVAILSHT) ? nused : nused-1;	//reset current shot #
				 	load_prog(program_ix,&act_program);						//	 save prior, load current
				 	lcd_updated = 1;
				} else if (field == PF_SHOTNO) {								// else if raising shot #
					ppshot_ix += 1;
					if (ppshot_ix >= AVAILSHT)										//	limit by wrapping
						ppshot_ix = 0;
					if (ppshot_ix >= (nused+1))										//	limit to programmed shots
						ppshot_ix = 0;
				} else if (field == PF_EDIT) {									// else if changing screen #
//					if (nused > ppshot_ix) {
//						curState = PP_SCRN3;
//					} else {
						curState = PP_SCRN2;												//	always go to screen 2
//					}
				}
				priorState = PP_CHANGE;													//	make sure to redraw screen
			} else if ((cmd & 0x7f) == KY_MINUS) {						// Screen 1 lower item value
				putrsDIAG("MINUS");
				if (field == PF_PROGNO) {												//	if lowering program #
					if (program_ix < 1) program_ix = AVAILPRG-1;	//	 limit by wrapping
					else program_ix--;
					ppshot_ix = 0;																//	 always reset to shot 1
//				 	if (field == PF_PROGNO)													//	if changed program
//						ppshot_ix = (nused < AVAILSHT) ? nused : nused-1;	//reset current shot #
				 	load_prog(program_ix,&act_program);						//	 save prior, load current
				 	lcd_updated = 1;
				} else if (field == PF_SHOTNO) {								//	else if lowering shot #
					if (ppshot_ix < 1) ppshot_ix = (nused == AVAILSHT)? AVAILSHT-1 : nused;
					else ppshot_ix--;															//	 limit by qwrapping
				} else if (field == PF_EDIT) {									//	else if changing screen
//					curState = PP_SCRN3;
					curState = PP_SCRN2;
				}
				priorState = PP_CHANGE;													//	make sure to redraw screen
			}
// v1.219 start
			}
// v1.219 end
	  	break;
		case PP_SCRN2:
			if (curState != priorState) {
		  	putrsDIAG("\n\rSCRN2:");
		  	show_prog2();
			}
	  	if ((cmd >= KY_LOCN06) && (cmd <= KY_LOCN31)) {								// if location was touched
// 07/29/09 start
//	  		if (key2loc[cmd] != KY_INVLOC) {										//	if key is a valid location
	  		if (key2loc[cmd + (model_type * 48)] != KY_INVLOC) {					//	if key is a valid location, using table offset for model
//		  		if (act_program.shot[ppshot_ix] != key2loc[cmd&0x7f]) { 			// if new location	01/05/09 added open brace
// 11/04/09 start
//		  		if (act_program.shot[ppshot_ix] != key2loc[(cmd + (model_type * 48))&0x7f]) { 	// if new location, using table offset for model
		  		if (act_program.shot[ppshot_ix] != key2loc[(cmd + (model_type * 48))]) { 	// if new location, using table offset for model
// 11/04/09 end
// 07/29/09 end
		  			prog_chgd = 1;													//	 program changed
// 07/29/09 start
//			  		act_program.shot[ppshot_ix] = key2loc[cmd&0x7f];				//	 save as current shot
//				  	act_program.method[ppshot_ix] = get60mph0spin(key2loc[cmd]-1);	// set defaults	12/31/08 changed from get50.. to get60..
// 11/04/09 start
//			  		act_program.shot[ppshot_ix] = key2loc[(cmd + (model_type * 48))&0x7f];				//	 save as current shot
			  		act_program.shot[ppshot_ix] = key2loc[(cmd + (model_type * 48))];				//	 save as current shot
// 11/04/09 end
				  	act_program.method[ppshot_ix] = get60mph0spin(key2loc[(cmd + (model_type * 48))]-1);	// set defaults	12/31/08 changed from get50.. to get60..
// 07/29/09 end
				}																	// 01/05/09 added close brace
			  	putrsDIAG("L");
			  	dumphex(act_program.shot[ppshot_ix]);
//					if (++ppshot_ix >= AVAILSHT) ppshot_ix = 0;
// v1.200 start
//	  			if ((act_program.feed < 2) || (act_program.feed > 12))
//	  				act_program.feed = DEF_FEED;
	  			if ((act_program.feed[ppshot_ix] < 2) || (act_program.feed[ppshot_ix] > 12))
	  				act_program.feed[ppshot_ix] = DEF_FEED;
// v1.200 end
//	  			if (nused < AVAILSHT)
//	  				nused++;
	  			putrsDIAG("NUSED ");
	  			dumphex(nused);
			 		priorState = PP_CHANGE;
			 		curState = PP_SCRN3;
		  	}
 			} else if ((cmd & 0x7f) == KY_MINUS) {
	 			prog_chgd = 1;											// deleting shot
	 			for (val2=ppshot_ix; val2 < (AVAILSHT-1); val2++) {
		 			act_program.shot[val2] = act_program.shot[val2+1];
		 			act_program.method[val2] = act_program.method[val2+1];
		 		}
	 			act_program.shot[AVAILSHT-1] = INV_SHOT;
	 			act_program.method[AVAILSHT-1] = 0;
				priorState = PP_CHANGE;
				curState = PP_SCRN1;
			}
			break;
		case PP_SCRN3:
			if (curState != priorState) {							    // if screen changed
				show_prog3();											//	redraw it
			}
// 07/13/09 start
	 			act_program.shot[AVAILSHT] = INV_SHOT;					// AVAILSHT-1 is the max shot number allowed, so in case there is non-zero data in
	 			act_program.method[AVAILSHT] = 0;						// the next shot, force it to be invalid here.
// 07/13/09 end
// 02/07/09 start
            if (pp_odashes && !pp_dashes) {             				// If dashes flag has just been cleared due to keypad inactivity,
                show_prog3();                                           // then update lcd screen data,
                pp_odashes = 0;                                         // and clear flag that indicates dashes were displayed.
            }                                                           // 
// 02/07/09 end
			if ((cmd & 0x7f) == KY_MENUDN) {							// if changing item to edit
// 07/03/09 start
//				ppparm_ix += 2;											//	raise item to edit index
//				if (ppparm_ix > (FEED_IX+2))							//	limit by wrapping
//					ppparm_ix = SPEED_IX;
				if (ppparm_ix == SPEED_IX)
					ppparm_ix = SPIN_IX;
				else if (ppparm_ix == SPIN_IX)
					ppparm_ix = FEED_IX;
				else if (ppparm_ix == FEED_IX)
					ppparm_ix = FEED_IX + 2;
				else if (ppparm_ix == FEED_IX + 2)
					ppparm_ix = SPEED_IX;
// 07/03/09 end
				priorState = PP_CHANGE;									//	redraw screen
			} else if ((cmd & 0x7f) == KY_MENUUP) {		        		// if changing item to edit
// 07/03/09 start
//				if (ppparm_ix == SPEED_IX) 								//	limit by wrapping
//					ppparm_ix = FEED_IX+2;
//				else
//					ppparm_ix -= 2;
				if (ppparm_ix == SPEED_IX)
					ppparm_ix = FEED_IX+2;
				else if (ppparm_ix == SPIN_IX)
					ppparm_ix = SPEED_IX;
				else if (ppparm_ix == FEED_IX)
					ppparm_ix = SPIN_IX;
				else if (ppparm_ix == FEED_IX+2)
					ppparm_ix = FEED_IX;
// 07/03/09 end
				priorState = PP_CHANGE;									// redraw screen
			} else if ((cmd & 0x7f) == KY_PLUS) {					    // else if raising item value
				if (ppparm_ix == SPEED_IX) {							// if raising speed
					prog_chgd = 1;
					locno = act_program.shot[ppshot_ix];			    // get location for this shot
					method = act_program.method[ppshot_ix];		        // get method for this shot
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//						val = court_locs[locno-1][method].speed;	        // get current speed
//						val2 = court_locs[locno-1][method].spin;	        // get current spin
//					}
//					else {	// CLUB model only
//						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
//						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
//					}
#ifndef PHENOMS
						val = court_locs[locno-1][method].speed;	        // get current speed
						val2 = court_locs[locno-1][method].spin;	        // get current spin
#else
						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
#endif
// v1.219 end
					val += P_SPEEDINC;									// increment speed
					if (val > P_SPEEDMAX)								// and wrap around when
//						val = P_SPEEDMIN;								// exceed limit, wrapping or ... 02/07/09
						val = P_SPEEDMAX;								// exceed limit, no wrapping 02/07/09
					if (getLocAvail(locno-1,val,val2,&method)) {        // if table contains speed/spin
						act_program.method[ppshot_ix] = method;	        // change to that method
                        pp_dashes = 0;                                  // 02/07/09
                    }
                    else {                                              // Set flags to indicate that dashes should 02/07/09
                        pp_dashes = 1;                                  // be displayed, instead of speed value. 02/07/09
                        pp_odashes = 1;                                 // pp_dashes is cleared due to expiration of dashes_tmr. 02/07/09
                    }                                                   // 02/07/09
				} else if (ppparm_ix == SPIN_IX) {					    // else if raising spin
					prog_chgd = 1;
					locno = act_program.shot[ppshot_ix];			    // get location for this shot
					method = act_program.method[ppshot_ix];		        // get method for this shot
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//						val = court_locs[locno-1][method].speed;	        // get current speed
//						val2 = court_locs[locno-1][method].spin;        	// get current spin
//					}
//					else {	// CLUB model only
//						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
//						val2 = court_locs_CLUB[locno-1][method].spin;        	// get current spin
//					}
#ifndef PHENOMS
						val = court_locs[locno-1][method].speed;	        // get current speed
						val2 = court_locs[locno-1][method].spin;        	// get current spin
#else
						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
						val2 = court_locs_CLUB[locno-1][method].spin;        	// get current spin
#endif
// v1.219 end
					val2 += P_SPININC;									// increment spin
					if (val2 > P_SPINMAX)								// and wrap around when
//						val2 = P_SPINMIN;								// exceed limit, wrapping or ... 02/07/09
						val2 = P_SPINMAX;								// exceed limit, no wrapping 02/07/09
					if (getLocAvail(locno-1,val,val2,&method))	{		// if table contains speed/spin
						act_program.method[ppshot_ix] = method;			// change to that method
                        pp_dashes = 0;                                  // 02/07/09
                    }
                    else {                                              // Set flags to indicate that dashes should 02/07/09
                        pp_dashes = 2;                                  // be displayed, instead of spin value. 02/07/09
                        pp_odashes = 2;                                 // pp_dashes is cleared due to expiration of dashes_tmr. 02/07/09
                    }                                                   // 02/07/09
				} else if (ppparm_ix == FEED_IX) {						// else if raising feed rate
					prog_chgd = 1;
// v1.200 start
//					act_program.feed++;
//					if (act_program.feed > P_FEEDMAX)					// limit by wrapping
//						act_program.feed = P_FEEDMAX;					// no wrapping 02/07/09
					act_program.feed[ppshot_ix]++;
					if (act_program.feed[ppshot_ix] > P_FEEDMAX)		// limit by wrapping
						act_program.feed[ppshot_ix] = P_FEEDMAX;		// no wrapping 02/07/09
// v1.200 end
				} else if (ppparm_ix == (FEED_IX + 2)) {				// else if changing screen
					ppshot_ix++;										// also advance shot #
					if (ppshot_ix > (AVAILSHT-1))	{					// limiting it as well
						ppshot_ix = 0;									// next on shot 6 means that we
						curState = PP_SCRN1;							// are done, go to screen 1
					} else {											// else continue with next shot
						curState = PP_SCRN2;							// so change to screen 2
					}
				}
				priorState = PP_CHANGE;									// make sure to redraw screen
			} else if ((cmd & 0x7f) == KY_MINUS) {						// if lowering item value
				if (ppparm_ix == SPEED_IX) {							// if lowering speed
					prog_chgd = 1;										// v1.191.0R added to fix bug
					locno = act_program.shot[ppshot_ix];			    // get location for this shot
					method = act_program.method[ppshot_ix];		        // get method for this shot
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//						val = court_locs[locno-1][method].speed;	        // get current speed
//						val2 = court_locs[locno-1][method].spin;	        // get current spin
//					}
//					else {	// CLUB model only
//						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
//						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
//					}
#ifndef PHENOMS
						val = court_locs[locno-1][method].speed;	        // get current speed
						val2 = court_locs[locno-1][method].spin;	        // get current spin
#else
						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
#endif
// v1.219 end
					putrsDIAG("S");
					dumphex(val);
					putrsDIAG("s");
					dumphex(val2);
					val -= P_SPEEDINC;								    // decrement speed
					if (val < P_SPEEDMIN)								// and wrap around when
//						val = P_SPEEDMAX;								// exceed limit, wrapping or ... 02/07/09
						val = P_SPEEDMIN;								// exceed limit, no wrapping 02/07/09
					putrsDIAG("New");
					dumphex(val);
					if (getLocAvail(locno-1,val,val2,&method)) {        // if table contains speed/spin
						act_program.method[ppshot_ix] = method;	        // change to that method
                        pp_dashes = 0;                                  // 02/07/09
                    }
                    else {                                              // Set flags to indicate that dashes should 02/07/09
                        pp_dashes = 1;                                  // be displayed, instead of spin value. 02/07/09
                        pp_odashes = 1;                                 // pp_dashes is cleared due to expiration of dashes_tmr. 02/07/09
                    }                                                   // 02/07/09
					putrsDIAG("M");
					dumphex(method);
				} else if (ppparm_ix == SPIN_IX) {					    // else if lowering spin
					prog_chgd = 1;										// v1.191.0R added to fix bug
					locno = act_program.shot[ppshot_ix];			    // get location for this shot
					method = act_program.method[ppshot_ix];		        // get method for this shot
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//						val = court_locs[locno-1][method].speed;	        // get current speed
//						val2 = court_locs[locno-1][method].spin;	        // get current spin
//					}
//					else {	// CLUB model only
//						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
//						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
//					}
#ifndef PHENOMS
						val = court_locs[locno-1][method].speed;	        // get current speed
						val2 = court_locs[locno-1][method].spin;	        // get current spin
#else
						val = court_locs_CLUB[locno-1][method].speed;	        // get current speed
						val2 = court_locs_CLUB[locno-1][method].spin;	        // get current spin
#endif
// v1.219 end
					val2 -= P_SPININC;									// decrement spin
					if (val2 < P_SPINMIN)								// and wrap around when
//						val2 = P_SPINMAX;								// exceed limit, wrapping or ... 02/07/09
						val2 = P_SPINMIN;								// exceed limit, no wrapping 02/07/09
					if (getLocAvail(locno-1,val,val2,&method)) {        // if table contains speed/spin
						act_program.method[ppshot_ix] = method;	        // change to that method
                        pp_dashes = 0;                                  // 02/07/09
                    }
                    else {                                              // Set flags to indicate that dashes should 02/07/09
                        pp_dashes = 2;                                  // be displayed, instead of spin value. 02/07/09
                        pp_odashes = 2;                                 // pp_dashes is cleared due to expiration of dashes_tmr. 02/07/09
                    }                                                   // 02/07/09
				} else if (ppparm_ix == FEED_IX) {
// The following line was missing, causing program to not be saved with a feed rate decrement. 02/08/09
					prog_chgd = 1;                                      // 02/08/09
// v1.200 start
//					act_program.feed--;
//					if (act_program.feed < P_FEEDMIN+1)					// If random feed selected (P_FEEDMIN is now random feed),
//						act_program.feed = P_FEEDMIN+1;					// then don't allow. Program mode does not support random feed.
					act_program.feed[ppshot_ix]--;
					if (act_program.feed[ppshot_ix] < P_FEEDMIN+1)		// If random feed selected (P_FEEDMIN is now random feed),
						act_program.feed[ppshot_ix] = P_FEEDMIN+1;		// then don't allow. Program mode does not support random feed.
// v1.200 end
				} else if (ppparm_ix == (FEED_IX + 2)) {		        // else if changing screen
//					priorState = PP_CHANGE;
					curState = PP_SCRN1;
				}
				priorState = PP_CHANGE;
// 02/07/09 start
            }
//			} else if (pp_speed_dashes) {                               // We are here because no keys were pressed, so
//              pp_speed_dashes = 0;                                    // if dashes were being displayed, due to previous
//              chartoa(speed_trans[locp->speed / 5],&lcd_line[1][9],2);// keypress, replace them with previous data.               
//              putrsDIAG("\n\rzzzz");
//              show_prog3();
//          }
// 02/07/09 end
			break;
		default:
			putrsDIAG("INVPP state = ");
			dumphex(curState);
			break;
	}
	return(0);
}


// v1.219 start
void remote_programming (void) {

}
// v1.219 end
