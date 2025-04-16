// C18 to XC8 Compiler Changes Start
// Replaced all strcpypgm2ram with strcpy.
// Replaced all strcatpgm2ram with strcat.
// C18 to XC8 Compiler Changes End

#include "types.h"
// C18 to XC8 Compiler Changes Start
//#include <p18f6722.h>				// Register definitions
#include <xc.h>						// Register definitions
// C18 to XC8 Compiler Changes End
#include <stdlib.h>
#include <string.h>
#include <adc.h>					// ADC library functions
// C18 to XC8 Compiler Changes Start
//#include <spi.h>					// SPI library functions
// C18 to XC8 Compiler Changes End
#include <pwm.h>					// PWM library functions
#include <portb.h>					// PORTB library function
// 07/03/09 start
//#include <timers.h>				// Timer library functions
// C18 to XC8 Compiler Changes Start
//#include "timers.h"					// Timer library functions
// C18 to XC8 Compiler Changes End
// 07/03/09 end
#include <capture.h>
// C18 to XC8 Compiler Changes Start
//#include <usart.h>
// C18 to XC8 Compiler Changes End
#include <ctype.h>
#include "ringbuf2.h"
// C18 to XC8 Compiler Changes Start
//#include "lcd.h"
// C18 to XC8 Compiler Changes End
#include <delays.h>
#include "lob.h"
#include "slave.h"
#include "state_mach.h"
#include "diag.h"
#include "a2d.h"
// v1.219 start
#include "lobprog.h"
// v1.219 end

//#define FEEDBACK	1

// MASTER uC PINOUT (PIC18F6722)
// 
// port   I/O function
// ====   ============
//
// RA0    ANALOG INPUT FROM PGA						- AN0
// RA1    LCD_RS
// RA2    LCD_R/W
// RA3    LCD_E
// RA4    OUTPUT TO ENABLE +12V (ACTIVE HIGH)		- RA4 DIG
// RA5    OUTPUT TO ENABLE +5V (ACTIVE HIGH)		- RA5 DIG
// RA6    OSC (20MHZ HS MODE)						- OSC2
// RA7    OSC (20MHZ HS MODE)						- OSC1
// 
// RB0    POWER SWITCH INTERRUPT INPUT (ACTIVE LOW)	- INT0
// RB1    BALL DETECT INTERRUPT INPUT (ACTIVE LOW)	- INT1
// RB2    DIPSWITCH 1 INPUT							- RB2
// RB3    DIPSWITCH 2 INPUT							- RB3
// RB4    DIPSWITCH 3 INPUT							- RB4
// RB5    DIPSWITCH 4 INPUT							- RB5
// RB6    IN-CIRCUIT PROGRAM CLOCK					- PGC
// RB7    IN-CIRCUIT PROGRAM DATA					- PGD
// 
// RC0    SLAVE uC CHIP SELECT OUTPUT (ACTIVE LOW)	- RC0
// RC1    BOTTOM SERVE MOTOR PWM					- ECCP2 PWM output
// RC2    TOP SERVE MOTOR PWM						- ECCP1 PWM output
// RC3    SPI CLOCK (SCK)							- SCK1 (MSSP1)
// RC4    SPI DATA IN (SDI)							- SDI1 (MSSP1)
// RC5    SPI DATA OUT (SDO)						- SDO1 (MSSP1)
// RC6    UART TXD TO REMOTE CONTROL				- TX1 (EUSART1)
// RC7    UART RXD FROM REMOTE CONTROL				- RX1 (EUSART1)
// 
// RD0    SWITCH MATRIX INPUT ROW 0	
// RD1    SWITCH MATRIX INPUT ROW 1
// RD2    SWITCH MATRIX INPUT ROW 2
// RD3    SWITCH MATRIX INPUT ROW 3
// RD4    SWITCH MATRIX INPUT ROW 4
// RD5    SWITCH MATRIX INPUT ROW 5
// RD6    EEPROM CHIP SELECT OUTPUT (ACTIVE LOW)	- RD6
// RD7    PGA CHIP SELECT OUTPUT (ACTIVE LOW)		- RD7
// 
// RE0    SWITCH MATRIX OUTPUT COLUMN 0
// RE1    SWITCH MATRIX OUTPUT COLUMN 1
// RE2    SWITCH MATRIX OUTPUT COLUMN 2
// RE3    SWITCH MATRIX OUTPUT COLUMN 3
// RE4    SWITCH MATRIX OUTPUT COLUMN 4
// RE5    SWITCH MATRIX OUTPUT COLUMN 5
// RE6    SWITCH MATRIX OUTPUT COLUMN 6
// RE7    SWITCH MATRIX OUTPUT COLUMN 7
// 
// RF0    LCD DATA BIT 0
// RF1    LCD DATA BIT 1
// RF2    LCD DATA BIT 2
// RF3    LCD DATA BIT 3
// RF4    LCD DATA BIT 4
// RF5    LCD DATA BIT 5
// RF6    LCD DATA BIT 6
// RF7    LCD DATA BIT 7
// 
// RG0    FEED MOTOR PWM							- ECCP3 PWM output
// RG1    DEBUG UART TXD							- TX2 (EUSART2)
// RG2    DEBUG UART RXD							- RX2 (EUSART2)
// RG3    TOP SERVE MOTOR TACH INPUT				- CCP4 capture STOPPINGinput
// RG4    BOTTOM SERVE MOTOR TACH INPUT				- CCP5 capture input
// RG5    VPP/EXTERNAL RESET						- NMCLR

#pragma config OSC=HSPLL
#pragma config BOREN=SBORDIS,BORV=2
//#pragma config WDT=OFF,WDTPS=32768			Cleaned up Power On/Off code and Power button detection code.
#pragma config WDT=OFF,WDTPS=2048					// Cleaned up Power On/Off code and Power button detection code.
#pragma config MCLRE=OFF
#pragma config CCP2MX=PORTC
#pragma config STVREN=ON
#pragma config LVP=OFF
// v1.197 start
#pragma config CP0=ON
#pragma config CP1=ON
#pragma config CP2=ON
#pragma config CP3=ON
#pragma config CP4=ON
#pragma config CP5=ON
#pragma config CP6=ON
#pragma config CP7=ON
#pragma config CPB=ON
// v1.197 end

// 07/30/09 start
//#pragma udata
// 07/30/09 end
// v1.201 start
extern unsigned long ee_progstart;
// v1.201 end
// v1.188 start
extern signed char ppparm_ix;					// personal program parameter currently under consideration
extern char curState;
// v1.188 end
// C18 to XC8 Compiler Changes Start
//extern const rom char line_start[];
extern const char line_start[];
// C18 to XC8 Compiler Changes End
extern		 signed short eladj;
//extern		char preset_type;				// sub mode for presets (e.g. random horiz, etc)
extern 		char man_parm;
extern		char preset_ix[NUMPRESETS];			// parameter index for each preset submode
extern		union parm manual;
extern		union parm preset[NUMPRESETS];		// where we store the values for each of the preset parameters
// C18 to XC8 Compiler Changes Start
//extern far rom struct el04_loc home_loc;		// all models except CLUB
//extern far rom struct el04_loc home_loc_CLUB;	// CLUB model only
//extern far rom ushort base_speeds[][2];
extern const struct el04_loc home_loc;		// all models except CLUB
extern const struct el04_loc home_loc_CLUB;	// CLUB model only
extern const ushort base_speeds[][2];
// C18 to XC8 Compiler Changes End
extern long play_mode;
// C18 to XC8 Compiler Changes Start
//extern char preset_type;
extern unsigned char preset_type;
// C18 to XC8 Compiler Changes End
extern char remote_enabled;
// v1.208 start
//extern const rom ushort preset_mask[];
// C18 to XC8 Compiler Changes Start
//extern const rom unsigned short long preset_mask[];
extern const unsigned short long preset_mask[];
// C18 to XC8 Compiler Changes End
// v1.208 end
// C18 to XC8 Compiler Changes Start
//extern far rom struct ee_hdr flash_header;
extern const struct ee_hdr flash_header;
// C18 to XC8 Compiler Changes End
//extern rom char* lob_model;
//extern rom char* lob_version;
//extern rom char* homingmsg;
//extern rom char* battmsg;
//extern rom char* shutdnmsg;
//extern rom char* waitmsg;
//extern char lcd_updated;
//extern char homeshown;
//extern union capstatus CapStatus;
//extern ram char lcd_line[4][17];
//extern rom unsigned short base_feeds[16];
//extern rom unsigned short base_feeds_cont[16];
//extern ram int batval;
//extern ram int lastbatval;
//extern ram ulong lastbata2d;
//extern ram ulong LastBatAvg;
extern const char* lob_model;
extern const char* lob_version;
extern const char* homingmsg;
extern const char* battmsg;
extern const char* shutdnmsg;
extern const char* waitmsg;
extern char	 lcd_updated;
extern char	homeshown;
extern union capstatus CapStatus;
// C18 to XC8 Compiler Changes Start
//extern char	 lcd_line[4][17];
extern unsigned char lcd_line[4][17];
// C18 to XC8 Compiler Changes End
extern const unsigned short base_feeds[16];
extern const unsigned short base_feeds_cont[16];
extern int batval;
extern int lastbatval;
extern ulong lastbata2d;
extern ulong LastBatAvg;
///////////////////
// 02/07/09 start
extern char dashes;
extern char pp_dashes;
// 02/07/09 end


// 11/04/09 start
// C18 to XC8 Compiler Changes Start
//extern rom char* lowbatt_msg;
extern const char* lowbatt_msg;
// C18 to XC8 Compiler Changes End
// 11/04/09 end

// v1.192.0U start
extern signed short elev_test_adjust;	// ifdef elev_test, this gets added to shots in Program mode
// v1.192.0U end

// v1.209 start
// C18 to XC8 Compiler Changes Start
//extern const rom char speed_trans[];
extern const char speed_trans[];
// C18 to XC8 Compiler Changes End
extern char lob_test(char cmd);
// v1.209 end

// v1.219 start
// C18 to XC8 Compiler Changes Start
//extern char load_prog(char progno, struct persprog* ppers);
extern char load_prog(signed char progno, struct persprog* ppers);
// C18 to XC8 Compiler Changes End

//extern struct persprog tmp_program;
// C18 to XC8 Compiler Changes Start
//extern far rom struct el04_loc court_locs[][NUMMETH];
//extern far rom struct el04_loc court_locs_CLUB[][NUMMETH];
extern const struct el04_loc court_locs[][NUMMETH];
extern const struct el04_loc court_locs_CLUB[][NUMMETH];
// C18 to XC8 Compiler Changes End
extern char prog_chgd;
extern char priorState;
extern char nused;
// v1.219 end
// C18 to XC8 Compiler Changes Start
extern void set_intensity(void);	// This was located in the old lcd.h file.
extern signed char program_ix;
// C18 to XC8 Compiler Changes End


//===============================================================================
// 07/30/09 start
//#pragma udata dpb_thrash=0x100
// C18 to XC8 Compiler Changes Start
//#pragma udata rambank1 = 0x100
// Above line replaced with -L-prambank1=0x100 XC8 driver option
// C18 to XC8 Compiler Changes End
// 07/30/09 end
//===============================================================================

unsigned short dpb_thrash;						// DEBUG
mini_variant elev_corr;
struct motor_status stat;
long top_pwm;
long bot_pwm;
long feed_pwm;
long accel_top;
long accel_bot;
long accel_feed;
int top_incr;
int bot_incr;
unsigned m_top_per;				// measured top period
unsigned d_top_per;				// desired top period		
unsigned m_bot_per;				// measured bot period
unsigned d_bot_per;				// desired bot period
long d_feed_per;				// desired feed period
unsigned top_t1;
unsigned top_t2;
unsigned last_top_t1;
unsigned last_top_t2;
unsigned bot_t1;
unsigned bot_t2;
char rem_feed_chg;
//#ifdef FEEDBACK	// v1.186 commented
unsigned last_bot_t1;
unsigned last_bot_t2;
//#endif			// v1.186 commented
long feed_limit;
unsigned char top_limit;
unsigned char bot_limit;
unsigned char DoDebug;
unsigned long max_count, min_count, start_count;
int ball_feed;
unsigned feed_slow;
int main_ostate;// = -1;
char SpiLock;
ulong last_feed_pwm;
volatile unsigned char BallElapsedTicks;
char BallTimerOn,FeedTimeoutOn,FeedEarlyOn,FirstShot;
char LargeSpdDrop;
volatile ulong BallElapsedHalfSec;
char timer4_cnt;
unsigned long DbgFeedType, DbgFeedPwm, DbgFeedPwm_d, DbgFeedTime, DbgTimerCnt;
unsigned long DbgBallTime, DbgSweepTime;
//#ifdef FEEDBACK	// v1.186 commented
char ServeDiag;
struct moterr ErrorStat;

char ErrorShutDown;
//#endif					// v1.186 commented

char toggle_play;			// mjz 10/31/08, used to return to playing mode


// 07/30/09 start
//#pragma udata di_inbuf = 0x200
// 07/30/09 end

// v1.209 start
// The following were moved so ring buffer size could be increased.
//struct rbuf di_inbuf;			// diagnostic input ring buffer
//struct rbuf di_outbuf;			// diagnostic output ring buffer
//struct rbuf rm_inbuf;			// remote control input ring buffer
// v1.209 endf

//===============================================================================
// 07/30/09 start
// C18 to XC8 Compiler Changes Start
//#pragma udata rambank2 = 0x200
// Above line replaced with -L-prambank2=0x200 XC8 driver option
// C18 to XC8 Compiler Changes End
// 07/30/09 end
//===============================================================================
// v1.209 start
// Moved the following line to allow for a larger ring buffer.
//struct rbuf rm_outbuf;			// remote control output ring buffer
// v1.209 end
//struct rbuf spi_inbuf;
//struct rbuf spi_outbuf;
unsigned SlvCmdDelay;
#define DIAG_BUF_SIZE 32
char DiagBuf[DIAG_BUF_SIZE];	// diagnostic command buffer
char DiagBufOfs;
volatile struct power_button power;// = { 0, 1, 1, 1, 1 }; // power button is stable unpressed
	

char FlashBuf[64];
char SlaveCmd[DIAG_BUF_SIZE];	// slave command buffer
char SlaveCmdOfs;
char SlaveRes[DIAG_BUF_SIZE];	// slave response buffer
char SlaveResOfs;
char LastSlaveCmd;
char SlaveMotorStatus;			// 0 = rdy, non-zero=busy

// slave response buffer structure
#define C_START	0
#define C_CNT	1
#define C_CMD	2
#define ACK	6
#define NAK	21

// min pwm value for serve motors	//11907
// 09/22/09 start
//#define MINPWM    0x1100000L				// 0x1100000/0x3ff0000 = 26% duty cycle
// v1.215 start
// Changed the following define to a variable so different values can be used for different models.
//#define MINPWM    0x0ec0000L				// 0x0ec0000/0x3ff0000 = 23% duty cycle
// v1.215 end
// New serve motors were too fast at the previous MINPWM value, to be able to go slow enough for minimum speeds.
// In Program mode, if the first shot was a 60mph/-3 spin, the top serve motor could not get slow enough for the shot,
// so the feed motor would never turn on.
// 09/22/09 end

// C18 to XC8 Compiler Changes Start
//far void set_serve(union parm* pp);
//far void varinit(void);
//far void nofunc (int code, long *args); 
//far void kbdiag (int code, long *args);
//far void eeprom2flash(int code, long* args);
//void poweroff(int code, long *args);
//far void lobmain(char key);
void set_serve(union parm* pp);
void varinit(void);
void nofunc (int code, long *args); 
void kbdiag (int code, long *args);
void eeprom2flash(int code, long* args);
void poweroff(int code, long *args);
void lobmain(char key);
// C18 to XC8 Compiler Changes End
// v1.208 start
//extern char lob_parm(char cmd, uchar *pix, char* pparm, ushort mask);

// C18 to XC8 Compiler Changes Start
//extern char lob_parm(char cmd, uchar *pix, char* pparm, unsigned short long mask);
extern char lob_parm(char cmd, uchar *pix, signed char* pparm, unsigned short long mask);
// C18 to XC8 Compiler Changes End

// v1.208 end
unsigned char debounced_remote(void);
unsigned char debounced_remote_USART2(void);	// v1.196 added
void main(void);
void Setup(void);
void FillDiagBuf(void);
void exec_diag(void);
ulong FeedPwm2Sec(ulong feed);
void CalcLimits(ulong count, unsigned *SpdLimit);
int SpeedInRange(void);
// C18 to XC8 Compiler Changes Start
//far void remote_diag(int code, long* args);
//far char check_keypad(void);
void remote_diag(int code, long* args);
char check_keypad(void);
// C18 to XC8 Compiler Changes End
void dumpmem(int code, long *args);
void setmem(int code, long *args);
void flash(int code, long* args);
extern void FlashWr(void);
extern void FlashEr(void);
extern void FlashRd(void);
extern void lcdiag(int code, long *args);
void chartoa(const char c, char* cp, char ndig); 
void spitest(int code, long *args);
void top_diag(int code, long *args);
void bot_diag(int code, long *args);
void feed_diag(int code, long *args);
void elev_diag(int code, long *args);
void sweep_diag(int code, long *args);
void stop_all(int code, long *args);
void chip_select(int code, long *args);
void ee_test(int code, long *args);
void a2d_diag(int code, long *arg);
void pwm_debug(int code, long *arg);
void slavecmd(int code, long *arg);
void CurShot_diag(int code, long *arg);
void NxtShot_diag(int code, long *arg);
void MtrResponse(int code, long *args);
void ForceShot(int code, long *args);
void SlvTimeOut(void);
void FeedMtrDone(void);
void StartFeed(void);
void StartFeedAtSpd(void);
void SetFeedImm(long feedpwm);
void StartFeedCont(long feedpwm);
void SetFeedRate(long feedpwm);
void StartTop(unsigned val);
void StartBot(unsigned val);
void SetFeedPwm(void);
void SetTopPwm(void);
void SetBotPwm(void);
void UpdTopPer(void);
void UpdBotPer(void);
void StartSweepRandom(int speed, int limit1, int limit2);
void StartElevRandom(int speed, int limit1, int limit2);
void HomeSweepMotor(void);
void SlowFeedMtr(int flag);
void StopFeedMtr(void);
void StopTopMtr(void);
void StopBotMtr(void);
void StopBidirMtrs(void);
void SndSlvMtrStatus(void);
int GetMtrResponse(void);
void PgaGain(int chan, char gain);
void PgaChan(int chan);
void ChipSelect(int dev);
void ChipReserve(int dev);
void ChipDeSelect(int dev);
void BuildCmd(int code, int cnt, long *args);
void abort_shot(void);
void CheckBattery(unsigned int val);
unsigned int BatteryAverage(unsigned int val);

struct cmd_entry {
	unsigned cmd;					// 2 character command if bit 15 on, only master password can execute 
	char	 code;				// code distinguishing similar functions (various meanings) 
	far void (*func)(int code, long *ary);	// generic diagnostic function to perform 
// C18 to XC8 Compiler Changes Start
//} rom centry[]= {
} const centry[]= {
// C18 to XC8 Compiler Changes End

///////////////////
//	{ 0x4645, 0, flash },				// FE Flash Erase
//	{ 0x4644, 1, flash },				// FD Flash Dump
//	{ 0x4657, 2, flash }, 			// FW Flash Write
//	{ 0x4654, 3, flash }, 			// FT Flash Erase Test
///////////////////
	{ 0x444d, 0, dumpmem },			// DM	Dump Memory 
	{ 0x534d, 0, setmem },			// SM	Set Memory
	{ 0x5357, 0, spitest },			// SW	Spi Write
	{ 0x5352, 1, spitest },			// SR	Spi Read
	{ 0x5131, 0, feed_diag },		// Q1	set desired feed motor pwm
	{ 0x5132, 1, feed_diag },		// Q2	stop feed motor
	{ 0x5133, 2, feed_diag },		// Q3	update feed motor pwm
	{ 0x514C, 3, feed_diag },		// QL	set feed motor pwm limit 
	{ 0x4C32, 3, feed_diag },		// L2	set feed motor pwm limit
	{ 0x5141, 4, feed_diag },		// QA	set feed motor acceleration
	{ 0x5143, 5, feed_diag },		// QC	set trip point (a2d) for feed motor
	{ 0x5154, 6, feed_diag },		// QT	set trip time (a2d) for feed motor
	{ 0x5153, 7, feed_diag },		// QS	start feed motor at slow speed
	{ 0x5134, 8, feed_diag },		// Q4   set feed slow speed
	{ 0x514e, 9, feed_diag },		// QN	start feed motor now
	{ 0x5631, 0, top_diag },		// V1	set desired top motor per
	{ 0x5632, 1, top_diag },		// V2	stop top motor
	{ 0x5633, 2, top_diag },		// V3	update top motor per
	{ 0x564c, 3, top_diag },		// VL	set top motor pwm limit
	{ 0x4C33, 3, top_diag },		// L3	set top motor pwm limit
	{ 0x5634, 4, top_diag },		// V4	read timer elapsed time
	{ 0x5641, 5, top_diag },		// VA	set feed motor acceleration
	{ 0x5731, 0, bot_diag },		// W1	set desired bot motor per
	{ 0x5732, 1, bot_diag },		// W2	stop bot motor
	{ 0x5733, 2, bot_diag },		// W3	update bot motor per
	{ 0x574c, 3, bot_diag },		// WL	set bot motor pwm limit
	{ 0x4C34, 3, bot_diag },		// L4	set bot motor pwm limit
	{ 0x5734, 4, bot_diag },		// W4	read timer elapsed time
	{ 0x5741, 5, bot_diag },		// WA	set bot motor acceleration
	{ 0x5354, 0, stop_all },		// ST	stop all motors
	{ 0x4353, 0, chip_select },		// CS	chip select of spi device
	{ 0x4552, 0, ee_test },			// ER	eeprom read
	{ 0x4557, 1, ee_test },			// EW	eeprom write
	{ 0x4554, 2, ee_test },			// ET eeprom test
	{ 0x4550, 3, ee_test },			// EP eeprom protect xx
	{ 0x4553, 4, ee_test }, 		// ES eeprom status
	{ 0x4953, 0, pwm_debug },		// IS	init sampling
	{ 0x4443, 1, pwm_debug },		// DC	dump counts
	{ 0x5344, 2, pwm_debug },		// SD	start debug
	{ 0x4144, 0, a2d_diag },		// AD	a2d Dump all channels
	{ 0x4145, 1, a2d_diag },		// AE	a2d diagnostics
	{ 0x4153, 2, a2d_diag },		// AS	a2d diagnostics
	{ 0x4241, 3, a2d_diag },		// BA   a2d diagnostics - display last battery level
	{ 0x4146, 4, a2d_diag },		// AF a2d diagnostics - adjust feed current sense trip point
	{ 0x4844, 6, top_diag },		// HD	set top and bot serve motors hold flag
	{ 0x5843, 0, sweep_diag },		// XC   set trip point (a2d) for sweep motor
	{ 0x5854, 1, sweep_diag },		// XT   set trip point (a2d) for sweep motor
	{ 0x5943, 0, elev_diag },		// YC   set trip point (a2d) for sweep motor
	{ 0x5954, 1, elev_diag },		// YT   set trip point (a2d) for sweep motor
	{ 0x4358, 0, CurShot_diag },	// CX   send sweep motor command to slave
	{ 0x4359, 1, CurShot_diag },	// CY	send elev motor command to slave
	{ 0x4354, 2, CurShot_diag },	// CT   start top motor
	{ 0x4342, 3, CurShot_diag },	// CB	start bot motor
	{ 0x4346, 4, CurShot_diag },	// CF	start feed motor when ready
	{ 0x434e, 5, CurShot_diag },	// CN	start feed motor Continuous!!!
	{ 0x4349, 6, CurShot_diag },	// CI	init shot 
	{ 0x4348, 7, CurShot_diag },	// CH	random horizontal
	{ 0x4356, 8, CurShot_diag },	// CV	random vertical
	{ 0x4331, 9, CurShot_diag },	// C1   
	{ 0x4e58, 0, NxtShot_diag },	// NX arg1  set X value in NextShot
	{ 0x4e59, 1, NxtShot_diag },	// NY arg1	set Y value in NextShot
	{ 0x4e54, 2, NxtShot_diag },	// NT arg1  set top value in NextShot
	{ 0x4e42, 3, NxtShot_diag },	// NB arg1	set bot value in NextShot
	{ 0x4e53, 4, NxtShot_diag },	// NS	do NextShot
	{ 0x4d52, 0, MtrResponse },		// MR	check for response from slave after MS command
	{ 0x4653, 0, ForceShot },			// FS fake a shot

									// slave commands
									//   sweep motor
	{ 0x5831, 0x41, slavecmd},		// X1 arg1	move sweep motor to arg1
	{ 0x5832, 0x42, slavecmd},		// X2       set PWM to 50% and enable motor
	{ 0x5833, 0x43, slavecmd},		// X3       stop sweep motor
	{ 0x5841, 0x44, slavecmd},		// XA arg1  set acceleration
	{ 0x5848, 0x45, slavecmd},		// XH       home sweep motor
	{ 0x584C, 0x46, slavecmd},		// XL       set limit (in degrees)
	{ 0x5850, 0x47, slavecmd},		// XP       show current position
	{ 0x5853, 0x48, slavecmd},		// XS arg1  set home speed
	{ 0x5856, 0x49, slavecmd},		// XV arg1  set top velocity
	{ 0x584f, 0x4a, slavecmd},		// XO arg1  set Sweep motor home offset
	{ 0X5852, 0X4b, slavecmd},		// XR arg1  start Sweep motor continuous
									//   elevation motor
	{ 0x5931, 0x61, slavecmd},		// Y1 arg1	move elevation motor to arg1
	{ 0x5932, 0x62, slavecmd},		// Y2       set PWM to 50% and enable motor
	{ 0x5933, 0x63, slavecmd},		// Y3       stop elevation motor
	{ 0x5941, 0x64, slavecmd},		// YA arg1  set acceleration
	{ 0x5948, 0x65, slavecmd},		// YH       home elevation motor
	{ 0x594C, 0x66, slavecmd},		// YL       set limit (in degrees)
	{ 0x5950, 0x67, slavecmd},		// YP       show current position
	{ 0x5953, 0x68, slavecmd},		// YS arg1  set home speed
	{ 0x5956, 0x69, slavecmd},		// YV arg1  set top velocity
	{ 0x594f, 0x6a, slavecmd},		// YO arg1  set ELEV motor home offset
	{ 0x5952, 0x6b, slavecmd},		// YR arg1  start ELEV motor continuous
									//   other slave commands
	{ 0x4453, 0x30, slavecmd},		// DS arg1	get value at arg1
	{ 0x5353, 0x31, slavecmd},		// SS arg1 arg2	  *arg1 = arg2
//	{ 0x5354, 0x32, slavecmd},		// ST		stop all motors
	{ 0x4f43, 0x35, slavecmd},		// OC 			tell slave to turn off SPI output
	{ 0x4f53, 0x36, slavecmd},		// OS 			tell slave to turn back on SPI output
	{ 0x4b42, 	 0,	kbdiag },			// KeyBoard diagnostics
	{ 0x4b43,		 1, kbdiag },			// Keyboard Cycle
	{ 0x504f,		 0,  poweroff },	// PowerOff
	{ 0x4c54, 0x33, slavecmd},		// LT arg1  set LED bits 
	{ 0x4d53, 0x34, slavecmd},		// MS        returns motor status bit 0 for SWEEP, 1=busy
																//           1 for ELEV,  1=busy
///////////////////
//	{ 0x4546,		 0, eeprom2flash},// EF - Eeprom to Flash development
//	{ 0x4345,		 1, eeprom2flash},// CE - Copy flash to Eeprom
//	{ 0x5845,		 2, eeprom2flash},// XE - erase eeprom
//	{ 0x5645,		 3, eeprom2flash},// VE - verify eeprom clear
//	{ 0x4332,		 4, eeprom2flash},// C2 - verify eeprom and flash match
//	{ 0x5243,		 0, remote_diag },// RC - remote control
//	{ 0x5A52,		 5, eeprom2flash},// Z1 - read EEPROM byte
//	{ 0x5A57,		 6, eeprom2flash},// Z2 - write EEPROM byte
///////////////////
	{ 0x0000, 0, nofunc }					// MUST BE LAST ITEM 
};

#define MSEC5_LSB	0xa8
#define MSEC5_MSB	0x61
#define TICK_CNT	155
#define HALF_TICK_CNT	205
#define QUARTER_TICK_CNT 230
unsigned long seconds;
volatile unsigned long ticks;
unsigned long batt_ticks;	// force delay while showing battery
unsigned char batt_cnt;		// number of sameples currently available
unsigned char BatAvgCnt;	// number of averaged battery level readings available
//===============================================================================
// 07/30/09 start
// C18 to XC8 Compiler Changes Start
//#pragma udata rambank3 = 0x300
// Above line replaced with -L-prambank3=0x300 XC8 driver option
// C18 to XC8 Compiler Changes End
// 07/30/09 end
//===============================================================================

unsigned short BatAvg[20];
unsigned long home_ticks;	// reference to simulate homing time
unsigned long repeat;			// keyboard repeat ticks
unsigned short keyhold;		// key pressed time (in ticks)
unsigned long odtix;
unsigned char msec5;
//unsigned long SlvCmdDelay;


int TickIntCnt;					// blinking cursor fix
char cursor_state;				// 0 = blinking cursor is currently off, 1 = currently on
#define cursor_tmr_preload	210
unsigned char cursor_tmr;		
#define dashes_tmr_preload	180					// 02/07/09
unsigned char dashes_tmr = dashes_tmr_preload;	// 02/07/09

// C18 to XC8 Compiler Changes Start
//const rom char ready[] = "\n\rREADY>";
//const rom char InvCmd[] = "\n\rInvalid Command>";
//const rom char InvArg[] = "\n\rInvalid Argument>";
//const rom char crlf[] = "\n\r";
//const rom char space[] = " ";
//const rom char comma[] = ",";
//const rom char ShutDown[] = "\n\rShut Down\n\r";
const char ready[] = "\n\rREADY>";
const char InvCmd[] = "\n\rInvalid Command>";
const char InvArg[] = "\n\rInvalid Argument>";
const char crlf[] = "\n\r";
const char space[] = " ";
const char comma[] = ",";
const char ShutDown[] = "\n\rShut Down\n\r";
// C18 to XC8 Compiler Changes End
#define CR	13
#define LF	10
#define ESC	27
#define BS	8

extern struct persprog act_program;
extern uchar loaded_progno;
extern char cur_st;
extern char prev_st;
char a2dChan;
char a2dChan2Dump;

//PGA gain values:
//Channel 0 = 1 (battery level)
//Channel 1 = 16 (top serve motor current sense)
//Channel 2 = 16 (bottom serve motor current sense)
//Channel 3 = 16 (sweep motor current sense)
//Channel 4 = 16 (elevation motor curren sense)
//Channel 5 = 8 (feed motor current sense)
//Channel 6 = 1 (feed motor back emf)
//Channel 7 not used

// C18 to XC8 Compiler Changes Start
//const rom char a2dGain[] = {0,6,6,6,6,4,0};
const char a2dGain[] = {0,6,6,6,6,4,0};
// C18 to XC8 Compiler Changes End

#define MAX_ACC		8
#define ACC_DELTA	0
#define	ACC_CHG		1
char power_on;
char power_state;
// C18 to XC8 Compiler Changes Start
//rom long tmp;
long tmp;
// C18 to XC8 Compiler Changes End
//char rxi_flag;
char top_dir;
char bot_dir;
int top_drift;
int top_delta;
//int top_delta1;
int top_last_delta;
int top_last_delta1;
unsigned TopSpdLimit[2];
int bot_drift;
int bot_delta;
int bot_delta1;
int bot_last_delta;
int bot_last_delta1;
unsigned BotSpdLimit[2];
unsigned top_mid;
int top_phase6_cnt;
int top_phase3_cnt;
unsigned bot_mid;
int bot_phase6_cnt;
int top_timer_cnt;
volatile int bot_timer_cnt;
void DelayFor18TCY(void) {
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop(); //3.6usec
}
// C18 to XC8 Compiler Changes Start
//#pragma code MYCODE
// C18 to XC8 Compiler Changes End

void DelayPORXLCD(void)
{
	Delay1KTCYx(75); // 15 msec
}

void DelayXLCD(void)
{
	Delay1KTCYx(10); //21);	// 4.2 msec
}

int bot_phase3_cnt;

struct {
	unsigned Run:1;					// 0 - for serve motors, changing serve motor speed
									// for feed motor, waiting for ball feed. Serve
									// motors are not updated when feed_stat.Run is set
	unsigned phase1:1;				// 1 - phase 1: incrementing pwm
	unsigned phase2:1;				// 2 - phase 2: decrementing pwm
	unsigned phase3:1;				// 3 - phase 3: not there yet, incr pwm still
	unsigned phase4:1;				// 4 - phase 4: decrement pwm
	unsigned phase5:1;				// 5 - phase 5: incr pwm, smaller increments
	unsigned phase6:1;				// 6 - phase 6: wait to settle
	unsigned phase7:1;				// 7 - phase 7: almost there, nudge it up a little
	unsigned tick:1;				// 8 - tick: new delta data
	unsigned Hold:1;				// 9 - hold: for server motors, don't update pwm
									// for feed motor, wait for serve motor pwm to
									// reach desired speed, then go to run state.
	unsigned Slow:1;				// 10 - Slow: feed motor only, maintain slow speed
	unsigned Cont:1;				// 11 - Continuous: feed motor only
	unsigned Ok2Updt:1;				// 12 - allows top and bot serve motors to update, even if feed_stat.Run is set
	unsigned BatTest:1;				// 13 - getting battery level, no update
} top_stat, bot_stat, feed_stat;

// 07/03/09 start
//struct shot CurShot,NextShot,LastShot;
//ushort CurShotState, NextShotState;
ushort CurShotState, NextShotState;	// For some unknown reason, the compiler fails to select the correct bank when CurShotState resides in
struct shot CurShot,NextShot,LastShot; // bank 6. Swapping these 2 lines causes it to reside in bank 5.
// 07/03/09 end
// 07/29/09 start
unsigned char model_type;
unsigned char pp_maxloc;
// 07/29/09 end

// delta < 1000
// C18 to XC8 Compiler Changes Start
//const rom int accel_low[MAX_ACC][2] = {
const int accel_low[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0xa0,   0x80,
	0x180,  0x100,
	0x200,  0x200,
	0x300,  0x400,
	0x400,  0x1000,
	0x1000, 0x5800,
};

// delta > 1000
// C18 to XC8 Compiler Changes Start
//const rom int accel_med[MAX_ACC][2] = {
const int accel_med[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0x100,  0x80,
	0x300,  0x100,
	0x800,  0x400,
	0x1000, 0x1000,
	0x1400, 0x2000,
	0x1800, 0x5800,
};

// delta > 2000
// C18 to XC8 Compiler Changes Start
//const rom int accel_hi[MAX_ACC][2] = {
const int accel_hi[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x80,
	0x80,   0x80,
	0x100,  0x80,
	0x300,  0x100,
	0x800,  0x800,			//0x400,
	0x1400, 0x1000,
	0x1800, 0x2000,
	0x2000, 0x5800,
};

// delta < 400 (decreasing speed)
// C18 to XC8 Compiler Changes Start
//const rom int accel_xlow_d[MAX_ACC][2] = {
const int accel_xlow_d[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0x100,  0x100,
	0x120,  0x400,	
	0x180,  0x800, 
	0x300,  0xc00,
	0x400,  0x1000,
	0x1000, 0x5800,
};

// delta < 1000 (decreasing speed)
// C18 to XC8 Compiler Changes Start
//const rom int accel_low_d[MAX_ACC][2] = {
const int accel_low_d[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0x100,  0x80,
	0x180,  0x100,
	0x200,  0x200,
	0x300,  0x400,
	0x400,  0x1000,
	0x1000, 0x5800,
};

// delta > 1000 (decreasing speed)
// C18 to XC8 Compiler Changes Start
//const rom int accel_med_d[MAX_ACC][2] = {
const int accel_med_d[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0x100,  0x80,
	0x300,  0x100,
	0x800,  0x400,
	0x1000, 0x400,		//0x1000,
	0x1400, 0x800,		//0x2000,
	0x1800, 0x5800,
};

// delta > 2000 (decreasing speed)
// C18 to XC8 Compiler Changes Start
//const rom int accel_hi_d[MAX_ACC][2] = {
const int accel_hi_d[MAX_ACC][2] = {
// C18 to XC8 Compiler Changes End
	0x10,   0x10,
	0x80,   0x40,
	0x100,  0x80,
	0x300,  0x100,
	0x800,  0x100,	// 0x400,
	0x1400, 0x100,	// 0x400,
	0x1800, 0x800,
	0x2000, 0x5800,
};
int accel_range[MAX_ACC][2];
int accel_range_bot[MAX_ACC][2];

//const rom char a2dGain[] = {0,0,0,0,0,0,0};
int a2dValue[NumA2dChan];
char a2dtest;

//===============================================================================
// 07/30/09 start
// C18 to XC8 Compiler Changes Start
//#pragma udata rambank4 = 0x400
// Above line replaced with -L-prambank4=0x400 XC8 driver option
// C18 to XC8 Compiler Changes End
// 07/30/09 end
//===============================================================================

#define MAXAMP	25
#define MINAMP	4
// a2d value, max count allowed before shut down
// 09/21/09 start
//const rom unsigned int CurSense[MAXAMP-MINAMP+2][2] = {
//	104,110,		// 4 amps, 22 seconds
//	130,105,		// 5       21
//	157,100,		// 6       20
//	183,95,			// 7       19
//	209,90,			// 8       18
//	235,85,			// 9       17
//	261,80,			// 10      16
//	288,75,			// 11      15
//	314,70,			// 12      14
//	340,65,			// 13      13
//	366,60,			// 14      12
//	392,55,			// 15      11
//	419,50,			// 16      10
//	445,45,			// 17      9
//	471,40,			// 18      8
//	497,35,			// 19      7
//	513,30,			// 20      6
//	549,25,			// 21      5
//	576,20,			// 22      4
//	602,15,			// 23      3
//	628,10,			// 24      2
//	654,5,			// 25      1
//	655,0,			// >25     0
//};

// C18 to XC8 Compiler Changes Start
//const rom unsigned int CurSense[MAXAMP-MINAMP+2][2] = {
const unsigned int CurSense[MAXAMP-MINAMP+2][2] = {
// C18 to XC8 Compiler Changes End
	104,110,		// 4 amps, 22 seconds
	130,105,		// 5       21
	157,100,		// 6       20
	183,95,			// 7       19
	209,90,			// 8       18
	235,85,			// 9       17
	261,80,			// 10      16
	288,75,			// 11      15
	314,70,			// 12      14
	340,65,			// 13      13
	366,60,			// 14      12
	392,55,			// 15      11
	419,50,			// 16      10
	445,45,			// 17      9
	471,40,			// 18      8
	497,35,			// 19      7
	513,30,			// 20      6
	549,25,			// 21      5
	576,20,			// 22      4
	602,15,			// 23      3
	628,10,			// 24      2
	654,5,			// 25      1
//	655,0,			// >25     0
// 09/22/09 start
//	728,0,			// >28     0
	884,0,			// >34     0
// 09/22/09 end
};
// 09/21/09 end

unsigned char CurSenseCt[2][23];				// serve motors a2d value/time

#define SWEEP	0
#define ELEV	1
#define FEED	2
unsigned char BDM_CurSenseCt[3];				// sweep, elev and feed motors' time count over trip point
unsigned int BDM_TripPoint[3];					// sweep, elev and feed motors' trip point
unsigned int BDM_TripTime[3];					// sweep, elev and feed motors' trip time - in .2 sec intervals
unsigned char BAT_CurSenseCt;					// battery time count under trip point
unsigned int BAT_TripPoint;						// battery trip point
unsigned int BAT_TripTime;						// battery trip time
unsigned int BAT_lastval;						// last battery a2d value

// 07/30/09 start
//#pragma udata
// 07/30/09 end

// This table is used to decide when to start the feed motor.
// Only used for Personal Program mode
// speed, lower limit, upper limit
#define FT_SPEED	0
#define	FT_LOWER	1
#define	FT_UPPER	2
// C18 to XC8 Compiler Changes Start
//far rom ulong FeedStartTbl[][3] = {
const ulong FeedStartTbl[][3] = {
// C18 to XC8 Compiler Changes End
	SRV_20MPH, 0x6750, 0x5250,
	SRV_25MPH, 0x5250, 0x4250,
	SRV_30MPH, 0x4250, 0x3800,
	SRV_35MPH, 0x3800, 0x3400,
	SRV_40MPH, 0x3400, 0x3000,
	SRV_42_5,  0x3200, 0x2900,
	SRV_45MPH,	0x3000,	0x2800,
	SRV_47_5,	0x2900,	0x2700,
	SRV_50MPH,	0x2800, 0x2600,
	SRV_55MPH,	0x2600,	0x2350,
	SRV_60MPH,	0x2350,	0x2150,
	SRV_65MPH,	0x2150,	0x2000,
	SRV_70MPH,	0x1d50, 0x1950,
	0,0,0
};

// v1.203 start
char rc_qualified = 0;		// the next received remote control byte has been qualified
// v1.203 end
// v1.204 start
char manual_horiz_decr = 0;	// 0 = manual mode sweep angle is increasing, 1 = decreasing
// v1.204 end
// v1.205 start
int ELEV_CORR_MIN;			// Changed from a #define to variable so it can be model dependent.
// v1.205 end
// v1.207 start
int ball_feed_bottom;
// v1.207 end
// v1.209 start
char RemoteStringVar[10];
char RemoteString[68];
// v1.213 start
//char RemoteDataReceived = 0;	// No longer used, replaced by rky2.
// v1.213 end
// v1.209 end
// v1.214 start
unsigned char LargeSpdDropCnt = 0;
// v1.214 end

// v1.215 start
long MINPWM;				// Changed from a define.
// v1.215 end

// v1.219 start
unsigned char speedmin;
unsigned char speedmax;
signed char spinmin;
signed char spinmax;
unsigned char RemoteDataCnt;
unsigned int RemotePgmMode;
unsigned char RemotePgmShot;
unsigned char RemotePgmLocn[AVAILSHT];
unsigned char RemotePgmSpeed[AVAILSHT];
// v1.223 start
//unsigned char RemotePgmSpin[AVAILSHT];
signed char RemotePgmSpin[AVAILSHT];
// v1.223 end
unsigned char RemotePgmFeed[AVAILSHT];
unsigned char RemotePgmMethod[AVAILSHT];
char RemoteDataReceived = 0;
char UDrillShots;
// v1.219 end
// v1.220 start
void RemoteSaveDrill(void);
// v1.220 end
// v1.221 start
char BatLevelDisplay;
unsigned char BatLevelDisplayTmr;
int BatPercent;
extern ulong start_delay;
unsigned char PlayLEDTmr;
unsigned char PlayLEDPass;
extern long PlayModeBlink;
// v1.221 end
// v1.209 start
//===============================================================================
// C18 to XC8 Compiler Changes Start
//#pragma udata rambank9 = 0x900
// Above line replaced with -L-prambank9=0x900 XC8 driver option
// C18 to XC8 Compiler Changes End
///////////////////
//===============================================================================
// The following were moved here so ring buffer size could be increased.
struct rbuf di_inbuf;			// diagnostic input ring buffer
struct rbuf di_outbuf;			// diagnostic output ring buffer
//===============================================================================
// C18 to XC8 Compiler Changes Start
//#pragma udata rambankA = 0xa00
// Above line replaced with -L-prambankA=0xa00 XC8 driver option
// C18 to XC8 Compiler Changes End
///////////////////
//===============================================================================
// The following were moved here so ring buffer size could be increased.
struct rbuf rm_inbuf;			// remote control input ring buffer
struct rbuf rm_outbuf;			// remote control output ring buffer
// v1.209 end


//---------------------------------------------------------------------
// Interrupt Code
//---------------------------------------------------------------------

// Designate isr as an interrupt function and save key registers

// C18 to XC8 Compiler Changes Start
#ifdef notdef
// C18 to XC8 Compiler Changes End

#pragma interrupt isr //save = TBLPTRU //PRODL,PRODH

// Locate ISR handler code at interrupt vector

#pragma code isrcode=0x0008

void isrhandler(void)	// This function directs execution to the
{								// actual interrupt code										
_asm
goto isr
_endasm
}


#pragma code

// C18 to XC8 Compiler Changes Start
#endif
// C18 to XC8 Compiler Changes End


//---------------------------------------------------------------------
// isr()
// Checks which interrupt had happened and invokes it
//---------------------------------------------------------------------

// C18 to XC8 Compiler Changes Start
//void isr(void)
void interrupt isr(void)
// C18 to XC8 Compiler Changes End
{
	ClrWdt();
	if (INTCONbits.INT0IE) {						// if external interrupt 0 is active
		if(INTCONbits.INT0IF) {						// if external interrupt 0 has occurred
			INTCONbits.INT0IE = 1;					// keep it active
			INTCONbits.INT0IF = 0;					// acknowledge the interrupt
		}											// this ensures that INT0 wakes us from sleeping
	}
	if (INTCONbits.PEIE) {							// if peripheral interrupts enabled
		if (INTCONbits.TMR0IF) {					// timer0 overflow interrupt
			TMR0L = MSEC5_LSB;
			TMR0H = MSEC5_MSB;
			stat.TickInt = TRUE;
			INTCONbits.TMR0IF = 0;
			++TickIntCnt;							// blinking cursor fix
		}
		if (PIR1bits.CCP1IF) {						// timer1 capture occurred
			top_t1 = top_t2;
			top_t2 = ReadCapture1();
			m_top_per = top_t2 - top_t1;			// calc elapsed time
			++top_timer_cnt;
			PIR1bits.CCP1IF = 0;					// clear interrupt flag
		}
		if (PIR2bits.CCP2IF) {						// timer3 capture occurred
			bot_t1 = bot_t2;
			bot_t2 = ReadCapture2();
			m_bot_per = bot_t2 - bot_t1;			// calc elapsed time
			++bot_timer_cnt;
			PIR2bits.CCP2IF = 0;					// clear interrupt flag
		}
		#ifdef DEBUGPORT							// v1.196 added
// v1.209 start
// If using remote control, debounced_remote_USART2 does Read2USART instead.
// When USART2 is used for diagnostics, a ring buffer is utilized.
// When USART2 is used for remote control, a ring buffer is not utilized.
// v1.209 end
		if (PIR3bits.RC2IF) {						// Usart 2 receiver interrupt
			while (DataRdy2USART( )) {
//				rxi_flag = 1;
				rbuf_ins(&di_inbuf, Read2USART());
			}
			PIR3bits.RC2IF = 0;						// clear interrupt flag
		}
		if (PIR3bits.TX2IF) {						// Usart 2 transmit interrupt
			if (rbuf_bct(&di_outbuf)) {				// if more data to send out
				  TXREG2 = rbuf_extr(&di_outbuf);	// Write the data byte to the USART2
			} else
				PIE3bits.TX2IE = 0;					// else disable transmit interrupt
			PIR3bits.TX2IF = 0;
		}
		#endif										// v1.196 added
		if (PIR3bits.TMR4IF) {
			if (!timer4_cnt) {
			++timer4_cnt;
			if (!SpiLock && !a2dtest && (stat.a2dStarted == FALSE) && !(msec5 & 0x01U)) {
				PgaGain(a2dChan,a2dGain[a2dChan]);
				if (a2dChan == 0)			// If checking battery level, 02/12/09
				DelayFor18TCY();			// then wait until after the battery spike due serve motor PWM rising edge. 02/12/09
				ADCON0 = 1;
				ADCON0bits.NOT_DONE = TRUE;					// start
				stat.a2dStarted = TRUE;
			}
			SetTopPwm();
			if (PIR1bits.CCP1IF) {								// timer1 capture occurred
				top_t1 = top_t2;
				top_t2 = ReadCapture1();
				m_top_per = top_t2 - top_t1;				// calc elapsed time
				++top_timer_cnt;
				PIR1bits.CCP1IF = 0;									// clear interrupt flag
			}
			SetBotPwm();
			SetFeedPwm();
			--timer4_cnt;
			}
			PIR3bits.TMR4IF = 0;
		}
	}
}

// set up variables for display


void UpdTopPer(void)
{
	int top_delta1;
// v1.206 start
// v1.214 start
//	unsigned char LargeSpdDropCnt;	// moved so it is available outside this function
// v1.214 end
// v1.206 end

		INTCONbits.GIE = 0;				// disable interrupts
		if ((top_delta == 0) && (m_top_per != d_top_per)) {
		} else {
//**			if (last_top_t2 != top_t1) {	// did we miss an update?
//**				top_last_delta = top_delta1;	// make delta same to avoid fake ball drop
//**			}
			last_top_t1 = top_t1;
			last_top_t2 = top_t2;
			top_delta1 = top_delta;
			top_last_delta1 = top_last_delta;
			top_delta = 0;
			top_last_delta = top_delta1;
			top_stat.tick = 1;
			top_timer_cnt = 0;

//			if ((feed_stat.Run) || (feed_stat.Hold && (feed_pwm > 0x2800000))) {
// v1.194 & v1.195 start
// The following first line only takes care of the start/stop oscillation modes where the feed function has special requirements.
// Now we need to check for ball thrown in Manual mode and Random Vertical/Random Horizontal modes.
//			if ((feed_stat.Run) || (feed_stat.Hold && ((feed_pwm >= (long)last_feed_pwm)||(feed_pwm > 0x2800000)))) {
			if ((feed_stat.Run) || (feed_stat.Hold && ((feed_pwm >= (long)last_feed_pwm)||(feed_pwm > 0x2800000)))
			|| ((feed_stat.Cont) && (play_mode & PM_MANUAL) && (play_mode & PM_PLAYING))
			|| ((feed_stat.Cont) && (preset_type <= PRE_RANDVERT) && (play_mode & PM_PRESET) && (play_mode & PM_PLAYING)
			&& (preset[preset_type].feed == P_FEEDMIN))) {	// necessary only if random feed rate is selected
// v1.194 & v1.195 end
// v1.206 start

//				if ((top_last_delta1 - top_last_delta) > ball_feed) {
//					LargeSpdDrop = 1;
//					putrsDIAG("\n\rLargeSpdDrop_A (unfiltered)");	// v1.194 & v1.195 added
//				}

// For 2-Line mode only, don't want to risk missing a ball thrown, so we look for a smaller change in serve motor speed to indicate
// a ball has been thrown. Since the serve motor speed does not change from shot to shot, we should not get false "ball thrown"
// detections due serve motor acceleration.
// Changes are only being implemented for 2-Line mode, so other play modes are not inadvertently adversley affected. It would take a 
// great deal of testing to vet all other modes.
// v1.214 start
// Don't increment count if LargSpdDrop already detected, otherwise extra count could carry over for next detection.
// The following routine was added to make 2-Line mode more sensitive to detecting a thrown ball. Debounce is added to tune out
// false detections.
				if (((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (LargeSpdDrop == 0))
					&& (((top_last_delta1 - top_last_delta) > 0x080) || ((bot_last_delta1 - bot_last_delta) > 0x080))) {
//					&& (((top_last_delta1 - top_last_delta) > 0x040) || ((bot_last_delta1 - bot_last_delta) > 0x040))) {
							++LargeSpdDropCnt;								// sample true, increment debounce count 
//							if (LargeSpdDropCnt > 10) {						// Debounce to reduce false ball detection.
							if (LargeSpdDropCnt > 5) {						// Debounce to reduce false ball detection.
//							if (LargeSpdDropCnt > 0) {						// Debounce to reduce false ball detection.
								LargeSpdDrop = 1;							// Passed all debounce samples
								LargeSpdDropCnt = 0;
								putrsDIAG("\n\rLargeSpdDrop_Z (unfiltered)");
							}
//				} else LargeSpdDropCnt = 0;
				} else {
					LargeSpdDropCnt = 0;
				}

// v1.214 end
				if ((top_last_delta1 - top_last_delta) > ball_feed) {	// This is the original routine used in all play modes for
					LargeSpdDrop = 1;									// detecting a ball thrown. It is still used in all modes, 
					putrsDIAG("\n\rLargeSpdDrop_Top (unfiltered)");		// but now there are extra routines, above and below, that	// v1.214 commented
				}														// are also used for 2-Line mode to increase sensitivity.
// v1.206 end
// v1.207 start
// Now check for drop in speed on bottom motor also.
// Can't use this, because ball_feed is based on the top serve motor speed only.
// To use this, a new variable ball_feed_bot must be declared, based on the bottom serve motor speed.
				if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	
					if ((bot_last_delta1 - bot_last_delta) > ball_feed_bottom) {
						LargeSpdDrop = 1;
						putrsDIAG("\n\rLargeSpdDrop_Bot (unfiltered)");	// v1.214 commented
					}
				}
			} else LargeSpdDropCnt = 0;
// v1.214 end
		}
		INTCONbits.GIE = 1;
// v1.214 start
#ifdef DEBUGPORT
		if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (LargeSpdDropCnt)) {	
			putrsDIAG("\n\rLargeSpdDropCnt = ");
			dumplong(LargeSpdDropCnt);
//			putrsDIAG("\n\rtop_last_delta1 = ");
//			dumplong(top_last_delta1);
//			putrsDIAG("\n\rtop_last_delta  = ");
//			dumplong(top_last_delta);
//			putrsDIAG("\n\rball_feed = ");
//			dumplong(ball_feed);
		}
// v1.214 end
// v1.194 & v1.195 start
//		if (LargeSpdDrop) {
//			putrsDIAG("\n\rtop_last_delta1 = ");
//			dumplong(top_last_delta1);
//			putrsDIAG("\n\rtop_last_delta = ");
//			dumplong(top_last_delta);
//			putrsDIAG("\n\rball_feed = ");
//			dumplong(ball_feed);
//		}
#endif
// v1.194 & v1.195 end
}


void UpdBotPer(void)
{ 	
		INTCONbits.GIE = 0;				// disable interrupts
		if ((bot_delta == 0) && (m_bot_per != d_bot_per)) {
		} else {
		last_bot_t1 = bot_t1;
		last_bot_t2 = bot_t2;
		bot_delta1 = bot_delta;
		bot_last_delta1 = bot_last_delta;
		bot_delta = 0;
		bot_last_delta = bot_delta1;
		bot_stat.tick = 1;
		}
//		bot_timer_cnt = 0;
		INTCONbits.GIE = 1;
}

void CalcTopPwm(void)
{
	unsigned drift, drift1, ix;
	int incr = 0;
	int delta;
	unsigned abs_delta, abs_delta1;
	int rtop_last_delta, rtop_last_delta1;
	int TooFast; 
	int SpdDir;			// direction of change, 1 = increasing, 0 = decreasing


	// check for timer1 capture interrupt
//	if ((top_pwm > 0x2000000L) && (m_bot_per == 0U)) {	// mjz 10/24/08, commented, bug fix
//	if ((top_pwm > 0x2000000L) && (m_top_per == 0U)) {	// mjz 10/24/08, bug fix
// mjz 10/24/08, catch 22, above would not work because we would not be here if the period was zero.
// If the period was zero, there would not have been an interrupt to bring us here.
	if ((top_pwm > 0x2000000L) && (m_top_per >= 60000U)) {	// mjz 10/24/08, if motor is very slow
		ErrorStat.TopCnt = 1;
		if (ErrorShutDown == 0) {
			ErrorShutDown = 1;
/*			putrsDIAG((rom char far *)"B\r\n"); /**/
		}
		return;
	}

	// drift is the accepted range within desired period. Once we reached steady speed, the period tends to drift.
	// As long as the value is within +- drift, we won't adjust the pwm. 
	drift = 8;
	if (m_top_per > 0x2000U) {
		drift += (m_top_per - 0x2000) >> 9;
	} else {
		if (m_top_per < 0x1800U) drift = 1;
		else
			drift -= (0x2000U - m_top_per) >> 11;
	}
	top_drift = drift;
	drift1 = drift << 1;
	delta = d_top_per - m_top_per;
	if (d_top_per > m_top_per) {
		abs_delta = delta;
		TooFast = 1;
	} else {
		abs_delta = 0 - delta;
		TooFast = 0;
	}
	SpdDir = /*(top_timer_cnt)? ((delta > top_last_delta)?1:0) : */((top_last_delta > top_last_delta1)?1:0);
	abs_delta1 = /*(top_timer_cnt) ? (delta - top_last_delta) : */(top_last_delta - top_last_delta1);
	if (abs_delta1 & 0x8000) abs_delta1 = 0 - abs_delta1;

	top_delta = delta;

	if (top_stat.BatTest) return;
	if ((d_top_per == 0U && top_pwm == 0L) || /*top_stat.Hold ||*/ (feed_stat.Run && !feed_stat.Ok2Updt)) {
		// if motor stopped, do nothing
		// if in holding pattern, do nothing
	} else if (m_top_per != d_top_per) {
		if (((top_pwm & 0x3ff0000L) < MINPWM) || (m_top_per == 0U)) top_pwm += accel_top;
		else if (top_stat.Run && top_dir) {                            // only set if incr speed
			delta = 0 - delta;
			rtop_last_delta = 0 - top_last_delta;
			rtop_last_delta1 = 0 - top_last_delta1;
			if (top_stat.phase1) {							// phase 1, incr pwm by accel_top
				if (m_top_per < top_mid) {					// if period passed mid point
					top_stat.phase1 = 0;					//    switch to phase 2
					top_stat.phase2 = 1;
					top_stat.tick = 0;
					incr = 0 - accel_top;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"B");
#endif
				} else {
					incr = accel_top;
					if ((top_pwm & 0x3ff0000) == 0x3ff0000) { // if at max pwm,
						top_stat.phase1 = 0;				  //   go to phase 3 where
						top_stat.phase3 = 1;				  //   we check if at max speed
						top_stat.tick = 0;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"C");
#endif
					}
				}
			} else if (top_stat.phase3 || top_stat.phase5) {// phase 3, incr pwm by accel_top/2
															// phase 5, incr pwm by accel_top/4
				if ((top_pwm & 0x3ff0000L) == 0x3ff0000L) {
					if ((rtop_last_delta1 - delta) < (int)drift1) {		// pwm not changing much, done
						if (++top_phase3_cnt > 2) {
							top_stat.phase3 = 0;
							top_stat.phase5 = 0;
							top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRA ");
#endif
							return;
						}
					} else
						top_phase3_cnt = 0;
				}
				if (m_top_per < top_mid) {					// if period passed mid point
					top_stat.phase3 = 0;					//    switch to phase 4
					top_stat.phase5 = 0;
					top_stat.phase4 = 1;
					incr = 0 - (accel_top >> 1);
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"D1");
#endif
				} else incr = (SpdDir)?(accel_top >> ((top_stat.phase3)?1:2)):accel_top;
			} else if (top_stat.phase2) {					// phase 2, decr pwm by accel_top
				if (top_stat.tick) {
					top_stat.tick = 0;
					ix = rtop_last_delta1 - rtop_last_delta;
					if (rtop_last_delta > rtop_last_delta1) ix = 0 - ix;
					if ((ix) < (drift >> 1)) {
						top_stat.phase2 = 0;
						top_stat.phase6 = 1;
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/					putrsDIAG((const rom char *)"F1 ");
#endif
						return;
					}
				}
				if (TooFast == 1) {						//  shot past desired period
					if (rtop_last_delta1 & 0x8000) {
						if (delta < rtop_last_delta1) {		//  still increasing speed
							incr = 0 - accel_top;     
						} else if ((0-delta)>(int)drift1) {		//  still above speed
							incr = 0 - (accel_top >> 1);     
						} else if ((delta - rtop_last_delta1) > (int)drift) {	// speed is decreasing fast
							incr = 0 + (accel_top >> 1);     
						} else {
							incr = 0;
							top_stat.phase2 = 0;
							top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRB ");
#endif
						}
					} else if ((rtop_last_delta1 - delta) < (int)drift1) {		// pwm not changing much, done
						top_stat.phase2 = 0;
						top_stat.phase6 = 1;
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/					putrsDIAG((const rom char *)"F2 ");
#endif
					} else {
						incr = 0 - (accel_top >> 1);
					}
				} else 			// Not Too Fast and phase 2
				if (abs_delta < drift1) {							// very close
					if (abs_delta1 < (drift >> 2)) {			// and not changing much
						incr = 0;
						top_stat.phase2 = 0;
						top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRC ");
#endif
					} else {
						top_stat.phase2 = 0;
						top_stat.phase6 = 1;
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"F3 ");
#endif
					}
				} else if (!SpdDir) {			// Slower than desired, and decreasing speed
					// need to test how far from desired, and adjust from there
					top_mid = d_top_per + (delta >> 1);
					top_stat.phase2 = 0;
					if (abs_delta > (drift << 5)) top_stat.phase3 = 1;
					else top_stat.phase5 = 1;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"C1 ");
#endif
					incr = accel_top >> 1;
				} else if (abs_delta1 < drift) {	// Slower than desired, speed not changing much, or increasing
					top_stat.phase2 = 0;
					if (abs_delta < (drift << 5)) {	
						incr = 0;
						top_stat.phase2 = 0;
						top_stat.Run = 0;
#ifdef DEBUGMSG
/**/				putrsDIAG((const rom char *)"TMRD ");
#endif
					} else {
						top_mid = d_top_per + (delta >> 1);
						top_stat.phase3 = 1;
#ifdef DEBUGMSG1
/**/					putrsDIAG((const rom char *)"C2 ");
#endif
						incr = accel_top >> 1;
					}
				} else {			// not TooFast, increasing
					if (abs_delta < (drift << 5))	{			// close to acceptable
						if (abs_delta1 < drift1)					//  acceleration is slowing
							incr = 0 - (accel_top >> 2);		//  don't decrease pwm too much
						else if (abs_delta1 < (drift1 + drift))
							incr = 0 - (accel_top >> 1);
						else
							incr = 0 - accel_top;
					} else
						incr = 0 - accel_top;
				}
			} else if (top_stat.phase4) {					// phase 2, decr pwm by accel_top/2
				if (top_stat.tick) {
					top_stat.tick = 0;
					if (abs_delta1 < (drift >> 1)) {		// not changing much
						top_stat.phase4 = 0;
						top_stat.phase6 = 1;
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/					putrsDIAG((const rom char *)"F4 ");
#endif
						return;
					}
				}
				if (TooFast == 1) {							// we are too fast, 
					if (!SpdDir) {						//   speed is decreasing
						if (abs_delta < (drift << 5)) {		// speed within acceptable range, done
							incr = 0;
							top_stat.phase4 = 0;
							top_stat.Run = 0;
#ifdef DEBUGMSG
/**/					putrsDIAG((const rom char *)"TMRE ");
#endif
						} else {
							if (abs_delta1 > drift)
								incr = 0 - (accel_top >> 1);
							else incr = 0 - (accel_top >> 2);
						}
					} else if (abs_delta1 > (drift >> 2)) {			// Too fast, speed is still increasing 
						if ((abs_delta1 > drift1) || (abs_delta > (drift << 5)))
							incr = 0 - accel_top;
						else if (abs_delta > (drift << 5)) 		// speed within acceptable range, done
							incr = 0 - accel_top;
						else if (abs_delta1 > drift)
							incr = 0 - (accel_top >> 1);
						else incr = 0 - (accel_top >> 2);
					} else {														// speed is increasing slowly
						if (abs_delta < drift1) {
							incr = 0;
							top_stat.phase4 = 0;
							top_stat.phase6 = 1;							//   go wait a tick and see what happens
							top_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"F5 ");
#endif
						} else {
							incr = 0 - (accel_top >> 2);
						}
					}
				} else {														// Too Slow
					if (SpdDir && (delta & 0x8000) && (abs_delta > (drift << 5))) {
						incr = 0 - accel_top;
					} else
					if (!SpdDir) {					// Too slow, and speed is decreasing
						top_stat.phase4 = 0;
						top_stat.phase6 = 1;							//   go wait a tick and see what happens
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"F6 ");
#endif
						return;
					} else if ((abs_delta1 < drift) || (abs_delta < drift1)) { // slower than desired, speed increasing,
																																	 //   almost there, but not changing much
						top_stat.phase4 = 0;
						top_stat.phase6 = 1;							//   go wait a tick and see what happens
						top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"F7 ");
#endif
						return;
					} else {														// slower than desired
						incr = 0 - ((delta > (int)(drift1 >> 1))?accel_top:(accel_top>>1));
					}
				}
			} else if (top_stat.phase6) {
				if (top_stat.tick) {
					top_stat.tick = 0;
					if (++top_phase6_cnt >= 3) {								// # of ticks in phase6
						if ((TooFast==1) && !SpdDir) {									// we are too fast, but coming down in speed
							top_stat.phase6 = 0;
							top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRF ");
									dumpshort(m_top_per);
#endif
							return;
						}
						if (TooFast) {
							if (SpdDir) {		// speed increasing
								// if we are not too far past desired speed, and not changing much, then done
// C18 to XC8 Compiler Changes Start
//								if ((abs_delta < (drift >> 2)) && (abs_delta1 < (drift >> 22))) {
								if ((abs_delta < (drift >> 2)) && (abs_delta1 < (drift >> 22))) {
// C18 to XC8 Compiler Changes End
									top_stat.phase6 = 0;
									top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRG ");
#endif
									return;
								} else {
									// decrease speed some
									top_stat.phase6 = 0;
									top_stat.phase4 = 1;
									incr = 0 - accel_top;
#ifdef DEBUGMSG1
/**/							putrsDIAG((const rom char *)"D2");
#endif
								}
							} else {		// TooFast, speed decreasing
								if (abs_delta < (drift << 5)) {
									top_stat.phase6 = 0;
									top_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRH ");
#endif
									return;
								} else {
									// decrease speed some
									top_stat.phase6 = 0;
									top_stat.phase4 = 1;
									incr = 0 - accel_top;
#ifdef DEBUGMSG1
/**/							putrsDIAG((const rom char *)"D3");
#endif
								}
							}
						} else {			// not TooFast
							if (SpdDir && (abs_delta < (drift << 5)) && (abs_delta1 < (drift >> 1))) {
							// Too slow, speed is increaing and almost there, then done
								top_stat.phase6 = 0;
								top_stat.Run = 0;
#ifdef DEBUGMSG
/**/						putrsDIAG((const rom char *)"TMRI ");
#endif
								return;
							} else {
								top_mid = d_top_per + delta - (delta >> 2);
								top_stat.phase6 = 0;
								top_stat.phase5 = 1;
								incr = accel_top;
#ifdef DEBUGMSG1
/**/							putrsDIAG((const rom char *)"E2 ");
#endif
							}
						}
					}
				} else {
					return;
				}
			} else if (top_stat.phase7) {
				if (top_stat.tick) {
					top_stat.tick = 0;
					top_stat.phase7 = 0;
					top_stat.phase6 = 1;
					top_phase6_cnt = 0;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"F8 ");
#endif
				} else {
					incr = accel_top >> 2;
					if (TooFast)
						incr = 0 - incr;
				}
			}			
			if ((top_pwm += incr) > 0x3ff0000L)
				top_pwm = 0x3ff0000L;
		}
	}
	INTCONbits.GIE = 0;					// disable interrupts
	top_incr = incr;
	INTCONbits.GIE = 1;
}

void SetTopPwm(void)
{
	unsigned drift, drift1, ix;
	int i;
	int incr = 0;
	int delta;

	if (ErrorShutDown) return;
	
	// drift is the accepted range within desired period. Once we reached steady speed, the period tends to drift.
	// As long as the value is within +- drift, we won't adjust the pwm. 
	drift = 8;
	if (m_top_per > 0x2000U) {
		drift += (m_top_per - 0x2000U) >> 9;
	} else {
		if (m_top_per < 0x1800U) drift = 1;
		else
			drift -= (0x2000U - m_top_per) >> 11;
	}
	top_drift = drift;
	drift1 = drift << 1;
	delta = (int)(d_top_per - m_top_per);

	if (top_stat.BatTest) return;
	if ((d_top_per == 0U && top_pwm == 0L) || /*top_stat.Hold ||*/ (feed_stat.Run && !feed_stat.Ok2Updt)) {
		// if motor stopped, do nothing
		// if in holding pattern, do nothing
	} else if (m_top_per != d_top_per) {
		if (((top_pwm & 0x3ff0000L) < MINPWM) || (m_top_per == 0U)) top_pwm += accel_top;
		else if (top_stat.Run && top_dir) {                            // only set if incr speed
			if ((top_pwm += top_incr) > 0x3ff0000L)
				top_pwm = 0x3ff0000L;
		} else if (top_stat.Run) {				// top_dir is 0, want slower speed
			top_stat.tick = 0;
			if (top_stat.phase1) {
				if (m_top_per > top_mid) {					// if period passed mid point
					top_stat.phase1 = 0;					//    switch to phase 2
					top_stat.phase2 = 1;
					delta = delta >> 2;
					if (delta > 500) delta = 500;
					top_mid = d_top_per - delta;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"J");
#endif
				} else {
					incr = accel_top;
				}
				if ((top_pwm -= incr) < MINPWM) top_pwm = MINPWM;
				// force free fall
//				SetDCPWM4(0);		// Write the new duty cycle.
//				return;
			} else if (top_stat.phase2) {				// phase 2, wait
				if (m_top_per > top_mid) {
					top_stat.phase2 = 0;
					top_stat.phase3 = 1;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"K");
#endif
				}
				// force free fall
//				SetDCPWM4(0);		// Write the new duty cycle.
//				return;
			} else if (top_stat.phase3) {				// phase 3, incr pwm
				if (delta < 0) {						// dropped pass desired speed
					top_stat.Run = 0;
					top_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRJ ");
#endif
				} else if (top_last_delta > top_last_delta1) {	// speed is increasing
					incr = accel_top;
					if (delta < (int)(drift1 << 1)) {
						top_stat.Run = 0;
						top_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRK ");
#endif
					}
				} else if ((top_last_delta1 - top_last_delta) < (int)(drift >> 1)) {	// speed not changing much
					if (delta > (int)(drift << 5)) {
						top_mid = m_top_per + (delta >> 3);
						top_stat.phase3 = 0;
						top_stat.phase1 = 1;
						incr = accel_top;
#ifdef DEBUGMSG1
/**/				putrsDIAG((const rom char *)"L");
#endif
					} else {
						top_stat.Run = 0;
						top_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRL ");
#endif
					}
				} else if (delta < (int)drift1) {	// we are close to desired speed
					top_stat.Run = 0;
					top_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"TMRM ");
#endif
				} else if (delta < (int)(drift << 5)) {
					incr = 0 - (accel_top >> 2);
				} else incr = 0 - (accel_top >> 2) - (accel_top >> 1);
			}
			if ((top_pwm -= incr) < MINPWM) top_pwm = MINPWM;
//			if (top_pwm == MINPWM) {	// force free fall
//				SetDCPWM4(0);		// Write the new duty cycle.
//				return;
//			}
		} else if (m_top_per < d_top_per) {					// need lower frequency
			if ((delta  > (int)drift) || (delta > top_last_delta)) {	// close enough, don't change
				for (i=0; i<MAX_ACC; i++) {
					if (accel_range[i][ACC_DELTA] < delta)
						incr = accel_range[i][ACC_CHG];
				}
				if (delta > top_last_delta) {				// speed is increasing
					incr = accel_top;						// even when we want it slower
				} else
				if (incr > accel_top) incr = accel_top;
				if ((top_pwm -= incr) < 0L) top_pwm = 0L;
			}
		} else {										// need higher frequency
			delta = 0 - delta;
			if ((top_pwm & 0x3ff0000L) == 0x3ff0000L) {		// if already at 100% duty cycle, don't incr
			} else {
				if (top_dir == 0) {						// we had shot past our desired speed
					for (i=0; i<MAX_ACC; i++) {
						accel_range[i][ACC_DELTA] = accel_low[i][ACC_DELTA];
						accel_range[i][ACC_CHG] = accel_low[i][ACC_CHG];
					}
					top_dir = 1;
				}
				if (delta > (int)drift) {	// close enough, don't change
					if (BallTimerOn && (top_last_delta < top_last_delta1) && (delta > (int)(drift << 4))) incr = accel_top;
					else
					for (i=0; i<MAX_ACC; i++) {
						if (accel_range[i][ACC_DELTA] < delta)
							incr = accel_range[i][ACC_CHG];
					}
					if (incr > accel_top) incr = accel_top;
					if ((top_pwm += incr) > 0x3ff0000L)
						top_pwm = 0x3ff0000L;
				}
			}
		}
		SetDCPWM4((unsigned int)((top_pwm & 0x3fc0000L) >> 16));		// Write the new duty cycle.
	}
}

void CalcBotPwm(void)
{
	unsigned drift, drift1, ix;
	int incr = 0;
	int delta;
	unsigned abs_delta, abs_delta1;
	int rbot_last_delta, rbot_last_delta1;
	int TooFast; 
	int SpdDir;			// direction of change, 1 = increasing, 0 = decreasing


	// check for timer1 capture interrupt
//	if ((bot_pwm > 0x2000000L) && (m_bot_per == 0U)) {	// mjz 10/24/08, commented, bug fix
// mjz 10/24/08, catch 22, above would not work because we would not be here if the period was zero.
// If the period was zero, there would not have been an interrupt to bring us here.
	if ((bot_pwm > 0x2000000L) && (m_bot_per >= 60000U)) {	// mjz 10/24/08, if motor is very slow
		ErrorStat.BotCnt = 1;
		if (ErrorShutDown == 0) {
			ErrorShutDown = 1;
// C18 to XC8 Compiler Changes Start
//			putrsDIAG((rom char far *)"B\r\n"); /**/
			putrsDIAG((const char far *)"B\r\n"); /**/
// C18 to XC8 Compiler Changes End
		}
		return;
	}

	// drift is the accepted range within desired period. Once we reached steady speed, the period tends to drift.
	// As long as the value is within +- drift, we won't adjust the pwm. 
	drift = 8;
	if (m_bot_per > 0x2000U) {
		drift += (m_bot_per - 0x2000U) >> 9;
	} else {
		if (m_bot_per < 0x1800U) drift = 1;
		else
			drift -= (0x2000U - m_bot_per) >> 11;
	}
	bot_drift = drift;
	drift1 = drift << 1;
	delta = d_bot_per - m_bot_per;
	if (d_bot_per > m_bot_per) {
		abs_delta = delta;
		TooFast = 1;
	} else {
		abs_delta = 0 - delta;
		TooFast = 0;
	}
	SpdDir = /*(bot_timer_cnt)? ((delta > bot_last_delta)?1:0) : */((bot_last_delta > bot_last_delta1)?1:0);
	abs_delta1 = /*(bot_timer_cnt) ? (delta - bot_last_delta) : */(bot_last_delta - bot_last_delta1);
	if (abs_delta1 & 0x8000) abs_delta1 = 0 - abs_delta1;

	bot_delta = delta;

	if (top_stat.BatTest) return;
	if ((d_bot_per == 0U && bot_pwm == 0L) || /*bot_stat.Hold ||*/ (feed_stat.Run && !feed_stat.Ok2Updt)) {
		// if motor stopped, do nothing
		// if in holding pattern, do nothing
	} else if (m_bot_per != d_bot_per) {
		if (((bot_pwm & 0x3ff0000L) < MINPWM) || (m_bot_per == 0U)) bot_pwm += accel_bot;
		else if (bot_stat.Run && bot_dir) {                            // only set if incr speed
			delta = 0 - delta;
			rbot_last_delta = 0 - bot_last_delta;
			rbot_last_delta1 = 0 - bot_last_delta1;
			if (bot_stat.phase1) {							// phase 1, incr pwm by accel_bot
				if (m_bot_per < bot_mid) {					// if period passed mid point
					bot_stat.phase1 = 0;					//    switch to phase 2
					bot_stat.phase2 = 1;
					bot_stat.tick = 0;
					incr = 0 - accel_bot;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"B");
#endif
				} else {
					incr = accel_bot;
					if ((bot_pwm & 0x3ff0000L) == 0x3ff0000L) { // if at max pwm,
						bot_stat.phase1 = 0;				  //   go to phase 3 where
						bot_stat.phase3 = 1;				  //   we check if at max speed
						bot_stat.tick = 0;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"C");
#endif
					}
				}
			} else if (bot_stat.phase3 || bot_stat.phase5) {// phase 3, incr pwm by accel_bot/2
															// phase 5, incr pwm by accel_bot/4
				if ((bot_pwm & 0x3ff0000L) == 0x3ff0000L) {
					if ((rbot_last_delta1 - delta) < (int)drift1) {		// pwm not changing much, done
						if (++bot_phase3_cnt > 2) {
							bot_stat.phase3 = 0;
							bot_stat.phase5 = 0;
							bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRA ");
#endif
							return;
						}
					} else
						bot_phase3_cnt = 0;
				}
				if (m_bot_per < bot_mid) {					// if period passed mid point
					bot_stat.phase3 = 0;					//    switch to phase 4
					bot_stat.phase5 = 0;
					bot_stat.phase4 = 1;
					incr = 0 - (accel_bot >> 1);
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"D1");
#endif
				} else incr = (SpdDir)?(accel_bot >> ((bot_stat.phase3)?1:2)):accel_bot;
			} else if (bot_stat.phase2) {					// phase 2, decr pwm by accel_bot
				if (bot_stat.tick) {
					bot_stat.tick = 0;
					ix = rbot_last_delta1 - rbot_last_delta;
					if (rbot_last_delta > rbot_last_delta1) ix = 0 - ix;
					if ((ix) < (drift >> 1)) {
						bot_stat.phase2 = 0;
						bot_stat.phase6 = 1;
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"F1 ");
#endif
						return;
					}
				}
				if (TooFast == 1) {						//  shot past desired period
					if (rbot_last_delta1 & 0x8000) {
						if (delta < rbot_last_delta1) {		//  still increasing speed
							incr = 0 - accel_bot;     
						} else if ((0-delta)>(int)drift1) {		//  still above speed
							incr = 0 - (accel_bot >> 1);     
						} else if ((delta - rbot_last_delta1) > (int)drift) {	// speed is decreasing fast
							incr = 0 + (accel_bot >> 1);     
						} else {
							incr = 0;
							bot_stat.phase2 = 0;
							bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRB ");
#endif
						}
					} else if ((rbot_last_delta1 - delta) < (int)drift1) {		// pwm not changing much, done
						bot_stat.phase2 = 0;
						bot_stat.phase6 = 1;
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"F2 ");
#endif
					} else {
						incr = 0 - (accel_bot >> 1);
					}
				} else 			// Not Too Fast and phase 2
				if (abs_delta < drift1) {							// very close
					if (abs_delta1 < (drift >> 2)) {			// and not changing much
						incr = 0;
						bot_stat.phase2 = 0;
						bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRC ");
#endif
					} else {
						bot_stat.phase2 = 0;
						bot_stat.phase6 = 1;
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"F3 ");
#endif
					}
				} else if (!SpdDir) {			// Slower than desired, and decreasing speed
					// need to test how far from desired, and adjust from there
					bot_mid = d_bot_per + (delta >> 1);
					bot_stat.phase2 = 0;
					if (abs_delta > (drift << 5)) bot_stat.phase3 = 1;
					else bot_stat.phase5 = 1;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"C1 ");
#endif
					incr = accel_bot >> 1;
				} else if (abs_delta1 < drift) {	// Slower than desired, speed not changing much, or increasing
					bot_stat.phase2 = 0;
					if (abs_delta < (drift << 5)) {	
						incr = 0;
						bot_stat.phase2 = 0;
						bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/				putrsDIAG((const rom char *)"BMRD ");
#endif
					} else {
						bot_mid = d_bot_per + (delta >> 1);
						bot_stat.phase3 = 1;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"C2 ");
#endif
						incr = accel_bot >> 1;
					}
				} else {			// not TooFast, increasing
					if (abs_delta < (drift << 5))	{			// close to acceptable
						if (abs_delta1 < drift1)					//  acceleration is slowing
							incr = 0 - (accel_bot >> 2);		//  don't decrease pwm too much
						else if (abs_delta1 < (drift1 + drift))
							incr = 0 - (accel_bot >> 1);
						else
							incr = 0 - accel_bot;
					} else
						incr = 0 - accel_bot;
				}
			} else if (bot_stat.phase4) {					// phase 2, decr pwm by accel_bot/2
				if (bot_stat.tick) {
					bot_stat.tick = 0;
					if (abs_delta1 < (drift >> 1)) {		// not changing much
						bot_stat.phase4 = 0;
						bot_stat.phase6 = 1;
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"F4 ");
#endif
						return;
					}
				}
				if (TooFast == 1) {							// we are too fast, 
					if (!SpdDir) {						//   speed is decreasing
						if (abs_delta < (drift << 5)) {		// speed within acceptable range, done
							incr = 0;
							bot_stat.phase4 = 0;
							bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/					putrsDIAG((const rom char *)"BMRE ");
#endif
						} else {
							if (abs_delta1 > drift)
								incr = 0 - (accel_bot >> 1);
							else incr = 0 - (accel_bot >> 2);
						}
					} else if (abs_delta1 > (drift >> 2)) {			// Too fast, speed is still increasing 
						if ((abs_delta1 > drift1) || (abs_delta > (drift << 5)))
							incr = 0 - accel_bot;
						else if (abs_delta > (drift << 5)) 		// speed within acceptable range, done
							incr = 0 - accel_bot;
						else if (abs_delta1 > drift)
							incr = 0 - (accel_bot >> 1);
						else incr = 0 - (accel_bot >> 2);
					} else {														// speed is increasing slowly
						if (abs_delta < drift1) {
							incr = 0;
							bot_stat.phase4 = 0;
							bot_stat.phase6 = 1;							//   go wait a tick and see what happens
							bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/					putrsDIAG((const rom char *)"F5 ");
#endif
						} else {
							incr = 0 - (accel_bot >> 2);
						}
					}
				} else {														// Too Slow
					if (SpdDir && (delta & 0x8000) && (abs_delta > (drift << 5))) {
						incr = 0 - accel_bot;
					} else
					if (!SpdDir) {					// Too slow, and speed is decreasing
						bot_stat.phase4 = 0;
						bot_stat.phase6 = 1;							//   go wait a tick and see what happens
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"F6 ");
#endif
						return;
					} else if ((abs_delta1 < drift) || (abs_delta < drift1)) { // slower than desired, speed increasing,
																																	 //   almost there, but not changing much
						bot_stat.phase4 = 0;
						bot_stat.phase6 = 1;							//   go wait a tick and see what happens
						bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"F7 ");
#endif
						return;
					} else {														// slower than desired
						incr = 0 - ((delta > (int)(drift1 >> 1))?accel_bot:(accel_bot>>1));
					}
				}
			} else if (bot_stat.phase6) {
				if (bot_stat.tick) {
					bot_stat.tick = 0;
					if (++bot_phase6_cnt >= 3) {								// # of ticks in phase6
						if ((TooFast==1) && !SpdDir) {									// we are too fast, but coming down in speed
							bot_stat.phase6 = 0;
							bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRF ");
									dumpshort(m_bot_per);
#endif
							return;
						}
						if (TooFast) {
							if (SpdDir) {		// speed increasing
								// if we are not too far past desired speed, and not changing much, then done
								if ((abs_delta < (drift >> 2)) && (abs_delta1 < (drift >> 22))) {
									bot_stat.phase6 = 0;
									bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRG ");
#endif
									return;
								} else {
									// decrease speed some
									bot_stat.phase6 = 0;
									bot_stat.phase4 = 1;
									incr = 0 - accel_bot;
#ifdef DEBUGMSG2
/**/							putrsDIAG((const rom char *)"D2");
#endif
								}
							} else {		// TooFast, speed decreasing
								if (abs_delta < (drift << 5)) {
									bot_stat.phase6 = 0;
									bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRH ");
#endif
									return;
								} else {
									// decrease speed some
									bot_stat.phase6 = 0;
									bot_stat.phase4 = 1;
									incr = 0 - accel_bot;
#ifdef DEBUGMSG2
/**/							putrsDIAG((const rom char *)"D3");
#endif
								}
							}
						} else {			// not TooFast
							if (SpdDir && (abs_delta < (drift << 5)) && (abs_delta1 < (drift >> 1))) {
							// Too slow, speed is increaing and almost there, then done
								bot_stat.phase6 = 0;
								bot_stat.Run = 0;
#ifdef DEBUGMSG
/**/						putrsDIAG((const rom char *)"BMRI ");
#endif
								return;
							} else {
								bot_mid = d_bot_per + delta - (delta >> 2);
								bot_stat.phase6 = 0;
								bot_stat.phase5 = 1;
								incr = accel_bot;
#ifdef DEBUGMSG2
/**/							putrsDIAG((const rom char *)"E2 ");
#endif
							}
						}
					}
				} else {
					return;
				}
			} else if (bot_stat.phase7) {
				if (bot_stat.tick) {
					bot_stat.tick = 0;
					bot_stat.phase7 = 0;
					bot_stat.phase6 = 1;
					bot_phase6_cnt = 0;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"F8 ");
#endif
				} else {
					incr = accel_bot >> 2;
					if (TooFast)
						incr = 0 - incr;
				}
			}			
			if ((bot_pwm += incr) > 0x3ff0000L)
				bot_pwm = 0x3ff0000L;
		}
	}
	INTCONbits.GIE = 0;					// disable interrupts
	bot_incr = incr;
	INTCONbits.GIE = 1;
}

void SetBotPwm(void)
{
	unsigned drift, drift1, ix;
	int i;
	int incr = 0;
	int delta;

	if (ErrorShutDown) return;

	// drift is the accepted range within desired period. Once we reached steady speed, the period tends to drift.
	// As long as the value is within +- drift, we won't adjust the pwm. 
	drift = 8;
	if (m_bot_per > 0x2000U) {
		drift += (m_bot_per - 0x2000U) >> 9;
	} else {
		if (m_bot_per < 0x1800U) drift = 1;
		else
			drift -= (0x2000U - m_bot_per) >> 11;
	}
	bot_drift = drift;
	drift1 = drift << 1;
	delta = d_bot_per - m_bot_per;

	if (top_stat.BatTest) return;
	if ((d_bot_per == 0U && bot_pwm == 0L) || /*bot_stat.Hold ||*/ (feed_stat.Run && !feed_stat.Ok2Updt)) {
		// if motor stopped, do nothing
		// if in holding pattern, do nothing
	} else if (m_bot_per != d_bot_per) {
		if (((bot_pwm & 0x3ff0000L) < MINPWM) || (m_bot_per == 0U)) bot_pwm += accel_bot;
		else if (bot_stat.Run && bot_dir) {                            // only set if incr speed
			if ((bot_pwm += bot_incr) > 0x3ff0000L)
				bot_pwm = 0x3ff0000L;
		} else if (bot_stat.Run) {				// bot_dir is 0, want slower speed
			bot_stat.tick = 0;
			if (bot_stat.phase1) {
				if (m_bot_per > bot_mid) {					// if period passed mid point
					bot_stat.phase1 = 0;					//    switch to phase 2
					bot_stat.phase2 = 1;
					delta = delta >> 2;
					if (delta > 500) delta = 500;
					bot_mid = d_bot_per - delta;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"J");
#endif
				} else {
					incr = accel_bot;
				}
				if ((bot_pwm -= incr) < MINPWM) bot_pwm = MINPWM;
				// force free fall
//				SetDCPWM5(0);		// Write the new duty cycle.
//				return;
			} else if (bot_stat.phase2) {				// phase 2, wait
				if (m_bot_per > bot_mid) {
					bot_stat.phase2 = 0;
					bot_stat.phase3 = 1;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"K");
#endif
				}
				// force free fall
//				SetDCPWM5(0);		// Write the new duty cycle.
//				return;
			} else if (bot_stat.phase3) {				// phase 3, incr pwm
				if (delta < 0) {						// dropped pass desired speed
					bot_stat.Run = 0;
					bot_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRJ ");
#endif
				} else if (bot_last_delta > bot_last_delta1) {	// speed is increasing
					incr = accel_bot;
					if (delta < (int)(drift1 << 1)) {
						bot_stat.Run = 0;
						bot_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRK ");
#endif
					}
				} else if ((bot_last_delta1 - bot_last_delta) < (int)(drift >> 1)) {	// speed not changing much
					if (delta > (int)(drift << 5)) {
						bot_mid = m_bot_per + (delta >> 3);
						bot_stat.phase3 = 0;
						bot_stat.phase1 = 1;
						incr = accel_bot;
#ifdef DEBUGMSG2
/**/				putrsDIAG((const rom char *)"L");
#endif
					} else {
						bot_stat.Run = 0;
						bot_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRL ");
#endif
					}
				} else if (delta < (int)drift1) {	// we are close to desired speed
					bot_stat.Run = 0;
					bot_stat.phase3 = 0;
#ifdef DEBUGMSG
/**/							putrsDIAG((const rom char *)"BMRM ");
#endif
				} else if (delta < (int)(drift << 5)) {
					incr = 0 - (accel_bot >> 2);
				} else incr = 0 - (accel_bot >> 2) - (accel_bot >> 1);
			}
			if ((bot_pwm -= incr) < MINPWM) bot_pwm = MINPWM;
//			if (bot_pwm == MINPWM) {	// force free fall
//				SetDCPWM5(0);		// Write the new duty cycle.
//				return;
//			}
		} else if (m_bot_per < d_bot_per) {					// need lower frequency
			if ((delta  > (int)drift) || (delta > bot_last_delta)) {	// close enough, don't change
				for (i=0; i<MAX_ACC; i++) {
					if (accel_range_bot[i][ACC_DELTA] < delta)
						incr = accel_range_bot[i][ACC_CHG];
				}
				if (delta > bot_last_delta) {				// speed is increasing
					incr = accel_bot;						// even when we want it slower
				} else
				if (incr > accel_bot) incr = accel_bot;
				if ((bot_pwm -= incr) < 0) bot_pwm = 0L;
			}
		} else {										// need higher frequency
			delta = 0 - delta;
			if ((bot_pwm & 0x3ff0000L) == 0x3ff0000L) {		// if already at 100% duty cycle, don't incr
			} else {
				if (bot_dir == 0) {						// we had shot past our desired speed
					for (i=0; i<MAX_ACC; i++) {
						accel_range_bot[i][ACC_DELTA] = accel_low[i][ACC_DELTA];
						accel_range_bot[i][ACC_CHG] = accel_low[i][ACC_CHG];
					}
					bot_dir = 1;
				}
				if (delta > (int)drift) {	// close enough, don't change
					if (BallTimerOn  && (bot_last_delta < bot_last_delta1) && (delta > (int)(drift << 4))) incr = accel_bot;
					else
					for (i=0; i<MAX_ACC; i++) {
						if (accel_range_bot[i][ACC_DELTA] < delta)
							incr = accel_range_bot[i][ACC_CHG];
					}
					if (incr > accel_bot) incr = accel_bot;
					if ((bot_pwm += incr) > 0x3ff0000L)
						bot_pwm = 0x3ff0000L;
				}
			}
		}
		SetDCPWM5((unsigned int)((bot_pwm & 0x3fc0000L) >> 16));		// Write the new duty cycle.
	}
}

void SetFeedPwm(void)
{
	unsigned delta;

	if (ErrorShutDown) return;
	
	if ((feed_pwm & 0x3ff0000) != d_feed_per) {
		if (feed_pwm > d_feed_per) {					// need lower frequency
			if ((feed_pwm -= accel_feed) < d_feed_per)
				feed_pwm = d_feed_per;
			if (feed_pwm < 0)					// shouldn't happen, just in case
				feed_pwm = 0;
		} else {										// need higher frequency
			if ((feed_pwm += accel_feed) > d_feed_per)
				feed_pwm = d_feed_per;
			if (feed_pwm > 0x3ff0000)			// shouldn't happen, just in case
				feed_pwm = 0x3ff0000;
		}
//		putrsDIAG("feed pwm set ");
		SetDCPWM3((unsigned int)((feed_pwm & 0x3ff0000) >> 16));		// Write the new duty cycle.
	}
}

void SvcShots(void)
{
	long arg;
	static long feed_rate_hs, StartXTime;
	long delta;
	
	switch (CurShotState) {

	case SHOT_IDLE:
		break;
	case SHOT_RUN:				// we are supposed to start the shot
		DbgFeedType = 0;
		DbgFeedPwm = 0;
		DbgFeedPwm_d = 0;
		DbgFeedTime = 0;
		DbgBallTime = 0;
		DbgSweepTime = -1L;
		// send SWEEP motor command to slave
		BallElapsedHalfSec = 0;
		FeedTimeoutOn = 0;
		FeedEarlyOn = 0;
		StartXTime = 0;
//#ifdef DEBUGMSG
//		putrsDIAG((rom far char *) "\n\r X=");
//		dumplong(CurShot.x);
//		putrsDIAG((rom far char *) " Y=");
//		dumplong(CurShot.y);
//		putrsDIAG((rom far char *) " T=");
//		dumplong(CurShot.top);
//		putrsDIAG((rom far char *) " B=");
//		dumplong(CurShot.bot);
// 07/30/09 start
//		putrsDIAG("feedrate = ");
//		dumplong(CurShot.feedrate);
// 07/30/09 end
//#endif
		if (FirstShot) *(unsigned int *)&feed_stat = 0;
		if (top_stat.Run || bot_stat.Run || feed_stat.Run || feed_stat.Hold) {
			// if last shot not done,  stop last shot.
			*(unsigned int*)&top_stat = 0;
			*(unsigned int*)&bot_stat = 0;
			if (!feed_stat.Cont) {
				*(unsigned int*)&feed_stat = 0;
//				StopFeedMtr();
			}
//			if (!((play_mode & PM_PRESET) && (preset_type >= PRE_TWOLINE))) {
			// if personal program mode, and last serve motor (top/bot) speed is the same
			//	don't adjust
			if (play_mode & PM_PERSONAL) {
				if (LastShot.top == CurShot.top) top_stat.Hold = 1;
				if (LastShot.bot == CurShot.bot) bot_stat.Hold = 1;
			}
		}
		feed_rate_hs = FeedPwm2Sec(last_feed_pwm) << 1;		// feed rate in 1/2 seconds: 4 means 2 sec
		if ((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND) && !FirstShot) {
			// calc time to start sweep motor. We want the movement just before the ball drops, to be less
			// predictable.
			short lastx = (short)LastShot.x;
			short curx = (short)CurShot.x;
			if (lastx > curx) delta  = lastx - curx;
			else	delta = curx - lastx;
			delta = delta / 27;
			delta = delta / 4;			// assume 2 1/2 seconds from side to side
			if (delta < 2)
				delta = 2;
			else if (delta < 4)
				delta += 1;
			StartXTime = feed_rate_hs - delta;
			if (StartXTime < 0) StartXTime = 0;
//	putrsDIAG((rom far char*)"StartXTime =");
//	dumplong(StartXTime);
		}
		if ((StartXTime < 2) || !((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND))) {
			arg = CurShot.x;
			BuildCmd(SC_X1, 1, &arg);
			StartXTime = 0;
			DbgSweepTime = 0;
// v1.206 start
			putrsDIAG("\n\rBuildCmd(SC_X1)");	// v1.214 commented
// v1.206 end
		}
		++CurShotState;

		BallElapsedTicks = HALF_TICK_CNT;
		BallElapsedHalfSec = 0;
		BallTimerOn = 1;
// v1.200 start
		putrsDIAG("\n\rBallTimerOn = 1");	// v1.214 commented
// v1.200 end
// v1.194 & v1.195 start
//		LargeSpdDrop = 0;
// v1.214 start
		if (!(play_mode & PM_MANUAL)) {					// For Manual mode, LargeSpdDrop is cleared after a new random feed rate
// For Manual mode, LargeSpdDrop is cleared after a new random feed rate.
// For 2-Line mode, LargeSpdDrop is cleared after serve motors are detected as speeding up following a ball throw.
//		if (!(play_mode & PM_MANUAL) && (!((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)))) {
// v1.214 end
			LargeSpdDrop = 0;							// is selected.
			putrsDIAG("\n\rLargeSpdDrop_A = 0");		// v1.214 commented
		}
// v1.194 & v1.195 end

		// if both serve motors are running, set a timeout to start feeding balls
		// if first shot after PLAY button pressed, let serve motors come to speed before feeding balls
		if (m_top_per && m_bot_per && !FirstShot) {
			if ((play_mode & PM_PRESET) && (preset_type >= PRE_TWOLINE)) {
				FeedTimeoutOn = 1;
				if (feed_rate_hs == 4) {									// if 2 second feed rate
					d_feed_per = last_feed_pwm << 16;				// start the motor now
//					putrsDIAG(" *1 ");
				}
			} else if (play_mode & PM_PERSONAL) {
				FeedEarlyOn = 1;
			}
		}
		if ((StartXTime < 2) || !((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND)))
		break;
	case SHOT_SNDY:				// X1 command already sent, except for Fully Random
		// send ELEV motor command to slave
		arg = CurShot.y + elev_corr.s;
		putrsDIAG("\n\rarg = ");
		dumplong(arg);
		BuildCmd(SC_Y1, 1, &arg);
		++CurShotState;
		break;
	case SHOT_STRTTOP:	
		// start top serve motor
#ifdef DEBUGMSG
		putrsDIAG((rom far char *)" StartTop");
#endif
// v1.207 start
		putrsDIAG("\n\rSHOT_STRTTOP:");	// v1.214 commented
		if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	
			if (LargeSpdDrop == 1) {
				CurShotState = SHOT_DONE;	// For 2-Line mode, need to end shot quickly, right after ball is thrown.
				break;
			}
		}
// v1.207 end
//**		top_stat.Hold = 0;
//**		if (CurShot.top != d_top_per)
		if (!top_stat.Hold)
			StartTop(CurShot.top);
		if (((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND)) && (StartXTime >= 2)) {
			if ((BallElapsedHalfSec >= (unsigned long)StartXTime)) {
				arg = CurShot.x;
				BuildCmd(SC_X1, 1, &arg);
				StartXTime = 0;
//				putrsDIAG("SM:");
//				dumpshort(BallElapsedHalfSec);
				DbgSweepTime = BallElapsedHalfSec;
			}		
		}		
		++CurShotState;
		break;
	case SHOT_STRTBOT:
		// start bot serve motor
#ifdef DEBUGMSG
		putrsDIAG((rom far char *)" StartBot");
#endif
//**		bot_stat.Hold = 0;
//**		if (CurShot.bot != d_bot_per)
		if (!bot_stat.Hold) {
			StartBot(CurShot.bot);
		}
		if (FeedTimeoutOn || FeedEarlyOn) {
//			StartFeed(last_feed_pwm);
			StartFeed();
			if (FeedTimeoutOn) {
				d_feed_per = last_feed_pwm << 16;			// start feed motor regardless
//					putrsDIAG(" *2 ");
			}
		} 
		if ((StartXTime >= 2) && (play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND)) {
			if ((BallElapsedHalfSec >= (unsigned long)StartXTime)) {
				arg = CurShot.x;
				BuildCmd(SC_X1, 1, &arg);
				StartXTime = 0;	
				CurShotState = SHOT_DLYSTAT;
//				putrsDIAG("SM:");
//				dumpshort(BallElapsedHalfSec);
				SlvCmdDelay = 10U;
				DbgSweepTime = BallElapsedHalfSec;
			} else {
// 07/30/09 start
//				putrsDIAG("\n\rBSR = ");
//				dumphex(BSR);
//				BSR = 6;
// 07/30/09 end
				CurShotState = SHOT_WAIT_SNDX;
			}	
			} else {
		++CurShotState;
			}	
		if (play_mode & PM_MANUAL) {
			// manual mode never go to SHOT_DONE
			LastShot.top = CurShot.top;
			LastShot.bot = CurShot.bot;
			LastShot.x	 = CurShot.x;
			LastShot.y	 = CurShot.y;
		}
		break;
	case SHOT_SNDSTAT:
		// ask for motor status from slave
// v1.206 start
//		putrsDIAG("\n\rSHOT_SNDSTAT:");	// v1.214 commented
		if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	
			if (LargeSpdDrop == 1) {
				CurShotState = SHOT_DONE;	// For 2-Line mode, need to end shot quickly, right after ball is thrown.
				break;
			}
		}
// v1.206 end
		arg = 0;
		BuildCmd(SC_MS, 0, &arg);
		stat.WaitResp = 1;
		++CurShotState;
		SlvCmdDelay = 10U;
		break;
	case SHOT_WAITSTAT:
		// wait for response to motor status command
// v1.206 start
//		putrsDIAG("\n\rSHOT_WAITSTAT:");	// v1.214 commented
		if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	
			if (LargeSpdDrop == 1) {
				CurShotState = SHOT_DONE;	// For 2-Line mode, need to end shot quickly, right after ball is thrown.
				break;
			}
		}
// v1.206 end
		switch (GetMtrResponse()) {
		case 2:										// slave response not command complete
			if (--SlvCmdDelay == 0U) {
				SlvTimeOut();
				CurShotState = SHOT_SNDSTAT;
			}
			break;
		case 1:										// both motors done
				if (!FirstShot &&(((play_mode & PM_PRESET) && (preset_type >= PRE_TWOLINE)) || (play_mode & PM_PERSONAL))) {
					PUTRSDIAG((const rom char *)"BDMD "); 
					if (BallTimerOn == 0)
						CurShotState = SHOT_FEED;
				} else if (play_mode & ~PM_ELIMMODE) {
					// if not test shot, start the feed motor
//				putrsDIAG(" *0 ");					
//					if (FirstShot) StartFeedAtSpd();
					if (FirstShot) {
						StartFeedAtSpd();	// start feed motor when serve motors at speed
					}
					else {
						StartFeed();
					}
				} else {
					StartFeedAtSpd();		// start feed motor when serve motors at speed
//				putrsDIAG(" **1 ");
				}
				FirstShot = 0;
			CurShotState = SHOT_FEED;
			break;
		case 0:
			SlvCmdDelay = 10U;
			CurShotState = SHOT_DLYSTAT;
			break;
		case 3:										// Sweep motor error
		case 4:										// Elevation motor eror
		case 5:										// Sweep and Elevation motors error
			CurShotState = SHOT_DONE;
			StopTopMtr();
			StopFeedMtr();
			StopBotMtr();
			break;
		}
		break;
	case SHOT_DLYSTAT:
// v1.206 start
//		putrsDIAG("\n\rSHOT_DLYSTAT:");	// v1.214 commented
		if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	
			if (LargeSpdDrop == 1) {
				CurShotState = SHOT_DONE;	// For 2-Line mode, need to end shot quickly, right after ball is thrown.
				break;
			}
		}
// v1.206 end
		if (--SlvCmdDelay == 0U) {
			CurShotState = SHOT_SNDSTAT;
			// if random modes, check if ball had been fed
			if (((play_mode & PM_PRESET) && (preset_type >= PRE_TWOLINE)) || (play_mode & PM_PERSONAL)) {
				if (BallTimerOn == 0)
					CurShotState = SHOT_FEED;
			}
		}
		break;
	case SHOT_FEED:																			// feed motor continuously on
		if (d_feed_per == 0 && last_feed_pwm == 0) 	// feed motor  not on, shot is done (preposition)
			CurShotState = 0;
		if (feed_stat.Slow || (feed_stat.Cont && !feed_stat.Hold && !feed_stat.Run)) {						// ball had been release, feed motor back to slow
#ifdef notdef
			if (feed_stat.Slow)
				putrsDIAG((rom far char *)"S");
			if (feed_stat.Hold)
				putrsDIAG((rom far char *)"H");
			if (feed_stat.Run)
				putrsDIAG((rom far char *)"R");
			if (feed_stat.Cont)
				putrsDIAG((rom far char *)"C");
#endif
			CurShotState = SHOT_DONE;
// 07/03/09 start
//			putrsDIAG("\n\rBSR = ");
//			dumphex(BSR);
// 07/03/09 end
		}
		break;
	case SHOT_DONE:
// v1.206 start
		putrsDIAG("\n\rSHOT_DONE:");	// v1.214 commented
// v1.206 end
#ifdef DEBUGMSG
//		if ((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND)) {
		if ((play_mode & PM_PRESET) && (preset_type >= PRE_TWOLINE)) {
			switch(DbgFeedType) {	
			case 1:
				PUTRSDIAG("FRR ");
				break;
			case 2:
				PUTRSDIAG("H ");
				break;
			case 3:
				PUTRSDIAG("HE ");	
				break;
			case 4:
				PUTRSDIAG("FR ");
				dumplong(DbgFeedPwm);
				dumplong(DbgFeedPwm_d);
			}
			dumplong(DbgFeedTime);
			dumplong(DbgSweepTime);
			dumplong(DbgBallTime);
		}
		PUTRSDIAG((rom far char *)"Done");
#endif
		CurShotState = SHOT_IDLE;
		LastShot.x = CurShot.x;
		LastShot.y = CurShot.y;
		LastShot.top = CurShot.top;
		LastShot.bot = CurShot.bot;
		break;
	case SHOT_WAIT_SNDX:
		if (BallTimerOn == 0) {
			CurShotState = SHOT_DONE;
		} else if ((BallElapsedHalfSec >= (unsigned long)StartXTime) || (BallTimerOn == 0)) {
			arg = CurShot.x;
			BuildCmd(SC_X1, 1, &arg);
			CurShotState = SHOT_DLYSTAT;
			SlvCmdDelay = 10U;
//			putrsDIAG("SM: ");
//			dumpshort(BallElapsedHalfSec);
			DbgSweepTime = BallElapsedHalfSec;
		}
		break;
	case SHOT_INIT:										// power on init, set elevation limit
		arg = ELEVATION_LIMIT;
		BuildCmd(SC_YL,1,&arg);
		++CurShotState;
		break;
	case SHOT_INIT1:									// home sweep motor
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//			arg = home_loc.sweep;
//		else
//			arg = home_loc_CLUB.sweep;	// CLUB model only
#ifndef PHENOMS
			arg = home_loc.sweep;
#else
			arg = home_loc_CLUB.sweep;	// CLUB models only
#endif
// v1.219 end
		BuildCmd(SC_XH, 1, &arg);
		++CurShotState;
		break;
		
	case SHOT_HOME_Y:
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2))	// all models except CLUBs
//			arg = home_loc.elev;
//		else
//			arg = home_loc_CLUB.elev;	// CLUB model only
#ifndef PHENOMS
			arg = home_loc.elev;
#else
			arg = home_loc_CLUB.elev;	// CLUB models only
#endif
// v1.219 end
		BuildCmd(SC_YH, 1, &arg);					// home elevation motor
		++CurShotState;
		break;
	case SHOT_SLOWTOP:
		// start top serve motor
// 07/03/09 start
//		if (CurShot.top == 0) {
		if (CurShot.top == 0UL) {					// top serve motor doesn't start without the "UL"
// 07/03/09 end
			StopTopMtr();
		} else StartTop(CurShot.top);						// !!! this need to be set up before hand for the slowest speed
		++CurShotState;
		break;
	case SHOT_SLOWBOT:
		// start bot serve motor
// 07/03/09 start
//		if (CurShot.bot == 0) {
		if (CurShot.bot == 0UL) {
// 07/03/09 end
			StopBotMtr();
		} else StartBot(CurShot.bot);						// !!! this need to be set up before hand for the slowest speed
		++CurShotState;
		break;
	case SHOT_INITSTAT:
		// ask for motor status from slave
		arg = 0;
		BuildCmd(SC_MS, 0, &arg);
		stat.WaitResp = 1;
		++CurShotState;
		SlvCmdDelay = 10U;
		break;
	case SHOT_INITWAIT:
		// wait for response to motor status command
		switch (GetMtrResponse()) {
		case 2:										// slave response not command complete
//$$			PUTRSDIAG((rom far char *)"B");
			if (--SlvCmdDelay == 0U) {
				SlvTimeOut();
				CurShotState = SHOT_INITSTAT;
			}
			break;
		case 1:										// both motors done
//$$			PUTRSDIAG((rom far char *)"I");
			CurShotState = SHOT_IDLE;
			break;
		case 0:	
//$$			PUTRSDIAG((rom far char *)"D");
			SlvCmdDelay = 10U;
			CurShotState = SHOT_INITDLY;
			break;
		case 3:										// sweep motor error
		case 4:										// elevation motor error
		case 5:										// both sweep and elevation motors error
			CurShotState = SHOT_IDLE;
			StopTopMtr();
			StopFeedMtr();
			StopBotMtr();
			break;
		}
		break;
	case SHOT_INITDLY:
		if (--SlvCmdDelay == 0U)
		CurShotState = SHOT_INITSTAT;
		break;

	case SHOT_RAN_INIT:							// init random horizontal
		// send ELEV motor command to slave
#ifdef DEBUGMSG
		putrsDIAG((rom far char *) " X=");
		dumplong(CurShot.x);
		dumplong(CurShot.xLeft);
		putrsDIAG((rom far char *) " Y=");
		dumplong(CurShot.y);
		dumplong(CurShot.yLo);
		putrsDIAG((rom far char *) " T=");
		dumplong(CurShot.top);
		putrsDIAG((rom far char *) " B=");
		dumplong(CurShot.bot);
		putrsDIAG(" F=");
		dumplong(CurShot.feedrate);
#endif
		if (FirstShot || top_stat.Run || bot_stat.Run || feed_stat.Run || feed_stat.Hold) {
			*(unsigned int*)&top_stat = 0;
			*(unsigned int*)&bot_stat = 0;
			*(unsigned int*)&feed_stat = 0;
			StopFeedMtr();
		}

		// Random Horizontal: send ELEV motor command to slave
		// Random Vertical: send SWEEP motor command to slave
		if (CurShot.y == CurShot.yLo) {
			arg = CurShot.y + elev_corr.s;
			BuildCmd(SC_Y1, 1, &arg);
// v1.190 start
		} else if((CurShot.y != CurShot.yLo) && (CurShot.x != CurShot.xLeft)) {	// Random Horizontal (sweep) with random elevation (triple oscillation)
			arg = CurShot.y + elev_corr.s;
			BuildCmd(SC_Y1, 1, &arg);
// It may be required to pre-position x here also, but it doesn't seem necessary.
// v1.190 end
		} else {
			arg = CurShot.x;
			BuildCmd(SC_X1, 1, &arg);
		}
		++CurShotState;
		break;

	case SHOT_RAN_STRTTOP:	
		// start top serve motor
#ifdef DEBUGMSG
		putrsDIAG((rom far char *)" StartTop");
#endif
		if (!top_stat.Hold)
			StartTop(CurShot.top);
		++CurShotState;
		break;
	case SHOT_RAN_STRTBOT:
		// start bot serve motor
#ifdef DEBUGMSG
		putrsDIAG((rom far char *)" StartBot");
#endif
		if (!bot_stat.Hold)
			StartBot(CurShot.bot);
		++CurShotState;
		break;
	case SHOT_RAN_SNDSTAT:
		// ask for motor status from slave
		arg = 0;
		BuildCmd(SC_MS, 0, &arg);
		stat.WaitResp = 1;
		++CurShotState;
		SlvCmdDelay = 10U;
		break;
	case SHOT_RAN_WAITSTAT:
		// wait for response to motor status command
		switch (GetMtrResponse()) {
		case 2:										// slave response command not complete
			if (--SlvCmdDelay == 0U) {
				SlvTimeOut();
				CurShotState = SHOT_RAN_SNDSTAT;
			}
			break;
		case 1:										// ready, both motors done
//		arg = CurShot.x;
//		BuildCmd(SC_X1, 1, &arg);
		// will need to change to specify left and right limits
			if (CurShot.y == CurShot.yLo)
				StartSweepRandom(CurShot.sweeprate, CurShot.x, CurShot.xLeft);
// v1.190 start
			else if ((CurShot.y != CurShot.yLo) && (CurShot.x != CurShot.xLeft)) {	// Random Horizontal (sweep) with random elevation (triple oscillation)
					StartElevRandom(CurShot.sweeprate, CurShot.y, CurShot.yLo);		// continuous elevation, rather than start/stop
// Either a delay or a putrsDIAG was required here if the sweep was started right after elevation or vice versa. Otherwise whichever
// motor was started 2nd, would not start oscillating. So instead, a new set of CurShot states were added to start up the sweep after
// the command that started up the elevation was done.
//					Delay1KTCYx(1);
//					putrsDIAG("\n\rOscillate Elevation");
//					StartSweepRandom(CurShot.sweeprate, CurShot.x, CurShot.xLeft);
//					putrsDIAG("\n\rOscillate Sweep");
					CurShotState = SHOT_RAN_SNDSTAT2;
					putrsDIAG("\n\rOscillate Elevation");
					break;
			}
// v1.190 end
			else
				StartElevRandom(CurShot.sweeprate, CurShot.y, CurShot.yLo);
			if (!FirstShot) {
				StartFeedCont(CurShot.feedrate);
			}
			else {
				SetFeedRate(CurShot.feedrate);
				StartFeedAtSpd();
			}
			CurShotState = SHOT_RAN_STARTFEED;
			break;
		case 0:										// motors busy
			SlvCmdDelay = 10U;
			CurShotState = SHOT_RAN_DLYSTAT;
			break;
		case 3:										// Sweep motor error
		case 4:										// Elevation motor eror
		case 5:										// Sweep and Elevation motors error
			CurShotState = SHOT_DONE;
			StopTopMtr();
			StopFeedMtr();
			StopBotMtr();
			break;
		}
		break;
	case SHOT_RAN_DLYSTAT:
		if (--SlvCmdDelay == 0U)
			CurShotState = SHOT_RAN_SNDSTAT;
		break;
// v1.190 start
	case SHOT_RAN_SNDSTAT2:
		// ask for motor status from slave
		arg = 0;
		BuildCmd(SC_MS, 0, &arg);
		stat.WaitResp = 1;
		++CurShotState;
		SlvCmdDelay = 10U;
		break;
	case SHOT_RAN_WAITSTAT2:
		// wait for response to motor status command
		switch (GetMtrResponse()) {
		case 2:										// slave response command not complete
			if (--SlvCmdDelay == 0U) {
				SlvTimeOut();
				CurShotState = SHOT_RAN_SNDSTAT2;
			}
			break;
		case 1:
// Case 1 = ready, both motors done. This never happens after the first motor starts oscillating, so now we look for case 0 and assume
// that the command to start the first motor is complete, i.e. case 2's have stopped, before we start the 2nd motor oscillating. If a
// case 1 does occur, it falls through to case 0 anyway, to start the 2nd motor oscillating.
		case 0:										// motors busy, use as last command done, occurs after case 2 goes away
			StartSweepRandom(CurShot.sweeprate, CurShot.x, CurShot.xLeft);
			putrsDIAG("\n\rOscillate Sweep");
			if (!FirstShot) {
				StartFeedCont(CurShot.feedrate);
			}
			else {
				SetFeedRate(CurShot.feedrate);
				StartFeedAtSpd();
			}
			CurShotState = SHOT_RAN_STARTFEED;
			break;
		case 3:										// Sweep motor error
		case 4:										// Elevation motor eror
		case 5:										// Sweep and Elevation motors error
			CurShotState = SHOT_DONE;
			StopTopMtr();
			StopFeedMtr();
			StopBotMtr();
			break;
		}
		break;
	case SHOT_RAN_DLYSTAT2:
		if (--SlvCmdDelay == 0U)
			CurShotState = SHOT_RAN_SNDSTAT2;
		break;
// v1.190 end

	case SHOT_RAN_STARTFEED:
		if (FirstShot) {
			// wait till feed motor started (serve motors at speed), start feed continuous
			if (!feed_stat.Hold) {
				FirstShot = 0;
				StartFeedCont(CurShot.feedrate);
				CurShotState = SHOT_RAN_LOOP;
			}
		} else CurShotState = SHOT_RAN_LOOP;
		break;
	case SHOT_RAN_LOOP:
		break;
	}
//	putrsDIAG("\n\rCurShotState = ");
//	dumplong(CurShotState);
}

// Initialization code
//
void Setup(void)
{

// C18 to XC8 Compiler Changes Start
//	unsigned *ptr;
// C18 to XC8 Compiler Changes End

	int ix;

	varinit();
	memset((char *)&stat,0,sizeof(stat));						//ptr = (unsigned *)&stat;			*ptr = 0;
	memset((char *)&top_stat, 0,sizeof(top_stat));	//ptr = (unsigned *)&top_stat;	*ptr = 0;
	memset((char *)&bot_stat, 0,sizeof(bot_stat));	//ptr = (unsigned *)&bot_stat;	*ptr = 0;
	memset((char *)&feed_stat,0,sizeof(feed_stat));	//ptr = (unsigned *)&feed_stat;	*ptr = 0;
//#ifdef FEEDBACK	// v1.186 commented
	memset((char *)&ErrorStat,0,sizeof(ErrorStat));
	ErrorShutDown = 0;
//#endif			// v1.186 commented
	TickIntCnt = 0;			// blinking cursor fix
	BallElapsedTicks = 0;
	BallElapsedHalfSec = 0;
	BallTimerOn = 0;
	FeedTimeoutOn = 0;
	FeedEarlyOn = 0;
	FirstShot = 0;
	LargeSpdDrop = 0;
	power_on = 0;
	power_state = PS_DONE;												// force initial sleep
	memset((char *)&LastShot,0, sizeof(LastShot));
	memset((char *)&CurShot,0, sizeof(CurShot));
	memset((char *)&NextShot,0,sizeof(NextShot));
	CurShotState = SHOT_IDLE;
	NextShotState = SHOT_IDLE;
	DiagBufOfs = 0;										// reset diagnostic buffer offset
	memset(DiagBuf,0,DIAG_BUF_SIZE);	// clear diagnostic buffer
	SlaveCmdOfs = 0;									// reset slave cmd buffer offset
	memset(SlaveCmd,0,DIAG_BUF_SIZE);	// clear slave cmd buffer
	SlaveResOfs = 0;									// reset slave response buffer offset
	memset(SlaveRes,0,DIAG_BUF_SIZE);	// clear slave response buffer

	WDTCONbits.SWDTEN = 1;
	LastSlaveCmd = 0;
	SlaveMotorStatus = 0;
	SpiLock = 0;
//#ifdef FEEDBACK	// v1.186 commented
	ServeDiag = 0;
//#endif			// v1.186 commented

	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;

//----------------------------------------------------------------------------------------
// Cleaned up Power On/Off code and Power button detection code.
// Be sure to enable power to other devices, before driving their pins.
	PORTAbits.RA5 = 1;					// enable +5V to enable slave
	PORTAbits.RA4 = 1;					// enable +12V
	PORTBbits.RB4 = 1;					// disable pot chip select
	PORTCbits.RC0 = 1;					// disable slave chip select
	PORTDbits.RD6 = 1;					// disable EEPROM chip select
	PORTDbits.RD7 = 1;					// disable PGA chip select
// The following line physically enables power to other devices.
	TRISA = 0xc1;		// RS,RW,E for LCD are outputs - original 0xc5;
	Delay1KTCYx(50);					// Allow power to other devices to settle for 10mS.
	ClrWdt();
//----------------------------------------------------------------------------------------
	TRISB = 0x2f;		// 0x3f; enable Pot Chip Select as output too,
	TRISC = 0x96;
	TRISD = 0x3f;
	TRISE = 0;
	TRISF = 0xff;		// outputs
	TRISG = 0x24;		// is it 0x3c? or is that the original value

	ADCON0 = 0;
	ADCON1 = 0x0e;						// Voltage reference:AVDD,AVSS
										// AN0 analog, AN1 - AN15 digital

// Cleaned up Power On/Off code and Power button detection code.
//	PORTD = 0xc0;						// disable EEPROM chip select and PGA chip select
//	PORTBbits.RB4 = 1;					// disable pot chip select

	// setup SPI port

	OpenSPI(SPI_FOSC_16, MODE_00, SMPEND);	// ESSP1, SPI Master mode, clock = Fosc/16
											// SPI bus mode 0,0
											// Input data sample at end of data out
	set_intensity();
	d_bot_per = 0U;
	d_top_per = 0U;
	m_bot_per = 0;
	m_top_per = 0;
	d_feed_per = 0;
	bot_pwm = 0L;
	top_pwm = 0L;
	bot_incr = 0;
	top_incr = 0;
	feed_pwm = 0;
	top_t1 = 0;
	top_t2 = 0;
	last_top_t1 = 0;
	last_top_t2 = 0;
	bot_t1 = 0;
	bot_t2 = 0;
//#ifdef FEEDBACK	// v1.186 commented
	last_bot_t1 = 0;
	last_bot_t2 = 0;
//#endif			// v1.186 commented
	DoDebug = 0;
	start_count = 0;
	min_count = 0;
	max_count = 0;
	rbuf_init(&di_inbuf,2);
	rbuf_init(&di_outbuf,2);

	// Timer1 and Timer2 are the clock sources for ECCP1
	// Timer3 and Timer4 are the clock sources for ECCP2,ECCP3,CCP4 and CCP5
	// We use timer4 for all the pwms (ECCP3, CCP4 and CCP5)
	//    and timer 1 form CCP1 and timer 3 for CCP2
	T3CONbits.T3CCP2 = 0;
	T3CONbits.T3CCP1 = 1;
	// ECCP3, ECCP4 and ECCP5 setup for pwm
	// Setup Timer4 to provide	19.53 Khz PWM @ 20MHz
	OpenPWM4(0xff);							// top serve motor
	OpenPWM3(0xff);							// feed motor
	OpenPWM5(0xff);							// bot serve motor
	OpenTimer4(T4_PS_1_1 & T2_POST_1_10 & TIMER_INT_ON);
	timer4_cnt = 0;

	SetDCPWM4(0);							// 0% initial duty cycle
	SetDCPWM3(0);							// 0% initial duty cycle
	SetDCPWM5(0);							// 0% initial duty cycle

	// Top serve motor capture module CCP1, Timer1
	OpenCapture1(CAPTURE_INT_ON & C1_EVERY_4_RISE_EDGE);
    TRISCbits.TRISC2=1;    //configure pin portc,2 an intput
	OpenTimer1(TIMER_INT_OFF & T1_OSC1EN_OFF & T1_SOURCE_INT & T1_PS_1_8);

	// Bot serve motor capture module CCP2, Timer3
	OpenCapture2(CAPTURE_INT_ON & C2_EVERY_4_RISE_EDGE);
    TRISCbits.TRISC1=1;    //configure pin portc,1 an input
// C18 to XC8 Compiler Changes Start
//	OpenTimer3(TIMER_INT_OFF & T3_OSC1EN_OFF & T3_SOURCE_INT & T3_PS_1_8);
	OpenTimer3(TIMER_INT_OFF & T3_SOURCE_INT & T3_PS_1_8);
//	T3_OSC1EN_OFF;
	T1CONbits.T1OSCEN=0;
// C18 to XC8 Compiler Changes End

// 07/02/09 start, moved following 2 lines down here because there was a problem with compiler version 3.31
	T3CONbits.T3CCP2 = 0;
	T3CONbits.T3CCP1 = 1;
// 07/02/09 end

	// diagnostic port
	/*
	 * Open the USART configured as
	 * 8N1, 19200 baud, in polled mode
	 * (For 57.6K baud, Sync=0,BRG16=1,BRGH=1,SPBRG=86)
	 */
	#ifdef DEBUGPORT														// v1.196 added
	baud2USART(8);															// enable 16 bit baud rate generator
	Open2USART (USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &
			USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 86);			// 57.6K baud
	TRISGbits.TRISG1 = 0;													// debug usart TXD as output
	TRISGbits.TRISG2 = 1;													// debug usart RXD as input
// v1.196 start
	#else
//	Open2USART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &	// polled mode, same as remote control UART1
//			USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 129);			// 8N1,2400 baud, same as remote control UART1
	Open2USART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &	// polled mode, same as remote control UART1
			USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 129);		// 8N1,9600 baud, same as WiFi module
	TRISGbits.TRISG1 = 0;													// debug usart TXD as output
	TRISGbits.TRISG2 = 1;													// debug usart RXD as input
	#endif
// v1.196 end
	Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &	// remote control polled mode
			USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 129);			// 8N1,2400 baud
	TRISCbits.TRISC6 = 0;													// remote control usart TXD as output
	TRISCbits.TRISC7 = 1;													// remote control usart RXD as input
	// Enable slave processor
// Cleaned up Power On/Off code and Power button detection code.
//	PORTAbits.RA5 = 1;						// enable +5V to enable slave
//	PORTAbits.RA4 = 1;						// enable +12V

	// Setup A/D converter
	ADCON2 = 0xBd;								// A/D Right justified, Acquisition=20 TAD, FOSC/16
	a2dChan = 0;									// start a2chan at 0
	a2dChan2Dump = 1;
	ADCON0 = 1;								// select chan 0, A/D module enabled
	a2dtest = 0;
//	a2dtest = 1;									// EEPROM DEBUG ONLY

	for (ix=0; ix<(MAXAMP-MINAMP+1); ix++) {
		CurSenseCt[0][ix] = 0;				// clear top serve motor current sensing elapsed time
		CurSenseCt[1][ix] = 0;				// clear bottom serve motor current sensing elapsed time
	}

//	BDM_TripPoint[SWEEP] = 266;					// default a2d for SWEEP motor is 1.3V
// v1.216 start
//	BDM_TripPoint[SWEEP] = 393;					// default a2d for SWEEP motor is 1.92v
// Moved the above assignment to the model specific assignments in main(), because the Phenom (Club) models require a higher
// over-current threshold for the sweep motor. This assignment, here in setup(), was called at power-up
// and at wake-up. Now that it is moved to main(), it is only called at power-up.
// v1.216 end
// C18 to XC8 Compiler Changes Start
	BDM_TripTime[SWEEP] = 3;					// default time for SWEEP motor is .6 sec
//	BDM_TripTime[SWEEP] = 2;					// default time for SWEEP motor is .4 sec, reduced to prevent destruction when XC8 compiler testing
// C18 to XC8 Compiler Changes End
	BDM_CurSenseCt[SWEEP] = 0;
//	BDM_TripPoint[ELEV] = 266;					// default a2d for ELEV motor is 1.3V
// C18 to XC8 Compiler Changes Start
	BDM_TripPoint[ELEV] = 393;					// default a2d for ELEV motor is 1.92v
	BDM_TripTime[ELEV] = 3;						// default time for ELEV motor is .6 sec
//	BDM_TripPoint[ELEV] = 200;					// default a2d for ELEV motor is X.XXv, reduced to prevent destruction when XC8 compiler testing
//	BDM_TripTime[ELEV] = 2;						// default time for ELEV motor is .4 sec, reduced to prevent destruction when XC8 compiler testing
// C18 to XC8 Compiler Changes End
	BDM_CurSenseCt[ELEV] = 0;
	BDM_TripPoint[FEED] = 245;					// default a2d for FEED motor is 1.2V
	BDM_TripTime[FEED] = 15;					// default time for SWEEP motor is 3 sec
	BDM_CurSenseCt[FEED] = 0;

	BAT_CurSenseCt = 0;
	BAT_TripTime = 25;							// default time for low battery is 5 sec
// v1.183 start
//	BAT_TripPoint = 532;						// default trip point, A/D count = (10.8V(47.5K/(47.5K + 150K))/5.0V * 1023 count
	BAT_TripPoint = 512;						// reduce by 0.4V, A/D count = (10.4(47.5K/(47.5K + 150K))/5.0v * 1023 count
// v1.183 end
// 11/04/09 start
//	BAT_TripPoint = 800;						// for debugging only, previous line is correct value
// 11/04/09 end

	// Set up timer0 to interrupt every 5 msec
	TMR0L = MSEC5_LSB;
	TMR0H = MSEC5_MSB;
	msec5 = TICK_CNT;
	seconds = 0;
	ticks = 0;
	repeat = AR_OFF;
	odtix = 0xffffffff;
	OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_EDGE_RISE &	T0_SOURCE_INT & T0_PS_1_1);

//	accel_feed = 0x5800;						// default acceleration for feed motor
	accel_feed = 0x480000;				// default acceleration for feed motor
	accel_top = 0x5800;						// default acceleration for top motor
	accel_bot = 0x5800;						// default acceleration for bot motor
	top_limit = 0x3ff;
	bot_limit = 0x3ff;
	feed_limit = 0x3ff;
	last_feed_pwm = feed_limit;
	feed_slow = 0;
	ball_feed = 0;
// v1.207 start
	ball_feed_bottom = 0;
// v1.207 end
	top_phase6_cnt = 0;
	top_phase3_cnt = 0;
	bot_phase6_cnt = 0;
	bot_phase3_cnt = 0;
	top_timer_cnt = 0;
	bot_timer_cnt = 0;

	for (ix=0; ix<MAX_ACC; ix++) {
		accel_range[ix][ACC_DELTA] = accel_low[ix][ACC_DELTA];
		accel_range[ix][ACC_CHG] = accel_low[ix][ACC_CHG];
		accel_range_bot[ix][ACC_DELTA] = accel_low[ix][ACC_DELTA];
		accel_range_bot[ix][ACC_CHG] = accel_low[ix][ACC_CHG];
	}

	// Enable interrupts
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
	INTCON2bits.INTEDG0 = 0;	// make falling edge (released->pressed)
//	INTCONbits.INT0IE = 1;		Cleaned up Power On/Off code and Power button detection code.
//	INTCONbits.INT0IF = 0;		Cleaned up Power On/off code and Power button detection code.
	PUTRSDIAG(crlf);
	PUTRSDIAG(ready);	

// C18 to XC8 Compiler Changes Start
//	OpenLCD((unsigned char)(EIGHT_BIT & LINES_5X7));	// DAN UNIQUE
	OpenXLCD((unsigned char)(EIGHT_BIT & LINES_5X7));	// DAN UNIQUE
// C18 to XC8 Compiler Changes End
	PUTRSDIAG("after openLCD ");
}


// Debounce the Power Button
//	CAUTION: This code depends upon power.count being updated via the TMR0 interrupt
//					 service routine. This occurs indrectly in main when the stat.TickInt
//					 flag is processed.
//
void debounce_power_button(void)
{
	power.current = PORTBbits.INT0;							// read current power button state
	if (power.current == (uchar)power.previous) {			// if button is unchanged
// v1.221 start
		if (power.count >= (uchar)STABLE_TICKS) {				//	and it has been for long enough
// v1.221 end
			power.previous_debounced = power.debounced;//save current as prior value
			power.debounced = power.current;				//	 set new power button value
		} else {																	//  else (not yet long enough)
		}																			//	 nothing else to do
	} else {																		// else (button changed)
// C18 to XC8 Compiler Changes Start
//		if (dpb_thrash < 0x32768U) dpb_thrash++;		//	DEBUG - count of bounces
		if (dpb_thrash < 32768) dpb_thrash++;		//	DEBUG - count of bounces
// C18 to XC8 Compiler Changes End
// v1.221 start
// If power button state changed from a short power button press to a power button release and battery checking has started,
// then show the battery level screen. Had to add the batt_cnt qualifier to keep the battery level screen from coming on
// at Power-On. Do nothing if this is an A/C machine (Phenom).
		if ((power_state == PS_SYSTEM_ON) && (power.current == POWER_RELEASED) && (power.count > 5) && (batt_cnt != 0)) {
#ifndef PHENOMS
			BatLevelDisplay = 1;
			lcd_updated = 1;
#endif
		}
// v1.221 end
		power.count = 0;													//	restart the stability count
	}
	power.previous = power.current;							// update instantaneous prior value
}


//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void)
{
	char i;
// v1.219 start
	char j;
	char k;
// v1.219 end
	unsigned long eecopy;
	unsigned long tmp1,tmp2,tmp3;
	long tmp5 = ELEVATION_ADJUST;
	long *args = &tmp5;
// v1.185 start
// v1.181 start
//	long sweep_home_offset = 0x438;	// Exaggerated count to see change.
//	long sweep_home_offset = 0xf0;	// Current value in slave code.
//	long *args2 = &sweep_home_offset;
// v1.181 end
	long sweep_offset;
// v1.185 end
	long tmp4;
	int TopDelta, BotDelta;
	char c;
// v1.211 start
//	char key, rky;
// v1.223 start
	char key, rky, rky2;
//	signed char key, rky, rky2;		// C18 to XC8 compiler change
// v1.223 end
// v1.211 end
	char sum;
// ELO4CHG #1.1 (start) 
// v1.221 start
	char startit;
// v1.221 end
// ELO4CHG #1.1 (end) 

//	varinit();
// v1.199 start
// Moved the following initialization here so it only occurs at battery connect, not wakeup.
// It was located in varinit() which was called from Setup() which is called at battery connect and wakeup.
	manual.speed = DEF_SPEED; 				// defaults parameters for manual program
	manual.elev =  DEF_ELEV;
	manual.horiz = DEF_SWEEP;
	manual.feed =  DEF_FEED;
	manual.spin =  DEF_SPIN;
// v1.208 start
//	for (i=0; i<10; i++) {				// init defined presets, 0 thru 9 presets
	for (i=0; i < NUMPRESETS; i++) {	// init defined presets, now 16 presets, changed to NUMPRESETS, since it is defined already
// v1.208 end
		preset[i].speed = DEF_SPEED;
		preset[i].elev	= DEF_ELEV;
		preset[i].horiz = DEF_SWEEP;
		preset[i].twolnrw = 0;
		preset[i].feed	= DEF_FEED;
		preset[i].spin	=  DEF_SPIN;
		preset[i].col	=  0;
		preset[i].row	=  0;
		preset[i].width =  0;
	}
// v1.199 end

	Setup();								// Setup peripherals and software

// C18 to XC8 Compiler Changes Start
// Replaced all of the #message directives with #warning directives
// C18 to XC8 Compiler Changes End
// v1.208 start
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

#if defined JS	// model is jumper selectable 10/02/09 added
	#warning ***** COMPILING FOR JUMPER SELECTABLE MODEL *****
	#warning *
	#warning *
	#warning *
// v1.199 start
//	if(PORTBbits.RB2 && !PORTBbits.RB3)								// jumper 0 out, jumper 1 in
//		model_type = EL05;
//	else if(PORTBbits.RB1 && PORTBbits.RB2 && PORTBbits.RB3)		// jumper "Ball" out, jumper 0 out, jumper 1 out
//		model_type = EL05LE;
//	else if(!PORTBbits.RB2 && PORTBbits.RB3)						// jumper 0 in, jumper 1 out
//		model_type = EL04;
//	else if(!PORTBbits.RB2 && !PORTBbits.RB3)						// jumper 0 in, jumper 1 in
//		model_type = CLUB;
//	else if(!PORTBbits.RB1 && PORTBbits.RB2 && PORTBbits.RB3)		// jumper "Ball" in, jumper 0 out, jumper 1 out
//		model_type = EL05LE2;

// v1.219 start, changes to model options because Grand models and Phenom models are now compiled separately, due to program code space constraints.
#ifndef PHENOMS
	#warning ***** COMPILING FOR GRAND MODELS *****
	#warning *
	#warning *
	#warning *
												   					//	 Jumpers
																	//	 =======
																	//	J2	W5	W4				Model Selected
																	//	===========				==============
	if(PORTBbits.RB1 && PORTBbits.RB3 && PORTBbits.RB2)				//	out	out	out				New EL05LE
		model_type = EL05LE2;										//
	else if(PORTBbits.RB1 && PORTBbits.RB3 && !PORTBbits.RB2)		//	out	out	in				EL04
		model_type = EL04;											//
	else if(PORTBbits.RB1 && !PORTBbits.RB3 && PORTBbits.RB2)		//	out	in	out				EL05
		model_type = EL05;											//
//	else if(PORTBbits.RB1 && !PORTBbits.RB3 && !PORTBbits.RB2)		//	out	in	in				Phenom 1 (Club with EL04 panel)
//		model_type = CLUB1;											//
//	else if(!PORTBbits.RB1 && PORTBbits.RB3 && PORTBbits.RB2)		//	in	out	out				Phenom 2 (Club with new EL05LE panel)
//		model_type = CLUB2;											//
	else if(!PORTBbits.RB1 && PORTBbits.RB3 && !PORTBbits.RB2)		//	in	out	in				Old EL05LE
		model_type = EL05LE;										//
	else model_type = EL04;											//	wrong jumper setting defaults to EL04 model

#else
	#warning ***** COMPILING FOR PHENOM MODELS *****
	#warning *
	#warning *
	#warning *
	if(PORTBbits.RB1 && !PORTBbits.RB3 && !PORTBbits.RB2)			//	out	in	in				Phenom 1 (Club with EL04 panel)
		model_type = CLUB1;											//
	else if(!PORTBbits.RB1 && PORTBbits.RB3 && PORTBbits.RB2)		//	in	out	out				Phenom 2 (Club with new EL05LE panel)
		model_type = CLUB2;											//
	else model_type = CLUB1;										//	wrong jumper setting defaults to CLUB1 (Phenom 1) model
#endif
// v1.219 end, changes to model options because Grand models and Phenom models are now compiled separately, due to program code space constraints.

// v1.199 end
#elif defined M5	// model is EL05
	#warning ***** COMPILING FOR ELITE GRAND V EL05_MODEL *****
	#warning
	#warning
	#warning
		model_type = EL05;
#elif defined M5LE	// model is EL05LE
	#warning ***** COMPILING FOR ELITE GRAND V LE EL05LE_MODEL *****
	#warning
	#warning
	#warning
		model_type = EL05LE;
// v1.199 start
//#elif defined CLB	// model is CLUB
//	#message ***** COMPILING FOR CLUB_MODEL *****
//	#message
//	#message
//	#message
//		model_type = CLUB;
#elif defined CLB1	// model is CLUB1
	#warning ***** COMPILING FOR PHENOM_1_MODEL *****
	#warning
	#warning
	#warning
		model_type = CLUB1;
#elif defined CLB2	// model is CLUB2
	#warning ***** COMPILING FOR PHENOM_2_MODEL *****
	#warning
	#warning
	#warning
		model_type = CLUB2;
// v1.199 end
#elif defined M4	// model is EL04
	#warning ***** COMPILING FOR GRAND EL04_MODEL *****
	#warning
	#warning
	#warning
		model_type = EL04;
#elif defined M5LE2	// model is EL05LE with new panel
	#warning ***** COMPILING FOR NEW ELITE GRAND V LE EL05LE2_MODEL *****
	#warning
	#warning
	#warning
		model_type = EL05LE2;
#else
// v1.199 start
//	#warning *** CURRENT MODELS ARE CLB, M4, M5, M5LE OR JS (JUMPER SELECTABLE) ***
	#warning *** CURRENT MODELS ARE CLB1, CLB2, M4, M5, M5LE OR JS (JUMPER SELECTABLE) ***
// v1.199 end
	#error ***** A MODEL MUST BE DEFINED IN THE BUILD OPTIONS *****	
#endif
// C18 to XC8 Compiler Changes Start
//#endif
// C18 to XC8 Compiler Changes End


// v1.185 start
	if (model_type == EL04) {
		lob_model = " ELITE GRAND IV ";
		pp_maxloc = 0;
		lowbatt_msg = "Battery Depleted";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST;
// v1.205 start
		ELEV_CORR_MIN = -40;			// minimum allowable adjustment during test shot calibration
// v1.205 end
// v1.215 start
		MINPWM = 0x0ec0000L;			// 0x0ec0000/0x3ff0000 = 23% duty cycle
// v1.215 end
// v1.216 start
		BDM_TripPoint[SWEEP] = 393;		// default a2d for SWEEP motor is 1.92v
// v1.216 end
	}
	else if (model_type == EL05) {
		lob_model = " ELITE GRAND V  ";
		pp_maxloc = 6; 
		lowbatt_msg = "Battery Depleted";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST;
// v1.205 start
		ELEV_CORR_MIN = -40;			// minimum allowable adjustment during test shot calibration
// v1.205 end
// v1.215 start
		MINPWM = 0x0ec0000L;			// 0x0ec0000/0x3ff0000 = 23% duty cycle
// v1.215 end
// v1.216 start
		BDM_TripPoint[SWEEP] = 393;		// default a2d for SWEEP motor is 1.92v
// v1.216 end
	}
	else if (model_type == EL05LE) {
		lob_model = "ELITE GRAND V LE";
		pp_maxloc = 18;
		lowbatt_msg = "Battery Depleted";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST;
// v1.205 start
		ELEV_CORR_MIN = -40;			// minimum allowable adjustment during test shot calibration
// v1.205 end
// v1.215 start
		MINPWM = 0x0ec0000L;			// 0x0ec0000/0x3ff0000 = 23% duty cycle
// v1.215 end
// v1.216 start
		BDM_TripPoint[SWEEP] = 393;		// default a2d for SWEEP motor is 1.92v
// v1.216 end
	}
// v1.199 start
//	else if (model_type == CLUB) {
//		lob_model = "   CLUB MODEL   ";
//		pp_maxloc = 18;
//		lowbatt_msg = " Power Problem  ";
//		lob_version	= "Version 1.199.00";
//		sweep_offset = SWEEP_ADJUST_CLUB;
//	}
	else if (model_type == CLUB1) {
		lob_model = "    PHENOM 1    ";
		pp_maxloc = 0;
		lowbatt_msg = " Power Problem  ";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST_CLUB;
// v1.205 start
		ELEV_CORR_MIN = -136;			// minimum allowable adjustment during test shot calibration, increased low range for Clubs
// v1.205 end
// v1.215 start
		MINPWM = 0x0d48000L;			// 0x0d48000/0x3ff0000 = 20.77% duty cycle
// v1.215 end
// v1.216 start
// v1.217 start
//		BDM_TripPoint[SWEEP] = 491;		// default a2d for SWEEP motor is 2.40v
// v1.216 end
		BDM_TripPoint[SWEEP] = 655;		// default a2d for SWEEP motor is 3.20v
// v1.217 end
	}
	else if (model_type == CLUB2) {
		lob_model = "    PHENOM 2    ";
		pp_maxloc = 18;
		lowbatt_msg = " Power Problem  ";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST_CLUB;
// v1.205 start
		ELEV_CORR_MIN = -136;			// minimum allowable adjustment during test shot calibration, increased low range for Clubs
// v1.205 end
// v1.215 start
		MINPWM = 0x0d48000L;			// 0x0d48000/0x3ff0000 = 20.77% duty cycle
// v1.215 end
// v1.216 start
// v1.217 start
//		BDM_TripPoint[SWEEP] = 491;		// default a2d for SWEEP motor is 2.40v
// v1.216 end
		BDM_TripPoint[SWEEP] = 655;		// default a2d for SWEEP motor is 3.20v
// v1.217 end
	}
// v1.199 end
	else if (model_type == EL05LE2) {
		lob_model = "ELITE GRAND V LE";
		pp_maxloc = 18;
		lowbatt_msg = "Battery Depleted";
		lob_version	= "Version 1.226.00";	// Do not change this format or smartphone apps will not receive correct machine firmware version.
		sweep_offset = SWEEP_ADJUST;
// v1.205 start
		ELEV_CORR_MIN = -40;			// minimum allowable adjustment during test shot calibration
// v1.205 end
// v1.215 start
		MINPWM = 0x0ec0000L;			// 0x0ec0000/0x3ff0000 = 23% duty cycle
// v1.215 end
// v1.216 start
// C18 to XC8 Compiler Changes Start
		BDM_TripPoint[SWEEP] = 393;		// default a2d for SWEEP motor is 1.92v
//		BDM_TripPoint[SWEEP] = 200;		// default a2d for SWEEP motor is X.XXv, reduced to prevent destruction when testing XC8 compiler
// C18 to XC8 Compiler Changes End
// v1.216 end
	}
// v1.185 end

//	eeprom2flash(TopDelta,args);
	elev_corr.c[0] = SEERead((long)&flash_header.elev_correction - (long)&flash_header) & 0xff;
	elev_corr.c[1] = SEERead(((long)&flash_header.elev_correction - (long)&flash_header) + 1)&0xff;
//	dumplong(elev_corr.s);					// v1.193 uncommented
	while(1) {	                    		// Loop forever
		ClrWdt();							// Clear the WDT
		if (bot_timer_cnt) {
			INTCONbits.GIE = 0;
			bot_timer_cnt = 0;
			INTCONbits.GIE = 1;
			UpdBotPer();			//	process bottom spin motor
			CalcBotPwm();
		}
		if (top_timer_cnt) {
			INTCONbits.GIE = 0;
			top_timer_cnt = 0;
			INTCONbits.GIE = 1;
			UpdTopPer();			//	process top spin motor
			CalcTopPwm();
		}
		if (stat.TickInt) {					// if got a tick interrupt
			if (repeat > AR_REPEAT) repeat--;
			keyhold++;
			++ticks;						//	keep count of them (up to a 32 bits in size)
			power.count++;					//  maintain power button debounce tick counter
			switch(ticks & 0x03) {			//	split processing into up to 4 phases
			case 0:							//	first phase processing
//#ifdef FEEDBACK	// v1.186 commented
				if (ErrorShutDown != 0) {
					if (ErrorShutDown == 1) {
						stop_all(0, (long *)0);
						if (ErrorStat.TopCnt)
							PUTRSDIAG((rom char far *)"Top Serve Motor Error\r\n");
						if (ErrorStat.BotCnt)
							PUTRSDIAG((rom char far *)"Bottom Serve Motor Error\r\n");
						ErrorShutDown = 2;
						CurShotState = SHOT_IDLE;
					}
					break;
				}
//#endif			// v1.186 commented
//				UpdTopPer();			//	process top spin motor
//				DUMPSHORT(bot_timer_cnt);
//				UpdBotPer();			//	process bottom spin motor
				if (top_stat.Run || top_stat.Hold || feed_stat.Run || feed_stat.Hold || feed_stat.Cont ||
						((CurShotState > (ushort)SHOT_IDLE) && (CurShotState < (ushort)SHOT_DONE))) {
//					UpdTopPer();			//	process top spin motor
//#ifdef FEEDBACK	// v1.186 commented
					if (ErrorShutDown) break;
//#endif			// v1.186 commented
#ifdef DEBUGMSG
					if ((ticks & 0x0f) == 0U) {
/**/					dumplong(top_pwm);
//**/					dumplong(bot_pwm);
					}
/**/				dumpshort(top_last_delta);
//**/				dumpshort(bot_last_delta);
					if (top_stat.Run)		//	if top spin motor is running
						PUTRSDIAG(".");		//	 show dots
#endif
//***				if (top_stat.Run)
//***					dumplong(top_pwm);
					// if feed motor is running, and there is a large drop in serve motor speed,
					// that means the ball just fed.
					// also, if we had started the feed motor, and getting close to full speed,
					//  check for ball drop
//					if ((feed_stat.Run) || (feed_stat.Hold && (feed_pwm > 0x2800000))) {
// v1.194 & v1.195 start
// The following first line only takes care of the start/stop oscillation modes where the feed function has special requirements.
// Now we need to check for ball thrown in Manual mode and Random Vertical/Random Horizontal modes.
//					if ((feed_stat.Run) || (feed_stat.Hold && ((feed_pwm >= (long)last_feed_pwm)||(feed_pwm > 0x2800000)))) {
					if ((feed_stat.Run) || (feed_stat.Hold && ((feed_pwm >= (long)last_feed_pwm)||(feed_pwm > 0x2800000)))
					|| ((feed_stat.Cont) && (play_mode & PM_MANUAL) && (play_mode & PM_PLAYING))
					|| ((feed_stat.Cont) && (preset_type <= PRE_RANDVERT) && (play_mode & PM_PRESET) && (play_mode & PM_PLAYING))) {
// v1.194 & v1.195 end

//						 if ((top_last_delta1 - top_last_delta) > ball_feed) {
#ifdef notdef
						if (DbgTimerCnt) {
							putrsDIAG("\n\rTC:");
							dumplong(DbgTimerCnt);
							DbgTimerCnt = 0;
						}
#endif
							if (LargeSpdDrop) {
// v1.200 start
//								if ((BallElapsedHalfSec <= 1U) ||	// if just started, ignore drop in speed
//									((BallElapsedHalfSec == 2) && (BallElapsedTicks < (unsigned char)QUARTER_TICK_CNT))) {	// 1.25 second
//										putrsDIAG("\n\r");
//										dumpshort(BallElapsedHalfSec);
//										dumpshort(BallElapsedTicks);

// If in 2-Line mode with medium or narrow widths and using the fastest ball feed rate, do not ignore LargeSpdDrop flag (ball thrown)
// for as long after the feed motor is started. At 2 second feed rates we can't wait too long. 2-Line wide runs a 3 second feed rate
// when a 2 second feed rate is selected, so it is not a problem.

// v1.206 start
//								if (((!((play_mode & PM_PRESET) && preset_type == PRE_TWOLINE && preset[preset_type].width != 0
//								&& preset[preset_type].feed == 2)) && ((BallElapsedHalfSec <= 1U)
//								|| ((BallElapsedHalfSec == 2) && (BallElapsedTicks < (unsigned char)QUARTER_TICK_CNT)))) // 1.25 second
//								|| (((play_mode & PM_PRESET) && preset_type == PRE_TWOLINE && preset[preset_type].width != 0)
//								&& preset[preset_type].feed == 2 && BallElapsedHalfSec < 1U))
//								{

// For 2-Line mode, do not throw away any ball detection.
								if (!((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE))
								&& ((BallElapsedHalfSec <= 1U) ||	// if just started, ignore drop in speed
									((BallElapsedHalfSec == 2) && (BallElapsedTicks < (unsigned char)QUARTER_TICK_CNT)))) {	// 1.25 second

// v1.206 end

									putrsDIAG("\n\r");
									dumpshort(BallElapsedHalfSec);
									dumpshort(BallElapsedTicks);

// v1.200 end

// v1.194 & v1.195 start
// This routine was only used in start/stop modes, e.g. full random and 2-line modes, and helps prevent false "ball thrown" detection.
// False detections would cause machine to move to next shot without ever throwing a ball at the current location.
// "Ball thrown" detection is now also required manual mode, random horizontal mode and random vertical mode, to know when to change
// the feed rate if the new random feed function is selected. Rather than adding code to run the BallTimerOn routine so that this
// false detection check routine may be used in those modes, this check is just bypassed in those modes, because a false "ball 
// thrown" detection in those modes would only cause the feed rate to change when using the new random feed function. This is not
// seen as a problem at this time.
// v1.200 start, the following line had logic error
//									if ((!(play_mode & PM_MANUAL)) && (!(play_mode & PM_PRESET) && (preset_type <= PRE_RANDVERT))) {
									if ((!(play_mode & PM_MANUAL)) && (!((play_mode & PM_PRESET) && (preset_type <= PRE_RANDVERT)))) {
// v1.200 end
// v1.194 & v1.195 end
									LargeSpdDrop = 0;
									putrsDIAG("\n\rLargeSpdDrop_B = 0");	// v1.194 & v1.195 added
									}
								} else {
//								top_stat.Hold = 0;
//								bot_stat.Hold = 0;
								*(unsigned int *)&top_stat = 0;
								*(unsigned int *)&bot_stat = 0;
								feed_stat.Ok2Updt = 0;
//								SlowFeedMtr(1);	
								FeedMtrDone();
								BallTimerOn = 0;
// v1.200 start
								putrsDIAG("\n\rBallTimerOn = 0");	// v1.214 commented
// v1.200 end
#ifdef DEBUGMSG
							if (BallElapsedHalfSec <= 2U) {
								putrsDIAG("\n\rBF:");
								dumpshort(BallElapsedHalfSec);
								dumpshort(top_last_delta1);
								dumpshort(top_last_delta);
								dumpshort(CurShotState);
							}
#endif
								DbgBallTime = BallElapsedHalfSec;
// v1.206 start
//								LargeSpdDrop = 0;
//								putrsDIAG("\n\rLargeSpdDrop = 0");	// v1.194 & v1.195 added
								if (!((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE))) {	// Don't clear it for 2-Line mode.
									LargeSpdDrop = 0;											// Need it for SvcShots().
									putrsDIAG("\n\rLargeSpdDrop_C = 0");
								}
// v1.206 end
							}
						}
					}
				}
				if (bot_stat.Run || bot_stat.Hold || feed_stat.Run || feed_stat.Hold) {
//					UpdBotPer();				//	process bottom spin motor
//#ifdef FEEDBACK	// v1.186 commented
					if (ErrorShutDown) break;
//#endif			// v1.186 commented
// ***				dumpshort(bot_delta1);
// ***				if (bot_stat.Run)
// ***					dumplong(bot_pwm);
				}
				// if we are waiting to feed a ball, and the serve motors are at correct speed,
				//   go ahead and update feed motor speed
// v1.201 start
				if (feed_stat.Hold && !feed_stat.Cont) {
//				if (feed_stat.Hold && !feed_stat.Cont && (!(play_mode & PM_PERSONAL))) {// forget all this minutia for program mode
// v1.201 end
					ulong feed_rate_hs = FeedPwm2Sec(last_feed_pwm) << 1;	// feed rate in 1/2 secs
					ulong StartFeedTime;									// in 1/2 seconds
// v1.201 start
//					putrsDIAG("\n\r(feed_stat.Hold && !feed_stat.Cont)");
//					putrsDIAG("\n\rfeed_rate_hs= ");
//					dumplong(feed_rate_hs);
//					putrsDIAG("\n\rlast_feed_pwm = ");
//					dumplong(last_feed_pwm);
// v1.201 end
					switch(feed_rate_hs) {
					case 4:	// 2sec
						StartFeedTime = 0;
						break;
					case 6:	// 3 sec
						StartFeedTime = 3;
						break;
					case 8: // 4 sec
						StartFeedTime = 5;
						break;
					default:
						StartFeedTime = feed_rate_hs - 3;
						break;
					}
					if (FeedTimeoutOn && (BallElapsedHalfSec >= (feed_rate_hs - 1))) {
// v1.201 start
//						putrsDIAG("\n\r(FeedTimeoutOn && (BallElapsedHalfSec >= (feed_rate_hs - 1)))");
// v1.201 end
						*(unsigned *)&top_stat = 0;
 						*(unsigned *)&bot_stat = 0;
						feed_stat.Hold = 0;
						feed_stat.Run = 1;
//						d_feed_per = (ulong)base_feeds[0] << 16;	// feed ball as soon as possible
#ifdef DEBUGMSG
                        putrsDIAG("FRR");
						dumpshort(feed_pwm >> 16);
#endif
						if (!DbgFeedType) {
							DbgFeedType = 1;
							DbgFeedTime = BallElapsedHalfSec;
						}
						FeedTimeoutOn = 0;
						FeedEarlyOn = 0;
					} else if (FeedEarlyOn && SpeedInRange() && (BallElapsedHalfSec >= StartFeedTime)) {
// v1.201 start
//						putrsDIAG("\n\r(FeedEarlyOn && SpeedInRange() && (BallElapsedHalfSec >= StartFeedTime))");
// v1.201 end
#ifdef DEBUGMSG
//						if (d_feed_per == 0) {
	                        putrsDIAG("HE ");
//						}
#endif
						if (!DbgFeedType) {
							DbgFeedType = 3;
							DbgFeedTime = BallElapsedHalfSec;
						}
						FeedEarlyOn = 0;
						if (feed_rate_hs < BallElapsedHalfSec)
							tmp4 = 2;
						else {
							tmp4 = (feed_rate_hs - BallElapsedHalfSec) >> 1;
							if (tmp4 < 2) tmp4 = 2;
						}
						d_feed_per = (ulong)base_feeds[tmp4] << 16;	// feed ball as soon as possible
// v1.200 start
//						putrsDIAG("\n\rd_feed_per #1=");
//						dumplong(d_feed_per);
// v1.200 end
//					putrsDIAG(" *3 ");						
					} else if ( !top_stat.Run && !bot_stat.Run ) {	/* }
							(top_last_delta1 == top_last_delta) && (bot_last_delta1 == bot_delta1)) { /* */
// v1.201 start
//						putrsDIAG("\n\r( !top_stat.Run && !bot_stat.Run )");
// v1.201 end
							TopDelta = (top_last_delta>0)?top_last_delta:(0-top_last_delta);
							BotDelta = (bot_last_delta>0)?bot_last_delta:(0-bot_last_delta);
          				if (( top_stat.Hold || (TopDelta < (top_drift<<5/*3*/)) || 
                        		((top_pwm & 0x3ff0000L) == 0x3ff0000L) || ((top_pwm & 0x3ff0000L) <= MINPWM)) 
                        	&& ( bot_stat.Hold || (BotDelta < (bot_drift<<5)) ||
                        		((bot_pwm & 0x3ff0000L) == 0x3ff0000L) || ((bot_pwm & 0x3ff0000L) <= MINPWM)) ) {
	            DbgFeedPwm = feed_pwm;
							feed_stat.Hold = 0;
							feed_stat.Run = 1;
							if (FeedTimeoutOn) {
								// feed motor already started, don't touch
							} else if (BallTimerOn) {
// v1.200 start
//								putrsDIAG("\n\rfeed_rate_hs=");
//								dumplong(feed_rate_hs);
//								putrsDIAG("\n\rBallElapsedHalfSec=");
//								dumplong(BallElapsedHalfSec);
// v1.200 end
								if (feed_rate_hs < BallElapsedHalfSec) 
									tmp4 = 2;
								else {
									tmp4 = (feed_rate_hs - BallElapsedHalfSec) >> 1;
									if (tmp4 < 2) tmp4 = 2;
								}
								d_feed_per = (ulong)base_feeds[tmp4 - 2] << 16;
// v1.200 start
//								putrsDIAG("\n\rd_feed_per #2=");
//								dumplong(d_feed_per);
// v1.200 end
								PUTRSDIAG(" *4 ");
							} else {
								tmp4 = (feed_rate_hs - 4) >> 1;		// minus ramp up time
								if (tmp4 < 2) tmp4 = 2;
								d_feed_per = (ulong)base_feeds[tmp4 - 2] << 16;
// v1.200 start
//								putrsDIAG("\n\rd_feed_per #3=");
//								dumplong(d_feed_per);
// v1.200 end
								PUTRSDIAG(" *5 ");
							}
							/*if (tmp4 >4)*/ feed_stat.Ok2Updt = 1;
							FeedTimeoutOn = 0;
							FeedEarlyOn = 0;
#ifdef DEBUGMSG
                        	putrsDIAG("FR");
                        	dumplong(BallElapsedHalfSec);
                        	dumplong(d_feed_per);
#endif
							DbgFeedPwm_d = d_feed_per;
							if (!DbgFeedType) {
								DbgFeedType = 4;
								DbgFeedTime = BallElapsedHalfSec;
							}
						}
					}
				}
// v1.201 start
//				else if (feed_stat.Hold && !feed_stat.Cont && (play_mode & PM_PERSONAL)) {// forget all this minutia for program mode
//					d_feed_per = last_feed_pwm << 16;
//					putrsDIAG("\n\rProgram Mode d_feed_per =");
//					dumplong(d_feed_per);
//				}					
// v1.201 end
				break;
			case 1:							// second phase
				if (CurShotState != (ushort)SHOT_IDLE) {
					SvcShots();				//	service shot handler
				}
				break;
			}
			if (BallTimerOn) {
				if (++BallElapsedTicks == 0U) {
					BallElapsedTicks = (ulong)HALF_TICK_CNT;
					++BallElapsedHalfSec;
				}
			}

        while (TickIntCnt) {					//	blinking cursor fix
//         	intoff();        					// it is ????.GIE = 0;
			INTCONbits.GIE = 0;
         	--TickIntCnt;
//         	inton();        					// ????.GIE = 1;
			INTCONbits.GIE = 1;

			if (++msec5 == 0U) {				// overflow, incr seconds
				msec5 = TICK_CNT; 
				++seconds;
//				DUMPSHORT(seconds);

//				if (cursor_state == 0)			// blinking cursor
//				cursor_state = 1;
//				else cursor_state = 0;
//				lcd_updated = 1;


//#ifdef notdef
				if (DoDebug && start_count) {
					INTCONbits.GIE = 0;
					tmp1 = start_count;
					tmp2 = min_count;
					tmp3 = max_count;
					INTCONbits.GIE = 1;
					dumpaddr(tmp1);
					dumplong(tmp2);
					dumplong(tmp3);
				}
//#endif
			}	// not currently using the third and fourth phase processing allotment


			if (++cursor_tmr == 0U) {
				cursor_tmr = cursor_tmr_preload;
				if (cursor_state == 0)			// blinking cursor
					cursor_state = 1;
				else
					cursor_state = 0;
				lcd_updated = 1;
			}

// 02/07/09 start
			if (dashes || pp_dashes) {					// If dashes are displayed, either from any preset or personal program mode,
				if (++dashes_tmr == 0U) {				// then if incremented timer expired,
					dashes_tmr = dashes_tmr_preload;	// reload timer for next time,
					dashes = 0;							// and clear byte that indicates preset mode dashes,
					pp_dashes = 0;						// and clear byte that indicates personal program dashes
					lcd_updated = 1;					// lcd needs updating
				}
			}
// 02/07/09 end

// v1.221 start
			if (BatLevelDisplay) {
				if (++BatLevelDisplayTmr == 0U) {
					BatLevelDisplayTmr = BatLevelDisplayTmrPreload;
					BatLevelDisplay = 0;
					lcd_updated = 1;
				}
			}
// v1.221 end

// v1.221 start
			if (start_delay) {											// Blink the Play LED with increasing speed until feed motor starts.
				if (++PlayLEDTmr == 0U) {								// To prevent PlayLEDTmr from rolling over and corrupting the blinking,
					PlayLEDTmr = PlayLEDTmrPreload;
					if (PlayLEDPass < 40)								// (PlayLEDPass * 2) + PlayLEDTmrPreload must stay less than 256.
						PlayLEDPass = PlayLEDPass + 1;
					PlayLEDTmr = (2*PlayLEDPass) + PlayLEDTmrPreload;	// then increase PlayLEDTmr preload for quicker blinking.
//					if (play_mode & PM_PLAYING)							// If the Play LED is on,
//						play_mode = play_mode & 0xfe;					// then turn it off,
					if (PlayModeBlink & PM_PLAYING) {					// If the Play LED is on,
						PlayModeBlink = PlayModeBlink & 0xfe;			// then turn it off,
					}
					else {
//						play_mode = play_mode | PM_PLAYING;				// else turn it on.
						PlayModeBlink = PlayModeBlink | PM_PLAYING;		// else turn it on.
					}
					BuildCmd('3',1,&PlayModeBlink);						// Toggle the Play LED.
				}
			} else {
				PlayLEDPass = 0;
				PlayLEDTmr = PlayLEDTmrPreload;
			}
// v1.221 end

		}										// blinking cursor fix


			if (stat.WaitResp) {								// waiting for response from slave
				ChipSelect(LOB_SLAVE);
				c = ReadSPI();
//$$				dumphex(c);
				ChipDeSelect(LOB_SLAVE);
				if ((SlaveResOfs != (char)0) || (c == (char)'R')) {
					SlaveRes[SlaveResOfs++] = c;
					if (SlaveResOfs >= DIAG_BUF_SIZE) {
						stat.WaitResp = 0;
						PUTRSDIAG((rom far char *)">");
					} else if ((SlaveResOfs > C_CNT) && (SlaveRes[C_CNT] <= SlaveResOfs)) {
//$$						PUTRSDIAG((rom far char *)"=");
						stat.WaitResp = 0;
					}
				} else if (c == NAK || c == ACK) {
//					if (c==NAK)
//						PUTRSDIAG((rom far char *)"N");
//$$					else
//$$						PUTRSDIAG((rom far char *)"A");
					stat.WaitResp = 0;
				}
			}
				// get a2d value of next channel
			if (!a2dtest && (msec5 & 0x01)) {
				if (stat.a2dStarted && (ADCON0bits.NOT_DONE == FALSE)) {
					unsigned int val;
					int i,j,chan;
					val = (unsigned int)(a2dValue[a2dChan] = ((((int)ADRESH) & 0xff) << 8) | (((int)ADRESL) & 0xff));	// get 10 bit value
					ADCON0 = 0;
					if (a2dChan == 0) {
////#ifndef NOMOTORS								
						// checking battery level
// 11/04/09 start
//						dumplong(val);
// 11/04/09 end
						if (BatAvgCnt < 20) {
							CheckBattery(val);
#ifdef notdef
						if (val <= BAT_TripPoint) {
							if (++BAT_CurSenseCt >= BAT_TripTime) {
								PUTRSDIAG("Low Battery!!!!");					
								stop_all(0,(long *)0);
								DUMPSHORT(a2dChan);
								DUMPLONG(val);
								PUTRSDIAG(ShutDown);
								BAT_CurSenseCt = BAT_TripTime;	// this is to prevent rollover
//#ifdef FEEDBACK	// v1.186 commented
								ErrorStat.LoBat = 1;
								if (ErrorShutDown == 0) ErrorShutDown = 1;
//#endif			// v1.186 commented
							}
						} else BAT_CurSenseCt = 0;			// reset count
#endif
					}
						BAT_lastval = val;
////#endif //NOMOTORS
					} else if ((a2dChan == 1) || (a2dChan == 2)) {
						// checking top and bottom serve motors
						chan = a2dChan - 1;
						if (val < CurSense[0][0]) {
							// less than 4 amps, it's ok
							if (CurSenseCt[chan][0]) {
								for (i=MINAMP,j=0; i<=MAXAMP; i++,j++) {
									CurSenseCt[chan][j] = 0;
								}
							}
						} else if (val >= CurSense[MAXAMP-MINAMP+1][0]) {
							// more than 25 amps, shut down all motors at once
							PUTRSDIAG("OVERCURRENT!@!!");
							stop_all(0,(long *)0);
							PUTRSDIAG(ShutDown);
//#ifdef FEEDBACK	// v1.186 commented
							if (chan)
								ErrorStat.BotCurSense = 1;
							else
								ErrorStat.TopCurSense = 1;
							if (ErrorShutDown == 0) ErrorShutDown = 1;
//#endif			// v1.186 commented
						} else {
							for (i=MINAMP,j=0; i<MAXAMP; i++,j++) {
								if (CurSense[j][0] <= val) {
									++CurSenseCt[chan][j];
									if (CurSenseCt[chan][j] >= CurSense[j][1]) {
										// exceeded time for level of current, shut down all motors
										PUTRSDIAG("OVERCURRENT22222");
										stop_all(0,(long *)0);
										DUMPSHORT(a2dChan);
										PUTRSDIAG(ShutDown);
//#ifdef FEEDBACK	// v1.186 commented
										if (chan)
											ErrorStat.BotCurSense = 1;
										else
											ErrorStat.TopCurSense = 1;
										if (ErrorShutDown == 0) ErrorShutDown = 1;
//#endif			// v1.186 commented
									}
								} else
									CurSenseCt[chan][j] = 0;
							}
						}
						
					} else if ((a2dChan == 3) || (a2dChan == 4) || (a2dChan == 5)) {
						// checking bidirectional motors and feed motor
						chan = a2dChan - 3;
						if (val < BDM_TripPoint[chan]) {		// ok if less than TripPoint
							BDM_CurSenseCt[chan] = 0;
						} else if (++BDM_CurSenseCt[chan] > BDM_TripTime[chan]) {
							// more than 25 amps, shut down all motors at once
							PUTRSDIAG("OVERCURRENT33333");
							stop_all(0,(long *)0);
							DUMPSHORT(a2dChan);
							PUTRSDIAG(ShutDown);
//#ifdef FEEDBACK	// v1.186 commented
							if (chan == 0)
								ErrorStat.SweepCurSense = 1;
							else if (chan == 1)
								ErrorStat.ElevCurSense = 1;
							else if (chan == 2)
								ErrorStat.FeedCurSense = 1;
							if (ErrorShutDown == 0) ErrorShutDown = 1;
//#endif			// v1.186 commented
						} 
					}
					if (++a2dChan >= NumA2dChan) a2dChan = 0;
					stat.a2dStarted = 0;
				}
			}
			ClrWdt();
			stat.TickInt = 0;
		}
// v1.209 start
		#ifdef DEBUGPORT
// v1.209 end
		FillDiagBuf();	// process input from serial port for diag commands
// v1.209 start
		#endif
// v1.209 end
		if ((stat.WaitResp == 0U) && SlaveResOfs) {
			if (SlaveResOfs == 1) {
				DUMPHEX(SlaveRes[0]);
			} 
			else
//				for (c=(C_CMD+1); c<(SlaveResOfs-1); c++)
				for (c=0,sum=0; c<SlaveResOfs; c++) {	// calculate response checksum, should add up to 0
					DUMPHEX(SlaveRes[c]);
					sum += SlaveRes[c];
				}
			if (LastSlaveCmd != (char)SC_MS) {
				for (c=(C_CMD+1); c<(SlaveResOfs-1); c++) {
					DUMPHEX(SlaveRes[c]);
				}
			} else {
				if (sum == 0)							// SC_MS command, only set SlaveMotorStatus if no
					SlaveMotorStatus = SlaveRes[C_CMD+1];	// checksum error
			}
			LastSlaveCmd = 0;
			SlaveResOfs = 0;
		}
//#ifdef notdef
		debounce_power_button();
		switch (power_state) {
		case PS_SYSTEM_ON:												// System ON - operating normally
			main_ostate = power_state;
			if ((power.debounced != (unsigned char)POWER_PRESSED) ||	// if power button not just pressed
					(power.previous_debounced == (unsigned char)POWER_PRESSED)) {
				break;
			}															// else - fall through
		case PS_OFFBUT:													// Power Button Pressed (by proxy - Setup)
//			putrsDIAG("Begin OFF cycle ");
			play_mode & PM_MODE;
//			StopFeedMtr();												//	turn off feed motor while we home
			stop_all(0,(long*)0);
// v1.219 start
//			if ((model_type != CLUB1) && (model_type != CLUB2)){		// all models except CLUBs
//				CurShot.x = home_loc.sweep;								//  set home position
//				CurShot.y = home_loc.elev;
//			}
//			else {	// CLUB model only
//				CurShot.x = home_loc_CLUB.sweep;						//  set home position
//				CurShot.y = home_loc_CLUB.elev;
//			}
#ifndef PHENOMS
				CurShot.x = home_loc.sweep;								//  set home position
				CurShot.y = home_loc.elev;
#else
				CurShot.x = home_loc_CLUB.sweep;						//  set home position
				CurShot.y = home_loc_CLUB.elev;
#endif
// v1.219 end
			CurShot.top = 0;											//	and stop spin motors
			CurShot.bot = 0;
	    	strcpy(&lcd_line[0][0], lob_model); 						//	 Show Homing message  
			strcpy(&lcd_line[1][0], lob_version);
// C18 to XC8 Compiler Changes Start
			lcd_line[1][14] = flash_header.vers[5];
			lcd_line[1][15] = flash_header.vers[6];
//			lcd_line[1][10] = flash_header.vers[5];
//			lcd_line[1][11] = flash_header.vers[6];
// C18 to XC8 Compiler Changes End
// 			Code has been moved to get better reading
//			batval = a2dValue[A2D_BATTERY];
//			if (batval < 530) 		 batval = 0;
//			else if (batval > 630) batval = 100;
//			else									 batval -= 530;
//	  	chartoa(batval,&lcd_line[1][batval==100?12:13],batval==100?3:2);
	    strcpy(&lcd_line[2][0], homingmsg);
	    strcpy(&lcd_line[3][0], waitmsg);
//	    clear_lcd(0x2);
	    homeshown = 1;													//	 indicate it is now showing
	    lcd_updated = 1;
			start_home();												//	begin homing the system
			home_ticks = ticks;
			power_state = PS_WAITHOME;									//  and wait until complete
			batt_cnt = 0;												//	allow battery data collection to restart
			break;
		case PS_WAKEUP:													//	System waking up - need to home
			BuildCmd(SC_YO,1,args);
// v1.185 start
			BuildCmd(SC_XO,1,&sweep_offset);							// There are 2 places the SC_YO  command are issued, so do the same for SC_XO.
// v1.185 end
			if ((home_ticks+5) > ticks) break;
			home_ticks = 0;
			batt_ticks = 0;
			PUTRSDIAG("Begin ON home ");	
// v1.219 start
//			if ((model_type != CLUB1) && (model_type != CLUB2)){		// all models except CLUBs
//				CurShot.x = home_loc.sweep;
//				CurShot.y = home_loc.elev;
//			}
//			else {														// CLUB model only
//				CurShot.x = home_loc_CLUB.sweep;
//				CurShot.y = home_loc_CLUB.elev;
//			}


#ifndef PHENOMS
				CurShot.x = home_loc.sweep;
				CurShot.y = home_loc.elev;
#else
				CurShot.x = home_loc_CLUB.sweep;
				CurShot.y = home_loc_CLUB.elev;
#endif
// v1.219 end
			CurShot.top = SRV_MINIMUM;
			CurShot.bot = SRV_MINIMUM;
			start_home();
			power_state = PS_WAIT_ON;
			break;
		case PS_WAIT_ON:												//	System waiting for power on home
			if (CurShotState == (ushort)SHOT_IDLE) {					//	 wait until home is complete to
				power_state = PS_SYSTEM_ON;								//	 transition to required normal operation
// mjz 10/24/08, start, 11/06/08, revised
#ifndef NOMACHINE	// v1.186
				if (ErrorShutDown == 0) {								// if there is not already an error
					if (m_top_per == 0)	{								// if the motor hasn't moved since power up
						ErrorStat.TopCnt = 1;
						if (ErrorShutDown == 0)
							ErrorShutDown = 1;
					}
					if (m_bot_per == 0) {								// if the motor hasn't moved since power up
						ErrorStat.BotCnt = 1;
						if (ErrorShutDown == 0)
							ErrorShutDown = 1;
					}
				}
#endif	// v1.186
			}
// mjz 10/24/08, end
			break;
		case PS_WAITHOME:			
			if ((power.debounced == (unsigned char)POWER_PRESSED) &&
				(power.previous_debounced != (unsigned char)POWER_PRESSED)) {
//				putrsDIAG("PRESS from WAITHOME ");
			}
			if (main_ostate != power_state){
//				SPUTRSDIAG("+");						
				main_ostate = power_state;			
			}
			if (homing()) break; // wait until home is done
			stop_all(0,(long*)0);							// when it is, stop any remaining motors
			power_on = 0;											// conclude power off sequence
			if (batt_cnt == 0) {													//	if haven't started LCD display
				batt_cnt = 1;
//battmsg				batval = LastBatAvg;		// get the average
				strcpy(&lcd_line[2][0], shutdnmsg);
//battmsg		 		chartoa(batval,&lcd_line[2][batval==100?11:12],batval==100?3:2); // display once	
		 		lcd_updated = 1;
				batt_ticks = ticks + BATTSHOWDELAY;					//	 double since wont execute as much code
			}
			if (batt_ticks && (batt_ticks > ticks) && !ErrorShutDown) break;
			power_state = PS_DONE;						// finish power off processingr
			main_ostate = power_state;
			break;
		case PS_DONE:												// normal operating state
			if (power.debounced == (unsigned char)POWER_PRESSED)
				SPUTRSDIAG("PRESS from DONE");
			if (main_ostate != power_state) {
				dumplong(main_ostate);
				SPUTRSDIAG(power_on ? "DONE-ON" : "DONE-OFF");
			}				
			main_ostate = power_state;
			power_state = PS_SYSTEM_ON;
			break;
		default:
			if (power.debounced == (unsigned char)POWER_PRESSED)
				SPUTRSDIAG("PRESS from default ");
			if (main_ostate != power_state) {
				SPUTRSDIAG("-");
			}
			main_ostate  = power_state;
			break;
		}
		if ((power_on == 0) && (power_state != PS_WAITHOME)) { // If powering off and home already complete
			int glitch;
			unsigned oval;
 			SPUTRSDIAG("POWER OFF");					//	inform programmer turning OFF
 			SPUTRSDIAG(crlf);
//			INTCONbits.INT0IF = 0;		Cleaned up Power On/Off code and Power button detection code.
//			INTCONbits.INT0IE = 1;				Cleaned up Power On/Off code and Power button detection code.
			power.debounced = POWER_RELEASED;	//	force power button to release before sleep
			power.previous_debounced = POWER_RELEASED;
			power.count = 0;
			power.current = POWER_RELEASED;
			power.previous = POWER_RELEASED;
			do {															// debounce wake up interrupts
				poweroff(0,(long *)&tmp);				//	execute power off sequence
				ClrWdt();							// Clear the WDT
				WDTCONbits.SWDTEN = 1;					//	woke up - enable watchdog
// fix current leak through Pot CS				TRISB = 0x2f;	//	enable INT0 bits so we can debounce

				for (power.count=0; power.count < (unsigned char)(STABLE_TICKS+1); power.count++) {
					debounce_power_button();			//	debounce
					ClrWdt();											//	prevent watchdog reset
					Delay1KTCYx(35);							// 8.5? msec
					Delay1KTCYx(35);							// 8.5? msec
					Delay1KTCYx(35);							// 8.5? msec
					Delay1KTCYx(35);							// 8.5? msec
				}																// exits only when debounced
				INTCONbits.INT0IF=0;
			} while (power.debounced == (unsigned char)POWER_RELEASED);// go back to sleep until power was pressed

			Setup();
			eladj = 0;
			elev_corr.s = (ushort)(SEERead((long)&flash_header.elev_correction - (long)&flash_header) & 0xff);
			elev_corr.s |= (((ushort)SEERead(((long)&flash_header.elev_correction - (long)&flash_header) + 1)) << 8)& 0xff00;
			BuildCmd(SC_YO,1,&tmp5);
// v1.185 start
			BuildCmd(SC_XO,1,&sweep_offset);	// There are 2 places the SC_YO  command are issued, so do the same for SC_XO.
// v1.185 end
// v1.193 start
//			putrsDIAG("\n\r\n\r\n\rPOWER ON - ");
			putrsDIAG("\n\r\n\r\n\rWaking Up ...");
			putrsDIAG("\n\rWakeUp elev_corr.s = ");	// v1.193 added
			dumplong(elev_corr.s);					// v1.193 uncommented
// v1.193 end
// v1.201 start
//			putrsDIAG("\n\ree_progstart = ");
//			dumplong(ee_progstart);
//			putrsDIAG("\n\rsizeof(struct persprog)*30 = ");
//			dumplong (sizeof(struct persprog)*30);
// v1.201 end

			DUMPHEX(power.count);
			power.count = 0;
			DUMPHEX(power.debounced);
			DUMPHEX(power.previous_debounced);
			DUMPHEX(power.current);
			DUMPHEX(power.previous);
			DUMPHEX(dpb_thrash);
			set_intensity();
			power.previous_debounced = POWER_PRESSED;	// prevent immediate power off 
			power_on = 1;
			power_state = PS_WAKEUP;//SYSTEM_ON;					// transition to do nothing
			home_ticks = ticks;
			batt_ticks = 0;
			cur_st = ST_IDLE;
  	  prev_st = -1;
#ifdef notdef	// RB2 now used for model configuration 07/29/09
  	  if (PORTBbits.RB2 == 0U) {
//				putrsDIAG("Copying EEProm to Flash\n");
				DisableSlaveSPI();												//	keep slave from interfering		
				for (i=0  ; i<0x40; i++) {
					if ((unsigned)SEERead((unsigned int)i) != 0xffU)
						break;
				}
				EnableSlaveSPI();												//	keep slave from interfering		
				if (i != 0x40) {
//					putrsDIAG("not all FFs ");
					DisableSlaveSPI();												//	keep slave from interfering		
					for (i=0; i<0x40; i++) {
						if (SEERead(i) != 0)
							break;
					}
					EnableSlaveSPI();												//	keep slave from interfering		
					if (i != 0x40) {
//						putrsDIAG("not all 00s ");
						putrsDIAG("Copying EEProm to Flash - Please wait...\n");
			  	  eecopy = 32767;
	  			  eeprom2flash(0,(long *)&eecopy);// COPYEEPROM
	  			  eecopy = 0;
	  			  eeprom2flash(4,(long *)&eecopy);	// VerifyEEPROM
	  			  putrsDIAG("Done!!!\n");
					} else {
//						putrsDIAG("eeprom all 00s - aborting copy\n");
					}
				} else {
//					putrsDIAG("eeprom all FFs - aborting copy\n");
				}
			}
#endif	// RB2 now used for model configuration 07/29/09

		}
//#endif
		if (stat.HaveCmd)
			exec_diag();
		key = check_keypad();
// v1.221 start
		if ((key == KY_PLAY) && !(play_mode & PM_PLAYING) && (cur_st != ST_TEST)) {
			start_delay = ticks + TWENTYSEC;				// If Play pressed at control board, allow time for user to get into position.
			PlayModeBlink = play_mode;						// Copy LEDs for blinking control without affecting original LED setting.
		}			
// v1.221 end
//		if (homing() && !homeshown) {										//	if homing and message not showing
		if (homing()) {																	// if homing and message not showing
			if (!homeshown) {
				strcpy(&lcd_line[0][0], lob_model); 		//	 Show Homing message  
				strcpy(&lcd_line[1][0], lob_version);
// C18 to XC8 Compiler Changes Start
				lcd_line[1][14] = flash_header.vers[5];
				lcd_line[1][15] = flash_header.vers[6];
//				lcd_line[1][10] = flash_header.vers[5];
//				lcd_line[1][11] = flash_header.vers[6];
// C18 to XC8 Compiler Changes End
	    	strcpy(&lcd_line[2][0], homingmsg);
    		strcpy(&lcd_line[3][0], waitmsg);
  		  homeshown = 1;																//	 indicate it is now showing
		    lcd_updated = 1;
			}
		} else if (power_state == PS_SYSTEM_ON) {				// else we are no longer homing
// 11/04/09 start
// For the Club model, even though it is an AC machine (power supply), we will continue to check the battery at power-up, but just
// remove the battery display and the delay to show it. This routine may be needed if power supply issues are observed.
// 11/04/09 end
			if (batt_cnt == 0) {													//	if first pass
				lastbatval = 0;															//	 clear average battery value
				lastbata2d = 0L;
				BAT_lastval = 0;
				batt_cnt++;
				top_stat.BatTest = 1;
			}
			if (BAT_lastval &&  (batt_cnt < 11)) {				// If just read battery level,
				batval = a2dValue[A2D_BATTERY];							//	 read the battery level
//				putrsDIAG("\n\rraw batval=");		// 02/12/09
//				dumplong(batval);					// 02/12/09

				lastbata2d += batval;
//				dumplong(batval);
				BAT_lastval = 0;
				if (batval < 530) 		 batval = 0;					//	 should never be less than 530
				else if (batval > 630) batval = 100;				//	 should never be more than 630
				else									 batval -= 530;				//	 value is directly in percent
				lastbatval += batval;												//	 accumulate for average
				batt_cnt++;																	//	 number of samples
			} else if (batt_cnt == 11) {									//  when sample is complete
				batval = lastbatval / 10;										//	 compute the average
// v1.221 start
				BatPercent = batval;										// Save value for display.
// v1.221 end
//				putrsDIAG("\r\navg% batval=");		// 02/12/09
//				dumplong(batval);					// 02/12/09

				LastBatAvg = BatteryAverage(batval);
// 11/04/09 start
//				if (top_stat.BatTest) batt_ticks = ticks + BATTSHOWDELAY;
				if (top_stat.BatTest) {
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2))
//						batt_ticks = ticks + BATTSHOWDELAY;
//					else
//						batt_ticks = ticks;		// No delay for Club model, no battery.
#ifndef PHENOMS
					batt_ticks = ticks + BATTSHOWDELAY;
#else
					batt_ticks = ticks;
#endif
// v1.219 end
				}
// 11/04/09 end
				if (top_stat.BatTest && (BatAvgCnt > 2)) {
					top_stat.BatTest = 0;
// v1.219 start
//					if ((model_type != CLUB1) && (model_type != CLUB2)) {
//						strcpypgm2ram(&lcd_line[2][0], battmsg);
////	 				chartoa(batval,&lcd_line[2][LastBatAvg==100?11:12],LastBatAvg==100?3:2); // display once	
//						if (batval >= 90) 
//							strcpypgm2ram(&lcd_line[2][11], "Full");
//						if ((batval < 90) && (batval >= 70))
//							strcpypgm2ram(&lcd_line[2][11], "High");	// Changed from Good to High. 12/31/08
//						if ((batval < 70) && (batval >= 50))
//							strcpypgm2ram(&lcd_line[2][10], "Medium");	// Changed from Half to Medium, 11 to 10 start position. 12/31/08
//						if (batval < 50)
//							strcpypgm2ram(&lcd_line[2][11], "Low!");
//		 				lcd_updated = 1;
////					batt_ticks = ticks + BATTSHOWDELAY;
//					}
#ifndef PHENOMS
						strcpy(&lcd_line[2][0], battmsg);
						if (batval >= 90) 
							strcpy(&lcd_line[2][11], "Full");
						if ((batval < 90) && (batval >= 70))
							strcpy(&lcd_line[2][11], "High");	// Changed from Good to High. 12/31/08
						if ((batval < 70) && (batval >= 50))
							strcpy(&lcd_line[2][10], "Medium");	// Changed from Half to Medium, 11 to 10 start position. 12/31/08
						if (batval < 50)
							strcpy(&lcd_line[2][11], "Low!");
		 				lcd_updated = 1;
#endif
// v1.219 end
				}
//				putrsDIAG("Battery Level: ");
//				dumplong(LastBatAvg);
//					putrsDIAG("\n\r");
				batval = lastbata2d / 10;
				batt_cnt = 1;
				lastbatval = 0;
				lastbata2d = 0L;
				CheckBattery(batval);
			}
		} else {
//			putrsDIAG("Done homing, not PWRSYSON");
		}


// ============================================================================
// ============================================================================
// REMOTE CONTROL ROUTINE
// ============================================================================
// ============================================================================
// v1.196 start
		#ifdef DEBUGPORT
		if (rky = debounced_remote()) {								// if remote sent a new character
		#else
// v1.211 start
//		if ((rky = debounced_remote()) ||							// If legacy remote or
//			(rky = debounced_remote_USART2())) {					// WiFi/Bluetooth remote sent a new character
		rky = debounced_remote();									// Get legacy remote data, if any. rky will be 0, if no data present.
		rky2 = debounced_remote_USART2();							// Get WiFi/Bluetooth remote data, if any. rky2 will be 0, if no data present.
// v1.219 start
//		if (rky2) {													// If WiFi/Bluetooth remote data received,
		if (RemoteDataReceived)	 {									// If WiFi/Bluetooth remote data received,
// v1.219 end
			rky = rky2;												// and copy to variable that is used in rest of remote routine.
// The above line means that if both legacy and WiFi/Bluetooth received data exist, the WiFi/Bluetooth data will overwrite
// the legacy remote data and the legacy remote data will be lost.
// v1.219 start
			RemoteDataCnt = 0;
// v1.219 end
		}
// v1.219 start
//		if (rky) {
		if (rky || RemoteDataReceived) {							// Need to allow for receiving 0x00 for remote programming.
// v1.219 end
		#endif
// v1.211 end
// v1.196 end
// v1.213 start
			if (rky == RKY_VERSION) {								// Check here so if the user connects with WiFi/Bluetooth remote, but
				strcpy (RemoteString,lob_version);			// the remote is not enabled at the machine, the version is still
				RemoteString[14] = flash_header.vers[5];			// transmitted.
				RemoteString[15] = flash_header.vers[6];
			}
// v1.213 end
			if (remote_enabled) {									// if remote is enabled
				dumphex(rky);										// for now just display it

// v1.219 start
				if (RemotePgmMode) {								// Check from high to low order bits.

					if (rky == RKY_COMMERR) {
						RemotePgmMode = RPM_NONE;
//						strcpy (RemoteString, "Communication Error");	
						RemoteString[0] = rky;
						RemoteString[1] = 0x00;
					}

					else if (rky == RKY_SHOTTABLES) {									// Get all shot table data.
						RemotePgmMode = RPM_SHOTTABLES;
#ifndef PHENOMS
						for (i=0; i<=pp_maxloc-1; i++) {								// Different models have different max selectable location.
							for (j=0; court_locs[i][j].speed !=0; j++) {
								while(Busy2USART());
								putc2USART(i+1);										// Send location.
								while(Busy2USART());
								putc2USART(speed_trans[court_locs[i][j].speed/5]);		// Send speed mapped for display.
								while(Busy2USART());
								putc2USART(court_locs[i][j].spin + 170);				// Send spin +170 to convert to 0xAA base for app.
							}
						}
#else
						for (i=0; i<=pp_maxloc-1; i++) {								// Different models have different max selectable location.
							for (j=0; court_locs_CLUB[i][j].speed !=0; j++) {
								while(Busy2USART());
								putc2USART(i+1);										// Send location.
								while(Busy2USART());
								putc2USART(speed_trans[court_locs_CLUB[i][j].speed/5]);	// Send speed mapped for display.
								while(Busy2USART());
								putc2USART(court_locs_CLUB[i][j].spin + 170);			// Send spin +170 to convert to 0xAA base for app.
							}
						}
#endif
						while(Busy2USART());
						putc2USART(0x00);
					}


					else if (rky == RKY_GETCMD) {
						RemoteString[0] = 0xf0;
//						RemoteString[0] = 0xf1;
						RemoteString[1] = 0x00;
					}

					else if (RemotePgmMode == RPM_EDITLOCN) {
						if (rky == RKY_DELSHOT) {					// After delete shot, should app reposition shots or reload from machine?
							UDrillShots--;							// Number of shots in drill is one less.
//				 			prog_chgd = 1;
				 			for (i=RemotePgmShot; i < (AVAILSHT-1); i++) {
//					 			act_program.shot[i] = act_program.shot[i+1];
//					 			act_program.method[i] = act_program.method[i+1];
					 			RemotePgmLocn[i] = RemotePgmLocn[i+1];
//					 			RemotePgmMethod[i] = RemotePgmMethod[i+1];
					 			RemotePgmSpeed[i] = RemotePgmSpeed[i+1];
					 			RemotePgmSpin[i] = RemotePgmSpin[i+1];
					 			RemotePgmFeed[i] = RemotePgmFeed[i+1];
					 		}
				 			RemotePgmLocn[AVAILSHT-1] = INV_SHOT;
				 			RemotePgmMethod[AVAILSHT-1] = 0;
//							RemotePgmMode = RPM_START;			// Start over.
//							priorState = PP_CHANGE;
//							curState = PP_SCRN1;
//					 		load_prog(program_ix,&act_program);	// 
							strcpy (RemoteString, "Shot Deleted");
						}
						else {
							RemotePgmLocn[RemotePgmShot] = rky;
//							RemotePgmMode = RPM_START;			// Start over.
							RemoteString[0] = rky;				// Echo back the value.
							RemoteString[1] = 0x00;				// Terminate call back.
						}
						RemotePgmMode = RPM_START;				// Start over.
					}
					else if (RemotePgmMode == RPM_EDITSPEED) {
//						RemotePgmSpeed = rky;
						for (i=0;speed_trans[i] <= rky;i++) {	// Save selected speed, translated back to internal value.
							RemotePgmSpeed[RemotePgmShot] = i*5;
						}
						RemotePgmMode = RPM_START;				// Start over.
						RemoteString[0] = rky;					// Echo back the value.
						RemoteString[1] = 0x00;					// Terminate call back.
					}
					else if (RemotePgmMode == RPM_EDITSPIN) {
//						RemotePgmSpin[RemotePgmShot] = rky;
						RemotePgmSpin[RemotePgmShot] = rky-170;
						RemotePgmMode = RPM_START;				// Start over.
						RemoteString[0] = rky;					// Echo back the value.
						RemoteString[1] = 0x00;					// Terminate call back.
					}
					else if (RemotePgmMode == RPM_EDITFEED) {
						RemotePgmFeed[RemotePgmShot] = rky;
						RemotePgmMode = RPM_START;				// Start over.
						RemoteString[0] = rky;					// Echo back the value.
						RemoteString[1] = 0x00;					// Terminate call back.
					}

// ----------------------------------------------------------------------------
// Entire shot edit.
// Feed selected, done with entire edit of a shot.
// ----------------------------------------------------------------------------
					else if (RemotePgmMode == RPM_SHOTFEED) {
//						strcpy (RemoteString, "Feed Selected");
						RemotePgmFeed[RemotePgmShot] = rky;
						RemoteString[0] = rky;					// Echo back the value.
						RemoteString[1] = 0x00;					// Terminate call back.
						RemotePgmMode = RPM_START;				// Start over.
					}

// ----------------------------------------------------------------------------
// Entire shot edit.
// Spin selected, feed next.
// ----------------------------------------------------------------------------
					else if (RemotePgmMode == RPM_SHOTSPIN) {
//						RemotePgmSpin[RemotePgmShot] = rky;
						RemotePgmSpin[RemotePgmShot] = rky-170;
 						// P_FEEDMIN is 1 which represents random feed. Random feed is not yet supported in User Program mode,
						// so here use P_FEEDMIN + 1.
						RemoteString[0] = rky;					// Echo back the selected value.
						RemoteString[1] = P_FEEDMIN + 1;		// Send minimum available feed.
						RemoteString[2] = P_FEEDMAX;			// Send maximum available feed.
						RemoteString[3] = P_FEEDINC;			// Send feed increment.
						RemoteString[4] = 0x00;					// Send null to terminate.
						RemoteDataCnt = 4;
						RemotePgmMode = RPM_SHOTFEED;
					}

// ----------------------------------------------------------------------------
// Entire shot edit.
// Speed selected, spin next.
// ----------------------------------------------------------------------------
					else if (RemotePgmMode == RPM_SHOTSPEED) {
//						RemotePgmSpeed[RemotePgmShot] = rky;	// Save selected speed.
						for (i=0;speed_trans[i] <= rky;i++) {	// Save selected speed, translated back to internal value.
							RemotePgmSpeed[RemotePgmShot] = i*5;
						}
						spinmin = P_SPINMAX;
						spinmax = P_SPINMIN;
// The following methods for selecting the spin range available, based on the already selected speed,
// require that the available spins allowed are contiguous within the range.
// For example, if shot location 12 at 60mph is selected and there are valid shots in the tables for -2 and +2 spins,
// then there must be valid shots in the tables for all spins between -2 and +2 also. 
#ifndef PHENOMS				// If combined models in one program, same as- if ((model_type != CLUB1) && (model_type != CLUB2)) {
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin < spinmin) {
										spinmin = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin > spinmax) {
										spinmax = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
#else
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin < spinmin) {
										spinmin = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin > spinmax) {
										spinmax = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
#endif
//						RemoteString[0] = spinmin;			// Send minimum available spin for this location.
//						RemoteString[1] = spinmax;			// Send maximum available spin for this location.
						RemoteString[0] = rky;				// Echo back selected value.
						RemoteString[1] = spinmin+170;		// Send minimum available spin for this location.
						RemoteString[2] = spinmax+170;		// Send maximum available spin for this location.
						RemoteString[3] = P_SPININC;		// Send spin increment.
						RemoteString[4] = 0x00;				// Send null to terminate.
						RemoteDataCnt = 4;					// Not sending null terminated strings and might be sending 0x00 as real data.
						RemotePgmMode = RPM_SHOTSPIN;
					}

// ----------------------------------------------------------------------------
// Entire shot edit.
// Location selected, speed next.
// ----------------------------------------------------------------------------
					else if (RemotePgmMode == RPM_SHOTLOCN) {
						if (rky == RKY_DELSHOT) {							// After delete shot, should app reposition shots or reload from machine?
							UDrillShots--;									// Number of shots in drill is one less.
//				 			prog_chgd = 1;
				 			for (i=RemotePgmShot; i < (AVAILSHT-1); i++) {
//					 			act_program.shot[i] = act_program.shot[i+1];
//					 			act_program.method[i] = act_program.method[i+1];
					 			RemotePgmLocn[i] = RemotePgmLocn[i+1];
//					 			RemotePgmMethod[i] = RemotePgmMethod[i+1];
					 			RemotePgmSpeed[i] = RemotePgmSpeed[i+1];
					 			RemotePgmSpin[i] = RemotePgmSpin[i+1];
					 			RemotePgmFeed[i] = RemotePgmFeed[i+1];
					 		}
				 			RemotePgmLocn[AVAILSHT-1] = INV_SHOT;
				 			RemotePgmMethod[AVAILSHT-1] = 0;
							RemotePgmMode = RPM_START;			// Start over.
//							priorState = PP_CHANGE;
//							curState = PP_SCRN1;
//					 		load_prog(program_ix,&act_program);	// 
							strcpy (RemoteString, "Shot Deleted");
						}
						else {
							RemotePgmLocn[RemotePgmShot] = rky - RKY_LOCN1+1;	// Shot locations are base 1.
							speedmin = P_SPEEDMAX;
							speedmax = P_SPEEDMIN;
#ifndef PHENOMS
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {	// court_locs are base 0.
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
									speedmin = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
									speedmax = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
#else
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
									speedmin = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
									speedmax = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
#endif
//							RemoteString[0] = speedmin;					// Send minimum available speed for this location.
//							RemoteString[1] = speedmax;					// Send maximum available speed for this location.
							RemoteString[0] = rky;						// Echo back selected value.
							RemoteString[1] = speed_trans[speedmin/5];	// Send minimum available speed, translated for display, for this location.
							RemoteString[2] = speed_trans[speedmax/5];	// Send maximum available speed, translated for display, for this location.
							RemoteString[3] = P_SPEEDINC;				// Send speed increment.
							RemoteString[4] = 0x00;						// Send null to terminate.
							RemotePgmMode = RPM_SHOTSPEED;
						}
					}
// ----------------------------------------------------------------------------
// Shot number received.
// If next command byte is a court location, this is an entire shot edit, so
// send speed range next and get ready to receive the selected new speed value.
// If next command byte is Delete Shot, then delete shot from drill and shift
// shots up to fill gap.
// If next command byte is Edit Location, Edit Speed, Edit Spin, or Edit Feed,
// then only editing a single parameter of the shot so send the selectable
// range and get ready to receive a selected new value for the parameter.
// ----------------------------------------------------------------------------
//					else if (RemotePgmMode & RPM_SHOTNUM) {
					else if (RemotePgmMode == RPM_SHOTNUM) {

#ifdef notdef
						if ((rky >= RKY_LOCN1) && (rky <= (RKY_LOCN1+NUMLOCS))) {
//							RemotePgmLocn[RemotePgmShot] = rky - RKY_LOCN1;
							RemotePgmLocn[RemotePgmShot] = rky - RKY_LOCN1+1;	// Shot locations are base 1.
							speedmin = P_SPEEDMAX;
							speedmax = P_SPEEDMIN;
#ifndef PHENOMS
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {	// court_locs are base 0.
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
									speedmin = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
									speedmax = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
#else
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
									speedmin = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
									speedmax = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
								}
							}
#endif
//							RemoteString[0] = speedmin;					// Send minimum available speed for this location.
//							RemoteString[1] = speedmax;					// Send maximum available speed for this location.
							RemoteString[0] = rky;						// Echo back selected value.
							RemoteString[1] = speed_trans[speedmin/5];	// Send minimum available speed, translated for display, for this location.
							RemoteString[2] = speed_trans[speedmax/5];	// Send maximum available speed, translated for display, for this location.
							RemoteString[3] = P_SPEEDINC;				// Send speed increment.
							RemoteString[4] = 0x00;						// Send null to terminate.
							RemotePgmMode = RPM_SHOTSPEED;
						}
						else if (rky == RKY_DELSHOT) {						// After delete shot, should app reposition shots or reload from machine?
							UDrillShots--;									// Number of shots in drill is one less.
//				 			prog_chgd = 1;
				 			for (i=RemotePgmShot; i < (AVAILSHT-1); i++) {
//					 			act_program.shot[i] = act_program.shot[i+1];
//					 			act_program.method[i] = act_program.method[i+1];
					 			RemotePgmLocn[i] = RemotePgmLocn[i+1];
//					 			RemotePgmMethod[i] = RemotePgmMethod[i+1];
					 			RemotePgmSpeed[i] = RemotePgmSpeed[i+1];
					 			RemotePgmSpin[i] = RemotePgmSpin[i+1];
					 			RemotePgmFeed[i] = RemotePgmFeed[i+1];
					 		}
				 			RemotePgmLocn[AVAILSHT-1] = INV_SHOT;
				 			RemotePgmMethod[AVAILSHT-1] = 0;
							RemotePgmMode = RPM_START;	// Start over.
//							priorState = PP_CHANGE;
//							curState = PP_SCRN1;
//					 		load_prog(program_ix,&act_program);			// 
							strcpy (RemoteString, "Shot Deleted");
						}
#endif
						if (rky == RKY_EDITSHOT) {						// Edit complete shot.
							for (i=0; i<=pp_maxloc-1; i++) {			// Different models have different max selectable location.
								RemoteString[i] = i+1;					// Internal base 0 (court_locs), display base 1.
							}
							RemoteString[i] = 0x00;
							RemotePgmMode = RPM_SHOTLOCN;
						}

						else if (rky == RKY_EDITLOCN) {
#ifndef PHENOMS
//							for (i=0,k=0; i<NUMLOCS; i++) {
							for (i=0,k=0; i<=pp_maxloc-1; i++) {				// Different models have different max selectable location.
								for (j=0; court_locs[i][j].speed !=0; j++) {
									if ((court_locs[i][j].speed == RemotePgmSpeed[RemotePgmShot])
									 && (court_locs[i][j].spin == RemotePgmSpin[RemotePgmShot])){
										RemoteString[k] = i+1;	// Internal base 0 (court_locs), display base 1.
										k++;
										break;
									}
								}
							}
#else
//							for (i=0,k=0; i<NUMLOCS; i++) {
							for (i=0,k=0; i<=pp_maxloc-1; i++) {				// Different models have different max selectable location.
								for (j=0; court_locs_CLUB[i][j].speed !=0; j++) {
									if ((court_locs_CLUB[i][j].speed == RemotePgmSpeed[RemotePgmShot])
									 && (court_locs_CLUB[i][j].spin == RemotePgmSpin[RemotePgmShot])){
										RemoteString[k] = i+1;	// Internal base 0 (court_locs), display base 1.
										k++;
										break;
									}
								}
							}
#endif
							RemoteString[k] = 0x00;
							RemoteDataCnt = k;
							RemotePgmMode = RPM_EDITLOCN;
						}
						else if (rky == RKY_EDITSPEED) {
							speedmin = P_SPEEDMAX;
							speedmax = P_SPEEDMIN;
#ifndef PHENOMS
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin == RemotePgmSpin[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
										speedmin = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
									}
								}
							}
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin == RemotePgmSpin[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
										speedmax = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed;
									}
								}
							}
#else
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin == RemotePgmSpin[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed < speedmin) {
										speedmin = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
									}
								}
							}
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin == RemotePgmSpin[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed > speedmax) {
										speedmax = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed;
									}
								}
							}
#endif
							RemoteString[0] = speed_trans[speedmin/5];	// Send minimum available speed, translated for display, for this location.
							RemoteString[1] = speed_trans[speedmax/5];	// Send maximum available speed, translated for display, for this location.
							RemoteString[2] = P_SPEEDINC;				// Send speed increment.
							RemoteString[3] = 0x00;						// Send null to terminate.
							RemotePgmMode = RPM_EDITSPEED;
						}
						else if (rky == RKY_EDITSPIN) {
							spinmin = P_SPINMAX;
							spinmax = P_SPINMIN;
#ifndef PHENOMS
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin < spinmin) {
										spinmin = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
							for (i=0; court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin > spinmax) {
										spinmax = court_locs[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
#else
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin < spinmin) {
										spinmin = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
							for (i=0; court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed !=0; i++) {
								if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].speed == RemotePgmSpeed[RemotePgmShot]) {
									if (court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin > spinmax) {
										spinmax = court_locs_CLUB[RemotePgmLocn[RemotePgmShot]-1][i].spin;
									}
								}
							}
#endif
							RemoteString[0] = spinmin+170;						// Send minimum available spin for this location.
							RemoteString[1] = spinmax+170;						// Send maximum available spin for this location.
							RemoteString[2] = P_SPININC;						// Send spin increment.
							RemoteString[3] = 0x00;								// Send null to terminate.
							RemoteDataCnt = 3;									// Not sending null terminated strings and might be sending 0x00 as real data.
							RemotePgmMode = RPM_EDITSPIN;
						}
						else if (rky == RKY_EDITFEED) {
 							// P_FEEDMIN is 1 which represents random feed. Random feed is not yet supported in User Program mode,
							// so here use P_FEEDMIN + 1.
							RemoteString[0] = P_FEEDMIN + 1;	// Send minimum available feed.
							RemoteString[1] = P_FEEDMAX;		// Send maximum available feed.
							RemoteString[2] = P_FEEDINC;		// Send feed increment.
							RemoteString[3] = 0x00;				// Send null to terminate.
							RemoteDataCnt = 3;
							RemotePgmMode = RPM_EDITFEED;
						}
					}

// ----------------------------------------------------------------------------
// Entire shot edit.
// Shot number selected, location next.
// ----------------------------------------------------------------------------
//					else if (RemotePgmMode & RPM_SHOTNUM) {
//						strcpypgm2ram (RemoteString, "Shot #");
//						RemotePgmShot = rky - RKY_SHOT1;
//						RemotePgmMode = RemotePgmMode | RPM_SHOTLOCN;
//					}

// ----------------------------------------------------------------------------
// Play drill.
// ----------------------------------------------------------------------------
					else if (rky == RKY_PLAYDRL) {									// Play currently selected User Drill.
						key = KY_PLAY;
//						play_mode = (play_mode & PM_ELIMMODE) | PM_PERSONAL;		//	change to personal program
//						play_mode |= PM_PLAYING;
//						BuildCmd('3',1,&play_mode);									//	change LED accordingly
//						strcpypgm2ram (RemoteString, "Play Drill");
						RemoteString[0] = rky;										// Echo back the command value.
						RemoteString[1] = 0x00;										// Terminate the call back data.
					}

// ----------------------------------------------------------------------------
// Edit drill.
// ----------------------------------------------------------------------------
					else if (rky == RKY_EDITDRL) {					// Edit current User Drill.
					 	load_prog(program_ix,&act_program);			// Load program
	
						j = AVAILSHT;
						for (i=0; i<AVAILSHT; i++) {
//							RemoteString[i*4] = act_program.shot[i];
							RemotePgmLocn[i] = act_program.shot[i];
//							RemoteString[i*4] = RemotePgmLocn[i];
							if (RemotePgmLocn[i] == INV_SHOT) {		// Invalid shot indicates end of drill set.
								j = i;
								break;
							} else {
								RemoteString[i*4] = RemotePgmLocn[i];
							}
						} 
						UDrillShots = j;							// Number of shots used in the user drill.

#ifndef PHENOMS
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmSpeed[i] = court_locs[(act_program.shot[i])-1][act_program.method[i]].speed;
							RemoteString[i*4+1] = speed_trans[RemotePgmSpeed[i]/5];
						}
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmSpin[i] = court_locs[(act_program.shot[i])-1][act_program.method[i]].spin;
							RemoteString[i*4+2] = RemotePgmSpin[i]+170;	// Convert to 0xAA base for app.

						}
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmFeed[i] = act_program.feed[i];
							RemoteString[i*4+3] = RemotePgmFeed[i];
						}
#else
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmSpeed[i] = court_locs_CLUB[(act_program.shot[i])-1][act_program.method[i]].speed;
							RemoteString[i*4+1] = speed_trans[RemotePgmSpeed[i]/5];
						}
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmSpin[i] = court_locs_CLUB[(act_program.shot[i])-1][act_program.method[i]].spin;
							RemoteString[i*4+2] = RemotePgmSpin[i]+170;	// Convert to 0xAA base for app.
						}
//						for (i=0; i<AVAILSHT; i++) {
						for (i=0; i<j; i++) {
							RemotePgmFeed[i] = act_program.feed[i];
							RemoteString[i*4+3] = RemotePgmFeed[i];
						}
#endif
						if (i == 0)	{					// If drill doesn't have any shots yet,
//							strcpypgm2ram (RemoteString, "Empty Drill");
//							RemoteString[0] = 0x00;
//							RemoteDataCnt = 1;
							RemoteString[0] = 0xff;
							RemoteString[1] = 0x00;
							RemoteDataCnt = 1;
						} else {
							RemoteString[i*4] = 0x00;		// end of data, i got incremented before exiting loop above, so don't increment here.
							RemoteDataCnt = i*4;			// i got incremented before exiting loop above, so don't increment here.
						}
						RemotePgmMode = RPM_START;		// Next data will be shot number.
					}

// ----------------------------------------------------------------------------
// Save drill.
// ----------------------------------------------------------------------------
					else if (rky == RKY_SAVEDRL) {					// Save current User Drill.
// v1.220 start
// Changed to RemoteSaveDrill function because it is also called in the Download Drill routine now.
// This reduced the code space enough to compile and fit.
					RemoteSaveDrill();
					strcpy (RemoteString, "Drill Saved to Machine");
// v1.220 end
					}

// ----------------------------------------------------------------------------
// Select drill.
// ----------------------------------------------------------------------------
					else if ((rky >= RKY_USRDRL1) && (rky < (RKY_USRDRL1+AVAILPRG))) {
//						strcpypgm2ram (RemoteString, "User Drill Selected");
						program_ix = rky - RKY_USRDRL1;
						strcpy (RemoteString, "Custom Drill ");
// C18 to XC8 Compiler Changes Start
//						RemoteString[13] = *btoa (program_ix+1, RemoteStringVar);	// Convert program number to ascii for display.
//						RemoteString[13] = itoa ((unsigned char *)(program_ix)+1, RemoteStringVar, 10);	// Convert program number to ascii for display.
						itoa (RemoteStringVar,(int)program_ix+1,10);	// Convert program number to ascii for display.
						RemoteString[13] = RemoteStringVar[0];
// C18 to XC8 Compiler Changes End
						if (program_ix < 9) {						// program_ix is base 0, 0 thru 8 is displayed as 1 thru 9.
							RemoteString[14] = 0x00;				// end of data
							RemoteDataCnt = 14;
						} else {									// Two digits.
							RemoteString[14] = RemoteStringVar[1];
							RemoteString[15] = 0x00;				// end of data
							RemoteDataCnt = 15;
						}
						key = KY_PROG;								// Necessary?
						lcd_updated = 1;							// Necessary?
						load_prog(program_ix,&act_program);
//		 				field = PF_PROGNO;							// field may not be accessible here.
	 					curState = PP_SCRN1;
	 					priorState = PP_CHANGE;						// Redraw machine display.
					}


// ----------------------------------------------------------------------------
// Select shot within a drill.
// ----------------------------------------------------------------------------
					else if ((rky >= RKY_SHOT1) && (rky < (RKY_SHOT1+AVAILSHT))) {
						strcpy (RemoteString, "Shot Selected");
//						RemoteString[0] = rky;
//						RemoteString[1] = 0x00;
//						RemoteDataCnt = 1;
						RemotePgmShot = rky - RKY_SHOT1;
						if ((RemotePgmShot+1) > UDrillShots)		// If a shot was added to drill, (RemotePgmShot is base 0, UDrillShots is base 1)
							UDrillShots = RemotePgmShot+1;
#ifdef notdef
#ifndef PHENOMS
						RemotePgmSpeed[RemotePgmShot] = court_locs[(act_program.shot[RemotePgmShot])-1][act_program.method[RemotePgmShot]].speed;
						RemotePgmSpin[RemotePgmShot] = court_locs[(act_program.shot[RemotePgmShot])-1][act_program.method[RemotePgmShot]].spin;
						RemotePgmFeed[RemotePgmShot] = act_program.feed[RemotePgmShot];
#else
						RemotePgmSpeed[RemotePgmShot] = court_locs_CLUB[(act_program.shot[RemotePgmShot])-1][act_program.method[RemotePgmShot]].speed;
						RemotePgmSpin[RemotePgmShot] = court_locs_CLUB[(act_program.shot[RemotePgmShot])-1][act_program.method[RemotePgmShot]].spin;
						RemotePgmFeed[RemotePgmShot] = act_program.feed[RemotePgmShot];
#endif
#endif
						RemotePgmMode = RPM_SHOTNUM;
					}

// ----------------------------------------------------------------------------
// Exit user drills.
// ----------------------------------------------------------------------------
					else if (rky == RKY_XUDRILLS){
						RemotePgmMode = 0;
						key = KY_PROG;
						priorState = PP_CHANGE;
						strcpy (RemoteString, "Exit User Drills");	
//						RemoteString[0] = rky;
//						RemoteString[1] = 0x00;
//						strcatpgm2ram (RemoteString, "Exit User Drills");	
					}

// v1.220 start
// ----------------------------------------------------------------------------
// Download a user drill from mobile device.
// ----------------------------------------------------------------------------
// The drill number on the machine is already selected from previous command.
					else if (rky == RKY_DWNLDDRL) {					// Download drill from mobile device.
						RemoteString[0] = RKY_DWNLDDRL;
						RemoteString[1] = 0x00;
						while(Busy2USART());
						putc2USART(RemoteString[0]);				// First echo back the command for acknowledgement.
						while(Busy2USART());
						putc2USART(RemoteString[1]);				//

																	// Read in drill data.
						for (i=0; i<30; i++) {						// 6 shots x 5 bytes + null terminator = 31 bytes
							while (!DataRdy2USART());
							RemoteString[i] = Read2USART();
							if (RemoteString[i] == 0) {
//								UDrillShots = i/5;					// Number of shots in the drill.
								UDrillShots = (i-1)/4;				// Number of shots in the drill.
								break;
							}
						}
//						UDrillShots = RemoteString[0];
						for (i=0; i<UDrillShots; i++) {
							RemotePgmLocn[i] = RemoteString[(i*4)+1];
//							RemotePgmSpeed[i] = RemoteString[(i*4)+2];
							for (j=0; speed_trans[j]<=RemoteString[(i*4)+2]; j++) {	// Search for the downloaded speed in the translation table.
								RemotePgmSpeed[i] = j*5;							// Save shot speed, translated back to internal value.
							}
//							RemotePgmSpin[i] = RemoteString[(i*4)+3];
							RemotePgmSpin[i] = RemoteString[(i*4)+3]-170;			// Save shot spin, translated from base 0xaa to base 0.
							RemotePgmFeed[i] = RemoteString[(i*4)+4];
						}
						RemoteSaveDrill();
						RemotePgmMode = RPM_DWNLDDRL;
					}

// v1.220 end
// ----------------------------------------------------------------------------
// Enter User Drills mode, but already in User Drills mode.
// 		Occurs when Back button is pressed while in Edit Drill view.
// ----------------------------------------------------------------------------
					else if (rky == RKY_UDRILLS){	// We are here because we reloaded the drill shot sequence data from within remote program mode.
						RemotePgmMode = 0;							// Clear all flags.
//						program_ix = 0;
						RemotePgmMode = RPM_START;	// "User Drills" view.
						if (play_mode & PM_PLAYING)
							key = KY_PLAY;
						strcpy (RemoteString,"User Program Mode");
						if (!(play_mode & PM_PERSONAL)) {
							program_ix = 0;
							key = KY_PROG;
//							lcd_updated = 1;
//							lobmain(KY_PROG);
						}
						j=0;
						RemoteString[j] = 0x50;					// P 
						RemoteString[++j] = 0x31;				// 1
					 	load_prog(0,&act_program);				// Load program 1
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x32;				// 2
					 	load_prog(1,&act_program);				// Load program 2
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x33;				// 3
					 	load_prog(2,&act_program);				// Load program 3
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x34;				// 4
					 	load_prog(3,&act_program);				// Load program 4
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x35;				// 5
					 	load_prog(4,&act_program);				// Load program 5
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x36;				// 6
					 	load_prog(5,&act_program);				// Load program 6
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT) {
									RemoteString[j] = 0xaa;
								}
							}
//						} else j++;
// v1.223 start
						if (model_type == EL05LE2 || model_type == CLUB2) {			// Only models to support more than 6 user drills.
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x37;				// 7
						 	load_prog(6,&act_program);				// Load program 7
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x38;				// 8
						 	load_prog(7,&act_program);				// Load program 8
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x39;				// 9
						 	load_prog(8,&act_program);				// Load program 9
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3a;				// 10
						 	load_prog(9,&act_program);				// Load program 10
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3b;				// 11
						 	load_prog(10,&act_program);				// Load program 11
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3c;				// 12
						 	load_prog(11,&act_program);				// Load program 12
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT) {
										RemoteString[j] = 0xaa;
									}
								}
//							} else j++;
						}
// v1.223 end
	
//						RemoteString[j] = 0xff;					// end of data
//						RemoteString[++j] = 0x00;				// end of data
						RemoteString[j] = 0x00;					// end of data
//						RemoteString[++j] = 0xaa;				// end of data
	
						RemoteDataCnt = j;	

					 	load_prog(program_ix,&act_program);		// After loading and displaying all program shot sequences,
					}											// load the current program back into buffer.

// ----------------------------------------------------------------------------
					else {
						RemotePgmMode = 0;
						strcpy (RemoteString, "Clear Program Mode");	
					}				
				} // end of- if (RemotePgmMode)

// ===============================================================================
// ===============================================================================
// ===============================================================================

//				if (rky == RKY_PAUSE) {							// if PLAY pressed
				else if (rky == RKY_PAUSE) {					// if PLAY pressed
// v1.219 end
					key = KY_PLAY;								// respond as if to keypad
// V1.209 start
					if (play_mode & PM_PLAYING)
						strcpy (RemoteString, "Pause");			// PM_PLAYING bit hasn't been touched yet,
					else {
						strcpy (RemoteString, "Play");			// so compensate by reversing the logic here.
// v1.221 start
						start_delay = 0;						// Start feeding balls immediately if Play is due to remote command and not the control board Play button.
// v1.221 end
					}
// v1.209 end
// 08/05/09 start
// v1.181 start
//				} else if (rky == RKY_TEST && model_type != EL04) {		// else if TEST pressed, 15-function remote not supported by EL04
				} else if (rky == RKY_TEST) {							// else if TEST pressed
// v1.181 end
					if (play_mode & PM_PLAYING) {						// If Play is on, this is a kluge to transition from any mode in play to TEST,
						lobmain(KY_PLAY);								// because the control panel doesn't allow TEST until PLAY is toggled off.
						play_mode &= PM_REMOTE;							// turn off play and mode LEDs
					}
					key = KY_TEST;
// 08/05/09 end
// v1.203 start
				} else if (rky == RKY_MANUAL) {											// else if MANUAL pressed
					if ((play_mode & PM_PLAYING) && (!(play_mode & PM_MANUAL)))
						key = KY_PLAY;													// If playing in any other mode, then stop play.
					if (!(play_mode & PM_MANUAL)) {										// if not already in MANUAL mode
						key = KY_MANUAL;												// respond as if to keypad
// V1.209 start
						strcpy (RemoteString,"Manual Play");
// v1.209 end
					} else {															// else (already paused MANUAL
						man_parm = HORIZ_IX;
// v1.204 start
//						if (manual.horiz != 3)			// If not at maximum positive angle (should use limits variable here),
//							lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
//						else manual.horiz = -3;			// else wrap around to maximum negative sweep angle.
						if (manual.horiz == 3)			// If at max angle (right),
							manual_horiz_decr = 1;		// then start decrementing angle.
						if (manual.horiz == -3)			// If at min angle (left),
							manual_horiz_decr = 0;		// then start incrementing angle.
						if (manual_horiz_decr)			// If decrementing...
// C18 to XC8 Compiler Changes Start
//							lob_parm(KY_MINUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
//						else							// If incrementing...
//							lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
							lob_parm(KY_MINUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
						else							// If incrementing...
							lob_parm(KY_PLUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
// C18 to XC8 Compiler Changes End

// v1.204 end
						set_serve(&manual);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//						btoa(manual.horiz, RemoteStringVar);						// Convert the variable into a string.
//						itoa ((signed char)(manual.horiz), RemoteStringVar,10);						// Convert the variable into a string.
						itoa (RemoteStringVar,manual.horiz,10);						// Convert the variable into a string.
// C18 to XC8 Compiler Changes End
						strcpy (RemoteString, "Angle: ");					// Start with parameter text
						if (manual.horiz > 0)
							strcat (RemoteString, "+");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.203 end
				} else if (rky == RKY_SWEEP) {							// else if SWEEP pressed
					if ((play_mode & PM_PLAYING) && (!(play_mode & PM_PRESET) || ((play_mode & PM_PRESET) && (preset_type != PRE_RANDHORZ))))
						key = KY_PLAY;									// If playing in any other mode, then stop play.
// v1.199 start
//					preset_type = PRE_RANDHORZ;							// with random horizontal selected
// v1.199 end
					if (!(play_mode & PM_PRESET)) {						// if not already in PRESET mode
// v1.199 start
						preset_type = PRE_RANDHORZ;						// select random horizontal mode
// v1.199 end
						key = KY_PRESET;								// respond as if to keypad
// V1.209 start
						strcpy (RemoteString,"Sweep Play");
// v1.209 end
					} else {											// else (already paused PRESET)
// v1.199 start
//						preset_ix[preset_type] = 6;
//						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
						if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) { // If already in this mode,
							preset_ix[preset_type] = 6;									// then set to row index and increment,
							lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep (ROW)
// v1.209 start
//						} else preset_type = PRE_RANDHORZ;								// else put in this mode.
							strcpy (RemoteString, "Row: ");						// Start with parameter text
							if (preset[PRE_RANDHORZ].col == 0)
								strcpy (RemoteStringVar, "A");					// then parameter variable
							else if (preset[PRE_RANDHORZ].col == 1)
								strcpy (RemoteStringVar, "Random");				// then parameter variable
							else if (preset[PRE_RANDHORZ].col == 2)
								strcpy (RemoteStringVar, "C");					// then parameter variable
							else if (preset[PRE_RANDHORZ].col == 3)
								strcpy (RemoteStringVar, "B");					// then parameter variable
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
						} else {		
							preset_type = PRE_RANDHORZ;									// else put in this mode.
							strcpy (RemoteString,"Sweep Play");
						}
// v1.209 end
// v1.199 end
					}
				} else if (rky == RKY_DEPTH) {							// else if DEPTH pressed
					if ((play_mode & PM_PLAYING) && (!(play_mode & PM_PRESET) || ((play_mode & PM_PRESET) && (preset_type != PRE_RANDVERT))))
						key = KY_PLAY;
// v1.199 start
//					preset_type = PRE_RANDVERT;							// with random vertical selected
// v1.199 end
					if (!(play_mode & PM_PRESET)) {						// if not already in PRESET mode
// v1.199 start
						preset_type = PRE_RANDVERT;						// select random vertical mode
// v1.199 end
						key = KY_PRESET;								// respond as if to keypad
// V1.209 start
						strcpy (RemoteString,"Depth Play");
// v1.209 end
					} else {											// else (already paused PRESET)
// v1.199 start
//						preset_ix[preset_type] = 7;
//						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]); // select the next depth
						if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDVERT)) { // If already in this mode,
							preset_ix[preset_type] = 7;									// then set to column index and increment,
							lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]); // select the next depth
// v1.209 start
//						} else preset_type = PRE_RANDVERT;								// else put in this mode.
// C18 to XC8 Compiler Changes Start
//							btoa(preset[PRE_RANDVERT].row+1, RemoteStringVar);			// row is base 0, display is base 1 so add 1
//							itoa((signed char)(preset[PRE_RANDVERT].row+1), RemoteStringVar,10);			// row is base 0, display is base 1 so add 1
							itoa(RemoteStringVar,preset[PRE_RANDVERT].row+1,10);			// row is base 0, display is base 1 so add 1
// C18 to XC8 Compiler Changes End
							strcpy (RemoteString, "Line: ");						// Start with parameter text
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
						} else {
							preset_type = PRE_RANDVERT;									// else put in this mode.
							strcpy (RemoteString,"Depth Play");
						}
// v1.209 end
// v1.199 end
					}
				} else if (rky == RKY_RANDOM) {							// else if RANDOM pressed (used for FULLRAND)
					if ((play_mode & PM_PLAYING) && (!(play_mode & PM_PRESET) || ((play_mode & PM_PRESET) && (preset_type != PRE_FULLRAND))))
						key = KY_PLAY;
					preset_type = PRE_FULLRAND; 							//		with fully random selected
// V1.209 start
					strcpy (RemoteString,"Random Play");
// v1.209 end
//					if (cur_st != ST_PREMN) {									//		if not already in PRESET mode
					if (!(play_mode & PM_PRESET)) {
						key = KY_PRESET;												//		 respond as if to keypad
					} else if (play_mode & PM_PLAYING) {			//		else if already playing
//						key = KY_PLAY;													//		 pause the machine
						preset_ix[preset_type] = 7;
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
					} else {																	//		else (already paused PRESET)
						preset_ix[preset_type] = 7;
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
					}

				} else if (rky == RKY_2LINE) {							//	 else if TWO LINE pressed
					if ((play_mode & PM_PLAYING) && (!(play_mode & PM_PRESET) || ((play_mode & PM_PRESET) && (preset_type != PRE_TWOLINE))))
						key = KY_PLAY;
// v1.199 start
//					preset_type = PRE_TWOLINE;								// with two line selected
// v1.199 end
					if (!(play_mode & PM_PRESET)) {
// v1.199 start
						preset_type = PRE_TWOLINE;							// select two line mode
// v1.199 end
						key = KY_PRESET;									// respond as if to keypad
// V1.209 start
						strcpy (RemoteString,"2-Line Play");
// v1.209 end
					} else {												// else (already paused PRESET)
// v1.199 start
//						preset_ix[preset_type] = 8;
//						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
						if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {	// If already in this mode,
							preset_ix[preset_type] = 8;									// then set to width index and increment,
							lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
// v1.209 start
//						} else preset_type = PRE_TWOLINE;									// else put in this mode.
							strcpy (RemoteString, "Width: ");						// Start with parameter text
							if (preset[PRE_TWOLINE].width == 0)
								strcpy (RemoteStringVar, "Wide");					// then parameter variable
							else if (preset[PRE_TWOLINE].width == 1)
								strcpy (RemoteStringVar, "Medium");					// then parameter variable
							else if (preset[PRE_TWOLINE].width == 2)
								strcpy (RemoteStringVar, "Narrow");					// then parameter variable
							strcat (RemoteString, RemoteStringVar);							// and concatenate.
						} else {
							preset_type = PRE_TWOLINE;										// else put in this mode.
							strcpy (RemoteString,"2-Line Play");
						}
// v1.209 end
// v1.199 end
					}

				} else if (rky == RKY_DRILLS) {							//	 else if DRILLS key pressed
					if (play_mode & PM_PLAYING)				// If playing in any play mode,
						key = KY_PLAY;					// then turn off play.
//	Some variables that may be relevant to "loop" playing.
//					ForceShot(0,0);							// Fake a shot to force a read of new preset type.
//					FirstShot = 1;							// First shot must have correct motors before feeding ball.
//					keepfeedon = 0;							// halt feed between sets
//					preshot_ix = 0;
//					preposition = 1;

// v1.208 start
//					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type == PRE_GRINDER) || (preset_type == PRE_POWER) || (preset_type == PRE_ALLCOURT) || (preset_type == PRE_TRANSITION) || (preset_type == PRE_ATTDEF) || (preset_type == PRE_FOREONLY))))
					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type >= PRE_GRINDER))))
// v1.208 end
						toggle_play = 1;
					if (play_mode & PM_PRESET) {					// If in Preset mode,
						if (preset_type == PRE_GRINDER) {			// and if in Grinder drill,
							preset_type = PRE_POWER;				// then increment to Power Baseliner drill,
// v1.209 start
							strcpy (RemoteString,"Power Baseliner Drill");
// v1.209 end
						}
						else if (preset_type == PRE_POWER) {		// else if in Power Baseliner drill,
							preset_type = PRE_ALLCOURT;				// then increment to All-Courter drill,
// v1.209 start
							strcpy (RemoteString,"All-Courter Drill");
// v1.209 end
						}
// v1.199 start
//						else if (preset_type == PRE_ALLCOURT && model_type == EL04) {	// EL04 only supports first 3 preset drills
						else if (preset_type == PRE_ALLCOURT && ((model_type == EL04) || (model_type == CLUB1))) {
							// EL04 & CLUB1 only support first 3 preset drills
// v1.199 end
// v1.212 start
//							preset_type = PRE_GRINDER;									// go back to GRINDER
// v1.209 start
//							strcpypgm2ram (RemoteString,"Grinder Drill");
// v1.209 end

// v1.215 start
//							if (model_type == EL04) {
//								preset_type = PRE_MOONBALLER;							// go back to MOONBALLER
//								strcpypgm2ram (RemoteString,"Moonballer Drill");
//							}
//							else {
//								preset_type = PRE_GRINDER;								// CLUB1 doesn't support new drills yet.
//								strcpypgm2ram (RemoteString,"Grinder Drill");
//							}
// v1.212 end
							preset_type = PRE_MOONBALLER;							// go back to MOONBALLER
							strcpy (RemoteString,"Moonballer Drill");
// v1.215 end
						}
						else if (preset_type == PRE_ALLCOURT) {		// else if in All-Courter drill,
							preset_type = PRE_TRANSITION;			// then increment to Transition drill,
// v1.209 start
							strcpy (RemoteString,"Approach Drill");
// v1.209 end
						}
						else if (preset_type == PRE_TRANSITION) {	// else if in Transition drill,
							preset_type = PRE_ATTDEF;				// then increment to Attack/Defend drill,
// v1.209 start
							strcpy (RemoteString,"Attack/Defend Drill");
// v1.209 end
						}
						else if (preset_type == PRE_ATTDEF) {		// else if in Attack/Defend drill,
							preset_type = PRE_FOREONLY;				// then increment to Forehand Only drill,
// v1.209 start
							strcpy (RemoteString,"Forehand Plus Drill");
// v1.209 end
						}

// v1.215 start
// v1.211 start
// EL04 & CLUB1 only support first 3 preset drills. They were already qualified out for the 2nd 3 preset drills.
// Here we also qualify out the CLUB2 (PHENOM 2) for the next new 6 preset drills, because court test data has not
// been developed for this model yet.
//						else if (preset_type == PRE_FOREONLY && model_type == CLUB2) {
//							preset_type = PRE_GRINDER;									// go back to GRINDER
//							strcpypgm2ram (RemoteString,"Grinder Drill");
//						}
// v1.211 end
// v1.215 end

// v1.208 start
						else if (preset_type == PRE_FOREONLY) {		// else if in Forehand Only drill,
							preset_type = PRE_BEGINNER;				// then increment to New Drill #1,
// v1.209 start
							strcpy (RemoteString,"Beginner Drill");
// v1.209 end
						}
						else if (preset_type == PRE_BEGINNER) {		// else if in New Drill #1 drill,
							preset_type = PRE_INTERMEDIATE;			// then increment to New Drill #2,
// v1.209 start
							strcpy (RemoteString,"Intermediate Drill");
// v1.209 end
						}
						else if (preset_type == PRE_INTERMEDIATE) {	// else if in New Drill #2 drill,
							preset_type = PRE_ADVANCED;				// then increment to New Drill #3,
// v1.209 start
							strcpy (RemoteString,"Advanced Drill");
// v1.209 end
						}
						else if (preset_type == PRE_ADVANCED) {		// else if in New Drill #3 drill,
							preset_type = PRE_MOONBALLER;			// then increment to New Drill #4,
// v1.209 start
							strcpy (RemoteString,"Moonballer Drill");
// v1.209 end
						}
						else if (preset_type == PRE_MOONBALLER) {	// else if in New Drill #4 drill,
							preset_type = PRE_SLICER;				// then increment to New Drill #5,
// v1.209 start
							strcpy (RemoteString,"Slicer Drill");
// v1.209 end
						}
						else if (preset_type == PRE_SLICER) {		// else if in New Drill #5 drill,
							preset_type = PRE_LEFTY;				// then increment to New Drill #6,
// v1.209 start
							strcpy (RemoteString,"Lefty Drill");
// v1.209 end
						}
// v1.208 end
						else {										// else if not already in a drill,
							preset_type = PRE_GRINDER;				// then start with Grinder drill.
// v1.209 start
							strcpy (RemoteString,"Grinder Drill");
// v1.209 end
						}
					} else {									// If not in Preset mode
						preset_type = PRE_GRINDER;				// then start with Grinder drill,
						key = KY_PRESET;						// and enter Preset mode.
// v1.209 start
						strcpy (RemoteString,"Grinder Drill");
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_SPDUP)) {
				} else if (rky == RKY_SPDUP) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {	// personal program shot editing screen
						ppparm_ix = SPEED_IX;
						key = KY_PLUS;
//					if (play_mode & PM_PRESET) {				// if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {			// if in preset mode (playing or not)
// v1.188 end
// v1.209 start
//						if (preset_type != PRE_FULLRAND) {		// fully random randomly chooses speed
						if (preset_type < PRE_FULLRAND) {		// Fully random and drills select speed internally.
// v1.209 end
							preset_ix[preset_type] = SPEED_IX;
							lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
							set_serve(&preset[preset_type]);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//							btoa((speed_trans[preset[preset_type].speed/5]), RemoteStringVar);
//							itoa((signed char)(speed_trans[preset[preset_type].speed/5]), RemoteStringVar,10);
							itoa(RemoteStringVar,speed_trans[preset[preset_type].speed/5],10);
// C18 to XC8 Compiler Changes End
							strcpy (RemoteString, "Speed: ");					// Start with parameter text
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
						}
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = SPEED_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
						lob_parm(KY_PLUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));// Random Horizontal
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//						btoa((speed_trans[manual.speed/5]), RemoteStringVar);
//						itoa((signed char)(speed_trans[manual.speed/5]), RemoteStringVar,10);
//						itoa((unsigned char *)(speed_trans[manual.speed/5]), (int)RemoteStringVar,10);
						itoa(RemoteStringVar,speed_trans[manual.speed/5],10);
// C18 to XC8 Compiler Changes End
						strcpy (RemoteString, "Speed: ");					// Start with parameter text
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_SPDDN)) {
				} else if (rky == RKY_SPDDN) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {	// personal program shot editing screen
						ppparm_ix = SPEED_IX;
						key = KY_MINUS;
//					if (play_mode & PM_PRESET) {				// if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {			// if in preset mode (playing or not)
// v1.188 end
// v1.209 start
//						if (preset_type != PRE_FULLRAND) {		// fully random randomly chooses speed
						if (preset_type < PRE_FULLRAND) {		// Fully random and drills select speed internally.
// v1.209 end
							preset_ix[preset_type] = SPEED_IX;
							lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
							set_serve(&preset[preset_type]);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//							btoa((speed_trans[preset[preset_type].speed/5]), RemoteStringVar);
//							itoa((signed char)(speed_trans[preset[preset_type].speed/5]), RemoteStringVar,10);
							itoa(RemoteStringVar,speed_trans[preset[preset_type].speed/5],10);
// C18 to XC8 Compiler Changes End
							strcpy (RemoteString, "Speed: ");					// Start with parameter text
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
						}
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = SPEED_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_MINUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV|PRE_FEED|PRE_SPIN));// Random Horizontal
						lob_parm(KY_MINUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV|PRE_FEED|PRE_SPIN));// Random Horizontal
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//						btoa((speed_trans[manual.speed/5]), RemoteStringVar);
//						itoa((signed char)(speed_trans[manual.speed/5]), RemoteStringVar,10);
						itoa(RemoteStringVar,speed_trans[manual.speed/5],10);
// C18 to XC8 Compiler Changes End
						strcpy (RemoteString, "Speed: ");					// Start with parameter text
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_SPNUP)) {	// if not personal program and SPIN + pressed
				} else if (rky == RKY_SPNUP) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {	// personal program shot editing screen
						ppparm_ix = SPIN_IX;
						key = KY_PLUS;
//					if (play_mode & PM_PRESET) {				// if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {			// if in preset mode (playing or not)
// v1.188 end
// v1.209 start
//						if (preset_type != PRE_FULLRAND) {		// fully random randomly chooses spin
						if (preset_type < PRE_FULLRAND) {		// Fully random and drills select spin internally.
// v1.209 end
							preset_ix[preset_type] = SPIN_IX;	// select spin parameter and fake + button
							lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
							set_serve(&preset[preset_type]);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//							btoa(preset[preset_type].spin, RemoteStringVar);
//							itoa((signed char)(preset[preset_type].spin), RemoteStringVar,10);
							itoa(RemoteStringVar,preset[preset_type].spin,10);
// C18 to XC8 Compiler Changes End
							strcpy (RemoteString, "Spin: ");						// Start with parameter text
							if (preset[preset_type].spin > 0)
								strcat (RemoteString, "+");
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
						}
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = SPIN_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV|PRE_FEED|PRE_SPIN));		// manual
						lob_parm(KY_PLUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV|PRE_FEED|PRE_SPIN));		// manual
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//						btoa(manual.spin, RemoteStringVar);
						itoa(RemoteStringVar,manual.spin,10);
// C18 to XC8 Compiler Changes End
						strcpy (RemoteString, "Spin: ");					// Start with parameter text
						if (manual.spin > 0)
							strcat (RemoteString, "+");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_SPNDN)) {
				} else if (rky == RKY_SPNDN) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {	// personal program shot editing screen
						ppparm_ix = SPIN_IX;
						key = KY_MINUS;
//					if (play_mode & PM_PRESET) {				// if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {			// if in preset mode (playing or not)
// v1.188 end
// v1.209 start
//						if (preset_type != PRE_FULLRAND) {		// fully random randomly chooses spin
						if (preset_type < PRE_FULLRAND) {		// Fully random and drills select spin internally.
// v1.209 end
							preset_ix[preset_type] = SPIN_IX;
							lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
							set_serve(&preset[preset_type]);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//							btoa(preset[preset_type].spin, RemoteStringVar);
//							itoa((signed char)(preset[preset_type].spin), RemoteStringVar,10);
							itoa(RemoteStringVar,preset[preset_type].spin,10);
// C18 to XC8 Compiler Changes End
							strcpy (RemoteString, "Spin: ");						// Start with parameter text
							if (preset[preset_type].spin > 0)
								strcat (RemoteString, "+");
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
						}
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = SPIN_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_MINUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// manual mode
						lob_parm(KY_MINUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// manual mode
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
// C18 to XC8 Compiler Changes Start
//						btoa(manual.spin, RemoteStringVar);
//						itoa((signed char)(manual.spin), RemoteStringVar,10);
						itoa(RemoteStringVar,manual.spin,10);
// C18 to XC8 Compiler Changes End
						strcpy (RemoteString, "Spin: ");						// Start with parameter text
						if (manual.spin > 0)
							strcat (RemoteString, "+");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_FDUP)) {
				} else if (rky == RKY_FDUP) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {	// personal program shot editing screen
						ppparm_ix = FEED_IX;
						key = KY_PLUS;
//					if (play_mode & PM_PRESET) {																	//  if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {																	//  if in preset mode (playing or not)
// v1.188 end
						preset_ix[preset_type] = FEED_IX;
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
						rem_feed_chg = 1;
// v1.209 start
						strcpy (RemoteString, "Feed: ");							// Start with parameter text
						if(preset[preset_type].feed == P_FEEDMIN) {
							strcpy (RemoteStringVar, "Random");					// and concatenate the new variable string.
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
						}
						else  {
// C18 to XC8 Compiler Changes Start
//							btoa(preset[preset_type].feed, RemoteStringVar);
//							itoa((signed char)(preset[preset_type].feed), RemoteStringVar,10);
							itoa(RemoteStringVar,preset[preset_type].feed,10);
// C18 to XC8 Compiler Changes End
							strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
						}
// v1.209 end
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = FEED_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// Random Horizontal
						lob_parm(KY_PLUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// Random Horizontal
// C18 to XC8 Compiler Changes End
// v1.209 start
						strcpy (RemoteString, "Feed: ");						// Start with parameter text
						if(manual.feed == P_FEEDMIN)
							strcpy (RemoteStringVar, "Random");				// and concatenate the new variable string.		
// C18 to XC8 Compiler Changes Start
//						else btoa(manual.feed, RemoteStringVar);
//						else itoa((signed char)(manual.feed), RemoteStringVar,10);
						else itoa(RemoteStringVar,manual.feed,10);
// C18 to XC8 Compiler Changes End
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// v1.188 start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_FDDN)) {
				} else if (rky == RKY_FDDN) {
					if ((play_mode & PM_PERSONAL) && (curState == PP_SCRN3)) {		// personal program shot editing screen
						ppparm_ix = FEED_IX;
						key = KY_MINUS;
//					if (play_mode & PM_PRESET) {									//  if in preset mode (playing or not)
					} else if (play_mode & PM_PRESET) {								//  if in preset mode (playing or not)
// v1.188 end
						preset_ix[preset_type] = FEED_IX;
						lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep
						rem_feed_chg = 1;
// v1.209 start
						strcpy (RemoteString, "Feed: ");						// Start with parameter text
						if(preset[preset_type].feed == P_FEEDMIN)
							strcpy (RemoteStringVar, "Random");				// and concatenate the new variable string.
						else
// C18 to XC8 Compiler Changes Start
//							btoa(preset[preset_type].feed, RemoteStringVar);
//							itoa((signed char)(preset[preset_type].feed), RemoteStringVar,10);
							itoa(RemoteStringVar,preset[preset_type].feed,10);
// C18 to XC8 Compiler Changes End
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
// v1.181 start
//					} else {
					} else if (play_mode & PM_MANUAL){
// v1.181 end
						man_parm = FEED_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_MINUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// Random Horizontal
						lob_parm(KY_MINUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));		// Random Horizontal
// C18 to XC8 Compiler Changes End
// v1.209 start
						strcpy (RemoteString, "Feed: ");						// Start with parameter text
						if(manual.feed == P_FEEDMIN)
							strcpy (RemoteStringVar, "Random");				// and concatenate the new variable string.		
// C18 to XC8 Compiler Changes Start
//						else btoa(manual.feed, RemoteStringVar);
//						else itoa((signed char)(manual.feed), RemoteStringVar,10);
						else itoa(RemoteStringVar,manual.feed,10);
// C18 to XC8 Compiler Changes End
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					}
// 08/05/09 start
//				}
// v1.192.0U start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_ELEVUP)) {
				} else if (rky == RKY_ELEVUP) {
					if (play_mode & PM_PERSONAL) {
#ifdef elev_test
						elev_test_adjust = elev_test_adjust + 10;	// ifdef elev_test, this gets added to shots in Program mode
#else
						elev_test_adjust = 0;
#endif
//					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
// v1.192.0U end
						preset_ix[preset_type] = COL_IX;	// Row and column indexes are reversed for Random Horizontal and Random Vertical.
// V1.182 start
// Reverted back on how the remote +/- keys control the elevation in Sweep and 2-Line modes, because now the actual preset tables (pre_rand,
// preset_sub) are changed instead, so the control panel +/- keys in these modes, works the same as the remote +/- keys.
// v1.181 start
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
//						lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
// v1.181 end
// v1.182 end
// v1.209 start
						strcpy (RemoteString, "Row: ");						// Start with parameter text
						if (preset[PRE_RANDHORZ].col == 0)
							strcpy (RemoteStringVar, "A");
						else if (preset[PRE_RANDHORZ].col == 1)
							strcpy (RemoteStringVar, "Random");
						else if (preset[PRE_RANDHORZ].col == 2)
							strcpy (RemoteStringVar, "C");					// Start with parameter text
						else if (preset[PRE_RANDHORZ].col == 3)
							strcpy (RemoteStringVar, "B");					// Start with parameter text
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
// 11/04/09 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && model_type == EL05LE) {
// v1.187 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (model_type == EL05LE || model_type == CLUB)) {
// 11/04/09 end
// v1.189 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (model_type != EL04)) { // all models except EL04 now support 2-Line row selection
					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) { // all models now support 2-Line row selection
// v1.189 end
// v1.187 end
						preset_ix[preset_type] = TWOLNRW_IX;
// V1.182 start
// Reverted back on how the remote +/- keys control the elevation in Sweep and 2-Line modes, because now the actual preset tables (pre_rand,
// preset_sub) are changed instead, so the control panel +/- keys in these modes, works the same as the remote +/- keys.
// v1.181 start
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
//						lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
// v1.181 end
// v1.182 end
// v1.209 start
						strcpy (RemoteString, "Row: ");						// Start with parameter text
						if (preset[PRE_TWOLINE].twolnrw == 0)
							strcpy (RemoteStringVar, "A");
						else if (preset[PRE_TWOLINE].twolnrw == 1)
							strcpy (RemoteStringVar, "Random");
						else if (preset[PRE_TWOLINE].twolnrw == 2)
							strcpy (RemoteStringVar, "C");
						else if (preset[PRE_TWOLINE].twolnrw == 3)
							strcpy (RemoteStringVar, "B");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					} else if (play_mode & PM_MANUAL) {
						man_parm = ELEV_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_PLUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));
						lob_parm(KY_PLUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
						strcpy (RemoteString, "Elev: ");						// Start with parameter text
// C18 to XC8 Compiler Changes Start
//						btoa(manual.elev, RemoteStringVar);
//						itoa((signed char)(manual.elev), RemoteStringVar,10);
						itoa(RemoteStringVar,manual.elev,10);
// C18 to XC8 Compiler Changes End
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					} else if (cur_st == ST_TEST) {
						key = KY_PLUS;
					}
// v1.192.0U start
//				} else if (!(play_mode & PM_PERSONAL) && (rky == RKY_ELEVDN)) {
				} else if (rky == RKY_ELEVDN) {
					if (play_mode & PM_PERSONAL) {
#ifdef elev_test
						elev_test_adjust = elev_test_adjust - 10;	// ifdef elev_test, this gets added to shots in Program mode
#else
						elev_test_adjust = 0;
#endif
//					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
// v1.192.0U end
						preset_ix[preset_type] = COL_IX;	// Row and column indexes are reversed for Random Horizontal and Random Vertical.
// V1.182 start
// Reverted back on how the remote +/- keys control the elevation in Sweep and 2-Line modes, because now the actual preset tables (pre_rand,
// preset_sub) are changed instead, so the control panel +/- keys in these modes, works the same as the remote +/- keys.
// v1.181 start
						lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
//						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
// v1.181 end
// v1.182 end
// v1.209 start
						strcpy (RemoteString, "Row: ");						// Start with parameter text
						if (preset[PRE_RANDHORZ].col == 0)
							strcpy (RemoteStringVar, "A");
						else if (preset[PRE_RANDHORZ].col == 1)
							strcpy (RemoteStringVar, "Random");
						else if (preset[PRE_RANDHORZ].col == 2)
							strcpy (RemoteStringVar, "C");
						else if (preset[PRE_RANDHORZ].col == 3)
							strcpy (RemoteStringVar, "B");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
// 11/04/09 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && model_type == EL05LE) {
// v1.187 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (model_type == EL05LE || model_type == CLUB)) {
// 11/04/09 end
// v1.189 start
//					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (model_type != EL04)) { // all models except EL04 now support 2-Line row selection
					} else if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) { // all models now support 2-Line row selection
// v1.189 end
// v1.187 end
						preset_ix[preset_type] = TWOLNRW_IX;
// V1.182 start
// Reverted back on how the remote +/- keys control the elevation in Sweep and 2-Line modes, because now the actual preset tables (pre_rand,
// preset_sub) are changed instead, so the control panel +/- keys in these modes, works the same as the remote +/- keys.
// v1.181 start
						lob_parm(KY_MINUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
//						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);
// v1.181 end
// v1.182 end
// v1.209 start
						strcpy (RemoteString, "Row: ");						// Start with parameter text
						if (preset[PRE_TWOLINE].twolnrw == 0)
							strcpy (RemoteStringVar, "A");
						else if (preset[PRE_TWOLINE].twolnrw == 1)
							strcpy (RemoteStringVar, "Random");
						else if (preset[PRE_TWOLINE].twolnrw == 2)
							strcpy (RemoteStringVar, "C");
						else if (preset[PRE_TWOLINE].twolnrw == 3)
							strcpy (RemoteStringVar, "B");
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					} else if (play_mode & PM_MANUAL) {
						man_parm = ELEV_IX;
// C18 to XC8 Compiler Changes Start
//						lob_parm(KY_MINUS,(uchar*)&man_parm,(char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));
						lob_parm(KY_MINUS,(uchar*)&man_parm,(signed char *)&manual,(ushort)(PRE_SPEED|PRE_ELEV |PRE_FEED|PRE_SPIN));
// C18 to XC8 Compiler Changes End
						set_serve(&manual);
// v1.209 start
						strcpy (RemoteString, "Elev: ");						// Start with parameter text
// C18 to XC8 Compiler Changes Start
//						btoa(manual.elev, RemoteStringVar);
//						itoa((signed char)(manual.elev), RemoteStringVar,10);
						itoa(RemoteStringVar,manual.elev,10);
// C18 to XC8 Compiler Changes End
						strcat (RemoteString, RemoteStringVar);						// and concatenate the new variable string.
// v1.209 end
					} else if (cur_st == ST_TEST) {
						key = KY_MINUS;
// V1.209 start
							strcpy (RemoteString,lcd_line[0]);
							strcat (RemoteString,"\n");
							strcat (RemoteString,lcd_line[1]);
							strcat (RemoteString,"\n");
							strcat (RemoteString,lcd_line[2]);
							strcat (RemoteString,"\n");
							strcat (RemoteString,lcd_line[3]);
// v1.209 end
					}
				}
// 08/05/09 end
// v1.209 start
				else if (rky == RKY_DISPLAY) {					// Show complete display on remote
					strcpy (RemoteString,lcd_line[0]);
					strcat (RemoteString,"\n");
					strcat (RemoteString,lcd_line[1]);
					strcat (RemoteString,"\n");
					strcat (RemoteString,lcd_line[2]);
					strcat (RemoteString,"\n");
					strcat (RemoteString,lcd_line[3]);
				}

// v1.219 start
				else if (rky == RKY_UDRILLS) {					// Get all User Drills.
					if ((model_type == EL04) || (model_type == CLUB1)) {
						strcpy (RemoteString, "Upgrade Available Contact\rLobster Sports");
					} else {
						CurShot.top = SRV_MINIMUM;
						CurShot.bot = SRV_MINIMUM;
						StartTop(CurShot.top);
						StartBot(CurShot.bot);
						RemotePgmMode = 0;							// Clear all flags.
						program_ix = 0;

						RemotePgmMode = RemotePgmMode | RPM_START;	// "User Drills" view.
						if (play_mode & PM_PLAYING)
							key = KY_PLAY;
//						strcpypgm2ram (RemoteString,"User Program Mode");
//						if (!(play_mode & PM_PERSONAL)) {
							program_ix = 0;
							key = KY_PROG;
//							lcd_updated = 1;
//							lobmain(KY_PROG);
//						}
	 					priorState = PP_CHANGE;					// Redraw machine display.
	
						j=0;
						RemoteString[j] = 0x50;					// P 
						RemoteString[++j] = 0x31;				// 1
					 	load_prog(0,&act_program);				// Load program 1
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x32;				// 2
					 	load_prog(1,&act_program);				// Load program 2
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x33;				// 3
					 	load_prog(2,&act_program);				// Load program 3
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x34;				// 4
					 	load_prog(3,&act_program);				// Load program 4
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x35;				// 5
					 	load_prog(4,&act_program);				// Load program 5
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
						RemoteString[j] = 0x50;					// P
						RemoteString[++j] = 0x36;				// 6
					 	load_prog(5,&act_program);				// Load program 6
//						if (act_program.shot[0] != INV_SHOT) {
							for (i=0, j++; i<AVAILSHT; i++, j++) {
								RemoteString[j] = act_program.shot[i];
								if (RemoteString[j] == INV_SHOT)
									RemoteString[j] = 0xaa;
							}
//						} else j++;
// v1.223 start
						if (model_type == EL05LE2 || model_type == CLUB2) {			// Only models to support more than 6 user drills.
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x37;				// 7
						 	load_prog(6,&act_program);				// Load program 7
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x38;				// 8
						 	load_prog(7,&act_program);				// Load program 8
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x39;				// 9
						 	load_prog(8,&act_program);				// Load program 9
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3a;				// 10
						 	load_prog(9,&act_program);				// Load program 10
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3b;				// 11
						 	load_prog(10,&act_program);				// Load program 11
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
							RemoteString[j] = 0x50;					// P
							RemoteString[++j] = 0x3c;				// 12
						 	load_prog(11,&act_program);				// Load program 12
//							if (act_program.shot[0] != INV_SHOT) {
								for (i=0, j++; i<AVAILSHT; i++, j++) {
									RemoteString[j] = act_program.shot[i];
									if (RemoteString[j] == INV_SHOT)
										RemoteString[j] = 0xaa;
								}
//							} else j++;
						}
// v1.223 end
	
//						RemoteString[j] = 0xff;					// end of data
//						RemoteString[++j] = 0x00;				// end of data
						RemoteString[j] = 0x00;					// end of data
//						RemoteString[++j] = 0xaa;				// end of data
	
						RemoteDataCnt = j;

					 	load_prog(0,&act_program);				// Load program 0 so it is the default active program
					}											// when entering the user drills.
				}

				else if (rky == RKY_SHOTTABLES) {									// Get all shot table data.
					RemotePgmMode = RPM_SHOTTABLES;
#ifndef PHENOMS
					for (i=0; i<=pp_maxloc-1; i++) {								// Different models have different max selectable location.
						for (j=0; court_locs[i][j].speed !=0; j++) {
							while(Busy2USART());
							putc2USART(i+1);										// Send location.
							while(Busy2USART());
							putc2USART(speed_trans[court_locs[i][j].speed/5]);		// Send speed mapped for display.
							while(Busy2USART());
							putc2USART(court_locs[i][j].spin + 170);				// Send spin +170 to convert to 0xAA base for app.
						}
					}
#else
					for (i=0; i<=pp_maxloc-1; i++) {								// Different models have different max selectable location.
						for (j=0; court_locs_CLUB[i][j].speed !=0; j++) {
							while(Busy2USART());
							putc2USART(i+1);										// Send location.
							while(Busy2USART());
							putc2USART(speed_trans[court_locs_CLUB[i][j].speed/5]);	// Send speed mapped for display.
							while(Busy2USART());
							putc2USART(court_locs_CLUB[i][j].spin + 170);			// Send spin +170 to convert to 0xAA base for app.
						}
					}
#endif
					while(Busy2USART());
					putc2USART(0x00);
				}

// v1.219 end

// Need the following flag so remote control send data could be done outside the remote control routine.
// For Test Mode the machine display doesn't get updated until after the remote control routine is exited.
// First tried to send all other non Test Mode remote conrol data at the end of the remote control routine
// just above here, but the only way to know if in Test Mode was with cur_st (current state). That doesn't
// get updated until after the remote control routine is exited.
// v1.211 start
// Moved the following line up so it only indicates when remote data came from the Wi-Fi/Bluetooth port and not
// the legacy remote port.
//				RemoteDataReceived = 1;
// v1.211 end
// v1.209 end
			}	// else toss the character, end of - if (remote_enabled)
		}		// end of - if ((rky = debounced_remote())

// mjz 10/31/08, start

//		if (power_state != PS_WAKEUP)
//			lobmain(key);

		if (power_state != PS_WAKEUP) {
			lobmain(key);
// v1.211 start
// v1.209 start
//			if (RemoteDataReceived) {
#ifndef DEBUGPORT	// v1.218 added
// v1.219 start
//			if (rky2) {
			if (RemoteDataReceived) {
				RemoteDataReceived = 0;
// v1.219 end
// v1.211 end

// v1.219 start
// The following handled a special transmit case where the machine transmitted all shot table data
// before we got here. It was handled this way so the data buffer didn't have to be increased for
// the large data transmission.
				if (RemotePgmMode == RPM_SHOTTABLES) {				// If shot table data was just sent,
// 					RemotePgmMode = RPM_NONE;						// then clear RemotePgmMode flags,
 					RemotePgmMode = RPM_START;						// back to beginning of user drills view,
				}
// v1.220 start
				else if (RemotePgmMode == RPM_DWNLDDRL) {
					RemotePgmMode = RPM_START;
				}
// v1.220 end
				else {
// v1.219 end
// Have to do remote control display Test Shot update here after key processing.
					if (cur_st == ST_TEST) {								// If in Test Shot mode, send entire display,
						strcpy (RemoteString,lcd_line[0]);
						strcat (RemoteString,"\n");
						strcat (RemoteString,lcd_line[1]);
						strcat (RemoteString,"\n");
						strcat (RemoteString,lcd_line[2]);
						strcat (RemoteString,"\n");
						strcat (RemoteString,lcd_line[3]);
					}
// v1.219 start
					else if (!(strlen (RemoteString))) {					// else if the remote string is empty,
						strcpy (RemoteString, "Not Available");		// else there was nothing to send.
					}
					if (RemoteDataCnt) {									// Check for data count first, rather than strings.
						for (i=0; i<=RemoteDataCnt; i++) {					// Don't check string length, before checking for
							while(Busy2USART());							// data, because the data may contain 0x00 in the
							putc2USART(RemoteString[i]);					// first byte which would cause strnlen to return
						}													// 0 length.
					}
					else {
						puts2USART (RemoteString);
					}
				}
// v1.219 end
				strcpy (RemoteString, "\0");					// Clear the string.
// v1.213 start
//				RemoteDataReceived = 0;								// No longer used, replaced by rky2.
// v1.213 end
// v1.219 start
//				}
// v1.219 end
			}	// end of - if (rky2)
// v1.209 end
#endif		// v1.218 added
			if (toggle_play) {
				lobmain(KY_PRESET);
				lobmain(KY_PLAY);
				toggle_play = 0;
			}
		}
// mjz 10/31/08, end


// ELO4CHG #1 (start) 
        // pretend we pressed the key again.
        // 1st time is just to change state(to preset mode), 2nd time 
        // will drill down to proper preset drill
// v1.199 start
//        if ((key > 6) && (key < 34) && model_type == EL04) {
        if ((key > 6) && (key < 34) && ((model_type == EL04) || (model_type == CLUB1))) {	// models with EL04 panel
// v1.199 end
				startit = 0;
                switch (key)  {
// v1.186 start
                case KY_GRINDER:
// v1.212 start
//					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type == PRE_GRINDER) || (preset_type == PRE_POWER) || (preset_type == PRE_ALLCOURT))))
					if ((play_mode & PM_PLAYING) && (play_mode & PM_PRESET) && (preset_type >= PRE_GRINDER))
						startit = 1;							// If already playing in a drill, then keep playing in any drill.
//					preset_type = PRE_GRINDER;

// v1.215 start
//					if (model_type == EL04) {
//						if (preset_type == PRE_GRINDER)
//							preset_type = PRE_MOONBALLER;
//						else preset_type = PRE_GRINDER;
//					}
//					else preset_type = PRE_GRINDER;				// CLUB1 doesn't support new drills yet.
// v1.212 end
					if (preset_type == PRE_GRINDER)
						preset_type = PRE_MOONBALLER;
					else preset_type = PRE_GRINDER;
// v1.215 end

					if (!(play_mode & PM_PRESET)) {				// If not in Preset mode,
						key = KY_PRESET;						// enter Preset mode.
					}
					if (startit == 1)
						goto keepplaying;						//don't turn off light
					goto skiptwoline;
                case KY_POWER:
// v1.212 start
//					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type == PRE_GRINDER) || (preset_type == PRE_POWER) || (preset_type == PRE_ALLCOURT))))
					if ((play_mode & PM_PLAYING) && (play_mode & PM_PRESET) && (preset_type >= PRE_GRINDER))
						startit = 1;							// If already playing in a drill, then keep playing in any drill.
//					preset_type = PRE_POWER;

// v1.215 start
//					if (model_type == EL04) {
//						if (preset_type == PRE_POWER)
//							preset_type = PRE_SLICER;
//						else preset_type = PRE_POWER;
//					}
//					else preset_type = PRE_POWER;				// CLUB1 doesn't support new drills yet.
// v1.212 end
					if (preset_type == PRE_POWER)
						preset_type = PRE_SLICER;
					else preset_type = PRE_POWER;
// v1.215 end

					if (!(play_mode & PM_PRESET)) {				// If not in Preset mode,
						key = KY_PRESET;						// enter Preset mode.
					}
					if (startit == 1)
						goto keepplaying;						//don't turn off light
					goto skiptwoline;
                case KY_ALLCOURTER:
// v1.212 start
//					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type == PRE_GRINDER) || (preset_type == PRE_POWER) || (preset_type == PRE_ALLCOURT))))
					if ((play_mode & PM_PLAYING) && (play_mode & PM_PRESET) && (preset_type >= PRE_GRINDER))
						startit = 1;							// If already playing in a drill, then keep playing in any drill.
//					preset_type = PRE_ALLCOURT;

// v1.215 start
//					if (model_type == EL04) {
//						if (preset_type == PRE_ALLCOURT)
//							preset_type = PRE_LEFTY;
//						else preset_type = PRE_ALLCOURT;
//					}
//					else preset_type = PRE_ALLCOURT;				// CLUB1 doesn't support new drills yet.
// v1.212 end
					if (preset_type == PRE_ALLCOURT)
						preset_type = PRE_LEFTY;
					else preset_type = PRE_ALLCOURT;
// v1.215 end

					if (!(play_mode & PM_PRESET)) {				// If not in Preset mode,
						key = KY_PRESET;						// enter Preset mode.
					}
					if (startit == 1)
						goto keepplaying;						//don't turn off light
					goto skiptwoline;
                case KY_FULLRAND:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND) && (play_mode & PM_PLAYING)) {
						startit = 1;	// don't turn off Play LED and keep playing
						goto keepplaying;	//don't turn off light
					} else
						preset_type = PRE_FULLRAND;
					goto skiptwoline;
                case KY_RNDSWEEP:
// v1.199 start
// When KY_SWEEP was pressed right after power-on, Sweep mode was selected, but the sweep row was also incremented. This was because
// preset_type was initialized to be PRE_RANDHORZ at power-on and play_mode was already changed to PM_PRESET before we got here. So
// the following routine would execute a KY_PLUS that incremented the sweep row. This problem was fixed with a kluge by initializing
// preset_type to 0xff at power-on (varinit.c), only for models with control panels that have direct keys to Sweep mode. For other
// models with control panels that have a Presets key instead, it is still initialized to PRE_RANDHORZ, so that the first Presets
// press takes us to Sweep mode.
// v1.199 end
					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
						preset_ix[preset_type] = 6;	// ROW_IX
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep (ROW)
						if (play_mode & PM_PLAYING) {
							startit = 1;	// don't turn off Play LED and keep playing
							goto keepplaying;
						}
					} else
						preset_type = PRE_RANDHORZ;
					goto skiptwoline;
                case KY_RNDDEPTH:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDVERT)) {
						preset_ix[preset_type] = 7;	// COL_IX
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next COL
						if (play_mode & PM_PLAYING) {
							startit = 1;	// don't turn off Play LED and keep playing
							goto keepplaying;
						}
					} else
						preset_type = PRE_RANDVERT;
					goto skiptwoline;
                case KY_TWOLINENARROW:
                case KY_TWOLINEMEDIUM:
                case KY_TWOLINEWIDE:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE) && (play_mode & PM_PLAYING)) {
						preset_ix[preset_type] = 8;	//	8 = WID_IX
						startit = 1;	// don't turn off Play LED and keep playing
						goto keepplaying;	//don't turn off light
					} else {
						preset_type = PRE_TWOLINE;
					}
// v1.186 end
skiptwoline:
                    play_mode &= ~PM_PLAYING;   // turn off play led
		            BuildCmd('3',1,&play_mode);	//	and turn on/off LED	
keepplaying:
                    lobmain(key);
                    lobmain(key);   // yep need this one also
// v1.221 start
//					if (startit)
//						lobmain(KY_PLAY);
					if (startit) {
						if (start_delay) {								// If still in a previous start delay,
							PlayLEDPass = 0;							// Reset blinking Play LED.
							PlayLEDTmr = PlayLEDTmrPreload;
							start_delay = ticks + TWENTYSEC;			// Start the Play delay all over again.
							PlayModeBlink = play_mode;					// Copy LEDs for blinking control without affecting original LED setting.
						}
						lobmain(KY_PLAY);   
					} else start_delay = 0;
// v1.221 end
                    break;
                }
        }
// ELO4CHG #1   (end

// v1.186 start
        // pretend we pressed the key again.
        // 1st time is just to change state(to preset mode), 2nd time 
        // will drill down to proper preset drill
//		if ((key > 6) && (key < 34) && model_type == EL04) {
// v1.191.0R start
//		if ((key > 6) && (key <= 0x24) && model_type == EL05LE2) {
// v1.199 start
//		if ((key > 6) && (key <= 0x24) && (model_type == EL05LE2 || model_type == CLUB)) {
		if ((key > 6) && (key <= 0x24) && (model_type == EL05LE2 || model_type == CLUB2)) {	// models with new LE panel
// v1.199 end
// v1.191.0R end
				startit = 0;
                switch (key)  {
                case KY_DRILLS:
// v1.208 start
//					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type == PRE_GRINDER) || (preset_type == PRE_POWER) || (preset_type == PRE_ALLCOURT) || (preset_type == PRE_TRANSITION) || (preset_type == PRE_ATTDEF) || (preset_type == PRE_FOREONLY))))
					if ((play_mode & PM_PLAYING) && ((play_mode & PM_PRESET) && ((preset_type >= PRE_GRINDER))))
// v1.208 end
						startit = 1;								// If already playing in a drill, then keep playing in any drill.
					if (play_mode & PM_PRESET) {					// If in Preset mode,
						if (preset_type == PRE_GRINDER) {			// and if in Grinder drill,
							preset_type = PRE_POWER;				// then increment to Power Baseliner drill,
						}
						else if (preset_type == PRE_POWER) {		// else if in Power Baseliner drill,
							preset_type = PRE_ALLCOURT;				// then increment to All-Courter drill,
						}
						else if (preset_type == PRE_ALLCOURT) {		// else if in All-Courter drill,
							preset_type = PRE_TRANSITION;			// then increment to Transition drill,
						}
						else if (preset_type == PRE_TRANSITION) {	// else if in Transition drill,
							preset_type = PRE_ATTDEF;				// then increment to Attack/Defend drill,
						}
						else if (preset_type == PRE_ATTDEF) {		// else if in Attack/Defend drill,
							preset_type = PRE_FOREONLY;				// then increment to Forehand Only drill,
						}

// v1.215 start
// v1.211 start
// EL04 & CLUB1 only support first 3 preset drills. They were already qualified out in hardware because they
// don't have a drills button. They have 3 buttons that go directly to the 3 drills.
// Here we also qualify out the CLUB2 (PHENOM 2) for the next new 6 preset drills, because court test data has not
// been developed for this model yet.
//						else if (preset_type == PRE_FOREONLY && model_type == CLUB2) {
//							preset_type = PRE_GRINDER;									// go back to GRINDER
//						}
// v1.211 end
// v1.215 end

// v1.208 start
						else if (preset_type == PRE_FOREONLY) {		// else if in PRE_FOREONLY drill,
							preset_type = PRE_BEGINNER;				// then increment to NewDrill1 drill,
						}
						else if (preset_type == PRE_BEGINNER) {		// else if in PRE_NEWDRILL1 drill,
							preset_type = PRE_INTERMEDIATE;			// then increment to PRE_NEWDRILL2 drill,
						}
						else if (preset_type == PRE_INTERMEDIATE) {	// else if in PRE_NEWDRILL2 drill,
							preset_type = PRE_ADVANCED;				// then increment to PRE_NEWDRILL3 drill,
						}
						else if (preset_type == PRE_ADVANCED) {		// else if in PRE_NEWDRILL3 drill,
							preset_type = PRE_MOONBALLER;			// then increment to PRE_NEWDRILL4 drill,
						}
						else if (preset_type == PRE_MOONBALLER) {	// else if in PRE_NEWDRILL4 drill,
							preset_type = PRE_SLICER;				// then increment to PRE_NEWDRILL5 drill,
						}
						else if (preset_type == PRE_SLICER) {		// else if in PRE_NEWDRILL5 drill,
							preset_type = PRE_LEFTY;				// then increment to PRE_NEWDRILL6 drill,
						}
// v1.208 end
						else {										// else if not already in a drill,
							preset_type = PRE_GRINDER;				// then start with Grinder drill.
						}
					} else {									// If not in Preset mode
						preset_type = PRE_GRINDER;				// then start with Grinder drill,
						key = KY_PRESET;						// and enter Preset mode.
					}
					if (startit == 1)
						goto continue_playing;	//don't turn off light
					goto skip2line;
                case KY_RANDOM:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_FULLRAND) && (play_mode & PM_PLAYING)) {
						startit = 1;	// don't turn off Play LED and keep playing
						goto continue_playing;	//don't turn off light
					} else
						preset_type = PRE_FULLRAND;
					goto skip2line;
                case KY_SWEEP:
// v1.199 start
// When KY_SWEEP was pressed right after power-on, Sweep mode was selected, but the sweep row was also incremented. This was because
// preset_type was initialized to be PRE_RANDHORZ at power-on and play_mode was already changed to PM_PRESET before we got here. So
// the following routine would execute a KY_PLUS that incremented the sweep row. This problem was fixed with a kluge by initializing
// preset_type to 0xff at power-on (varinit.c), only for models with control panels that have direct keys to Sweep mode. For other
// models with control panels that have a Presets key instead, it is still initialized to PRE_RANDHORZ, so that the first Presets
// press takes us to Sweep mode.
// v1.199 end
					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDHORZ)) {
						preset_ix[preset_type] = 6;	// ROW_IX
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next sweep (ROW)
						if (play_mode & PM_PLAYING) {
							startit = 1;	// don't turn off Play LED and keep playing
							goto continue_playing;
						}
					} else {
						preset_type = PRE_RANDHORZ;
					}
					goto skip2line;
                case KY_DEPTH:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_RANDVERT)) {
						preset_ix[preset_type] = 7;	// COL_IX
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next COL
						if (play_mode & PM_PLAYING) {
							startit = 1;	// don't turn off Play LED and keep playing
							goto continue_playing;
						}
					} else
						preset_type = PRE_RANDVERT;
					goto skip2line;
                case KY_2LINE:
					if ((play_mode & PM_PRESET) && (preset_type == PRE_TWOLINE)) {
						preset_ix[preset_type] = 8;	//	8 = WID_IX
						lob_parm(KY_PLUS,(uchar *)&preset_ix[preset_type],&(preset[preset_type].value[0]),preset_mask[preset_type]);	// select the next COL
						if (play_mode & PM_PLAYING) {
							startit = 1;	// don't turn off Play LED and keep playing
							goto continue_playing;	//don't turn off light
						}
					} else
						preset_type = PRE_TWOLINE;
skip2line:
                    play_mode &= ~PM_PLAYING;   // turn off play led
		            BuildCmd('3',1,&play_mode);	//	and turn on/off LED	
continue_playing:
                    lobmain(key);
                    lobmain(key);   // yep need this one also
// v1.221 start
//					if (startit)
//						lobmain(KY_PLAY);
					if (startit) {
						if (start_delay) {								// If still in a previous start delay,
							PlayLEDPass = 0;							// Reset blinking Play LED.
							PlayLEDTmr = PlayLEDTmrPreload;
							start_delay = ticks + TWENTYSEC;			// Start the Play delay all over again.
							PlayModeBlink = play_mode;					// Copy LEDs for blinking control without affecting original LED setting.
						}
						lobmain(KY_PLAY);   
					} else start_delay = 0;
// v1.221 end
                    break;
                }
        }
// v1.186 end

	}	// end - while(1), loop forever
}		// end - void main(void)

unsigned long last_received = 0;
// returns first key after n second silence
unsigned char debounced_remote(void)
{
	unsigned char rc = 0;									// assume there is nothing to return
	
	if (RCSTA1bits.OERR) {
		RCSTA1bits.CREN = 0;
		RCSTA1bits.CREN = 1;
		putrsDIAG("Overrun error cleared.\n");
	}
	if (DataRdy1USART()) {									// if received a character
		rc = Read1USART();									// get it from the remote
		if ((ticks - last_received) < REMDELAY) {			// if too little time between chars
			rc = 0;											// clear it
		}
		last_received = ticks;								// save new timestamp for char
	}
	return(rc);												// and return character if any
}

// v1.196 start
unsigned char debounced_remote_USART2(void)
{
	unsigned char rc = 0;									// assume there is nothing to return


	if (RCSTA2bits.OERR) {
		RCSTA2bits.CREN = 0;
		RCSTA2bits.CREN = 1;
		putrsDIAG("\n\rUSART2 Overrun error cleared.");
	}
	if (DataRdy2USART()) {									// if received a character
		rc = Read2USART();									// get it from the remote

		if (rc == RKY_RCQUAL) {								// If byte is a "Z",
			rc_qualified = 1;								// then next byte is good (qualified),
			rc = 0;											// throw away
		}
		else if (rc_qualified != 1) {						// else if not "Z" and previous byte wasn't a "Z",
			rc = 0;											// then this was an unqualified byte.
		}
		else {
			rc_qualified = 0;								// else last byte was qualifier and this is the qualified byte
			RemoteDataReceived = 1;							// Need to allow for receiving 0x00 for remote programming.
		}
	}
	return(rc);												// and return character if any
}

// v1.220 start
void RemoteSaveDrill (void)
{						char i;
						char j;

						prog_chgd = 1;
						for (i=0; i<UDrillShots; i++) {
							act_program.shot[i] = RemotePgmLocn[i];
							act_program.feed[i] = RemotePgmFeed[i];
#ifndef PHENOMS
							for (j=0; court_locs[RemotePgmLocn[i]-1][j].speed !=0; j++) {
								if ((court_locs[RemotePgmLocn[i]-1][j].speed == RemotePgmSpeed[i])
								 && (court_locs[RemotePgmLocn[i]-1][j].spin == RemotePgmSpin[i])){
									RemotePgmMethod[i] = j;
									break;
								}
							}
#else
							for (j=0; court_locs_CLUB[RemotePgmLocn[i]-1][j].speed !=0; j++) {
								if ((court_locs_CLUB[RemotePgmLocn[i]-1][j].speed == RemotePgmSpeed[i])
								 && (court_locs_CLUB[RemotePgmLocn[i]-1][j].spin == RemotePgmSpin[i])){
									RemotePgmMethod[i] = j;
									break;
								}
							}
#endif
							act_program.method[i] = RemotePgmMethod[i];
						}
						for (i=UDrillShots; i<AVAILSHT; i++) {				// Make all remaining unused shots invalid.
							act_program.shot[i] = INV_SHOT;
							act_program.method[i] = 0;
							act_program.feed[i] = DEF_FEED;						
						}
					 	save_prog(program_ix,&act_program);					// Save program.
//						strcpypgm2ram (RemoteString, "Drill Saved");
	 					priorState = PP_CHANGE;								// Redraw machine display.
}
// v1.220 end

// C18 to XC8 Compiler Changes Start
//#pragma code MYCODE
// C18 to XC8 Compiler Changes End

// v1.220 end
//far void nofunc (int code, long *args) 
void nofunc (int code, long *args) 
// v1.220 end
{ }

void FillDiagBuf()
{
	char c;
	int ct = rbuf_bct(&di_inbuf);
	while (ct--) {
		c = rbuf_extr(&di_inbuf);
		if (!stat.HaveCmd && !stat.InCmd) {
			rbuf_ins(&di_outbuf, c);						//   echo it back
			c = toupper(c);									//	 if lower case - make upper 
			if (c == CR || c == LF) {						//   CR/LF terminates string
				c = 0;
				stat.HaveCmd = 1;
			}
			if (c == (char)',') c = ' ';					//   , and ' ' both separators
			if (c == BS) {
				if (DiagBufOfs)
					--DiagBufOfs;
			} else if (c == ESC) {							//   ESC aborts commands
				DiagBufOfs = 0;
			} else
				DiagBuf[DiagBufOfs++] = c;
			DiagBuf[DiagBufOfs] = 0;
			if (DiagBufOfs >= (DIAG_BUF_SIZE + 1)) {		//	 if buffer full, terminate 
				stat.HaveCmd = 1;
				stat.CmdErr = 1;							//   and indicate error
			} 
		}
	}
}

// Generic parse function
//	locates end of field, terminates it and if desired converts text to numeric
//	cp		- pointer to string to be parsed
//	delim	- delimiter character
//	ulp		- optional address of numeric result (if null, dont do conversion)
//	returns:
//		NULL  - parsing is complete
//		other - pointer to next field to be parsed
//
unsigned char *parse_next(unsigned char *cp, unsigned char delim, unsigned long *ulp)
{
	unsigned char *np;

	if (cp) {										// ensure valid incoming string
		np = (unsigned char *)strchr((char *)cp, delim);		// locate next delimiter
		if (np) {									// if one is found
			*np++ = 0;							//	replace delimiter with termination
			if (ulp) { 							// if field is number (result pointer is valid)
				*ulp = atoh((char *)cp);	//	convert ascii decimal text into binary
			}
		} else if (*cp) {					// else if not found (but more text)
			if (ulp)								//	if field is number (result pointer is valid)
				*ulp = atoh((char *)cp);	//	convert ascii decimal text into binary
		}
	}
	return(np);								// return pointer to start of next field
}

// Looks up command in table - if it is found, gets all required arguments 
//	and calls the appropriate function. Includes multi-level password support. 
// 
void exec_diag(void)
{
//	variant u;
	char i, j, k; 
	long jj = 0;
	unsigned cmd;
	unsigned char *cptr = &DiagBuf[2];
	long argary[8];

	cmd = ((unsigned)DiagBuf[0] << 8) + (unsigned)DiagBuf[1];
	if (*cptr == ' ') ++cptr;

	for (i=0; centry[i].cmd; i++) {								// check all commands in table 
//		if ((centry[i].cmd & password_type) == cmd){
		if (centry[i].cmd == cmd){								//	if matches
			stat.InCmd = 1;
			if (stat.CmdErr)									//	 if invalid argument was entered 
				PUTRSDIAG (InvArg);				//		display message to that effect 
			else {												//	 else (all arguments were valid) 
				// parse command string here
				for (j=0; j< 8; j++)
					argary[j] = 0;
				for (j=0; j<8; j++) {
					cptr = parse_next(cptr,' ',(unsigned long *)&argary[j]);
					if ((int)strlen((char *)cptr) == 0) break;
				}
 				centry[i].func (centry[i].code, &argary[0]);	//		execute command with arguments 
			}
			break;												//	 cease command search 
		}
	}
	if (!centry[i].cmd)											// if current command is end of table 
		putrsDIAG (InvCmd);									//	display message 
	DiagBufOfs = 0;
	stat.CmdErr = 0;
	stat.HaveCmd = 0;
	stat.InCmd = 0;
	putrsDIAG (ready);
}

// Displays memory contents
//
void dumpmem(int code, long *args)
{
	unsigned long ix;
	int ix1;

	ix = args[0];
	for (ix1=0; ix1<(int)0x80; ix1++) {
		if ((ix1 & 0x0f) == 0)
			dumpaddr(ix);
		else if ((ix1 & 3) == 0) {
			rbuf_ins(&di_outbuf, ' ');
		}
		dumphex(*(unsigned char *)ix++);
	}
	putrsDIAG(crlf);
}

// Set Memory - 1 byte
//
void setmem(int code, long *args)
{
	unsigned char *cptr = (unsigned char *)args[0];
	*cptr = args[1];
}

// Flash memory access routines

// C18 to XC8 Compiler Changes Start
#ifdef notdef
// C18 to XC8 Compiler Changes End

char *FlashRead(unsigned long addr, char *buf, int count)
{
	int cnt;
	char x = TBLPTRU;

	for (cnt=0; cnt<count; cnt++) {
// C18 to XC8 Compiler Changes Start
//		buf[cnt] = *(char rom far *)addr++;
		buf[cnt] = *(char *)addr++;
// C18 to XC8 Compiler Changes End
	}
	TBLPTRU = x;	// if addr > 64k, TBLPTRU would be off, so restore it
	return buf;
}

// this is always writing 64 bytes
void FlashWrite( char *buf, unsigned long addr)
{
	extern unsigned char CodeAddrU,CodeAddrH,CodeAddrL;
	extern unsigned char DataAddrH,DataAddrL;
	ulong dbufaddr = (ulong)buf;

	DataAddrL = dbufaddr & 0xff;
	DataAddrH = (dbufaddr >> 8) & 0xff;
	
	addr = addr & 0xfffc0;		// force to 64 byte boundary
	CodeAddrL = addr & 0xff;
	CodeAddrH = (addr >> 8) & 0xff;
	CodeAddrU = (addr >> 16) & 0xff;
	FlashWr();
}

// erase 64 byte block of flash memory
// must be on 64 byte boundary
void FlashErase(unsigned long addr)
{
	extern unsigned char CodeAddrU,CodeAddrH,CodeAddrL;
	extern unsigned char DataAddrH,DataAddrL;

	CodeAddrL = addr & 0xc0;				// forcing to 64 byte boundary
	CodeAddrH = (addr >> 8) & 0xff;
	CodeAddrU = (addr >> 16) & 0xff;
	FlashEr();
}

void flash(int code, long *args)
{
	unsigned long ix;
	unsigned long ix1;
	int cnt;
	extern unsigned char CodeAddrU,CodeAddrH,CodeAddrL;
	extern unsigned char DataAddrH,DataAddrL;
// C18 to XC8 Compiler Changes Start
//	far rom char* fhp = (far rom char*)&flash_header;
	char* fhp = (char*)&flash_header;
// C18 to XC8 Compiler Changes End
	int done;
	long dix, dix1;

	ix = args[0];
	ix1 = args[1];

	switch (code) {
	case 0:					// flash erase
		// FE xxxxx		erase 64 byte block at xxxxx
		FlashErase(ix);
		break;
	case 1:					// flash dump
		FlashRead(ix,&FlashBuf[0],0x40);
		dumpsDIAG(ix, FlashBuf);
//		PUTRSDIAG((const rom char *)crlf);
//		for (cnt=0; cnt<0x40; cnt++,ix++) {
//			if ((cnt & 0x0f) == 0)
//				dumpaddr(ix);
//			else if ((cnt & 3) == 0) {
//				rbuf_ins(&di_outbuf, ' ');
//			}
//			dumphex(FlashBuf[cnt]);
//		}
//		PUTRSDIAG((const rom char *)crlf);
		break;
	case 2:					// flash write
		// FW xxxx yyyyy	Write with data starting at ram xxxx to flash starting at yyyyy.
		//                  Flash address must be at 64 byte boundary. 64 bytes of data will
		//                  be written
		FlashWrite((char *)ix,ix1);
		break;
	case 3:					// FT - flash erase test
		DisableSlaveSPI();	// keep slave from interfering
		for (dix=0; dix <args[0]; dix+=64) {
//			putrsDIAG("\n\rErasing");
//			dumplong(dix);
			FlashErase((ulong)0x18000 + dix);
		}
		EnableSlaveSPI();		// keep slave from interfering
		for (ix=0; (ix<dix)&& done; ix++) {
			if (fhp[ix] != 0xff) {
				done = 0;
				dumpaddr(ix);
				dumphex(fhp[ix]);
				putrsDIAG("s/b ");
				dumphex(0xff);
			}
		}
		break;
	default:
		break;
	}
}


// C18 to XC8 Compiler Changes Start
#endif
// C18 to XC8 Compiler Changes End

// Spi test
//
void spitest(int code, long *args)
{
	unsigned char data, status;
	switch (code) {
	case 0:				// spi write
		ChipSelect(args[0]);
		if (WriteSPI((char)args[1]))
			putrsDIAG("\n\rWRITE COLLISION\n\r");
		ChipDeSelect(args[0]);
		break;
	case 1:				// spi read
		ChipSelect(args[0]);
//		if (DataRdySPI()) {
//			putrsDIAG("\n\rSpi Data Ready:");
//			dumphex(SSP1BUF);
//		}
//		putrsDIAG(crlf);
		data = ReadSPI();
		ChipDeSelect(args[0]);
		putrsDIAG(crlf);
		dumphex(data);
		break;
	}
}

// a2d diagnostics commands
void a2d_diag(int code, long *args)
{
	unsigned long val;
	int i;

	switch (code) {
	case 0:
		a2dtest = 0;
		for (i=0; i<NumA2dChan; i++) {
			dumpaddr(i);
			dumplong(a2dValue[i]);
			dumphex(a2dGain[i]);
		}
		break;
	case 1:
		a2dtest = 1;
		PgaGain(args[0], args[1]);
		ADCON0 = 0x01;					// chan 0, A/D enabled
		ADCON0bits.NOT_DONE = TRUE;		// start
		dumpaddr(args[0]);
		while (ADCON0bits.NOT_DONE == TRUE);
		val = ADRESH;
		val = (val & 0xff) << 8;
		val |= ((unsigned int)ADRESL) & 0xff;
		a2dValue[args[0]] = (int)val;
		ADCON0 = 0;
		dumplong(val);
		dumphex(args[1]);
		break;
	case 2:
		if (args[0] == 8) {
			int i,j;
			for (i=MINAMP, j=0; i<=MAXAMP; i++,j++) {
				dumpshort(CurSenseCt[0][j]);
			}
		}
		a2dChan2Dump = args[0];
		break;
	case 3:	// display last battery a2d reading
		dumplong(BAT_lastval);
		break;
	case 4: 	// AF a2d diagnostics - adjust feed current sense trip point
		if (args[0])
			BDM_TripPoint[FEED] = (unsigned int)args[0];
		if (args[1])
			BDM_TripTime[FEED] = (unsigned int)args[1];
		break;
	}
}

// serial eeprom commands
#define SEE_WRST 0x01		// write status
#define SEE_RD	0x03		// read
#define	SEE_WR	0x02		// write
#define SEE_ST	0x05		// get status
#define SEE_WRE	0x06		// write enable

// Tell slave to change SPI output bit to input
// We have a problem with the slave SPI output driving the bus,
// even when the slave is not selected. Until Microchip comes up with a solution,
// this is a work around. Must have slave disable SPI output, before accessing serial
// eeprom. The other SPI devices do not have output.
void DisableSlaveSPI(void)
{
	char c, c1;
	
	c = NAK;															// set slave response to go through while loop at least once
	while ((c == NAK) || (c1 == NAK)) {
		BuildCmd(SC_OC,0,(long *)0);								// tell slave to turn off SPI output
		Delay1KTCYx(5);
		ChipSelect(0);												//	select slave
		c = ReadSPI();												//	acquire any slave response
		ChipDeSelect(0);											//	release slave
		Delay1KTCYx(5);
		ChipSelect(0);												//	select slave
		c1 = ReadSPI();												//	acquire any slave response
		ChipDeSelect(0);											//	release slave
		if ((c == NAK) || (c1 == NAK)) {
			putrsDIAG("\nNAK");
		}
	}
}

void EnableSlaveSPI(void)
{
	char c;
	BuildCmd(SC_OS,0,(long *)0);								// tell slave to turn on SPI output
	Delay1KTCYx(5);
	ChipSelect(0);												//	select slave
	c = ReadSPI();												//	acquire any slave response
	ChipDeSelect(0);											//	release slave
}

char SEERead(unsigned int addr)
{
	char data;

	ChipReserve(1);
	PORTDbits.RD6 = 1;		// eeprom chip deselect
	PORTDbits.RD6 = 0;	// eeprom chip select
	WriteSPI(SEE_RD);						// send read command
	WriteSPI((addr >> 8) & 0xff);		//      addrH
	WriteSPI(addr & 0xff);				//      addrL
	data = ReadSPI();					// get data byte
	PORTDbits.RD6 = 1;					// eeprom chip deselect
	ChipDeSelect(1);
//	if (addr < 0x40) {
//		putrsDIAG("EER:");
//		dumplong(addr);
//		dumphex(data);
//	}
	return data;
}

void SEEWrite(unsigned int addr, char data)
{
	char status;
	
	ChipReserve(1);
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	status = 1;
	while (status & 1) {					// wait till not WIP
		PORTDbits.RD6 = 0;					// eeprom chip select
		WriteSPI(SEE_ST);						// read status command
		status = ReadSPI();
		PORTDbits.RD6 = 1;					// eeprom chip deselect
	}
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WRE);						// write enable command
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WRST);						// send write status command
	WriteSPI(0);									// BP1 and BP0 turned off, WPEN = 0
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	status = 1;
	while (status & 1) {					// wait till not WIP
		PORTDbits.RD6 = 0;					// eeprom chip select
		WriteSPI(SEE_ST);						// read status command
		status = ReadSPI();
		PORTDbits.RD6 = 1;					// eeprom chip deselect
	}
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WRE);						// write enable command
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WR);							// send write command
	WriteSPI((addr >> 8) & 0xff);	//      addrH
	WriteSPI(addr & 0xff);				//      addrL
	WriteSPI(data);								//      data
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	status = 1;
	while (status & 1) {					// wait till not WIP
		PORTDbits.RD6 = 0;					// eeprom chip select
		WriteSPI(SEE_ST);						// read status command
		status = ReadSPI();
		PORTDbits.RD6 = 1;					// eeprom chip deselect
	}
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WRE);						// write enable command
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	PORTDbits.RD6 = 0;						// eeprom chip select
	WriteSPI(SEE_WRST);						// send write status command
	WriteSPI(0x8c);								// BP1 and BP0 turned on
	Delay1KTCYx(1);
	PORTDbits.RD6 = 1;						// eeprom chip deselect
	status = 1;
	while (status & 1) {					// wait till not WIP
		PORTDbits.RD6 = 0;					// eeprom chip select
		WriteSPI(SEE_ST);						// read status command
		status = ReadSPI();
		PORTDbits.RD6 = 1;					// eeprom chip deselect
	}
	status = 1;
	while (status & 1) {					// wait till not WIP
		PORTDbits.RD6 = 0;					// eeprom chip select
		WriteSPI(SEE_ST);						// read status command
		status = ReadSPI();
		PORTDbits.RD6 = 1;					// eeprom chip deselect
	}
	ChipDeSelect(1);
}

// eeprom test
void ee_test(int code, long *args)
{
	unsigned int addr;
	unsigned char data, status,status1,status2,status3;

	DisableSlaveSPI();
	switch (code) {
	case 0:												// read
		addr = args[0];
		data = SEERead(addr);
		putrsDIAG(crlf);
		dumphex(data);
		putrsDIAG(crlf);
		break;
	case 1:				// write
		addr = args[0];
		data = (char)args[1];
		SEEWrite(addr,data);
		break;
	case 2:				// test
		addr = args[0];
		data = (char)args[1];
		ChipReserve(1);
		PORTDbits.RD6 = 1;		// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		status1 = status;
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRE);			// write enable command
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRST);			// send write status command
		WriteSPI(0);						// BP1 and BP0 turned off, WPEN = 0
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRE);			// write enable command
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WR);				// send write command
		WriteSPI((addr >> 8) & 0xff);		// addrH
		WriteSPI(addr & 0xff);	// addrL
		WriteSPI(data);					// data
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		status2 = status;
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRE);			// write enable command
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRST);			// send write status command
		WriteSPI(0x8c);					// BP1 and BP0 turned on
		Delay1KTCYx(1);
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		status3 = status;
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		ChipDeSelect(1);
		EnableSlaveSPI();
		dumplong(status1);
		dumplong(status2);
		dumplong(status3);
		dumplong(status);
		return;
	case 3:								// block protect on/off
		addr = args[0];
		ChipReserve(1);
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		status1 = status;
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRE);			// write enable command
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		PORTDbits.RD6 = 0;			// eeprom chip select
		WriteSPI(SEE_WRST);			// send write status command
		if (addr == 0)
			WriteSPI(0);					// BP1 and BP0 turned off, WPEN = 0
		else
			WriteSPI(0x8c);	
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		ChipDeSelect(1);
		break;
	case 4:
				// get status
		ChipReserve(1);
		PORTDbits.RD6 = 1;			// eeprom chip deselect
		status = 1;
		while (status & 1) {	// wait till not WIP
			PORTDbits.RD6 = 0;		// eeprom chip select
			WriteSPI(SEE_ST);		//read status command
			status = ReadSPI();
			PORTDbits.RD6 = 1;		// eeprom chip deselect
		}
		ChipDeSelect(1);
		dumplong(status);
		break;
	}
	EnableSlaveSPI();
}
void ChipDeSelect(int dev)
{
	switch (dev) {
	case LOB_SLAVE:				// slave
		PORTCbits.RC0 = 1;
		break;
	case LOB_EEPROM:			// serial eeprom
		PORTDbits.RD6 = 1;	// eeprom chip select
		break;
	case LOB_PGA:					// pga
		PORTDbits.RD7 = 1;	// pga chip select
		break;
	case LOB_POT:					// pot
		PORTBbits.RB4 = 1;	// pot_cs#
		break;
	default:	// anything else, don't do anything
		break;
	}
	SpiLock = 0;		// ok for isr to access spi
//	DelayFor18TCY();
}

// ChipSelect
void ChipSelect(int dev)
{
	SpiLock = 1;		// lock out isr from accessing spi
	// disable all first
	PORTBbits.RB4 = 1;	// pot_cs#
	PORTCbits.RC0 = 1;	// slave chip select
	PORTDbits.RD6 = 1;	// eeprom chip select
	PORTDbits.RD7 = 1;	// pga chip select

	switch (dev) {
	case LOB_SLAVE:				// slave
		PORTCbits.RC0 = 0;
		break;
	case LOB_EEPROM:			// serial eeprom
		PORTDbits.RD6 = 0;	// eeprom chip select
		break;
	case LOB_PGA:					// pga
		PORTDbits.RD7 = 0;	// pga chip select
		break;
	case LOB_POT:					// pot
		PORTBbits.RB4 = 0;	// pot_cs#
		break;
	default:	// anything else, turn off all devices
		break;
	}
//	DelayFor18TCY();
}

// ChipReserve
void ChipReserve(int dev)
{
	SpiLock = 1;		// lock out isr from accessing spi
	// disable all devices
	PORTBbits.RB4 = 1;	// pot_cs#
	PORTCbits.RC0 = 1;	// slave chip select
	PORTDbits.RD6 = 1;	// eeprom chip select
	PORTDbits.RD7 = 1;	// pga chip select
}

// Chip Select test
//
void chip_select(int code, long *args)
{
	ChipSelect(args[0]);
}

// Set gain value for channel chan
void PgaGain(int chan, char gain)
{
	ChipSelect(LOB_PGA);	// enable PGA chip select
	WriteSPI(0x40);				// Send Command write to gain register
	WriteSPI(gain);				// write gain value to gain register
	ChipDeSelect(LOB_PGA);
	ChipSelect(LOB_PGA);
	WriteSPI(0x41);				// Send Command write to channel register
	WriteSPI(chan);				// write channel number to channel register
	ChipDeSelect(LOB_PGA);// disable PGA chip select
}

// Enable Pga channel 
void PgaChan(int chan)
{
	ChipSelect(LOB_PGA);				// enable PGA chip select
	WriteSPI(0x41);				// Send Command write to channel register
	WriteSPI(chan);				// write channel number to channel register
	ChipDeSelect(LOB_PGA);
}

// set feed motor speed to slow
// Enter:
// flag = 1	start feed motor at slow speed. If feed_slow is 0, feed motor is not started
// flag = 0	set feed motor speed to slow. When top and botton serve motors are at speed,
//          Hold will be turned off, and Run will be set to ramp feed_motor to desired speed
void SlowFeedMtr(int flag)
{
	if (flag)
		PUTRSDIAG("slowfeedmtr(1) ");
	else
		PUTRSDIAG("slowfeedmtr(0) ");
	feed_stat.Run = FALSE;
	if (!feed_stat.Cont) {
		d_feed_per = (unsigned long)feed_slow << 16;
//							putrsDIAG(" *6 ");
		feed_pwm = (unsigned long)feed_slow << 16;
		SetDCPWM3(feed_pwm >> 16);		// Write the new duty cycle.
	}
	if (flag) {
		if (!feed_stat.Cont)
		feed_stat.Slow = TRUE;
		feed_stat.Hold = FALSE;
	} else {
		feed_stat.Hold = TRUE;
		feed_stat.Slow = FALSE;
	}
}

void FeedMtrDone(void)
{
	feed_stat.Hold = FALSE;
	feed_stat.Run = FALSE;
	if (!feed_stat.Cont) {
		// if feed motor not in continuous mode, stop feed motor
// C18 to XC8 Compiler Changes Start
//		d_feed_per = feed_slow << 16;
		d_feed_per = (unsigned long)feed_slow << 16;
// C18 to XC8 Compiler Changes End
//							putrsDIAG(" *7 ");
		feed_stat.Slow = TRUE;					// set flag for SvcShots
		feed_pwm = (unsigned long)feed_slow << 16;
		SetDCPWM3(feed_pwm >> 16);		// Write the new duty cycle.
	}
}

// stop feed motor
void StopFeedMtr(void)
{
	unsigned *ptr = (unsigned *)&feed_stat;
//	PUTRSDIAG("stopfeedmtr() ");
	*ptr = 0;
	feed_pwm = d_feed_per = 0;
	SetDCPWM3(feed_pwm << 2);		// Write the new duty cycle.
}

// Abort CurShot
//
void abort_shot(void)
{
	*(unsigned int*)&top_stat = 0;
	*(unsigned int*)&bot_stat = 0;
	*(unsigned int*)&feed_stat = 0;
	CurShotState = SHOT_IDLE;
	StopFeedMtr();
	StopBidirMtrs();		// this is needed for continuous sweep/elev motors
// v1.194 & v1.195 start
	putrsDIAG("\n\rabort_shot");
// v1.194 & v1.195 end
}

// stop top serve motor
void StopTopMtr(void)
{
	unsigned *ptr = (unsigned *)&top_stat;
	top_pwm = 0L;
	d_top_per = 0;
	SetDCPWM4(top_pwm << 2);		// Write the new duty cycle.
	*ptr = 0;
}

// stop bottom serve motor
void StopBotMtr(void)
{
	unsigned *ptr = (unsigned *)&bot_stat;
	bot_pwm = 0L;
	d_bot_per = 0;
	SetDCPWM5(bot_pwm << 2);		// Write the new duty cycle.
	*ptr = 0;
}

// stop SWEEP and ELEV motors
void StopBidirMtrs(void)
{
	slavecmd(SC_ST, 0);				// stop bidirectional motors
}

void StartSweepRandom(int speed, int limit_r, int limit_l)
{
	long arg[2];
	arg[0] = (long)speed;
	arg[1] = ((long)limit_l << 16) | ((long)limit_r & 0xffff);
	slavecmd(SC_XR,&arg[0]);
}

void StartElevRandom(int speed, int limit_top, int limit_bot)
{
	long arg[2];
	arg[0] = (long)speed;
	arg[1] = ((long)limit_bot << 16) | ((long)limit_top & 0xffff);
	slavecmd(SC_YR,&arg[0]);
}

// sweep test
//
void sweep_diag(int code, long *args)
{
	switch (code) {	
	case 0:			// set trip point (a2d) for sweep motor
		BDM_TripPoint[SWEEP] = args[0];
		break;
	case 1:			// set trip time (a2d) for sweep motor
		BDM_TripTime[SWEEP] = args[0];
		break;
	}
}

// elev test
//
void elev_diag(int code, long *args)
{
	switch (code) {	
	case 0:			// set trip point (a2d) for sweep motor
		BDM_TripPoint[ELEV] = args[0];
		break;
	case 1:			// set trip time (a2d) for sweep motor
		BDM_TripTime[ELEV] = args[0];
		break;
	}
}

// diag command to set up a shot sequence
void CurShot_diag(int code, long *args)
{
	switch (code) {
	case 0:							// CX   send sweep motor command to slave
		CurShot.x = args[0];
		BuildCmd(SC_X1, 1, args);
		break;
	case 1:							// CY	send elev motor command to slave
		CurShot.y = args[0];
		args[0] += elev_corr.s;
		BuildCmd(SC_Y1, 1, args);
		break;
	case 2:							// CT   start top motor
		StartTop(CurShot.top);
		break;
	case 3:							// CB	start bot motor
		StartBot(CurShot.bot);
		break;
	case 4:							// CF	start feed motor when ready
//		StartFeed(feed_limit);
		SetFeedRate(feed_limit);
		StartFeed();
		break;
	case 5:							// CN	start feed motor Continuous!!!
		StartFeedCont(args[0] & 0x3ff);
		break;
	case 6:							// CI	init shots
		CurShot.top = 0x6000;
		CurShot.bot = 0x6000;
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//			CurShot.x = home_loc.sweep;
//			CurShot.y = home_loc.elev;
//		}
//		else {	// CLUB model only
//			CurShot.x = home_loc_CLUB.sweep;
//			CurShot.y = home_loc_CLUB.elev;
//		}
#ifndef PHENOMS
			CurShot.x = home_loc.sweep;
			CurShot.y = home_loc.elev;
#else
			CurShot.x = home_loc_CLUB.sweep;
			CurShot.y = home_loc_CLUB.elev;
#endif
// v1.219 end
		CurShotState = SHOT_INIT;
		break;
	case 7:							// CH	random horizontal
		CurShot.top = 0x5000;
		CurShot.bot = 0x5000;
		CurShot.x = 0x175;
		CurShot.y = 100;
//		CurShot.xLeft = 0 - 0x175;
		CurShot.xLeft = 0;			// just to see sweep lopsided
		CurShot.yLo = 100;
		CurShot.feedrate = 0x3ff;
		CurShot.sweeprate = 0x1ff;
		CurShotState = SHOT_RAN_INIT;
		break;
	case 8:							// CV	random vertical
		CurShot.top = 0x5000;
		CurShot.bot = 0x5000;
		CurShot.x = 0x175;
		CurShot.y = 100;
		CurShot.xLeft = 0x175;
		CurShot.yLo = 10;
		CurShot.feedrate = 0x3ff;
		CurShot.sweeprate = 0x1ff;
		CurShotState = SHOT_RAN_INIT;
		break;
	case 9:
//		StartFeed(feed_limit);
		SetFeedRate(feed_limit);
		StartFeed();
		CurShot.top = args[0];
		CurShot.bot = args[0];
// v1.219 start
//		if ((model_type != CLUB1) && (model_type != CLUB2)){	// all models except CLUBs
//			CurShot.x = home_loc.sweep;
//			CurShot.y = home_loc.elev;
//		}
//		else {	// CLUB model only
//			CurShot.x = home_loc_CLUB.sweep;
//			CurShot.y = home_loc_CLUB.elev;
//		}
#ifndef PHENOMS
			CurShot.x = home_loc.sweep;
			CurShot.y = home_loc.elev;
#else
			CurShot.x = home_loc_CLUB.sweep;
			CurShot.y = home_loc_CLUB.elev;
#endif
// v1.219 end
		start_shot();
		break;
	}
}

void NxtShot_diag(int code, long *args)
{
	switch (code) {
	case 0:							// NX arg1  set X value in NextShot
		NextShot.x = args[0];
		break;
	case 1:							// NY arg1	set Y value in NextShot
		NextShot.y = args[0];
		break;
	case 2:							// NT arg1  set top value in NextShot
		NextShot.top = args[0];
		break;
	case 3:							// NB arg1	set bot value in NextShot
		NextShot.bot = args[0];
		break;
	case 4:							// NS	do NextShot
		CurShot.x = NextShot.x;
		CurShot.y = NextShot.y;
		CurShot.top = NextShot.top;
		CurShot.bot = NextShot.bot;
		CurShotState = SHOT_RUN;
		NextShot.x = 0;
		NextShot.y = 0;
		NextShot.top = 0;
		NextShot.bot = 0;
		break;
	}
}

void MtrResponse(int code, long *args)
{
	if (stat.WaitResp == FALSE) {
		dumphex((char)SlaveMotorStatus);
	}
}

// tell slave to give us status on the bidirectional motors
void SndSlvMtrStatus(void)
{
	BuildCmd(SC_MS, 0, (long *)0);
	stat.WaitResp = TRUE;
}

// assuming SC_MS was sent, check for response
// returns 1 if ready, 0 if motors busy, 2 if command not done
//         3 if sweep motor error, 4 if elevation motor error, 5 if both motors error
int GetMtrResponse(void)
{
#ifdef NOHOME	
	return(1);
#else // HOME
	int rc;
	if ((LastSlaveCmd == (char)SC_MS) && stat.WaitResp) rc = 2;
	else if (SlaveMotorStatus) {
		switch (SlaveMotorStatus & 0x0c) {
		case 0:
			rc = 0;				// no error, just busy
			break;
		case 4:					// sweep motor error
			rc = 3;
//#ifdef FEEDBACK	// v1.186 commented
			ErrorStat.SweepCnt = 1;
//#endif			// v1.186 commented
			PUTRSDIAG((const rom char *)"Sweep Motor Error\n\r");
			break;
		case 8:					// elevation motor error
			rc = 4;
//#ifdef FEEDBACK	// v1.186 commented
			ErrorStat.ElevCnt = 1;
//#endif			// v1.186 commented
			PUTRSDIAG((const rom char *)"Elevation Motor Error\n\r");
			break;
		case 0x0c:				// sweep and elevation motor error
			rc = 5;
//#ifdef FEEDBACK	// v1.186 commented
			ErrorStat.SweepCnt = 1;
			ErrorStat.ElevCnt = 1;
//#endif			// v1.186 commented
			PUTRSDIAG((const rom char *)"Sweep and Elevation Motors Error\n\r");
			break;
		}
//#ifdef FEEDBACK	// v1.186 commented
		if (rc)
			if (ErrorShutDown == 0) ErrorShutDown = 1;
//#endif			// v1.186 commented
	} else rc = 1;
	return (rc);
#endif // NOHOME else HOME
}

//void StartFeed(long feedpwm)
//{
//#ifdef FEEDBACK
//	if (ErrorShutDown) return;
//#endif
//	PUTRSDIAG("StartFeed:");
//	DUMPSHORT(feedpwm);
//	if (!feed_stat.Cont) { // if feed motor is not continuously on
////		d_feed_per = feedpwm << 16;
//		last_feed_pwm = feedpwm;
//	}
//  ball_feed = ((d_top_per < 0x2000U)?0x180:0x200) + ((d_top_per - 0x1000U) >> 7);
//	SlowFeedMtr(0);		// feed motor full speed when serve motors at param
//}

void SetFeedRate(long feedpwm)
{
	last_feed_pwm = feedpwm;
	if (d_feed_per) {						// feed motor already running
		d_feed_per = feedpwm << 16;
//		putrsDIAG(" *8 ");
	}
}

void StartFeed(void)
{
	unsigned *ptr = (unsigned *)&feed_stat;
//#ifdef FEEDBACK	// v1.186 commented
	if (ErrorShutDown) return;
//#endif			// v1.186 commented
// v1.194 & v1.195 start
// v1.200 start
	putrsDIAG("\n\rStartFeed");	// v1.214 commented
// v1.200 end
// v1.194 & v1.195 end
	DUMPSHORT(feedpwm);
	d_feed_per = last_feed_pwm << 16;
//	putrsDIAG(" *9 ");
	ball_feed = ((d_top_per < 0x2000U)?0x180:0x200) + ((d_top_per - 0x1000U) >> 7);
// v1.207 start
	ball_feed_bottom = ((d_bot_per < 0x2000U)?0x180:0x200) + ((d_bot_per - 0x1000U) >> 7);
// v1.207 end
// v1.206 start
	putrsDIAG("\n\rball_feed = ");	// v1.214 commented
	dumplong(ball_feed);			// v1.214 commented
// v1.206 end
	if (feed_stat.Cont) {
		*ptr = 0;
		feed_stat.Cont = TRUE;
	} else *ptr = 0;
	if (d_feed_per) {
		feed_stat.Hold = TRUE;
	}	
}

void StartFeedAtSpd(void)
{
// v1.194 & v1.195 start
	putrsDIAG("\n\rStartFeedAtSpd");
// v1.194 & v1.195 end
	ball_feed = ((d_top_per < 0x2000U)?0x180:0x200) + ((d_top_per - 0x1000U) >> 7);
// v1.207 start
	ball_feed_bottom = ((d_bot_per < 0x2000U)?0x180:0x200) + ((d_bot_per - 0x1000U) >> 7);
// v1.207 end
// v1.206 start
	putrsDIAG("\n\rball_feed = ");
	dumplong(ball_feed);
// v1.206 end
// 07/30/09 start
//	if (last_feed_pwm)
	if (last_feed_pwm) {
		feed_stat.Hold = 1;
    }
// 07/30/09 end
}

// call this to set feed rate. If the feed motor is running, it will have immediate effect.
void SetFeedImm(long feedpwm)
{
//#ifdef FEEDBACK	// v1.186 commented
	if (ErrorShutDown) return;
//#endif			// v1.186 commented
// v1.194 & v1.195 start
	putrsDIAG("\n\rSetFeedImm");
// v1.194 & v1.195 end
	if (d_feed_per) {
		d_feed_per = feedpwm << 16;
//		putrsDIAG(" *10 ");
	}
	last_feed_pwm = feedpwm;
}

void StartFeedCont(long feedpwm)
{
//#ifdef FEEDBACK	// v1.186 commented
	if (ErrorShutDown) return;
//#endif			// v1.186 commented
// v1.194 & v1.195 start
	putrsDIAG("\n\rStartFeedCont");
	ball_feed = ((d_top_per < 0x2000U)?0x180:0x200) + ((d_top_per - 0x1000U) >> 7); // just like StartFeed and StartFeedAtSpd routines
// v1.207 start
	ball_feed_bottom = ((d_bot_per < 0x2000U)?0x180:0x200) + ((d_bot_per - 0x1000U) >> 7);
// v1.207 end
// v1.206 start
	putrsDIAG("\n\rball_feed = ");
	dumplong(ball_feed);
// v1.206 end
// v1.194 & v1.195 end
	d_feed_per = feedpwm << 16;
//	putrsDIAG(" *11 ");
	last_feed_pwm = feedpwm;
	feed_stat.Hold = FALSE;
	feed_stat.Run = FALSE;
	feed_stat.Cont = TRUE;
//	PUTRSDIAG((const rom char*)"\n\StartFeedCont=");
//	DUMPLONG(feedpwm);
}

// feed test
//
void feed_diag(int code, long *args)
{
	switch (code) {
	case 1:			// stop feed motor
		StopFeedMtr();
		break;
	case 0:			// set desired pwm
//		StartFeed(feed_limit);
		SetFeedRate(feed_limit);
		StartFeed();
		break;
	case 2:			// update feed motor pwm
		SetFeedPwm();
		putrsDIAG(crlf);
		DUMPLONG(feed_pwm);
		break;
	case 3:			// set feed motor pwm limit
		if (args[0] > 0x3ff) args[0] = 0x3ff;
		feed_limit = args[0] & 0x3ff;
		break;
	case 4:			// set feed motor acceleration
		accel_feed = args[0];
		break;
	case 5:			// set trip point (a2d) for feed motor
		BDM_TripPoint[FEED] = args[0];
		break;
	case 6:			// set trip time (a2d) for feed motor
		BDM_TripTime[FEED] = args[0];
		break;
	case 8:
		feed_slow = args[0];
	case 7:			// start feed motor at slow speed
//		SlowFeedMtr(1);
		FeedMtrDone();					// if feed_slow is 0, will stop feed motor
		break;
	case 9:			// change feed rate now
		SetFeedImm(args[0]);
		break;
	}
}

void StartTop(unsigned val){
	unsigned delta;
	int ix;

//#ifdef FEEDBACK	// v1.186 commented
	if (ErrorShutDown) return;
//#endif			// v1.186 commented
//		if ((val == 0U) || (val == 0xffffU)) {
//			StopTopMtr();
//			return;
//		}

	if ((val > SRV_MINIMUM) || (val < SRV_MAXIMUM)) { 		// stop all motors if invalid values
		stop_all(0,(long*)0);
		ErrorStat.TopPeriod = 1;
		ErrorShutDown = 1;
		return;
	}
	INTCONbits.GIE = 0;
		if (m_top_per == 0U) {
			delta = 6000U - val;
			top_dir = 1;			// increasing speed
		} else if (m_top_per > val) {
			delta = m_top_per - val;
			top_dir = 1;			// increasing speed
		} else {
			delta = val - m_top_per;
			top_dir = 0;			// decreasing speed
		}
		if (m_top_per && delta > 500U) {
			top_stat.phase2 = FALSE;
			top_stat.phase1 = TRUE;
			if (top_dir) {			// increasing speed
				if (delta > 0x1000U) {
					if (val > 0x3000U)
						top_mid = val + 0xb00U + ((0x4000U - delta) >> 5) + ((0x4000U - delta) >> 4);
					else
						top_mid = val + 0x900U + ((0x4000U - delta) >> 5);
				} else {
					top_mid = m_top_per - (delta >> 2);
				}
			} else {
				top_mid = m_top_per + (delta >> 4);
				PUTRSDIAG("Mid:");
				DUMPSHORT(top_mid);
			}
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range[ix][ACC_DELTA] = accel_hi[ix][ACC_DELTA];
				accel_range[ix][ACC_CHG] = accel_hi[ix][ACC_CHG];
			}
			top_stat.Run = TRUE;
		} else if (m_top_per != 0U && delta > 2000U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range[ix][ACC_DELTA] = (top_dir)?accel_hi[ix][ACC_DELTA]:accel_hi_d[ix][ACC_DELTA];
				accel_range[ix][ACC_CHG] = (top_dir)?accel_hi[ix][ACC_CHG]:accel_hi_d[ix][ACC_CHG];
			}
		} else if (m_top_per != 0U && delta > 1000U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range[ix][ACC_DELTA] = (top_dir)?accel_med[ix][ACC_DELTA]:accel_med_d[ix][ACC_DELTA];
				accel_range[ix][ACC_CHG] = (top_dir)?accel_med[ix][ACC_CHG]:accel_med_d[ix][ACC_CHG];
			}
		} else if (m_top_per != 0U && delta > 400U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range[ix][ACC_DELTA] = (top_dir)?accel_low[ix][ACC_DELTA]:accel_low_d[ix][ACC_DELTA];
				accel_range[ix][ACC_CHG] = (top_dir)?accel_low[ix][ACC_CHG]:accel_low_d[ix][ACC_CHG];
			}
		} else {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range[ix][ACC_DELTA] = (top_dir)?accel_low[ix][ACC_DELTA]:accel_xlow_d[ix][ACC_DELTA];
				accel_range[ix][ACC_CHG] = (top_dir)?accel_low[ix][ACC_CHG]:accel_xlow_d[ix][ACC_CHG];
			}
		}
		
	d_top_per = val;
//	INTCONbits.GIE = 0;
	if (feed_stat.Run) {
		// we shouldn't get here
		SlowFeedMtr(0);
		SetTopPwm();
//		UpdTopPer();
	}
	INTCONbits.GIE = 1;
	CalcLimits(val, &TopSpdLimit[0]);
}

// top motor test
//
void top_diag(int code, long *args)
{
	unsigned delta,ix;

	switch (code) {
	case 1:			// stop top motor
		StopTopMtr();
		break;
	case 0:			// set desired per
		StartTop((unsigned)args[0]);
		break;
	case 2:			// update top motor pwm
//		UpdTopPer();				// save value for printing only
		SetTopPwm();
// C18 to XC8 Compiler Changes Start
//		putrsDIAG((const rom char *)"\n\rtop_t2=");
//		dumplong(last_top_t2);
//		putrsDIAG((const rom char *)" top_t1=");
//		dumplong(last_top_t1);
//		putrsDIAG((const rom char *)" period=");
//		dumplong(last_top_t2 - last_top_t1);
//		putrsDIAG((const rom char *)" dperiod=");
		putrsDIAG((const char *)"\n\rtop_t2=");
		dumplong(last_top_t2);
		putrsDIAG((const char *)" top_t1=");
		dumplong(last_top_t1);
		putrsDIAG((const char *)" period=");
		dumplong(last_top_t2 - last_top_t1);
		putrsDIAG((const char *)" dperiod=");
// C18 to XC8 Compiler Changes End
		dumpaddr(d_top_per);
		dumplong(top_pwm);
		break;
	case 3:			// set top motor pwm limit
		top_limit = (unsigned)args[0];
		break;
	case 4:			// read timer elapsed time
		UpdTopPer();
// C18 to XC8 Compiler Changes Start
//		putrsDIAG((const rom char *)"\n\rperiod=");
//		dumplong(last_top_t2 - last_top_t1);
//		putrsDIAG((const rom char *)" delta=");
//		dumplong(top_last_delta);
//		putrsDIAG((const rom char *)" last_delta=");
//		dumplong(top_last_delta1);
//		putrsDIAG((const rom char *)" dperiod=");
		putrsDIAG((const char *)"\n\rperiod=");
		dumplong(last_top_t2 - last_top_t1);
		putrsDIAG((const char *)" delta=");
		dumplong(top_last_delta);
		putrsDIAG((const char *)" last_delta=");
		dumplong(top_last_delta1);
		putrsDIAG((const char *)" dperiod=");
// C18 to XC8 Compiler Changes End
		dumplong(d_top_per);
		dumplong(top_pwm);
		break;
	case 5:			// set top motor acceleration
		accel_top = (unsigned)args[0];
		break;
	case 6:			// set top/bottom motor hold flags
		top_stat.Hold = (unsigned)args[0];
		bot_stat.Hold = (unsigned)args[1];
		break;
	}
}

void StartBot(unsigned val){
	unsigned delta;
	int ix;

//#ifdef FEEDBACK	// v1.186 commented
	if (ErrorShutDown) return;
//#endif			// v1.186 commented
//		if ((val == 0U) || (val == 0xffffU)) {
//			StopBotMtr();
//			return;
//		}
	if ((val > SRV_MINIMUM) || (val < SRV_MAXIMUM)) { 		// stop all motors if invalid values
		stop_all(0,(long*)0);
		ErrorStat.BotPeriod = 1;
		ErrorShutDown = 1;
		return;
	}
	INTCONbits.GIE = 0;
		if (m_bot_per == 0U) {
			delta = 6000U - val;
			bot_dir = 1;			// increasing speed
		} else if (m_bot_per > val) {
			delta = m_bot_per - val;
			bot_dir = 1;			// increasing speed
		} else {
			delta = val - m_bot_per;
			bot_dir = 0;			// decreasing speed
		}
		if (m_bot_per && delta > 500U) {
			bot_stat.phase2 = FALSE;
			bot_stat.phase1 = TRUE;
			if (bot_dir) {			// increasing speed
				if (delta > 0x1000U) {
					if (val > 0x3000U)
						bot_mid = val + 0xb00U + ((0x4000U - delta) >> 5) + ((0x4000U - delta) >> 4);
					else
						bot_mid = val + 0x900U + ((0x4000U - delta) >> 5);
				} else {
					bot_mid = m_bot_per - (delta >> 2);
				}
			} else {
				bot_mid = m_bot_per + (delta >> 4);
				PUTRSDIAG("BMid:");
				DUMPSHORT(bot_mid);
			}
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range_bot[ix][ACC_DELTA] = accel_hi[ix][ACC_DELTA];
				accel_range_bot[ix][ACC_CHG] = accel_hi[ix][ACC_CHG];
			}
			bot_stat.Run = TRUE;
		} else if (m_bot_per != 0U && delta > 2000U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range_bot[ix][ACC_DELTA] = (bot_dir)?accel_hi[ix][ACC_DELTA]:accel_hi_d[ix][ACC_DELTA];
				accel_range_bot[ix][ACC_CHG] = (bot_dir)?accel_hi[ix][ACC_CHG]:accel_hi_d[ix][ACC_CHG];
			}
		} else if (m_bot_per != 0U && delta > 1000U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range_bot[ix][ACC_DELTA] = (bot_dir)?accel_med[ix][ACC_DELTA]:accel_med_d[ix][ACC_DELTA];
				accel_range_bot[ix][ACC_CHG] = (bot_dir)?accel_med[ix][ACC_CHG]:accel_med_d[ix][ACC_CHG];
			}
		} else if (m_bot_per != 0U && delta > 400U) {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range_bot[ix][ACC_DELTA] = (bot_dir)?accel_low[ix][ACC_DELTA]:accel_low_d[ix][ACC_DELTA];
				accel_range_bot[ix][ACC_CHG] = (bot_dir)?accel_low[ix][ACC_CHG]:accel_low_d[ix][ACC_CHG];
			}
		} else {
			for (ix=0; ix<MAX_ACC; ix++) {
				accel_range_bot[ix][ACC_DELTA] = (bot_dir)?accel_low[ix][ACC_DELTA]:accel_xlow_d[ix][ACC_DELTA];
				accel_range_bot[ix][ACC_CHG] = (bot_dir)?accel_low[ix][ACC_CHG]:accel_xlow_d[ix][ACC_CHG];
			}
		}
		
	d_bot_per = val;
//	INTCONbits.GIE = 0;
	if (feed_stat.Run) {
		// we shouldn't get here
		SlowFeedMtr(0);
		SetBotPwm();
//		UpdBotPer();
	}
	INTCONbits.GIE = 1;
	CalcLimits(val, &BotSpdLimit[0]);
}

// bot motor test
//
void bot_diag(int code, long *args)
{
	unsigned delta,ix;
	switch (code) {
	case 1:			// stop bot motor
		StopBotMtr();
		break;
	case 0:			// set desired per
		StartBot((unsigned)args[0]);
		break;
	case 2:			// update bot motor pwm
		UpdBotPer();
		SetBotPwm();
		dumpaddr(m_bot_per);
		dumplong(d_bot_per);
		dumplong(bot_pwm);
		break;
	case 3:			// set bot motor pwm limit
		bot_limit = (unsigned)args[0];
		break;
	case 4:			// read timer elapsed time
		UpdBotPer();
		dumpaddr(m_bot_per);
		dumplong(bot_pwm);
		break;
	case 5:			// set bot motor acceleration
		accel_bot = args[0];
		break;
	}
}

// stop all motors
//
void stop_all(int code, long *args)
{
	if (DoDebug) {
		start_count = 0;
		min_count = 0;
		max_count = 0;
	}
	StopTopMtr();
	StopFeedMtr();
	StopBotMtr();
	StopBidirMtrs();
	CurShotState = SHOT_IDLE;
}

void pwm_debug(int code, long *args)
{
	switch (code) {
	case 0:							// init sampling
		INTCONbits.GIE = 0;
		max_count = m_bot_per;
		min_count = m_bot_per;
		INTCONbits.GIE = 1;
		break;
	case 1:							// dump counts
		dumpaddr(start_count);
		dumplong(min_count);
		dumplong(max_count);
		break;
	case 2:							// start debug
		DoDebug = 1;
		break;
	default:
		break;
	}
}

void slavecmd(int code, long *args)
{
	variant y;
	// X? are for sweep motor, Y? are elevation motor
	char c = (char)code;
	switch (c) {
	case SC_Y1:						// Y1 args[0]
		args[0] += elev_corr.s;
//		putrsDIAG("SC_Y1:");
//		dumplong(args[0]);
	case SC_X1:						// X1 args[0] - move 
	case SC_XA:						// XA args[0] - set acceleration
	case SC_YA:						// YA args[0]
	case SC_XL:						// XL args[0] - set limit (degrees)
	case SC_YL:						// YL args[0]
	case SC_XS:						// XS args[0] - set home speed
	case SC_YS:						// YS args[0]
	case SC_XV:						// XV args[0] - set max velocity
	case SC_YV:						// YV args[0]
	case SC_XO:						// XO args[0] - set home offset
	case SC_YO:						// YO args[0]
	case SC_LT:						// LT args[0] - set LED bits (lsb)
	case SC_XH:						// XH		  - home motor
	case SC_YH:						// YH
		BuildCmd(code, 1, args);
		break;
	case SC_YR:
		y.l = args[1];
		y.s[0] += elev_corr.s;
		y.s[1] += elev_corr.s;
		args[1] = y.l;
	case SC_XR:
		BuildCmd(code, 2, args);
		break;
	case SC_DS:						// DS args[0] - get value at args[0]
		BuildCmd(code, 1, args);
		stat.WaitResp = TRUE;
		break;
	case SC_X2:						// X2         - 50% pwm, enable motor
	case SC_Y2:						// Y2
	case SC_X3:						// X3		  - stop motor
	case SC_Y3:						// Y3
	case SC_ST:						// ST		  - stop all motors
	case SC_OC:						// OC			- Slave SPI output off
	case SC_OS:						// OS			- Slave SPI output on
		BuildCmd(code, 0, args);
		break;
	case SC_XP:						// XP         - return current position
	case SC_YP:						// YP
	case SC_MS:						// MS        returns motor status bit 0 for SWEEP, 1=busy
									//                                    1 for ELEV,  1=busy
		BuildCmd(code, 0, args);
		// wait for response
		stat.WaitResp = TRUE;
		break;
	case SC_SS:						// SS a0 a1   - *(a0) = a1
		BuildCmd(code, 2, args);
		break;
	}
}

// Builds Slave Command
//
void BuildCmd(int code, int cnt, long *args)
{
	unsigned long val;
	unsigned char checksum;
	char *cptr;
	int i;
	char c;

	// if in error shutdown mode, only allow shutdown command to 
	//  go to the slave
	if (ErrorShutDown && (code != (int)SC_ST)) {
		return;
	}
//	if (code == SC_Y1) {
//		putrsDIAG("SC_Y1:");
//		dumplong(args[0]);		
//	}
	SlaveCmdOfs = 0;
	checksum = 0;
	checksum = SlaveCmd[SlaveCmdOfs++] = 'S';					// Start character
	checksum += (SlaveCmd[SlaveCmdOfs++] = (cnt+1) << 2);		// # longs
	checksum += (SlaveCmd[SlaveCmdOfs++] = code);				// command
	while (cnt) {												// for each long
		--cnt;
		val = *args++;											//	get it
		for (i=0; i<4; i++) {									//	split it into bytes
			checksum += (SlaveCmd[SlaveCmdOfs++] = val & 0xff);	//	 and sum it
			val >>= 8;
		}
	}
	SlaveCmd[SlaveCmdOfs++] = 0 - checksum;						// finally write checksum
	LastSlaveCmd = (char)code;									// remember as last command

	cptr = &SlaveCmd[0];										// send the command
	CRLF();
	for (i=0; i<SlaveCmd[1]; i++) {								// process each byte of command
		c = *cptr;												//	acquire next byte
// C18 to XC8 Compiler Changes Start debugging
//		putrsDIAG("\n\rc(master) = ");
//		dumphex(c);
//		Delay1KTCYx(5);
// C18 to XC8 Compiler Changes End debugging
		DUMPHEX(*cptr);											//	must be here for delay - figure out something else
		ChipSelect(LOB_SLAVE);									//	select slave
		WriteSPI(*cptr++);										//	send byte
		ChipDeSelect(LOB_SLAVE);								//	release slave
// C18 to XC8 Compiler Changes Start
//		Delay1KTCYx(2);
// For the XC8 compiler, this delay had to be increased to prevent the elevation motor from occasionally going past home and hitting a mechanical stop, thereby self destructing.
// To help confirm the necessary delay, I first decreased it from 2 to 1 and then the elevation motor went past the home sensor and hit the mechanical stop every time, i.e. the
// bug got much worse. During testing, the over-current threshold and over-current duration were reduced for the sweep and elevation motors to prevent self destruction.
		Delay1KTCYx(3);											//	
// C18 to XC8 Compiler Changes End
	}
	CRLF();
	ChipSelect(LOB_SLAVE);										//	select slave
	c = ReadSPI();												//	acquire any slave response
// C18 to XC8 Compiler Changes Start debugging
//		putrsDIAG("\n\rc(slave) = ");
//		dumphex(c);
//		Delay1KTCYx(5);
// C18 to XC8 Compiler Changes End debugging
	DUMPHEX(c);					
	ChipDeSelect(LOB_SLAVE);									//	release slave
	if (code == (int)SC_MS)										//	if this command is a motor status
		SlaveMotorStatus = 0x03;								//	 set motor status to busy
}

// command to slave had timed out, clear any data in buffer
void SlvTimeOut(void)
{
	SlaveResOfs = 0;
	stat.WaitResp = FALSE;
}

// C18 to XC8 Compiler Changes start
//far void remote_diag(int code, long* args)
void remote_diag(int code, long* args)
// C18 to XC8 Compiler Changes end
{
	putrsDIAG("\r\n");
	do {
		while (!DataRdy1USART());
		dumphex(Read1USART());
	} while (DataRdy1USART());
}

// C18 to XC8 Compiler Changes start
//far void kbdiag (int code, long *args)
void kbdiag (int code, long *args)
// C18 to XC8 Compiler Changes end
{
	unsigned char uc, i;
	unsigned char col;
	static volatile unsigned short loop;

	switch (code) {
	case 0:
		putrsDIAG(lob_model);
		dumphex(args[0]);
		putrsDIAG(space);
		PORTE = args[0]^0xff;
		uc = PORTD & 0x3f;
		dumphex(uc);
		break;
	case 1:
		putrsDIAG(lob_model);
		for (i=0,col=1; (col&0xff); col<<=1,i++) {
//			dumphex(col);
			PORTE = col^0xff;
			uc = PORTD & 0x3f;
			if (uc != 0x3fU) break;
//			dumphex(uc);
//			putrsDIAG(space);
//			for (loop; loop<args[0]; loop++)
//				;
		}
		if (i == 8U) {
			putrsDIAG("No key.");
			break;
		}
		i *= 6;
		uc ^= 0x3f;
		for (loop=1; uc; uc>>=1,loop++)
/*			dumphex(loop)/**/;
		dumphex(i+loop-2);
		break;
	}
	putrsDIAG(crlf);		
}

// Diagnostic Call format
//	perform a power OFF sequence
//
void poweroff(int code, long *args)
{
//-----------------------------------------------------------------------------
// Cleaned up Power On/Off code and Power button detection code.	
// If power.count=0, then we are  not here due to Power button glitch when in
// sleep mode. So we can save EEPROM data, otherwise the SPI is not yet enabled.
	if (power.count == 0)
//-----------------------------------------------------------------------------
// This following line was causing a machine lockup after wakeup due to a Power
// button glitch.
	save_prog(loaded_progno,&act_program);
	PORTA=0;							// Deactivate +12v and +5v? - turn the slave OFF
	PUTRSDIAG("slave shutdown ");
	CloseSPI();						// Disable SPI
	Close1USART();				// Disable UARTs
	Close2USART();
	ClosePWM4();
	ClosePWM3();
	ClosePWM5();
	CloseTimer4();
	CloseCapture1();
	CloseTimer1();
	CloseCapture2();
	CloseTimer3();
	ADCON0 = 0;
	CloseTimer1();
	CloseTimer0();
//	TRISA=0xff;						// All I/O ports to input
//	TRISB=0xff;
//	TRISC=0xff;
//	TRISD=0xff;
//	TRISE=0xff;
//	TRISF=0xff;
//	TRISG=0xff;
	PORTA=0;								// All I/O ports to zero.
	PORTB=0;
	PORTC=0;
	PORTD=0;
	PORTE=0;
	PORTF=0;
	PORTG=0;
	TRISA=0x00;							// All I/O ports to output, except Power button
	TRISB=0x01;							// input (INT0), which is used to wake uC.
	TRISC=0x00;
	TRISD=0x00;
	TRISE=0x00;
	TRISF=0x00;
	TRISG=0x00;
//----------------------------------------------------------------------------------------
	WDTCONbits.SWDTEN = 0;	// Turn off Watch Dog Timer
	INTCONbits.GIE = 0;		// Set so when we wake up, we restart from sleep instruction rather than vector to ISR.
	INTCONbits.PEIE = 0;
	INTCONbits.INT0IF = 0;// Cleaned up Power On/Off code and Power button detection code.
	INTCONbits.INT0IE = 1;// Cleaned up Power On/Off code and Power button detection code.
	Sleep();							// good night sweet prince...
	Nop();								// just cuz
	INTCONbits.INT0IF = 0;// Cleaned up Power On/Off code and Power button detection code.
	INTCONbits.INT0IE = 0;// Cleaned up Power On/Off code and Power button detection code, don't allow power switch to interrupt, just wake.
}

ulong FeedPwm2Sec(ulong feed)
{
	int i;

	if (!feed) return 0;			// no feed rate
	for (i=0; i<14; i++) {
		if ((ulong)base_feeds_cont[i] <= feed)
			return (i+2);
	}
	return (6);
	
}

void CalcLimits(ulong count, unsigned *SpdLimit)
{
	int i;
	// convert count back to speed index
	for (i=0; FeedStartTbl[i][FT_SPEED]; i++) {
		if (count == FeedStartTbl[i][FT_SPEED]) {
			*SpdLimit++ = FeedStartTbl[i][FT_LOWER];
			*SpdLimit = FeedStartTbl[i][FT_UPPER];
			break;
		}
	}
}

int SpeedInRange()
{
	if ((m_top_per <= TopSpdLimit[0]) && (m_top_per >= TopSpdLimit[1])
		&& (m_bot_per <= BotSpdLimit[0]) && (m_bot_per >= BotSpdLimit[1]))
		return 1;
	else return 0;

}

void ForceShot(int code, long* args)
{
	LargeSpdDrop = 1;
}

void CheckBattery(unsigned int val)
{
		if (val <= BAT_TripPoint) {
			if (++BAT_CurSenseCt >= BAT_TripTime) {
				PUTRSDIAG("Low Battery!!!!");
// 11/04/09 start
//				putrsDIAG("\n\rLow Battery!!!!\n\r");
//				dumplong(val);
//				putrsDIAG("\n\r");
// 11/04/09 end
				stop_all(0,(long*)0);
				DUMPSHORT(a2dChan);
				DUMPLONG(val);
				PUTRSDIAG(ShutDown);
				BAT_CurSenseCt = BAT_TripTime;	// this is to prevent rollover
//#ifdef FEEDBACK	// v1.186 commented
				ErrorStat.LoBat = 1;
				if (ErrorShutDown == 0)	ErrorShutDown = 1;
//#endif			// v1.186 commented
			}
		} else BAT_CurSenseCt = 0;		// reset count
}

unsigned int BatteryAverage(unsigned int val)
{
	int i, j;
	unsigned int BatAvgVal = 0;
	unsigned int minVal, maxVal;
	
	if (BatAvgCnt < 20) {
		BatAvg[BatAvgCnt++] = val;
	} else {
		for (i=0; i<19; i++)
			BatAvg[i] = BatAvg[i+1];
		BatAvg[19] = val;
	}
	if (BatAvgCnt < 4) {
		maxVal = 0;
		for (i=0; i<BatAvgCnt; i++) 
			if (BatAvg[i] > maxVal) maxVal = BatAvg[i];
			BatAvgVal = maxVal;
	} else {
		for (i=0; i<BatAvgCnt; i++) BatAvgVal += BatAvg[i];
		BatAvgVal = BatAvgVal / BatAvgCnt;
		minVal = BatAvgVal - BatAvgVal / 10;
		maxVal = BatAvgVal + BatAvgVal / 10;
		for (i=0,BatAvgVal=0,j=0; i<BatAvgCnt; i++) {
			if ((BatAvg[i] >= minVal) && (BatAvg[i] <= maxVal)) {
				BatAvgVal += BatAvg[i];
				++j;
			}
		}
		if (!j) {
			for (i=0,BatAvgVal=0,j=0; i<BatAvgCnt; i++) { 
				if (BatAvg[i] >= minVal) {
					BatAvgVal += BatAvg[i];
					++j;
				}
			}
		}
		if (j) BatAvgVal = BatAvgVal / j;
	}
	return BatAvgVal;
}
