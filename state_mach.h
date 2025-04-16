// State Machine Definitions
//

// EL04 Machine States
#define ST_SLEEP 	0
#define ST_IDLE		1
#define ST_MANMN	2
#define ST_PREMN	3
#define ST_PRGMN	4
#define ST_PLAY		5
#define ST_ABORT	6
#define ST_TEST		7
#define ST_MAX		8

struct state_mach {
	uchar *cur_state;  	// current state
	uchar *prev_state; 	// previous state
// C18 to XC8 Compiler Changes
//const rom uchar *pxtbl;   		// state transition table pointer
  const uchar *pxtbl;   		// state transition table pointer
// C18 to XC8 Compiler Changes
  const 	uchar  state_ct;    // count of states in state transition table
};

#ifdef notdef
struct state_mach_EL04 {	// for EL04 only
	uchar *cur_state;  	// current state
	uchar *prev_state; 	// previous state
  const rom uchar *pxtbl;   		// state transition table pointer
  const 	uchar  state_ct;    // count of states in state transition table
};
#endif

struct button_def {
  long style ;
  char *text ;
  int  id;
  int  x;
  int  y;
  int  w;
  int  h;
  int  flags;
};
