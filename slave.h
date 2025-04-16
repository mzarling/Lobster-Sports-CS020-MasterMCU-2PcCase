// Slave Definitions
//
// (C)2007 Censerve(R) Consulting, Inc.
//				 All rights reserved.
//
// slave command codes
#define	SC_X1	'A'				// X1 args[0] - move 
#define SC_Y1	'a'				// Y1 args[0]
#define	SC_X2	'B'				// X2         - 50% pwm, enable motor
#define	SC_Y2	'b'				// Y2
#define	SC_X3	'C'				// X3		  - stop motor
#define	SC_Y3	'c'				// Y3
#define SC_XA	'D'				// XA args[0] - set acceleration
#define	SC_YA	'd'				// YA args[0]
#define	SC_XH	'E'				// XH		  - home motor
#define	SC_YH	'e'				// YH
#define	SC_XL	'F'				// XL args[0] - set limit (degrees)
#define	SC_YL	'f'				// YL args[0]
#define	SC_XP	'G'				// XP         - return current position
#define	SC_YP	'g'				// YP
#define	SC_XS	'H'				// XS args[0] - set home speed
#define	SC_YS	'h'				// YS args[0]
#define	SC_XV	'I'				// XV args[0] - set max velocity
#define	SC_YV	'i'				// YV args[0]
#define	SC_XO	'J'				// XO args[0] - set home offset
#define	SC_YO	'j'				// YO args[0]
#define SC_XR	'K'				// XR args[0] - start sweep motor continuous
#define SC_YR	'k'				// YR args[0] - start elev motor continuous
#define	SC_DS	'0'				// DS args[0] - get value at args[0]
#define	SC_SS	'1'				// SS a0 a1   - *(a0) = a1
#define	SC_ST	'2'				// ST		  - stop all motors
#define	SC_LT	'3'				// LT args[0] - set LED bits (lsb)
#define	SC_MS	'4'				// MS        returns motor status bit 0 for SWEEP, 1=busy
								//                                    1 for ELEV,  1=busy
#define SC_OC	'5'				// OC 			 tell slave to turn off SPI output
#define SC_OS '6'				// OS				 tell slave to turn on SPI output

