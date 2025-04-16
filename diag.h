// Diagnostic Definitions
//
//	(C)2006,7 Censerve(R) Consulting, Inc.
//						All rights reserved.

// C18 to XC8 Compiler Changes
//void dumprsDIAG(unsigned long ix, const rom char* cp);
void dumprsDIAG(unsigned long ix, const char* cp);
// C18 to XC8 Compiler Changes
void dumpsDIAG(unsigned long ix,char* cp);
// C18 to XC8 Compiler Changes
//void putrsDIAG(const rom char *data);
void putrsDIAG(const char *data);
// C18 to XC8 Compiler Changes
void putsDIAG(char *data);
void dumphex(unsigned char c);
void dump2port(unsigned uc);
void dumphex2port(unsigned char c);
void dumpaddr(unsigned int addr);
void dumplong(unsigned long addr);
void dumpshort(unsigned long addr);
unsigned long atoh(char *cptr);
#define dumpwrd(val) dumpshort((unsigned long)val)

#ifdef DBGSTATES
#define SPUTRSDIAG(str)	putrsDIAG(str)
#define SDUMPHEX(x)			dumphex(x)
#define SDUMPLONG(x)			dumplong(x)
#define SDUMPSHORT(x)		dumpshort(x)
#define SDUMPADDR(x)			dumpaddr(x)
#else  // !DBGSTATES
#define SPUTRSDIAG(str)
#define SDUMPHEX(x)
#define SDUMPLONG(x)
#define SDUMPSHORT(x)
#define SDUMPADDR(x)
#endif //
