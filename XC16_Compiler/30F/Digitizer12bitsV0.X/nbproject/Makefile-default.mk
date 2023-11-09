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
FINAL_IMAGE=${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Digitizer12bitsV0.c 30F_UART.c GPS_CMD.c 30F_TIMER.c 30F_ADC.c 30F_DS1307.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Digitizer12bitsV0.o ${OBJECTDIR}/30F_UART.o ${OBJECTDIR}/GPS_CMD.o ${OBJECTDIR}/30F_TIMER.o ${OBJECTDIR}/30F_ADC.o ${OBJECTDIR}/30F_DS1307.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Digitizer12bitsV0.o.d ${OBJECTDIR}/30F_UART.o.d ${OBJECTDIR}/GPS_CMD.o.d ${OBJECTDIR}/30F_TIMER.o.d ${OBJECTDIR}/30F_ADC.o.d ${OBJECTDIR}/30F_DS1307.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Digitizer12bitsV0.o ${OBJECTDIR}/30F_UART.o ${OBJECTDIR}/GPS_CMD.o ${OBJECTDIR}/30F_TIMER.o ${OBJECTDIR}/30F_ADC.o ${OBJECTDIR}/30F_DS1307.o

# Source Files
SOURCEFILES=Digitizer12bitsV0.c 30F_UART.c GPS_CMD.c 30F_TIMER.c 30F_ADC.c 30F_DS1307.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4013
MP_LINKER_FILE_OPTION=,--script=p30F4013.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Digitizer12bitsV0.o: Digitizer12bitsV0.c  .generated_files/flags/default/96e6d614608cc2446d8ca9ebd0a9fa3ff7d80be9 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Digitizer12bitsV0.o.d 
	@${RM} ${OBJECTDIR}/Digitizer12bitsV0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Digitizer12bitsV0.c  -o ${OBJECTDIR}/Digitizer12bitsV0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Digitizer12bitsV0.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_UART.o: 30F_UART.c  .generated_files/flags/default/c3cb34d58ad5300feb34b00c141e9846082186d3 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_UART.o.d 
	@${RM} ${OBJECTDIR}/30F_UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_UART.c  -o ${OBJECTDIR}/30F_UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_UART.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/GPS_CMD.o: GPS_CMD.c  .generated_files/flags/default/f2519977d16770bfe668bd6241b88289ba3c75b4 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPS_CMD.o.d 
	@${RM} ${OBJECTDIR}/GPS_CMD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  GPS_CMD.c  -o ${OBJECTDIR}/GPS_CMD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GPS_CMD.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_TIMER.o: 30F_TIMER.c  .generated_files/flags/default/f18ac1f927f1b958504f6c636be83632a6355e10 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_TIMER.o.d 
	@${RM} ${OBJECTDIR}/30F_TIMER.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_TIMER.c  -o ${OBJECTDIR}/30F_TIMER.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_TIMER.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_ADC.o: 30F_ADC.c  .generated_files/flags/default/d8cd5b435b50972aca2cffab36b60885c4b1c95f .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_ADC.o.d 
	@${RM} ${OBJECTDIR}/30F_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_ADC.c  -o ${OBJECTDIR}/30F_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_ADC.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_DS1307.o: 30F_DS1307.c  .generated_files/flags/default/a3153e196f33a3a9de119c99739146f401c101d5 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_DS1307.o.d 
	@${RM} ${OBJECTDIR}/30F_DS1307.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_DS1307.c  -o ${OBJECTDIR}/30F_DS1307.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_DS1307.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/Digitizer12bitsV0.o: Digitizer12bitsV0.c  .generated_files/flags/default/36649aa87fbb951a9b405eeda661002f15b905ad .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Digitizer12bitsV0.o.d 
	@${RM} ${OBJECTDIR}/Digitizer12bitsV0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Digitizer12bitsV0.c  -o ${OBJECTDIR}/Digitizer12bitsV0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Digitizer12bitsV0.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_UART.o: 30F_UART.c  .generated_files/flags/default/983ed6cd947a8e8028f464463dc5b876b204e28c .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_UART.o.d 
	@${RM} ${OBJECTDIR}/30F_UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_UART.c  -o ${OBJECTDIR}/30F_UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_UART.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/GPS_CMD.o: GPS_CMD.c  .generated_files/flags/default/b8e213f2e681ab277daa1778de234b25bbfea19b .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPS_CMD.o.d 
	@${RM} ${OBJECTDIR}/GPS_CMD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  GPS_CMD.c  -o ${OBJECTDIR}/GPS_CMD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GPS_CMD.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_TIMER.o: 30F_TIMER.c  .generated_files/flags/default/7c7f3877fcb1f5cd5c7fb3118233a54dacd39555 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_TIMER.o.d 
	@${RM} ${OBJECTDIR}/30F_TIMER.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_TIMER.c  -o ${OBJECTDIR}/30F_TIMER.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_TIMER.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_ADC.o: 30F_ADC.c  .generated_files/flags/default/124ca3b31fbabae6b2cbbd5026f1b3cd6a18e75e .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_ADC.o.d 
	@${RM} ${OBJECTDIR}/30F_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_ADC.c  -o ${OBJECTDIR}/30F_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_ADC.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/30F_DS1307.o: 30F_DS1307.c  .generated_files/flags/default/2fbbbda7be0fa0a0990bb1ed4a234ca8d0d68414 .generated_files/flags/default/fd67b544a79261c2fe49714d5749bb13d89b49dc
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/30F_DS1307.o.d 
	@${RM} ${OBJECTDIR}/30F_DS1307.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  30F_DS1307.c  -o ${OBJECTDIR}/30F_DS1307.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/30F_DS1307.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Digitizer12bitsV0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
