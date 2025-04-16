//	Ring buffer Routines
//

// C18 to XC8 Compiler Changes
//#include "types.h"
//#include <p18f6722.h>				// Register definitions
#include <xc.h>				// Register definitions
// C18 to XC8 Compiler Changes
#include "ringbuf2.h"
#include "string.h"
// C18 to XC8 Compiler Changes
//#include "usart.h"
// C18 to XC8 Compiler Changes

// v1.208 start

// C18 to XC8 Compiler Changes
// Replaced all of the #message directives with #warning directives.
// C18 to XC8 Compiler Changes

#if defined DEBUGPORT	// Not to be released. Serial port used for debug rather than WiFi/Bluetooth interface.
	#warning ***** COMPILING FOR DEBUG PORT *****
	#warning *
	#warning *****    DO NOT RELEASE        *****
	#warning *
	#warning ***** COMPILING FOR DEBUG PORT *****
	#warning *
	#warning *****    DO NOT RELEASE        *****
	#warning *
	#warning *
	#warning *
	#warning *
#endif
// v1.208 end

void rbuf_init(struct rbuf * rbptr, int channo )
{
	rbptr->rix=0;
	rbptr->wix=0;
	rbptr->ct=0;
	rbptr->chan = channo;
	
}

void rbuf_setup(struct rbuf * rbptr, int channo )
{
	rbptr->rix=0;
	rbptr->wix=0;
	rbptr->ct=0;
	rbptr->chan = channo;
}

// Insert single character into buffer
void rbuf_ins(struct rbuf * rbptr, char c)
{
//	unsigned short x = intoff();
	if (rbptr->ct < (RBUFSZ-1)) {		// if buffer not yet full
		rbptr->dat[rbptr->wix++] = c;	//	write character to buffer at write index
		rbptr->ct++;					//	update number of characters
		rbptr->wix &= RBUFSZ-1;			//	maintain circularity of buffer
		if (rbptr->chan == 1) 
			PIE1bits.TX1IE = 1;			// enable usart 1 transmit interrupt
		else if (rbptr->chan == 2)
			PIE3bits.TX2IE = 1;
	}
//	inton(x);
}

// Insert string into buffer
void rbuf_in_str(struct rbuf * rbptr, char *str) 
{
	char c;

//#pragma warn -pia
	while (c = *str++)					// while more characters in the string
//#pragma warn +pia
		rbuf_ins(rbptr, c);				//	insert each into the buffer
}

// Insert string into buffer
void rbuf_in_bstr(struct rbuf * rbptr, unsigned char *str, unsigned long count) 
{
	while (count--)						// while more characters in the string
//#pragma warn -pia
		rbuf_ins(rbptr, *str++);		//	insert each into the buffer
//#pragma warn +pia
}

// Extract single character from buffer
char rbuf_extr(struct rbuf * rbptr )
{
	char c = 0;							// empty buffer return code
//	unsigned short x = intoff();

	if (rbptr->ct) {					// if buffer not empty
		c = rbptr->dat[rbptr->rix++];	//	read character from buffer at read index
		rbptr->ct--;					//	update number of characters
		rbptr->rix &= RBUFSZ-1;
	}
//	inton(x);
	return(c);							// return results
}

// Detects when buffer is sufficiently full to disable filling
//	appropriate action is to be provided by caller (e.g. XOFF or deny RTS)
short rbuf_hiwat(struct rbuf * rbptr )
{
	return (rbptr->ct > ((RBUFSZ*3)/4)) ? 1 : 0;
}

// Detects when buffer is sufficiently empty to enable filling
//	appropriate action is to be provided by caller (e.g. XON or assert RTS)
short rbuf_lowat(struct rbuf * rbptr )
{
	return (rbptr->ct < (RBUFSZ/4)) ? 1 : 0;
}

// Reports number of bytes buffered
short rbuf_bct(struct rbuf * rbptr )
{
	return (rbptr->ct);
}
