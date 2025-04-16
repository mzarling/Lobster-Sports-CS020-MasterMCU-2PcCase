;*********************************************************************
;*
;*  This contains definitions for the RTC library
;*
;*********************************************************************
;* FileName:            16RTCInt.inc
;* Dependencies:
;* Processor:
;* Assembler:           MPASMWIN 02.70.02 or higher
;* Linker:              MPLINK 2.33.00 or higher
;* Company:             Microchip Technology, Inc.
;*
;* Software License Agreement
;*
;* The software supplied herewith by Microchip Technology Incorporated
;* (the "Company") for its PICmicro® Microcontroller is intended and
;* supplied to you, the Company's customer, for use solely and
;* exclusively on Microchip PICmicro Microcontroller products. The
;* software is owned by the Company and/or its supplier, and is
;* protected under applicable copyright laws. All rights are reserved.
;* Any use in violation of the foregoing restrictions may subject the
;* user to criminal sanctions under applicable laws, as well as to
;* civil liability for the breach of the terms and conditions of this
;* license.
;*
;* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
;* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
;* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
;* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
;* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
;* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;*
;*
;* Author                         Date            Comment
;*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;* Abdelwahb Fassi-Fihri      Dec 20, 2002    pre-Initial Release
;* Abdelwahb Fassi-Fihri      Mar 14, 2003    pre-Initial Release (V1.00)
;*
;*
;********************************************************************/

#ifndef         __16RTCInt_INC
                #define       __16RTCInt_INC


        include "16RTCInt.def"
#ifdef     RTC_12800YEARDAYOFWEEKON
           #ifndef    RTC_CLOCKON
                      error    "RTC_CLOCKON must be ON when 12800YEARDAYOFWEEKON is ON"
           #endif
           #ifndef    RTC_DAYMONTHYEARON
                      error    "RTC_DAYMONTHYEARON must be ON when 12800YEARDAYOFWEEKON is ON"
           #endif
           #ifndef    RTC_CENTURYON
                      error    "RTC_CENTURYON must be ON when 12800YEARDAYOFWEEKON"
           #endif
           #ifndef    RTC_DOWON
                      error    "RTC_DOWON must be ON when 12800YEARDAYOFWEEKON is ON"
           #endif
           #ifdef     RTC_USERDOWON
                      error    "USERDOWON Cannot be ON when 12800YEARDAYOFWEEKON is ON"
           #endif
           #ifdef     RTC_100YEARDAYOFWEEKON
                      error    "RTC_100YEARDAYOFWEEKON Cannot be ON when 12800YEARDAYOFWEEKON is ON"
           #endif
#endif


#ifdef     RTC_DOWON
           #ifdef     RTC_USERDOWON
                      error    "USERDOWON Cannot be ON when RTC_DOWON is ON"
           #endif
           #ifndef     RTC_100YEARDAYOFWEEKON
                       #ifndef        RTC_12800YEARDAYOFWEEKON
                       error    "RTC_DOWN Cannot be ON when 12800YEARDAYOFWEEKON or RTC_100YEARDAYOFWEEKON is not ON"
                       #endif
           #endif
#endif


#ifdef     RTC_100YEARDAYOFWEEKON
           #ifndef    RTC_CLOCKON
                      error    "RTC_CLOCKON must be ON when 100YEARDAYOFWEEKON is ON"
           #endif
           #ifndef    RTC_DAYMONTHYEARON
                      error    "RTC_DAYMONTHYEARON must be ON when 100YEARDAYOFWEEKON is ON"
           #endif
           #ifndef    RTC_DOWON
                      error    "RTC_DOWON must be ON when 100YEARDAYOFWEEKON is ON"
           #endif
           #ifdef     RTC_USERDOWON
                      error    "USERDOWON Cannot be ON when 100YEARDAYOFWEEKON is ON"
           #endif

#endif


#ifndef RTC_Source
;****************************************************************************
;Definations of Shared Parameters
;****************************************************************************

    EXTERN  _vRTCvariables

    EXTERN  RTCIntInit
    EXTERN  RTCIntISR

    #ifdef  RTC_DAYMONTHYEARON
    EXTERN  RTCIntCheckLeapYear
    #endif

    #ifdef  RTC_100YEARDAYOFWEEKON
    EXTERN        RTCIntDayOfWeek
    #endif

    #ifdef  RTC_12800YEARDAYOFWEEKON
    EXTERN        RTCIntDayOfWeek
    #endif

#else
;****************************************************************************
;Definations of Shared Parameters
;****************************************************************************
    GLOBAL  _vRTCvariables

    GLOBAL  RTCIntInit
    GLOBAL  RTCIntISR

    #ifdef  RTC_DAYMONTHYEARON
    GLOBAL  RTCIntCheckLeapYear
    #endif

    #ifdef  RTC_100YEARDAYOFWEEKON
    GLOBAL        RTCIntDayOfWeek
    #endif

    #ifdef  RTC_12800YEARDAYOFWEEKON
    GLOBAL        RTCIntDayOfWeek
    #endif


#endif






#define     Secf              00                ; second flag in vRTCStatus
#define     Minf              01                ; minute flag in vRTCStatus
#define     Hourf             02                ; hour flag in vRTCStatus
#define     Dayf              03                ; Day flag in vRTCStatus
#define     DOWf              04                ; Day of the week flag in vRTCStatus
#define     Monthf            05                ; Month flag in vRTCStatus
#define     Yearf             06                ; year flag in vRTCStatus
#define     lyf               07                ; leap-year flag in vRTCStatus


#ifdef  RTC_CLOCKON
#define        vRTCStatus     _vRTCvariables        ;Used to store flags
#define        vSec           _vRTCvariables+1      ;Used to store Secondes         [0-59]
#define        vMin           _vRTCvariables+2      ;Used to store Minutes          [0-59]
#define        vHour          _vRTCvariables+3      ;Used to store Houres           [0-23]
#endif

#ifdef  RTC_DAYMONTHYEARON
#define        vDay           _vRTCvariables+4      ;Used to store Day of the Month [1-31]
#define        vMonth         _vRTCvariables+5      ;Used to store Month            [1-12]
#define        vYear          _vRTCvariables+6      ;Used to store Year             [0-99]

     #ifdef  RTC_CENTURYON
     #define        vCentury       _vRTCvariables+7      ;Used to store Century          [0-127]
           #ifdef  RTC_USERDOWON
           #define        vDOW           _vRTCvariables+8      ;Used to store Day of the Week  [0-6] ;  [vDay+vMonth_temp+vYear1+vCenturyDivBy4-vCentury_temp]
           #endif
     #else
           #ifdef  RTC_USERDOWON
           #define        vDOW           _vRTCvariables+7      ;Used to store Day of the Week  [0-6] ;  [vDay+vMonth_temp+vYear1+vCenturyDivBy4-vCentury_temp]
           #endif
     #endif


#endif



#ifdef  RTC_DOWON
#define        vDOW           _vRTCvariables+8      ;Used to store Day of the Week  [0-6] ;  [vDay+vMonth_temp+vYear1+vCenturyDivBy4-vCentury_temp]
       #ifdef  RTC_12800YEARDAYOFWEEKON
               #define        vMonth_temp     _vRTCvariables+9     ;Used in 12700DOW to hold [(13 * vMonth - 1)/5]
               #define        vYear1          _vRTCvariables+11    ;Used in 12700DOW to hold (vYear - 1) (if Jan of Feb)
               #define        vYear_temp      _vRTCvariables+10    ;Used in 12700DOW to hold [vYear/4]
               #define        vCentury_temp   _vRTCvariables+12    ;Used in 12700DOW to hold (vCentury_temp*2)
               #define        vCenturyDivBy4  _vRTCvariables+13    ;Used in 12700DOW to hold [vCentury_temp/4]
       #endif
       #ifdef  RTC_100YEARDAYOFWEEKON
               #define        temp            _vRTCvariables+9     ;Used in 100DOW
       #endif


#endif



;##############################################################################
; For PIC16xxx
;##############################################################################



;****************************************************************************
; Macro:        mClearTIMER1
;
; Overview:     Clears Timer1 (clears TMR1H and TMR1L).
;
; Input:        None
;
; Output:       None
;
; Side Effects: Databank changed
;****************************************************************************
mClearTIMER1    macro
        banksel TMR1H
        clrf     TMR1H                      ; Clear TMR1H
        clrf     TMR1L                      ; Clear TMR1L
        endm


;****************************************************************************
; Macro:        mLoadTIMER1
;
; Overview:     Loads TIMER1 with 0x8000.
;
; Input:        None
;
; Output:       None
;
; Side Effects: Databank, W changed
;****************************************************************************
mLoadTIMER1     macro
        banksel TMR1H
        movlw   0x80                      ;
        movwf   TMR1H                     ; Load TMR1H with 0x80
        clrf    TMR1L
        endm



;****************************************************************************
; Macro:        mReLoadTIMER1
;
; Overview:     Adds 0x80 to TMR1H
;
; Input:        None
;
; Output:       None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************
mReLoadTIMER1         macro
        banksel TMR1H
        movlw   0x80                      ;
        addwf   TMR1H                     ; Load TMR1H with 0x80
        endm



;****************************************************************************
; Macro: mRTCIntClearSeconds
;
; PreCondition: None
;
; Overview:
;       This macro is used reset the seconds in the RTC
;
; Input: None
;
;
; Output: None
;
; Side Effects: Databank changed
;
; Stack requirement: None
;
;****************************************************************************

mRTCIntClearSeconds     macro
        banksel     vSec
        clrf        vSec                     ;clear the seconds register (vSec)
        endm

;****************************************************************************
; Macro: mRTCIntSetSeconds
;
; PreCondition: None
;
; Overview:
;       This macro is used set the seconds in the RTC
;
; Input: Seconds
;
;
; Output: None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************

mRTCIntSetSeconds        macro   Seconds

        banksel     vSec
        movlw       Seconds
        movwf       vSec                   ;Set the seconds register (vSec)
        endm

;****************************************************************************
; Macro: mRTCIntSetMinutes
;
; PreCondition: None
;
; Overview:
;       This macro is used to set the minutes in the RTC
;
; Input: Minutes
;
;
; Output: None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************

mRTCIntSetMinutes       macro   Minutes
        banksel     vMin
        movlw       Minutes
        movwf       vMin                     ;set the minutes register (vMin)
        endm




;****************************************************************************
; Macro: mRTCIntSetHours
;
; PreCondition: None
;
; Overview:
;       This routine is used to set the hours in the RTC
;
; Input: Hours
;
;
; Output: None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************

mRTCIntSetHours         macro   Hours
        banksel     vHour
        movlw       Hours
        movwf       vHour                    ;set the houres register (vHour)
        endm



;****************************************************************************
; Macro:        mRTCintSetTime
;
; Overview:     Sets the time.
;
; Input:        Hours, Minutes, Seconds
;
; Output:       None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************
mRTCintSetTime    macro     vHours, vMinutes, vSeconds
        banksel vHour
        movlw   vHours
        movwf   vHour
        movlw   vMinutes
        movwf   vMin
        movlw   vSeconds
        movwf   vSec
        endm

;****************************************************************************
; Macro: mRTCIntSetDay
;
; PreCondition: None
;
; Overview:
;       This macro is used to set the day of the month in the RTC
;
; Input: Day of the Month
;
;
; Output: None
;
; Side Effects: Databank, W, STATUS and PCLATH changed
;
; Stack requirement: up to 2 level deep
;
;****************************************************************************
#ifdef  RTC_DAYMONTHYEARON
mRTCIntSetDay       macro       Day
        banksel     vDay
        movlw       Day
        movwf       vDay                    ;set the days register (vDay)
  #ifdef    RTC_DOWON
        pagesel     RTCIntDayOfWeek
        call        RTCIntDayOfWeek
  #endif
        endm
#endif


;****************************************************************************
; Macro: mRTCIntSetDOW
;
; PreCondition: None
;
; Overview:
;       This routine is used to set the day of the week in the RTC
;
; Input: Day of the Week (Sunday=0...Saturday=6)
;
;
; Output: None
;
; Side Effects: Databank, W changed
;
; Stack requirement: None
;
;****************************************************************************
#ifdef  RTC_USERDOWON
mRTCIntSetDOW       macro       DOW
        banksel     vDOW
        movlw       DOW
        movwf       vDOW                   ;set the days register (vDay)
        endm
#endif


;****************************************************************************
; Macro: mRTCIntSetMonth
;
; PreCondition: None
;
; Overview:
;       This macro is used to set the months in the RTC
;
; Input: Month of the year
;
;
; Output: None
;
; Side Effects: Databank, W, STATUS and PCLATH changed
;
; Stack requirement: up to 2 level deep
;
;****************************************************************************
#ifdef  RTC_DAYMONTHYEARON
mRTCIntSetMonth     macro       Month
        banksel     vMonth
        movlw       Month
        movwf       vMonth                      ;set the months register (vMonth)
  #ifdef    RTC_DOWON
        pagesel     RTCIntDayOfWeek
        call        RTCIntDayOfWeek
  #endif
        endm
#endif


;****************************************************************************
; Macro: mRTCIntSetYear
;
; PreCondition: None
;
; Overview:
;       This macro is used to set the years in the RTC
;
; Input: year
;
;
; Output: None
;
; Side Effects: Databank, W, STATUS and PCLATH changed
;
; Stack requirement: up to 2 level deep
;
;****************************************************************************
#ifdef  RTC_DAYMONTHYEARON
mRTCIntSetYear      macro       Year
        banksel     vYear
        movlw       Year
        movwf       vYear                       ;set the months register (vMonth)
  #ifdef    RTC_DOWON
        pagesel     RTCIntDayOfWeek
        call        RTCIntDayOfWeek
  #endif
        endm
#endif

;****************************************************************************
; Macro: mRTCIntSetCentury
;
; PreCondition: None
;
; Overview:
;       This macro is used to set the centuries in the RTC
;
; Input: Century
;
;
; Output: None
;
; Side Effects: Databank, W, STATUS and PCLATH changed
;
; Stack requirement: up to 2 level deep
;
;****************************************************************************
#ifdef  RTC_CENTURYON
mRTCIntSetCentury   macro       Century
        banksel     vCentury
        movlw       Century
        movwf       vCentury                       ;set the months register (vMonth)
  #ifdef    RTC_DOWON
        pagesel     RTCIntDayOfWeek
        call        RTCIntDayOfWeek
  #endif
        endm
#endif

;****************************************************************************
; Macro:        mRTCintSetDate
;
; Overview:     Sets the Date.
;
; Input:        Month, Day, Year, Century
;
; Output:       None
;
; Side Effects: Databank, W, STATUS and PCLATH changed
;
; Stack requirement: up to 2 level deep
;
;****************************************************************************
#ifdef  RTC_DAYMONTHYEARON
mRTCintSetDate    macro     Month, Day, Century, Year
        banksel         vDay
        movlw           Day
        movwf           vDay
        movlw           Month
        movwf           vMonth
        movlw           Year
        movwf           vYear
    #ifdef       RTC_CENTURYON
        movlw           Century
        movwf           vCentury
    #endif
    #ifdef    RTC_DOWON
        pagesel     RTCIntDayOfWeek
        call        RTCIntDayOfWeek
    #endif
        endm
#endif
#endif


