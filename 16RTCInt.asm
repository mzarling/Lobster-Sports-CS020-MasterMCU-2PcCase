;*********************************************************************
;*
;*  This implements a generic library functionality to support a Real
;*  Time Clock (RTC) for the PIC16xxxx.
;*  This RTC can go up to 12800 years.
;*
;*********************************************************************
;* FileName:            16RTCInt.asm
;* Dependencies:        P16xxx.inc
;*                      16RTCInt.inc
;* Processor:           PIC16xxxx
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
;*
;* Author                         Date            Comment
;*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;* Abdelwahb Fassi-Fihri      Dec 20, 2002    pre-Initial Release (V1.00)
;*
;********************************************************************/


#define TMR1_OSC_MODULE        ;Module definition to generate error message for
                               ;Processor which do not have this module.

#define _ADD_PROC_INC_FILE
       include  "P16xxx.inc"

#define RTC_Source
       include "16RTCInt.inc"

        errorlevel      -302
        errorlevel      -305



RTCIntRAM          UDATA


#ifdef        RTC_CLOCKON
  #ifdef       RTC_DAYMONTHYEARON
     #ifdef    RTC_CENTURYON
        #ifdef      RTC_USERDOWON
           _vRTCvariables         RES     9
        #else;  if   RTC_USERDOWOFF
               #ifdef    RTC_100YEARDAYOFWEEKON
                  _vRTCvariables         RES     10
               #else; if  RTC_100YEARDAYOFWEEKOFF
                     #ifdef    RTC_12800YEARDAYOFWEEKON
                        _vRTCvariables         RES     14
                     #else; if  RTC_12800YEARDAYOFWEEKOFF
                        _vRTCvariables         RES     8
                     #endif
               #endif
        #endif
     #else; if   RTC_CENTURYOFF
           #ifdef      RTC_USERDOWON
              _vRTCvariables         RES     8
           #else;  if   RTC_USERDOWOFF
                  #ifdef    RTC_100YEARDAYOFWEEKON
                     _vRTCvariables         RES     10
                  #else; if  RTC_100YEARDAYOFWEEKOFF
                     _vRTCvariables         RES     7
                  #endif
           #endif
     #endif
  #else; if      RTC_DAYMONTHYEAROFF
        _vRTCvariables         RES     4
  #endif
#endif




;****************************************************************************






_RTCcode   code


;****************************************************************************
; Function: RTCIntInit
;
; PreCondition: None
;
; Overview:
;       This routine is used for RTC Initialization
;       It initializes according to compile time selections.
;
; Input: None
;
;
; Output: None
;
; Side Effects: Databank, W and STATUS changed
;
; Stack requirement: 1 level deep
;
;****************************************************************************

RTCIntInit:

        GLOBAL          RTCIntInit
        banksel         T1CON                   ;Change to bank 0
        movlw           0x07                    ;
        movwf           T1CON                   ;enable Timer1 for extrnal osc
        movlw           0x80                    ;
        movwf           TMR1H                   ;make TMR1 reg equal to 0x8000
        clrf            TMR1L                   ;
        bcf             PIR1,TMR1IF             ;clear Timer1 interrupt flag
        bsf             STATUS,RP0              ;change to bank 1
        bsf             PIE1,TMR1IE             ;enable Timer1 interrupt
        bcf             STATUS,RP0              ;change to bank 0
        bsf             INTCON,PEIE             ;enable peripheral interrupt
        bsf             T1CON,T1OSCEN           ;enable Timer1 Oscillator
        bsf             INTCON,GIE              ;enable Globale interrupt

#ifdef  RTC_CLOCKON
        banksel         vSec                    ;DO I NEED BANKSEL BEFOR EACH ONE?
        clrf            vSec                    ;Seconds = 00;
        clrf            vMin                    ;Minutes = 00;
        clrf            vHour                   ;Hours = 00;
#endif

#ifdef  RTC_DAYMONTHYEARON
        movlw           0x01                    ;
        movwf           vDay                    ;Day = 01;
        movwf           vMonth                  ;Month = 01 (january);
        clrf            vYear                   ; Year = 00;
#endif

#ifdef  RTC_DOWON
        movlw           0x06
        movwf           vDOW                    ;DOW = Saturday
#endif

#ifdef  RTC_USERDOWON
        movlw           0x06
        movwf           vDOW                    ;DOW = Saturday
#endif


#ifdef  RTC_CENTURYON
        movlw           0x14                    ;
        movwf           vCentury                ;Century = 20;
#endif

        movlw           0x80                    ;
        movwf           vRTCStatus              ; leap-year flag = 1; all others = 0

        return





;****************************************************************************
; Function: RTCIntISR
;
; PreCondition: None
;
; Overview:
;       This is a Interrupt service routine for Timer1 Interrupt.
;       It handles updating the RTC.
;       Call it from Interrupt service routine if an interrupt was caused by
;       Timer1.
;
; Input:
;
;
; Output:   Updates the Real time clock (sec,min,hour,day,dow,month,year,
;           century, and vRTCStatus which contains the flags that are set
;           when one of the variables in RTC changes) .
;
;
; Side Effects: Databank, PCLATH, Wreg, STATUS reg changed
;
; Stack requirement: 2 level deep
;
;****************************************************************************

RTCIntISR:
        GLOBAL          RTCIntISR

; Clock
        banksel         PIR1
        btfss           PIR1,TMR1IF
        goto            EndISR
        bcf             PIR1,TMR1IF             ;Clear Timer1 interrupt Flag
        movlw           0x80
        addwf           TMR1H                   ;Add 0x80 to TMR1H to count 1 second
        banksel         vRTCStatus
        bsf             vRTCStatus,Secf         ;set the Seconds flag (Secf) to indicate a change in vSec
        incf            vSec,F                  ;increment the secondes (vSec++)
        movf            vSec,W                  ;move vSec to W
        sublw           0x3c                    ;subtract 60 from W (to check if full minute)
        btfss           STATUS,Z                ;if (60-W) is not equal to zero then
        goto            EndISR                  ;return from interrupt
        clrf            vSec                    ;else clear the seconds vSec = 00;
        bsf             vRTCStatus,Minf         ;set the minutes flag to indicate a change in vMin
        incf            vMin,F                  ;increment the minutes
        movf            vMin,W                  ;move vMin to W
        sublw           0x3c                    ;subtract 60 from w to check if full hour
        btfss           STATUS,Z                ;if (60-W) is not equal to zero then
        goto            EndISR                  ;return from interrupt
        clrf            vMin                    ;else clear the Minutes vMin=00;
        bsf             vRTCStatus,Hourf        ;set the Hours flag to indicate in change in vHour
        incf            vHour,F                 ;increment the hours
        movf            vHour,W                 ;move vHour to W
        sublw           0x18                    ;subtract 24 from W to check if full day
        btfss           STATUS,Z                ;if (24-W) is not equal to zero then
        goto            EndISR                  ;return from interrupt
#ifdef RTC_DAYMONTHYEARON
        clrf            vHour                   ;else clear the hours vHour=00
; Calendar
        bsf             vRTCStatus,Dayf         ;set the days flag to indicate a change in vDay
        incf            vDay                    ;increment the days
#ifdef RTC_DOWON
        incf            vDOW                    ;increment the day of the week
        movf            vDOW,W                  ;W = vDOW
        sublw           0x07                    ;subtract 7 from W
        btfsc           STATUS,Z                ;if (7-W) is equal to zero then
        clrf            vDOW                    ;clear vDOW to indicate a beginning of a new week with a sunday
#endif
#ifdef RTC_USERDOWON
        incf            vDOW                    ;increment the day of the week
        movf            vDOW,W                  ;W = vDOW
        sublw           0x07                    ;subtract 7 from W
        btfsc           STATUS,Z                ;if (7-W) is equal to zero then
        clrf            vDOW                    ;clear vDOW to indicate a beginning of a new week with a sunday
#endif

        movf            vMonth,W                ;move vMonth to W
        sublw           0x02                    ;Subtract 2 from W to check if it's february
        btfss           STATUS,Z                ;if (2-W) is not equal to zero then
        goto            noleap                 ;it's not february, don't worry about leap year.
        btfss           vRTCStatus,lyf          ;if leap_year flag (lyf) is set then
        goto            noleap


        movlw           HIGH DaysTable
        movwf           PCLATH
        movlw           0x00                    ;W=0
        goto            leap



noleap

        movlw           LOW DaysTable           ;get low 8-bits of address
        addwf           vMonth,w                ;add the 8-bit address to vMonth-1
        movlw           HIGH DaysTable          ;This is to change the PCLATH address to make sure ;the table read in "MONTHTBL" WORKS.
        btfsc           STATUS,C                ;check if the page crossed
        addlw           0x01                    ;if yes then increment the high address
        movwf           PCLATH
        movf            vMonth,W                ;W = vMonth;
leap
        call            DaysTable               ;call the DaysTable table to get the number of day for the current month
        subwf           vDay,W                  ;subtract vDay from W (vDay-#ofDayForCurrentMonth) to check for last day of the month



         btfss        STATUS,Z
         btfss        STATUS,C
         goto         RestorePCLATH
         goto         newday

RestorePCLATH
        movlw         high($+2)                         ;restore PCLATH to it's original address before
        movwf         PCLATH                            ;the table read was performed
        goto          EndISR

newday

        movlw         high($+2)                         ;restore PCLATH to it's original address before
        movwf         PCLATH                            ;the table read was performed
        movlw           0x01                    ;else it's the first day of a new month, move 01 to W
        movwf           vDay                    ;vDay=W=01
        bsf             vRTCStatus,Monthf       ;set the months flag (Monthf) to indicate a change in the vMonth
        incf            vMonth                  ;increment the months (vMonth++);
        movf            vMonth,W                ;move vMonth to W
        sublw           0x0d                    ;subtact 13 from W (to check for new year)
        btfss           STATUS,Z                ;if (13-W) is not equal to zero then
        goto            EndISR                  ;return from interrupt
        movlw           0x01                    ;else it's the first month of the year, move 01 to W
        movwf           vMonth                  ;vMonth=W=01
        bsf             vRTCStatus,Yearf        ;set the years flag (Yearf) to indicate a change in vYear
        incf            vYear                   ;increment the years            (vYear++)
        bcf             vRTCStatus,lyf          ;clear the leap year flag
        movf            vYear,W                 ;W = vYear
        andlw           0x03                    ;W = W & 0x03
        btfsc           STATUS,Z                ;if ((W & 00000011) == 0) then
        bsf             vRTCStatus,lyf          ;set the leap year flag (lyf) to indicate a 4-year leap year.
        movf            vYear,W                 ;W = vYear
        sublw           0x64                    ;subtract 100 from W
        btfss           STATUS,Z                ;if ((W-100) != 0)
        goto            EndISR                  ;return from interrupt

  #ifdef RTC_CENTURYON
        clrf            vYear                   ;clear the years (vYear = 00)
        incf            vCentury                ;increment the centries
        bcf             vRTCStatus,lyf          ;clear the leap year flag
        movf            vCentury,W              ;W = vCentury;
        andlw           0x03                    ;W = W & 0x03
        btfsc           STATUS,Z                ;if ((vCentury & 00000011) == 0) then
        bsf             vRTCStatus,lyf          ;set the leap year flag (lyf) to indiacte a 400-year leap year
        goto            EndISR                  ;return from interrupt

  #else
        clrf            vYear                   ;clear the years (vYear = 00)
        goto            EndISR                  ;return from interrupt
  #endif


#else
        clrf            vHour                   ;else clear the hours (vHour=00)
        goto            EndISR                  ;return from interrupt
#endif




EndISR

        return


#ifdef RTC_DAYMONTHYEARON
DaysTable
         addwf       PCL                          ; Number of Days per month
         retlw       0x1d                         ; Leap-Day     29
         retlw       0x1f                         ; January      31
         retlw       0x1c                         ; February     28
         retlw       0x1f                         ; Mars         31
         retlw       0x1e                         ; April        30
         retlw       0x1f                         ; May          31
         retlw       0x1e                         ; June         30
         retlw       0x1f                         ; July         31
         retlw       0x1f                         ; August       31
         retlw       0x1e                         ; September    30
         retlw       0x1f                         ; October      31
         retlw       0x1e                         ; November     30
         retlw       0x1f                         ; December     31
#endif









;****************************************************************************
; Function: RTCIntCheckLeapYear
;
; PreCondition: None
;
; Overview:
;       This routine is used after setting the time to check if current year is
;        a leap-year or not.  If it is then it sets the lyf flag otherwise it clears it.
;
; Input: vYear, vCentury
;
;
; Output: sets or clears vRTCStatus,lyf (leap year flag) bit.
;
; Side Effects: Databank, Wreg, STATUS reg changed
;
; Stack requirement: 1 level deep
;
;****************************************************************************
#ifdef  RTC_DAYMONTHYEARON
RTCIntCheckLeapYear:
        banksel vRTCStatus
#ifdef  RTC_CENTURYON
        
        movf    vYear,F                           ;if year is 00, check if leap century (every 400 years)
        btfss   STATUS,Z                          ;if it's not zero
        goto    CheckForLeapYear                  ;check of leap year


        bcf     vRTCStatus,lyf                    ;clear leap year flag
        movlw   B'00000011'                       ;if the 2-LSbs of vCentury are zero then this is a leap year (leap Century)
        andwf   vCentury,W                        ;keep only the 2-LSbs
        btfsc   STATUS,Z                          ;if it's zero
        bsf     vRTCStatus,lyf                    ;set the leap-year flag
        return
                                  ;
#endif

CheckForLeapYear
        bcf     vRTCStatus,lyf                    ;clear leap-year flag
        movlw   B'00000011'                       ;if the 2-LSbs of vYear are zero then this is a leap year
        andwf   vYear,W                           ;keep only the 2-LSbs
        btfsc   STATUS,Z                          ;if it's zero
        bsf     vRTCStatus,lyf                    ;set the leap-year flag
        return                                    ;
#endif


 ;****************************************************************************
; Function: RTCIntDayOfWeek
;
; PreCondition: called when date is being set
;
; Overview:
;               if RTC_100YEARDAYOFWEEKON
;               .........................
;       This routine is used to set the day of the week in the RTC
;       FOR ANY DATE BETWEEN JANUARY 1 1995 AND DECEMBER 31 2094,
;       CALCULATE THE DAY OF THE WEEK [0=SUNDAY, 1 = MONDAY,
;       .... 6 = SATURDAY].
;
;       ENTER WHICH MONTH [1-12] IN "vMonth", DAY-OF-MONTH [1-X]
;       IN "vDay", AND YEAR [95-99, 00-94] IN "vYear".  IF "vYear"
;       IS LESS THAN 95, THIS SUBROUTINE ASSUMES THAT WE'RE IN
;       THE 21ST CENTURY.  OTHERWISE, IT ASSUMES THAT WE'RE IN
;       THE 20TH.
;
;       ON EXIT, DAY-OF-WEEK IS IN "vDOW".  "DAY", "vMonth", AND
;       "vYear" ARE UNCHANGED.
; --------------------------------------------------------------
; --------------------------------------------------------------
;
;                If RTC_12800YEARDAYOFWEEKON
;                ...........................
;       This routine is used to set the day of the week in the RTC
;       For any date between March 1st, 0000 and December 31st, 12700
;       CALCULATE THE DAY OF THE WEEK [0=SUNDAY, 1 = MONDAY,
;       .... 6 = SATURDAY].
;
;       ENTER WHICH MONTH [1-12] IN "vMonth", DAY-OF-MONTH [1-31]
;       IN "vDay", AND YEAR [00-99] IN "vYear" and CENTURY [00-127] in
;       "vCentury".

;       ON EXIT, DAY-OF-WEEK IS IN "vDOW".  "DAY", "vMonth", AND
;       "vYear" ARE UNCHANGED.
;
;
; Input: vDay, vMonth, and vYear
;
;
; Output: vDOW
;
; Side Effects: Databank, PCLATH, Wreg, STATUS reg changed
;
; Stack requirement: 3 level deep
;
;****************************************************************************






RTCIntDayOfWeek:
#ifdef RTC_100YEARDAYOFWEEKON
        banksel vMonth
        decf    vMonth,W                          ;W = vMonth - 1.
        movwf   temp
        movlw   LOW MonthTable                    ;get low 8-bits of address
        addwf   temp,w                            ;add the 8-bit address to vMonth-1
        movlw   HIGH MonthTable                   ;This is to change the PCLATH address to make sure ;the table read in "MONTHTBL" WORKS.
        btfsc   STATUS,C                          ;check if the page crossed
        addlw   0x01                              ;if yes then increment the high address
        movwf   PCLATH
        movf    temp,W                            ;put the temp=vMonth-1 into W
        call    MonthTable                        ;get the DOW for the 1st of the current month
        movwf   vDOW                              ;then put it in vDOW.

        movlw   high($+2)                         ;restore PCLATH to it's original address before
        movwf   PCLATH                            ;the table read was performed

        movlw   0x03
        subwf   vMonth,W                          ;if (vMonth >= March)
        btfss   STATUS,C
        goto    NoLeapYear                       ;then don't worry about leap year goto NoLeapYear

        movlw   B'00000011'                       ;else if the 2-LSbs of vYear are zero then this is a leap year
        andwf   vYear,W                           ;keep only the 2-LSbs
        btfss   STATUS,Z                          ;if it's not zero
        goto    NoLeapYear                       ;then goto NoLeapYear
        incf    vDOW                              ;else IT'S MARCH 1ST OR LATER, AND THIS YEAR'S A LEAP YEAR.
                                                  ;add a day to vDOW.

NoLeapYear

        bcf     STATUS,C                          ;Clear the carry bit
        rlf     vDOW                              ;vDOW*2
        decf    vYear                             ;vYear-1                             = 2*vDOW + (vYear-1)/2
;                                                                                       ----------------------.
        rrf     vYear,W                           ;                                                2
        incf    vYear
        addwf   vDOW
        bcf     STATUS,C
        rrf     vDOW

        movf    vDay,W                            ;vDOW = vDOW + vDay + vYear.
        addwf   vYear,W
        addwf   vDOW

        movlw   .95
        subwf   vYear,W
        btfss   STATUS,C                          ; if vYear < 95
        decf    vDOW                              ;Then vDOW = vDOW - 1.

        movlw   0x07
_Modulo7
        subwf   vDOW                              ;vDOW = vDOW MOD 7.
        btfsc   STATUS,C                          ;if vDOW-7 is positive
        goto    _Modulo7                          ;go subtract 7 again
        addwf   vDOW                              ;else if vDOW-7 is negative then add 7 and that is the final vDOW

        return


MonthTable
        addwf     PCL
        retlw     .7                           ;Day-Of-Week for first day
        retlw     .10                          ;of each month + 7 (in 1995)
        retlw     .10                          ;7=Sunday
        retlw     .13                          ;8=Monday
        retlw     .8                           ;9=Tuesday
        retlw     .11                          ;10=Wednesday
        retlw     .13                          ;11=Thursday
        retlw     .9                           ;12=Friday
        retlw     .12                          ;13=Saturday.
        retlw     .7
        retlw     .10
        retlw     .12


#endif



#ifdef       RTC_12800YEARDAYOFWEEKON
;The following equation called is "Zellers Congruence"
;                                               vDOW                                 %7
;                       Month_temp                                                (13*vMonth-1)/5
;                       vDOW            vYear1    vYear_temp      vCenturyDivBy4     vCentury_temp
;vDOW = [vDay + [(13 * vMonth - 1)/5] + vYear +  [vYear/4]   +    [vCentury/4]   -   2*vCentury]%7
;               (8  + 4 + 1)vMonth
;
;
;  [] means no decimals
;%7 is modulus 7 which is
;vDay Max=31
;vMonth Max=12,   in this case March=1, April=2,...,Dec=10,Jan=11,Feb=12
;vYear Max=99,    if    March<= vMonth <=December  then use vYear but if Jan or Feb then subtract one vYear-1
;vCentury Max=127


                 banksel        vCentury
                 movf           vCentury,W
                 banksel        vCentury_temp
                 movwf          vCentury_temp          ;copy vCentury into vCentury_temp
                 movf           vYear,W                ;
                 movwf          vYear_temp             ;copy vYear into vYear_temp
                 movf           vMonth,W
                 movwf          vMonth_temp            ;copy vMonth into vMonth_temp
                 sublw          0x02                   ;subtract 2 from vMonth_temp to check if Jan or Feb?
                 btfsc          STATUS,C
                 goto           JanFeb                 ;if vMonth_temp<=2 then go to JanFeb
                 movlw          0x02                   ;else subract 2 from vMonth_temp
                 subwf          vMonth_temp            ;Month used for calculating DOW March=1, April=2,...,Dec=10
                 goto           Start_equation

JanFeb
                 movlw          0x0A                   ;add 10 to Jan and Feb
                 addwf          vMonth_temp            ;Month used for calculating DOW Jan=11, Feb=12
                 movf           vYear_temp             ;We need to subtract 1 year for Jan and Feb
                 btfsc          STATUS,Z               ;but first we need to check if the year is 00
                 goto           Year_zero              ;if vYear_temp is zero then go to Year_zero
                 decf           vYear_temp             ;Year used if Jan or Feb (vYear-1)
                 goto           Start_equation

Year_zero
                 movlw          .99                    ;since vYear_temp =00 we need to
                 movwf          vYear_temp             ;put 99 in the years
                 decf           vCentury_temp          ;use this century for calculations
                                                       ;e.g. if the year was 2000 then vYear_temp=99 and vCentury_temp=19
Start_equation
                 bcf            STATUS,C               ;clear the carry bit
                 rlf            vMonth_temp,W          ; vMonth_temp*2 = vMonth*2
                 movwf          vDOW                    ; Put vMonth_temp into vDOW
                 rlf            vDOW                    ;vDOW= vDOW*2 = vMonth*4
                 rlf            vDOW,W                  ;W= vDOW*2 = vMonth*8
                 addwf          vDOW,W                  ;W= vDOW+vMonth*8=vMonth*8 + vMonth*4
                 addwf          vMonth_temp            ; vMonth*8 + vMonth*4 + vMonth_temp = 13*vMonth
                 decf           vMonth_temp,W          ;vMonth_temp - 1 = 13*vMonth - 1
                 movwf          vDOW
                 clrf           vMonth_temp

DivideBy5
                 movlw          0x05
                 subwf          vDOW                    ; Subtract 5 from vMonth_temp
                 btfss          STATUS,C               ;
                 goto           DivideYearBy4          ; if there is a carry after the subtraction go to DivideYearBy4
                 incf           vMonth_temp            ; else increment vMonth_temp = [(13*vMonth-1)/5]
                 goto           DivideBy5              ; (keep sutracting 5 and incrementing vMonth_temp
                                                       ;until the last subraction generates a Carry.
DivideYearBy4
                 movf           vYear_temp,W
                 movwf          vYear1                 ;copy vYear_temp into vYear1
                 bcf            STATUS,C               ;Clear Carry
                 rrf            vYear_temp             ; vYear_temp/2
                 bcf            STATUS,C               ;Clear Carry
                 rrf            vYear_temp             ; vYeartemp/2 = [vYear/4]
DivideCenturyBy4
                 bcf            STATUS,C               ;Clear Carry
                 rrf            vCentury_temp,W        ; vCentury_temp/2
                 movwf          vCenturyDivBy4         ;Copy vCentury_temp/2 to vCenturyDivBy4
                 bcf            STATUS,C               ;Clear Carry
                 rrf            vCenturyDivBy4         ; vCenturyDivBy4/2=[vCentury_temp/4]

MultiplyCenturyBy2
                 bcf            STATUS,C               ;Clear Carry
                 rlf            vCentury_temp          ;vCentury_temp*2

AddingItAllUp
                 movf           vDay,W                 ;copy vDay into W_reg
                 addwf          vMonth_temp,W          ;vDay + vMonth_temp = W_reg
                 addwf          vYear1,W               ;W_reg + vYear1  = vDay +vMonth_temp + vyear1
                 addwf          vYear_temp,W           ;W_reg + vYear_temp =  vDay +vMonth_temp + vyear1 + vYear_temp
                 addwf          vCenturyDivBy4,W       ;W_reg + vCenturyDivBy4 = vDay +vMonth_temp + vyear1 + vYear_temp + vCenturyDivBy4
                 movwf          vDOW                   ;copy W_reg into vDOW
                 movf           vCentury_temp,W
                 subwf          vDOW                   ;vDOW - vCentury_temp
                 btfss          STATUS,C
                 goto           NegativeNumber         ;if the result is negative go to NegativeNumber else continue

PositiveNumber
                 movlw          0x07
                 subwf          vDOW                   ;subtract 7 from vDOW
                 btfsc          STATUS,Z               ;if the subtration is zero (this means Sunday)
                 goto           FoundDOW               ;goto FoundDOW
                 btfsc          STATUS,C               ;else check if the subtration is negative
                 goto           PositiveNumber         ;if vAddition is still positive go back to PositiveNumber
                                                       ;and do it again else keep going
Add7ToGetDOW
                 movlw          0x07
                 addwf          vDOW                   ;since last vDOW subtraction generated a Carry then
                                                       ;add 7 to get the remainder of vDOW%7 (vDOW Modulo 7)
FoundDOW
                 return                                ;return vDOW


NegativeNumber
                 movlw          0x07
                 addwf          vDOW                   ;Since vDOW is a negative Number then Add 7
                 btfsc          STATUS,Z               ;Check if Zero (this means Sunday)
                 goto           FoundDOW2              ;If Zero go to FoundDOW2
                 btfss          STATUS,C               ;if carry is generated jump to FounDOW2
                 goto           NegativeNumber         ;else go back and subract 7 again

FoundDOW2
                 return                                ;return vDOW


#endif


           END



