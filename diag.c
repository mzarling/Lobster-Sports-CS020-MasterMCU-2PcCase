// Diagnostic Functions
//	(C)2006,7 Censerve(R) Consulting, Inc.
//						All rights reserved.
#include "types.h"
#include "diag.h"
// C18 to XC8 Compiler Changes
//#include <p18f6722.h>
#include <xc.h>
// C18 to XC8 Compiler Changes
#include "ringbuf2.h"
#include <string.h>

extern struct rbuf di_outbuf;

// C18 to XC8 Compiler Changes
//extern rom char crlf[];
extern const char crlf[];
// C18 to XC8 Compiler Changes

// C18 to XC8 Compiler Changes
//static const rom char s_digits[] = "0123456789abcdef";
const char s_digits[] = "0123456789abcdef";
// C18 to XC8 Compiler Changes

//** Note! These need to be buffered and done in interrupt service
//**       routines (or scheduled). Otherwise, the system is hung
//**       until all the characters are transmitted
//**       The same is true for spi communications


// C18 to XC8 Compiler Changes
//void putrsDIAG(const rom char *data)
void putrsDIAG(const char *data)
// C18 to XC8 Compiler Changes

{
	#ifdef	DEBUGPORT					// v1.196 added
	do
	{
		rbuf_ins(&di_outbuf, *data);
	} while (*data++);
	#endif								// v1.196 added
}

void putsDIAG(char *data)
{
	#ifdef	DEBUGPORT					// v1.196 added
	do
	{  // Transmit a byte
		rbuf_ins(&di_outbuf, *data);
	} while( *data++ );
	#endif								// v1.196 added
}

void dumphex(unsigned char c)
{
	#ifdef	DEBUGPORT					// v1.196 added
	unsigned char uc;
	uc = s_digits[(c>>4)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[c & 0x0f];
	rbuf_ins(&di_outbuf, uc);
	rbuf_ins(&di_outbuf, ' ');
	#endif								// v1.196 added
}

void dump2port(unsigned uc)
{
	while (!PIR3bits.TX2IF);
	TXREG2 = uc;
}

void dumphex2port(unsigned char c)
{
	unsigned char uc;

	uc = s_digits[(c>>4)&0x0f];
	dump2port(uc);
	uc = s_digits[c & 0x0f];
	dump2port(uc);
	dump2port(',');
}

void dumpaddr(unsigned int addr)
{
	#ifdef	DEBUGPORT					// v1.196 added
	unsigned char uc;

	putrsDIAG(crlf);
	uc = s_digits[(addr>>12)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>8)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>4)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[addr&0x0f];
	rbuf_ins(&di_outbuf, uc);
// C18 to XC8 Compiler Changes
//	putrsDIAG((rom char *)": ");
	putrsDIAG((const char *)": ");
// C18 to XC8 Compiler Changes
	#endif								// v1.196 added
}

void dumplong(unsigned long addr)
{
	#ifdef	DEBUGPORT					// v1.196 added
	unsigned char uc;

	uc = s_digits[(addr>>28)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>24)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>20)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>16)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>12)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>8)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[(addr>>4)&0x0f];
	rbuf_ins(&di_outbuf, uc);
	uc = s_digits[addr&0x0f];
	rbuf_ins(&di_outbuf, uc);
// C18 to XC8 Compiler Changes
//	putrsDIAG((rom char *)",");
	putrsDIAG((const char *)",");
// C18 to XC8 Compiler Changes
	#endif								// v1.196 added
}

void dumpshort(unsigned long addr)
{
	#ifdef	DEBUGPORT					// v1.196 added
	unsigned char uc;

	if (addr & 0xf000) {
		uc = s_digits[(addr>>12)&0x0f];
		rbuf_ins(&di_outbuf, uc);
	}
	if (addr & 0xff00) {
		uc = s_digits[(addr>>8)&0x0f];
		rbuf_ins(&di_outbuf, uc);
	}
	if (addr & 0xfff0) {
		uc = s_digits[(addr>>4)&0x0f];
		rbuf_ins(&di_outbuf, uc);
	}
	uc = s_digits[addr&0x0f];
	rbuf_ins(&di_outbuf, uc);
// C18 to XC8 Compiler Changes
//	putrsDIAG((rom char *)",");
	putrsDIAG((const char *)",");
// C18 to XC8 Compiler Changes
	#endif								// v1.196 added
}

unsigned long atoh(char *cptr)
{
	unsigned long rc = 0;
	int i,j;
	char c;
	char sign = 0;

	if (*cptr == (char)'-') {
		sign = 1;
		++cptr;
	}

	if ((i=strlen(cptr)) > 8) return -1;
	for (j=0; j<i; j++) {
		c = cptr[j];
		rc = ((c<(char)'A')?(c-(char)'0'):((c-(char)'A')+10)) | (rc << 4);
	}
	if (sign) rc = 0 - rc;
	return rc;
}

// C18 to XC8 Compiler Changes
//void dumprsDIAG(ulong ix, const rom char* cp)
void dumprsDIAG(ulong ix, const char* cp)
// C18 to XC8 Compiler Changes

{
	#ifdef	DEBUGPORT					// v1.196 added
	int cnt;

// C18 to XC8 Compiler Changes
//	putrsDIAG((const rom char *)crlf);
	putrsDIAG((const char *)crlf);
// C18 to XC8 Compiler Changes
	for (cnt=0; cnt<0x40; cnt++,ix++) {
		if ((cnt & 0x0f) == 0)
			dumpaddr(ix);
		else if ((cnt & 3) == 0) {
			rbuf_ins(&di_outbuf, ' ');
		}
		dumphex(cp[cnt]);
	}
// C18 to XC8 Compiler Changes
//	putrsDIAG((const rom char *)crlf);
	putrsDIAG((const char *)crlf);
// C18 to XC8 Compiler Changes
	#endif								// v1.196 added
}

void dumpsDIAG(ulong ix, char* cp)
{
	#ifdef	DEBUGPORT					// v1.196 added
	int cnt;

// C18 to XC8 Compiler Changes
//	putrsDIAG((const rom char *)crlf);
	putrsDIAG((const char *)crlf);
// C18 to XC8 Compiler Changes
	for (cnt=0; cnt<0x40; cnt++,ix++) {
		if ((cnt & 0x0f) == 0)
			dumpaddr(ix);
		else if ((cnt & 3) == 0) {
			rbuf_ins(&di_outbuf, ' ');
		}
		dumphex(cp[cnt]);
	}
// C18 to XC8 Compiler Changes
//	putrsDIAG((const rom char *)crlf);
	putrsDIAG((const char *)crlf);
// C18 to XC8 Compiler Changes
	#endif								// v1.196 added
}
