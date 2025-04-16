// Lobster EEPROM Access Routines
//
//	Hi Level routines presume the layout defined for the EL04
//
#include "types.h"
#include "lob.h"
//#include "usart.h"
// C18 to XC8 Compiler Changes
//#include "c:\mcc18\h\spi.h"
// C18 to XC8 Compiler Changes
#include "ringbuf.h"
#include "diag.h"
// C18 to XC8 Compiler Changes
//#include "p18f6722.h"
#include <xc.h>
#include <delays.h>
// C18 to XC8 Compiler Changes

#ifdef DANWIN
#undef ClrWdt
void ClrWdt(void);
#endif
void ChipSelect(int dev);
void ChipDeSelect(int dev);
void DelayXLCD(void);
char *FlashRead(unsigned long addr, char *buf, int count);
void FlashWrite( char *buf, unsigned long addr);
void FlashErase(unsigned long addr);

extern char prog_chgd;
// C18 to XC8 Compiler Changes start
//extern char FlashBuf[];
extern char FlashBuf[64];
// C18 to XC8 Compiler Changes end

// C18 to XC8 Compiler Changes
//extern far rom ushort base_speeds [][2];
//extern far rom struct ee_hdr flash_header;
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
extern const ushort base_speeds [][2];
extern const struct ee_hdr flash_header;
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];
// C18 to XC8 Compiler Changes

// v1.219 start
extern char RemoteString[68];
// v1.219 end


char ee_buf64[64];
unsigned long ee_progstart=0;
// 07/29/09 start
extern unsigned char pp_maxloc;
// 07/29/09 end
extern unsigned char model_type;

void eeprom2flash(int code, long* args)
{
	unsigned char c;
	unsigned char data;
	long ix, ixb;
	long dix;

// C18 to XC8 Compiler Changes
//	far rom char* fhp = (far rom char*)&flash_header;
	const char* fhp = (const char*)&flash_header;
// C18 to XC8 Compiler Changes
	
	switch (code) {
	case 0:											// COPY EEPROM TO FLASH
		for (dix=0; dix<args[0]; dix+=64) {
			DisableSlaveSPI();												//	keep slave from interfering		
			for (ix=0; ix<64; ix++) {
				ee_buf64[ix] = SEERead(dix+ix);
			}
			FlashErase((ulong)0x18000 + dix);
			FlashWrite(ee_buf64,(ulong)0x18000 + dix);
			EnableSlaveSPI();												//	keep slave from interfering		
		}
		break;
	case 1:											// COPY FLASH TO EEPROM
		for (dix=0; dix<args[0]; dix+=64) {
			FlashRead((ulong)&fhp[dix],ee_buf64,64);
			DisableSlaveSPI();											//	keep slave from interfering		
			for (ix=0; ix<64; ix++) {
				SEEWrite(dix+ix,ee_buf64[ix]);
			}
			EnableSlaveSPI();											//	keep slave from interfering		
		}
		break;
	case 2:											// ERASE EEPROM
		ix = args[0];
		c = args[2];
		DisableSlaveSPI();												//	keep slave from interfering		
//		INTCONbits.GIE = 0;
		dix = args[1] + args[0];
		do {
			for (  ; ix<dix; ix++) {
				SEEWrite(ix,c);
			}
		} while (ix < (args[0] + args[1]));
//		INTCONbits.GIE = 1;
		EnableSlaveSPI();												//	keep slave from interfering		
		ClrWdt();
		break;
	case 3:
		c = (char)args[2];
		ix = args[0];
		putrsDIAG("\n\rVerify EEPROM ");
		DisableSlaveSPI();
		for (   ; ix < (args[1] + args[0]); ix++) {
			data = SEERead(ix);
//			if (data != c) {
//				dumpaddr(ix);
//				dumphex(data);
//				putrsDIAG("S/B ");
//				dumphex(c);
//			}
		}
		DisableSlaveSPI();
		break;
	case 4:									// Compare EEProm and FLASH
		ix = args[0];
		ix &= ~(0x3f);
		ixb = 0;
		putrsDIAG("\n\rCompare EEPROM to FLASH");
		DisableSlaveSPI();											//	keep slave from interfering		
		while (ix < 32767) 
		for (ixb=0; ixb<64; ixb++,ix++) {
			ee_buf64[ix&0x3f] = SEERead(ix);
			if (ee_buf64[ix&0x3f] != fhp[ix]) {
				dumpaddr(ix);
				dumphex(ee_buf64[ix&0x3f]);
				putrsDIAG("s/b ");
				dumphex(fhp[ix]);
			}
		}
		EnableSlaveSPI();												//	keep slave from interfering		
		break;
	case 5:									// Read EEPROM byte
		ix = args[0];					//	get offset within EEPROM to read
		DisableSlaveSPI();
		ixb = SEERead(ix);		//	read the byte
		EnableSlaveSPI();
		putrsDIAG("\nRead: ");
		dumphex(ixb&0xff);
		break;
	case 6:									// Write EEPROM byte
		ix = args[0];					//	get offset within EEPROM to write
		ixb = args[1];				//	get data to write
		DisableSlaveSPI();
		SEEWrite(ix,ixb&0xff);//	write the byte
		EnableSlaveSPI();
		break;
	}
}

uchar loaded_progno = AVAILPRG; // data is not valid

// Loads the requested personal program from the EEPROM after saving any pre-existing
//	valid program in ram
//		progno - program number (base 1)
//		ppers	 - pointer to a personal program structure
//	returns:
//		0 = success
//		non-0 = error code
//
// C18 to XC8 Compiler Changes Start
//char load_prog(char progno, struct persprog* ppers)
char load_prog(signed char progno, struct persprog* ppers)
// C18 to XC8 Compiler Changes End
{
	char i, spd;
	signed char spn;
	int eeaddr;
	int ppErr = 0;
	
//	putrsDIAG("Load");	putrsDIAG("PROG#");	dumphex(progno);
	if ((progno < 0) || (progno >= AVAILPRG )) {	// if invalid prog #
//		putrsDIAG("BAD PROG #  \n\r");
		return (1);											// report an error
	} else {												// else (prog # is valid)
		save_prog(loaded_progno,ppers);						// save currently bufferred program
		eeaddr = progno * sizeof(struct persprog);			// determine address within EEPROM
		eeaddr += ee_progstart;								// offset into EEPROM for these tables
//		putrsDIAG("at");
//		dumplong(eeaddr);
		DisableSlaveSPI();									// keep slave from interfering		
		for (i=0; i<NUMSHOT; i++) {							// load shot locations first
			ppers->shot[i] = SEERead(eeaddr++);				// get location for each shot
			if (i <= AVAILSHT) {							// if shot is not filler
				if ((ppers->shot[i] < INV_SHOT) ||			// if location is invalid
// 07/29/09 start
//						(ppers->shot[i] > PP_MAXLOC)) {
						(ppers->shot[i] > pp_maxloc)) {
// 07/29/09 end
							ppErr = 1;									// flag to clear program
							putrsDIAG("\n\rInvalid Location");
				}
			}
		}
		for (i=0; i<NUMSHOT; i++) {										// load shot methods next
			ppers->method[i] = SEERead(eeaddr++);						// get method for each location
			if (i <= AVAILSHT) {										// if shot is not filler
				if (ppers->method[i] < 0) {								// if method is invalid
					ppErr = 1;
					putrsDIAG("\n\rInvalid Method");
				} else {												// else if method values are invalid
// v1.219 start
//					if (model_type != CLUB1 && model_type != CLUB2){	// all models except CLUBs
// C18 to XC8 Compiler Changes Start
////					spd = court_locs[ppers->shot[i]][ppers->method[i]].speed;
////					spn = court_locs[ppers->shot[i]][ppers->method[i]].spin;
//						spd = court_locs[(ppers->shot[i])-1][ppers->method[i]].speed;	// Shot lookup tables start with 0 for Location 1.
//						spn = court_locs[(ppers->shot[i])-1][ppers->method[i]].spin;	// Shot lookup tables start with 0 for Location 1.
// C18 to XC8 Compiler Changes End
//					}
//					else {	// CLUB model only
// C18 to XC8 Compiler Changes Start
////					spd = court_locs_CLUB[ppers->shot[i]][ppers->method[i]].speed;
////					spn = court_locs_CLUB[ppers->shot[i]][ppers->method[i]].spin;
//						spd = court_locs_CLUB[(ppers->shot[i])-1][ppers->method[i]].speed;
//						spn = court_locs_CLUB[(ppers->shot[i])-1][ppers->method[i]].spin;
// C18 to XC8 Compiler Changes End
//					}
#ifndef PHENOMS
// C18 to XC8 Compiler Changes Start
// ** The following has been a bug for a long time, but it didn't surface until the compiler was changed from C18 to XC8. The shot lookup tables start with 0
// ** representing Location 1, so the location should have been reduced by 1 before looking up in the tables. This did not show as a problem in C18 because
// ** valid data existed where the lookup was incorrectly looking. With the XC8 compiler, location 18 could not be saved in a program, because the table that
// ** followed the shot location tables (Grinder Drill), was no longer compiled and located right after the shot location tables. In the C18 compiler, location
// ** 18 was allowed to be saved because with the preset drills compiled and located right after the shot tables, there was still valid data when looking beyond
// ** the 18th location, when 18 base 0 really caused a lookup to location 19.
//						spd = court_locs[ppers->shot[i]][ppers->method[i]].speed;
//						spn = court_locs[ppers->shot[i]][ppers->method[i]].spin;
					if (ppers->shot[i] != INV_SHOT) {
						spd = court_locs[(ppers->shot[i])-1][ppers->method[i]].speed;	// Shot lookup tables start with 0 for Location 1.
						spn = court_locs[(ppers->shot[i])-1][ppers->method[i]].spin;	// Shot lookup tables start with 0 for Location 1.
// C18 to XC8 Compiler Changes End
#else
// C18 to XC8 Compiler Changes Start
//						spd = court_locs_CLUB[ppers->shot[i]][ppers->method[i]].speed;
//						spn = court_locs_CLUB[ppers->shot[i]][ppers->method[i]].spin;
					if (ppers->shot[i] != INV_SHOT) {
						spd = court_locs_CLUB[(ppers->shot[i])-1][ppers->method[i]].speed;	// Shot lookup tables start with 0 for Location 1.
						spn = court_locs_CLUB[(ppers->shot[i])-1][ppers->method[i]].spin;	// Shot lookup tables start with 0 for Location 1.
// C18 to XC8 Compiler Changes End
#endif
// v1.219 end
						if ((spd < P_SPEEDMIN) || (spd > P_SPEEDMAX) || ((spd % 5) != 0)) {
							ppErr = 1;												//		 flag to clear personal program
							putrsDIAG("\n\rInvalid Speed");
						}
						if ((spn < P_SPINMIN) || (spn > P_SPINMAX)) {
							ppErr = 1;
							putrsDIAG("\n\rInvalid Spin");
						}
					}
				}
			}
		}
// v1.200 start
//		ppers->feed = SEERead(eeaddr++);										// load single feed rate for program
//		if (ppErr || (ppers->feed < P_FEEDMIN) || (ppers->feed > P_FEEDMAX)) {
//			putrsDIAG("\n\rClearing program\n\r");
//			for (i=0; i<=AVAILSHT; i++) {										// any invalid value causes
//				ppers->shot[i] = INV_SHOT;										// all usable shots to be
//				ppers->method[i] = 0;											// cleared
//			}
//			ppers->feed = DEF_FEED;												// now right before they are used
//		}
		for (i=0; i<NUMSHOT; i++) {												// load shot methods next
			ppers->feed[i] = SEERead(eeaddr++);									// get feed for each location
//			putrsDIAG("\n\rppers->feed[i]=");
//			dumplong(ppers->feed[i]);
			if (i <= AVAILSHT) {													//	 if shot is not filler
				if ((ppers->feed[i] < P_FEEDMIN) || (ppers->feed[i] > P_FEEDMAX)) {	// if feed is invalid
					ppErr = 1;
					putrsDIAG("\n\rInvalid Feed");
				}
			}
		}
		if (ppErr) {
			putrsDIAG("\n\rClearing program\n\r");
			for (i=0; i<=AVAILSHT; i++) {										// any invalid value causes
				ppers->shot[i] = INV_SHOT;										// all usable shots to be
				ppers->method[i] = 0;											// cleared
				ppers->feed[i] = DEF_FEED;										// now right before they are used
			}
		}
// Everywhere (i=0; i<=AVAILSHT; i++) is used, the loop is run 1 more time than necessary, because AVAILSHT is base 1 and i starts
// at 0 in the loop. If AVAILSHT is 6 then there are 6 possible shots, 1 thru 6, not 0 thru 6. Currently this is not a problem 
// because there is room allocated in EEPROM for 30 shots (NUMSHOT) and AVAILSHT is set to 6. If AVAILSHT were to equal NUMSHOT, 
// then the extra run through the loop may cross a boundary for other data in EEPROM.
// v1.200 end

		EnableSlaveSPI();														// resume normal slave operation
		loaded_progno = progno;													// loaded_prog becomes incoming request
	}	
//	for (i=0; i<AVAILSHT; i++)
//		dumphex(ppers->shot[i]);
//	putrsDIAG("good\n\r");
	return (0);
}

// Saves the requested personal program from the EEPROM
//		progno - program number (base 1)
//		ppers	 - pointer to a personal program structure
//	returns:
//		0 = success
//		non-0 = error code
//
// C18 to XC8 Compiler Changes Start
//char save_prog(char progno, struct persprog* ppers)
char save_prog(signed char progno, struct persprog* ppers)
// C18 to XC8 Compiler Changes End
{
	char i;
	int eeaddr;
	
//	putrsDIAG("Save");
//	putrsDIAG("PROG#");
//	dumphex(progno);
	if ((progno < 0) || (progno >= AVAILPRG )) {	// if invalid prog #
//		putrsDIAG("BAD PROG #  \n\r");
		return (1);																//	report an error
//	} else {//if (loaded_pp,rogno != progno) {	// else if program # changed
	} else if (prog_chgd != 0) {						// else if program data changed
		prog_chgd = 0;									// reset changed flag
//		putrsDIAG("prev#");
//		dumphex(loaded_progno);
//		for (i=0; i<AVAILSHT; i++)
//			dumphex(ppers->shot[i]);
		eeaddr = progno * sizeof(struct persprog);		// determine address within EEPROM
		eeaddr += ee_progstart;							// offset into EEPROM for these tables
//		putrsDIAG("@");
//		dumplong(eeaddr);
		DisableSlaveSPI();								// keep slave from interfering		
		for (i=0; i<NUMSHOT; i++) {						// load shot locations first
			SEEWrite(eeaddr++,ppers->shot[i]);			// get location for each shot
		}
		for (i=0; i<NUMSHOT; i++) {						// load shot methods next
			SEEWrite(eeaddr++,ppers->method[i]);		// get method for each location
		}
// v1.200 start
//		SEEWrite(eeaddr++,ppers->feed);					// load single feed rate for program
		for (i=0; i<NUMSHOT; i++) {						// load shot locations first
			SEEWrite(eeaddr++,ppers->feed[i]);			// load feed rate for each shot
		}
// v1.200 end
		EnableSlaveSPI();								// resume normal slave operation
		loaded_progno = progno;							// invalidate currently loaded data
//	}	else {
//		putrsDIAG("same program ");
//		dumplong(loaded_progno);
	}		
//	putrsDIAG("good\n\r");
	return (0);
}

// Compares court tables, etc. in flash with those in eeprom
//	returns 0 if same, non-0 if different
//
char eeprom_changed()
{
	unsigned long i;
	int eeaddr = 0;
	
	FlashRead((ulong)base_speeds,FlashBuf,sizeof(FlashBuf));
	for (i=0; i<sizeof(FlashBuf); i++) {
		SEEWrite(eeaddr++,FlashBuf[i]);
	}
	return 0;
}
