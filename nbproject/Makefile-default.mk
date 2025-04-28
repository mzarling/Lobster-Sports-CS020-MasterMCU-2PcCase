#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=master.c lobstate.c keypad.c loblcd.c lobmain.c state_mach.c lobeeprom.c lobprog.c varinit.c lobparm.c lobpre.c diag.c loberr.c EL04CLT.c ringbuf.c ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c ../CS020_MasterMCU/EL04/xlcd/openxlcd.c ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c ../CS020_MasterMCU/EL04/xlcd/readaddr.c ../CS020_MasterMCU/EL04/xlcd/readdata.c ../CS020_MasterMCU/EL04/xlcd/setcgram.c ../CS020_MasterMCU/EL04/xlcd/setddram.c ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c ../CS020_MasterMCU/EL04/xlcd/writdata.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/master.p1 ${OBJECTDIR}/lobstate.p1 ${OBJECTDIR}/keypad.p1 ${OBJECTDIR}/loblcd.p1 ${OBJECTDIR}/lobmain.p1 ${OBJECTDIR}/state_mach.p1 ${OBJECTDIR}/lobeeprom.p1 ${OBJECTDIR}/lobprog.p1 ${OBJECTDIR}/varinit.p1 ${OBJECTDIR}/lobparm.p1 ${OBJECTDIR}/lobpre.p1 ${OBJECTDIR}/diag.p1 ${OBJECTDIR}/loberr.p1 ${OBJECTDIR}/EL04CLT.p1 ${OBJECTDIR}/ringbuf.p1 ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 ${OBJECTDIR}/_ext/1477896233/openxlcd.p1 ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 ${OBJECTDIR}/_ext/1477896233/readaddr.p1 ${OBJECTDIR}/_ext/1477896233/readdata.p1 ${OBJECTDIR}/_ext/1477896233/setcgram.p1 ${OBJECTDIR}/_ext/1477896233/setddram.p1 ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 ${OBJECTDIR}/_ext/1477896233/writdata.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/master.p1.d ${OBJECTDIR}/lobstate.p1.d ${OBJECTDIR}/keypad.p1.d ${OBJECTDIR}/loblcd.p1.d ${OBJECTDIR}/lobmain.p1.d ${OBJECTDIR}/state_mach.p1.d ${OBJECTDIR}/lobeeprom.p1.d ${OBJECTDIR}/lobprog.p1.d ${OBJECTDIR}/varinit.p1.d ${OBJECTDIR}/lobparm.p1.d ${OBJECTDIR}/lobpre.p1.d ${OBJECTDIR}/diag.p1.d ${OBJECTDIR}/loberr.p1.d ${OBJECTDIR}/EL04CLT.p1.d ${OBJECTDIR}/ringbuf.p1.d ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d ${OBJECTDIR}/_ext/1477896233/readdata.p1.d ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d ${OBJECTDIR}/_ext/1477896233/setddram.p1.d ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d ${OBJECTDIR}/_ext/1477896233/writdata.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/master.p1 ${OBJECTDIR}/lobstate.p1 ${OBJECTDIR}/keypad.p1 ${OBJECTDIR}/loblcd.p1 ${OBJECTDIR}/lobmain.p1 ${OBJECTDIR}/state_mach.p1 ${OBJECTDIR}/lobeeprom.p1 ${OBJECTDIR}/lobprog.p1 ${OBJECTDIR}/varinit.p1 ${OBJECTDIR}/lobparm.p1 ${OBJECTDIR}/lobpre.p1 ${OBJECTDIR}/diag.p1 ${OBJECTDIR}/loberr.p1 ${OBJECTDIR}/EL04CLT.p1 ${OBJECTDIR}/ringbuf.p1 ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 ${OBJECTDIR}/_ext/1477896233/openxlcd.p1 ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 ${OBJECTDIR}/_ext/1477896233/readaddr.p1 ${OBJECTDIR}/_ext/1477896233/readdata.p1 ${OBJECTDIR}/_ext/1477896233/setcgram.p1 ${OBJECTDIR}/_ext/1477896233/setddram.p1 ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 ${OBJECTDIR}/_ext/1477896233/writdata.p1

# Source Files
SOURCEFILES=master.c lobstate.c keypad.c loblcd.c lobmain.c state_mach.c lobeeprom.c lobprog.c varinit.c lobparm.c lobpre.c diag.c loberr.c EL04CLT.c ringbuf.c ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c ../CS020_MasterMCU/EL04/xlcd/openxlcd.c ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c ../CS020_MasterMCU/EL04/xlcd/readaddr.c ../CS020_MasterMCU/EL04/xlcd/readdata.c ../CS020_MasterMCU/EL04/xlcd/setcgram.c ../CS020_MasterMCU/EL04/xlcd/setddram.c ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c ../CS020_MasterMCU/EL04/xlcd/writdata.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F6722
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/master.p1: master.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master.p1.d 
	@${RM} ${OBJECTDIR}/master.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/master.p1 master.c 
	@-${MV} ${OBJECTDIR}/master.d ${OBJECTDIR}/master.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/master.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobstate.p1: lobstate.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobstate.p1.d 
	@${RM} ${OBJECTDIR}/lobstate.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobstate.p1 lobstate.c 
	@-${MV} ${OBJECTDIR}/lobstate.d ${OBJECTDIR}/lobstate.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobstate.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/keypad.p1: keypad.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keypad.p1.d 
	@${RM} ${OBJECTDIR}/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/keypad.p1 keypad.c 
	@-${MV} ${OBJECTDIR}/keypad.d ${OBJECTDIR}/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/loblcd.p1: loblcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/loblcd.p1.d 
	@${RM} ${OBJECTDIR}/loblcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/loblcd.p1 loblcd.c 
	@-${MV} ${OBJECTDIR}/loblcd.d ${OBJECTDIR}/loblcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/loblcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobmain.p1: lobmain.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobmain.p1.d 
	@${RM} ${OBJECTDIR}/lobmain.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobmain.p1 lobmain.c 
	@-${MV} ${OBJECTDIR}/lobmain.d ${OBJECTDIR}/lobmain.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobmain.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/state_mach.p1: state_mach.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/state_mach.p1.d 
	@${RM} ${OBJECTDIR}/state_mach.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/state_mach.p1 state_mach.c 
	@-${MV} ${OBJECTDIR}/state_mach.d ${OBJECTDIR}/state_mach.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/state_mach.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobeeprom.p1: lobeeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobeeprom.p1.d 
	@${RM} ${OBJECTDIR}/lobeeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobeeprom.p1 lobeeprom.c 
	@-${MV} ${OBJECTDIR}/lobeeprom.d ${OBJECTDIR}/lobeeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobeeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobprog.p1: lobprog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobprog.p1.d 
	@${RM} ${OBJECTDIR}/lobprog.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobprog.p1 lobprog.c 
	@-${MV} ${OBJECTDIR}/lobprog.d ${OBJECTDIR}/lobprog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobprog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/varinit.p1: varinit.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/varinit.p1.d 
	@${RM} ${OBJECTDIR}/varinit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/varinit.p1 varinit.c 
	@-${MV} ${OBJECTDIR}/varinit.d ${OBJECTDIR}/varinit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/varinit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobparm.p1: lobparm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobparm.p1.d 
	@${RM} ${OBJECTDIR}/lobparm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobparm.p1 lobparm.c 
	@-${MV} ${OBJECTDIR}/lobparm.d ${OBJECTDIR}/lobparm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobparm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobpre.p1: lobpre.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobpre.p1.d 
	@${RM} ${OBJECTDIR}/lobpre.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobpre.p1 lobpre.c 
	@-${MV} ${OBJECTDIR}/lobpre.d ${OBJECTDIR}/lobpre.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobpre.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/diag.p1: diag.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diag.p1.d 
	@${RM} ${OBJECTDIR}/diag.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/diag.p1 diag.c 
	@-${MV} ${OBJECTDIR}/diag.d ${OBJECTDIR}/diag.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/diag.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/loberr.p1: loberr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/loberr.p1.d 
	@${RM} ${OBJECTDIR}/loberr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/loberr.p1 loberr.c 
	@-${MV} ${OBJECTDIR}/loberr.d ${OBJECTDIR}/loberr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/loberr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/EL04CLT.p1: EL04CLT.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EL04CLT.p1.d 
	@${RM} ${OBJECTDIR}/EL04CLT.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/EL04CLT.p1 EL04CLT.c 
	@-${MV} ${OBJECTDIR}/EL04CLT.d ${OBJECTDIR}/EL04CLT.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/EL04CLT.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ringbuf.p1: ringbuf.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ringbuf.p1.d 
	@${RM} ${OBJECTDIR}/ringbuf.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ringbuf.p1 ringbuf.c 
	@-${MV} ${OBJECTDIR}/ringbuf.d ${OBJECTDIR}/ringbuf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ringbuf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/busyxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/busyxlcd.d ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/openxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/openxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/openxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/openxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/openxlcd.d ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/putrxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/putrxlcd.d ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/putsxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/putsxlcd.d ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/readaddr.p1: ../CS020_MasterMCU/EL04/xlcd/readaddr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readaddr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/readaddr.p1 ../CS020_MasterMCU/EL04/xlcd/readaddr.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/readaddr.d ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/readdata.p1: ../CS020_MasterMCU/EL04/xlcd/readdata.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readdata.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readdata.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/readdata.p1 ../CS020_MasterMCU/EL04/xlcd/readdata.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/readdata.d ${OBJECTDIR}/_ext/1477896233/readdata.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/readdata.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/setcgram.p1: ../CS020_MasterMCU/EL04/xlcd/setcgram.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setcgram.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/setcgram.p1 ../CS020_MasterMCU/EL04/xlcd/setcgram.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/setcgram.d ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/setddram.p1: ../CS020_MasterMCU/EL04/xlcd/setddram.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setddram.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setddram.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/setddram.p1 ../CS020_MasterMCU/EL04/xlcd/setddram.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/setddram.d ${OBJECTDIR}/_ext/1477896233/setddram.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/setddram.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.d ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/writdata.p1: ../CS020_MasterMCU/EL04/xlcd/writdata.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/writdata.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/writdata.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug --addrqual=ignore --mode=pro -P -N255 -I"../../../../cs020_sw/Master_v1p224p34-XC8/EL04/main" -I"C:/Program Files (x86)/Microchip/xc8/v1.31/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.31/include/plib" -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/writdata.p1 ../CS020_MasterMCU/EL04/xlcd/writdata.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/writdata.d ${OBJECTDIR}/_ext/1477896233/writdata.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/writdata.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/master.p1: master.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master.p1.d 
	@${RM} ${OBJECTDIR}/master.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/master.p1 master.c 
	@-${MV} ${OBJECTDIR}/master.d ${OBJECTDIR}/master.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/master.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobstate.p1: lobstate.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobstate.p1.d 
	@${RM} ${OBJECTDIR}/lobstate.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobstate.p1 lobstate.c 
	@-${MV} ${OBJECTDIR}/lobstate.d ${OBJECTDIR}/lobstate.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobstate.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/keypad.p1: keypad.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keypad.p1.d 
	@${RM} ${OBJECTDIR}/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/keypad.p1 keypad.c 
	@-${MV} ${OBJECTDIR}/keypad.d ${OBJECTDIR}/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/loblcd.p1: loblcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/loblcd.p1.d 
	@${RM} ${OBJECTDIR}/loblcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/loblcd.p1 loblcd.c 
	@-${MV} ${OBJECTDIR}/loblcd.d ${OBJECTDIR}/loblcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/loblcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobmain.p1: lobmain.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobmain.p1.d 
	@${RM} ${OBJECTDIR}/lobmain.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobmain.p1 lobmain.c 
	@-${MV} ${OBJECTDIR}/lobmain.d ${OBJECTDIR}/lobmain.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobmain.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/state_mach.p1: state_mach.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/state_mach.p1.d 
	@${RM} ${OBJECTDIR}/state_mach.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/state_mach.p1 state_mach.c 
	@-${MV} ${OBJECTDIR}/state_mach.d ${OBJECTDIR}/state_mach.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/state_mach.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobeeprom.p1: lobeeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobeeprom.p1.d 
	@${RM} ${OBJECTDIR}/lobeeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobeeprom.p1 lobeeprom.c 
	@-${MV} ${OBJECTDIR}/lobeeprom.d ${OBJECTDIR}/lobeeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobeeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobprog.p1: lobprog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobprog.p1.d 
	@${RM} ${OBJECTDIR}/lobprog.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobprog.p1 lobprog.c 
	@-${MV} ${OBJECTDIR}/lobprog.d ${OBJECTDIR}/lobprog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobprog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/varinit.p1: varinit.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/varinit.p1.d 
	@${RM} ${OBJECTDIR}/varinit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/varinit.p1 varinit.c 
	@-${MV} ${OBJECTDIR}/varinit.d ${OBJECTDIR}/varinit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/varinit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobparm.p1: lobparm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobparm.p1.d 
	@${RM} ${OBJECTDIR}/lobparm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobparm.p1 lobparm.c 
	@-${MV} ${OBJECTDIR}/lobparm.d ${OBJECTDIR}/lobparm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobparm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lobpre.p1: lobpre.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lobpre.p1.d 
	@${RM} ${OBJECTDIR}/lobpre.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lobpre.p1 lobpre.c 
	@-${MV} ${OBJECTDIR}/lobpre.d ${OBJECTDIR}/lobpre.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lobpre.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/diag.p1: diag.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diag.p1.d 
	@${RM} ${OBJECTDIR}/diag.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/diag.p1 diag.c 
	@-${MV} ${OBJECTDIR}/diag.d ${OBJECTDIR}/diag.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/diag.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/loberr.p1: loberr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/loberr.p1.d 
	@${RM} ${OBJECTDIR}/loberr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/loberr.p1 loberr.c 
	@-${MV} ${OBJECTDIR}/loberr.d ${OBJECTDIR}/loberr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/loberr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/EL04CLT.p1: EL04CLT.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EL04CLT.p1.d 
	@${RM} ${OBJECTDIR}/EL04CLT.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/EL04CLT.p1 EL04CLT.c 
	@-${MV} ${OBJECTDIR}/EL04CLT.d ${OBJECTDIR}/EL04CLT.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/EL04CLT.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ringbuf.p1: ringbuf.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ringbuf.p1.d 
	@${RM} ${OBJECTDIR}/ringbuf.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ringbuf.p1 ringbuf.c 
	@-${MV} ${OBJECTDIR}/ringbuf.d ${OBJECTDIR}/ringbuf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ringbuf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/busyxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/busyxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/busyxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/busyxlcd.d ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/busyxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/openxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/openxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/openxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/openxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/openxlcd.d ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/openxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/putrxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/putrxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/putrxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/putrxlcd.d ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/putrxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/putsxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/putsxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/putsxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/putsxlcd.d ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/putsxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/readaddr.p1: ../CS020_MasterMCU/EL04/xlcd/readaddr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readaddr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/readaddr.p1 ../CS020_MasterMCU/EL04/xlcd/readaddr.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/readaddr.d ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/readaddr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/readdata.p1: ../CS020_MasterMCU/EL04/xlcd/readdata.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readdata.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/readdata.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/readdata.p1 ../CS020_MasterMCU/EL04/xlcd/readdata.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/readdata.d ${OBJECTDIR}/_ext/1477896233/readdata.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/readdata.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/setcgram.p1: ../CS020_MasterMCU/EL04/xlcd/setcgram.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setcgram.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/setcgram.p1 ../CS020_MasterMCU/EL04/xlcd/setcgram.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/setcgram.d ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/setcgram.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/setddram.p1: ../CS020_MasterMCU/EL04/xlcd/setddram.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setddram.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/setddram.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/setddram.p1 ../CS020_MasterMCU/EL04/xlcd/setddram.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/setddram.d ${OBJECTDIR}/_ext/1477896233/setddram.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/setddram.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1: ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1 ../CS020_MasterMCU/EL04/xlcd/wcmdxlcd.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.d ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/wcmdxlcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1477896233/writdata.p1: ../CS020_MasterMCU/EL04/xlcd/writdata.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1477896233" 
	@${RM} ${OBJECTDIR}/_ext/1477896233/writdata.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1477896233/writdata.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,+speed,-space,-debug --addrqual=ignore --mode=pro -P -N255 -I"../../../../cs020_sw/Master_v1p224p34-XC8/EL04/main" -I"C:/Program Files (x86)/Microchip/xc8/v1.31/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.31/include/plib" -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/_ext/1477896233/writdata.p1 ../CS020_MasterMCU/EL04/xlcd/writdata.c 
	@-${MV} ${OBJECTDIR}/_ext/1477896233/writdata.d ${OBJECTDIR}/_ext/1477896233/writdata.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1477896233/writdata.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD)  -odist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)    --double=32 --float=32 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -DJS -P -N255 -I"." -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include" -I"C:/Program Files (x86)/Microchip/xc8/v1.33/include/plib" --warn=-1 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD)  -odist/${CND_CONF}/${IMAGE_TYPE}/CS020_MasterMCU_2PcCase.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
