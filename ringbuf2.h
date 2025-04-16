//	Ring buffer Class
//
// v1.209 start
//#define RBUFSZ 32
#define RBUFSZ 96
// v1.209 end

// Ring Buffer 
//	provides general insert extract and flow control functionality
//
struct rbuf {
//	public:
//	rbuf();													// default/global/array constructor
//	~rbuf() { };											// destructor does nothing
//	void ins(char c);										// insert single character into buffer
//	void in_str(char *str);									// insert string into buffer
//	void in_bstr(unsigned char *str, unsigned long count);	// insert string with binary data into buffer
//	char extr();											// extract single character from buffer
//	short hiwat();											// returns true if buffer is close to full
//	short lowat();											// returns true if buffer is close to empty
//	short bct();											// returns number of bytes buffered
//	void setup();
//	private:
//	rbuf(rbuf &r);											// disallow copy constructor
	char dat[RBUFSZ];										// ring buffer
//	char term;												// termination character
//	char term_ct;											// number of termination characters in the buffer
	volatile short rix, wix;								// read and write indiced
	volatile short ct;										// count of bytes in buffer
	int chan;												// usart channel ( 1 or 2 )
};

void rbuf_init(struct rbuf * rbptr, int );					// constructor
void rbuf_ins(struct rbuf * rbptr, char c);					// insert single character into buffer
void rbuf_in_str(struct rbuf * rbptr, char *str);			// insert string into buffer
void rbuf_in_bstr(struct rbuf * rbptr, unsigned char *str, unsigned long count);// insert string with binary data into buffer
char rbuf_extr(struct rbuf * rbptr );						// extract single character from buffer
short rbuf_hiwat(struct rbuf * rbptr );						// returns true if buffer is close to full
short rbuf_lowat(struct rbuf * rbptr );						// returns true if buffer is close to empty
short rbuf_bct(struct rbuf * rbptr );						// returns number of bytes buffered
void rbuf_setup(struct rbuf * rbptr, int );
