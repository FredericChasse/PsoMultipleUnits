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
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
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
SOURCEFILES_QUOTED_IF_SPACED=source/ChinookPorts.c source/ChinookTimer.c source/ChinookUart.c source/ChinookSpi.c source/ChinookAdc.c source/ChinookWdt.c source/ChinookSkadi.c source/ChinookI2c.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/ChinookPorts.o ${OBJECTDIR}/source/ChinookTimer.o ${OBJECTDIR}/source/ChinookUart.o ${OBJECTDIR}/source/ChinookSpi.o ${OBJECTDIR}/source/ChinookAdc.o ${OBJECTDIR}/source/ChinookWdt.o ${OBJECTDIR}/source/ChinookSkadi.o ${OBJECTDIR}/source/ChinookI2c.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/ChinookPorts.o.d ${OBJECTDIR}/source/ChinookTimer.o.d ${OBJECTDIR}/source/ChinookUart.o.d ${OBJECTDIR}/source/ChinookSpi.o.d ${OBJECTDIR}/source/ChinookAdc.o.d ${OBJECTDIR}/source/ChinookWdt.o.d ${OBJECTDIR}/source/ChinookSkadi.o.d ${OBJECTDIR}/source/ChinookI2c.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/ChinookPorts.o ${OBJECTDIR}/source/ChinookTimer.o ${OBJECTDIR}/source/ChinookUart.o ${OBJECTDIR}/source/ChinookSpi.o ${OBJECTDIR}/source/ChinookAdc.o ${OBJECTDIR}/source/ChinookWdt.o ${OBJECTDIR}/source/ChinookSkadi.o ${OBJECTDIR}/source/ChinookI2c.o

# Source Files
SOURCEFILES=source/ChinookPorts.c source/ChinookTimer.c source/ChinookUart.c source/ChinookSpi.c source/ChinookAdc.c source/ChinookWdt.c source/ChinookSkadi.c source/ChinookI2c.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/ChinookPorts.o: source/ChinookPorts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookPorts.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookPorts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookPorts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookPorts.o.d" -o ${OBJECTDIR}/source/ChinookPorts.o source/ChinookPorts.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookTimer.o: source/ChinookTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookTimer.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookTimer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookTimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookTimer.o.d" -o ${OBJECTDIR}/source/ChinookTimer.o source/ChinookTimer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookUart.o: source/ChinookUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookUart.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookUart.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookUart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookUart.o.d" -o ${OBJECTDIR}/source/ChinookUart.o source/ChinookUart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookSpi.o: source/ChinookSpi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookSpi.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookSpi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookSpi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookSpi.o.d" -o ${OBJECTDIR}/source/ChinookSpi.o source/ChinookSpi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookAdc.o: source/ChinookAdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookAdc.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookAdc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookAdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookAdc.o.d" -o ${OBJECTDIR}/source/ChinookAdc.o source/ChinookAdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookWdt.o: source/ChinookWdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookWdt.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookWdt.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookWdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookWdt.o.d" -o ${OBJECTDIR}/source/ChinookWdt.o source/ChinookWdt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookSkadi.o: source/ChinookSkadi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookSkadi.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookSkadi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookSkadi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookSkadi.o.d" -o ${OBJECTDIR}/source/ChinookSkadi.o source/ChinookSkadi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookI2c.o: source/ChinookI2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookI2c.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookI2c.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookI2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookI2c.o.d" -o ${OBJECTDIR}/source/ChinookI2c.o source/ChinookI2c.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/source/ChinookPorts.o: source/ChinookPorts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookPorts.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookPorts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookPorts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookPorts.o.d" -o ${OBJECTDIR}/source/ChinookPorts.o source/ChinookPorts.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookTimer.o: source/ChinookTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookTimer.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookTimer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookTimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookTimer.o.d" -o ${OBJECTDIR}/source/ChinookTimer.o source/ChinookTimer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookUart.o: source/ChinookUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookUart.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookUart.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookUart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookUart.o.d" -o ${OBJECTDIR}/source/ChinookUart.o source/ChinookUart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookSpi.o: source/ChinookSpi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookSpi.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookSpi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookSpi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookSpi.o.d" -o ${OBJECTDIR}/source/ChinookSpi.o source/ChinookSpi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookAdc.o: source/ChinookAdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookAdc.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookAdc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookAdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookAdc.o.d" -o ${OBJECTDIR}/source/ChinookAdc.o source/ChinookAdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookWdt.o: source/ChinookWdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookWdt.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookWdt.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookWdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookWdt.o.d" -o ${OBJECTDIR}/source/ChinookWdt.o source/ChinookWdt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookSkadi.o: source/ChinookSkadi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookSkadi.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookSkadi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookSkadi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookSkadi.o.d" -o ${OBJECTDIR}/source/ChinookSkadi.o source/ChinookSkadi.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/ChinookI2c.o: source/ChinookI2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/ChinookI2c.o.d 
	@${RM} ${OBJECTDIR}/source/ChinookI2c.o 
	@${FIXDEPS} "${OBJECTDIR}/source/ChinookI2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -I"headers" -I"source" -MMD -MF "${OBJECTDIR}/source/ChinookI2c.o.d" -o ${OBJECTDIR}/source/ChinookI2c.o source/ChinookI2c.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) r dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    
else
dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) r dist/${CND_CONF}/${IMAGE_TYPE}/ChinookLib.X.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    
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
