// State Machine Code
//
#include "types.h"
#include "state_mach.h"
//#include <usart.h>
#include "lob.h"
#include "diag.h"

// C18 to XC8 Compiler Changes
//extern const rom struct state_mach lobmach;
//extern const rom struct state_mach lobmach_EL04;
//extern const rom struct state_mach lobmach_EL05LE2;	// v1.186
//extern const rom uchar xmsk[];
extern const struct state_mach lobmach;
extern const struct state_mach lobmach_EL04;
extern const struct state_mach lobmach_EL05LE2;	// v1.186
extern const uchar xmsk[];
// C18 to XC8 Compiler Changes

// State Machine Service
//  called from key detection code
// 
void sm_service(ushort key)
{
	uchar state		= *lobmach.cur_state;										// and its current state

	if (state > lobmach.state_ct)	{												//	if state is out of limits
	} else if (key < (ushort)LOBNUMBUT) {									//	else if button ok
		SPUTRSDIAG("[ky");
		SDUMPHEX(key);
		SPUTRSDIAG("ost");
		SDUMPHEX(state);
		*lobmach.cur_state = (ushort)((ushort)lobmach.pxtbl[state + 
																	(ushort)key * (ushort)lobmach.state_ct]);//	 determine new state
		*lobmach.prev_state = state;												//	 save new state
		SPUTRSDIAG("new");
		SDUMPHEX(*lobmach.cur_state);
	} else {
		*lobmach.cur_state = key >> 8;
	}
	SPUTRSDIAG("]\r\n");
}

void sm_service_EL04(ushort key)
{
	uchar state		= *lobmach_EL04.cur_state;										// and its current state

	if (state > lobmach_EL04.state_ct)	{												//	if state is out of limits
	} else if (key < (ushort)LOBNUMBUT) {									//	else if button ok
		SPUTRSDIAG("[ky");
		SDUMPHEX(key);
		SPUTRSDIAG("ost");
		SDUMPHEX(state);
		*lobmach_EL04.cur_state = (ushort)((ushort)lobmach_EL04.pxtbl[state + 
																	(ushort)key * (ushort)lobmach_EL04.state_ct]);//	 determine new state
		*lobmach_EL04.prev_state = state;												//	 save new state
		SPUTRSDIAG("new");
		SDUMPHEX(*lobmach_EL04.cur_state);
	} else {
		*lobmach_EL04.cur_state = key >> 8;
	}
	SPUTRSDIAG("]\r\n");
}

// v1.186 start
void sm_service_EL05LE2(ushort key)
{
	uchar state		= *lobmach_EL05LE2.cur_state;										// and its current state

	if (state > lobmach_EL05LE2.state_ct)	{												//	if state is out of limits
	} else if (key < (ushort)LOBNUMBUT) {									//	else if button ok
		SPUTRSDIAG("[ky");
		SDUMPHEX(key);
		SPUTRSDIAG("ost");
		SDUMPHEX(state);
		*lobmach_EL05LE2.cur_state = (ushort)((ushort)lobmach_EL05LE2.pxtbl[state + 
																	(ushort)key * (ushort)lobmach_EL05LE2.state_ct]);//	 determine new state
		*lobmach_EL05LE2.prev_state = state;												//	 save new state
		SPUTRSDIAG("new");
		SDUMPHEX(*lobmach_EL05LE2.cur_state);
	} else {
		*lobmach_EL05LE2.cur_state = key >> 8;
	}
	SPUTRSDIAG("]\r\n");
}
// v1.186 end
