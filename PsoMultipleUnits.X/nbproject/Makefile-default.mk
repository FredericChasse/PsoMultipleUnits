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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Objects/Algo/SteadyState.c Objects/Perturb/Perturb.c source/LedDriver.c source/Codec/Codec.c source/Codec/Protocol.c source/main.c source/StateMachine.c source/Interrupts.c source/Setup.c source/StateFunctions.c source/SkadiFunctions.c source/Potentiometer.c Objects/Units/Unit.c Objects/Units/UnitArray.c Objects/Units/UnitMapping.c Utilities/BipBuffer/BipBuffer.c Utilities/ByteBuffer/ByteBuffer.c Utilities/ChainedList/LinkedList.c Utilities/MathFunctions/MathFunctions.c Utilities/PcgRng/pcg_basic.c Utilities/PcgRng/Rng.c Objects/Algo/Pso.c Objects/Algo/PsoParticle.c Objects/Algo/PsoSwarm.c Objects/Algo/Characterization.c Objects/Algo/ExtremumSeeking.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Objects/Algo/SteadyState.o ${OBJECTDIR}/Objects/Perturb/Perturb.o ${OBJECTDIR}/source/LedDriver.o ${OBJECTDIR}/source/Codec/Codec.o ${OBJECTDIR}/source/Codec/Protocol.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/StateMachine.o ${OBJECTDIR}/source/Interrupts.o ${OBJECTDIR}/source/Setup.o ${OBJECTDIR}/source/StateFunctions.o ${OBJECTDIR}/source/SkadiFunctions.o ${OBJECTDIR}/source/Potentiometer.o ${OBJECTDIR}/Objects/Units/Unit.o ${OBJECTDIR}/Objects/Units/UnitArray.o ${OBJECTDIR}/Objects/Units/UnitMapping.o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o ${OBJECTDIR}/Utilities/PcgRng/Rng.o ${OBJECTDIR}/Objects/Algo/Pso.o ${OBJECTDIR}/Objects/Algo/PsoParticle.o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o ${OBJECTDIR}/Objects/Algo/Characterization.o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Objects/Algo/SteadyState.o.d ${OBJECTDIR}/Objects/Perturb/Perturb.o.d ${OBJECTDIR}/source/LedDriver.o.d ${OBJECTDIR}/source/Codec/Codec.o.d ${OBJECTDIR}/source/Codec/Protocol.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/StateMachine.o.d ${OBJECTDIR}/source/Interrupts.o.d ${OBJECTDIR}/source/Setup.o.d ${OBJECTDIR}/source/StateFunctions.o.d ${OBJECTDIR}/source/SkadiFunctions.o.d ${OBJECTDIR}/source/Potentiometer.o.d ${OBJECTDIR}/Objects/Units/Unit.o.d ${OBJECTDIR}/Objects/Units/UnitArray.o.d ${OBJECTDIR}/Objects/Units/UnitMapping.o.d ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d ${OBJECTDIR}/Objects/Algo/Pso.o.d ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d ${OBJECTDIR}/Objects/Algo/Characterization.o.d ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Objects/Algo/SteadyState.o ${OBJECTDIR}/Objects/Perturb/Perturb.o ${OBJECTDIR}/source/LedDriver.o ${OBJECTDIR}/source/Codec/Codec.o ${OBJECTDIR}/source/Codec/Protocol.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/StateMachine.o ${OBJECTDIR}/source/Interrupts.o ${OBJECTDIR}/source/Setup.o ${OBJECTDIR}/source/StateFunctions.o ${OBJECTDIR}/source/SkadiFunctions.o ${OBJECTDIR}/source/Potentiometer.o ${OBJECTDIR}/Objects/Units/Unit.o ${OBJECTDIR}/Objects/Units/UnitArray.o ${OBJECTDIR}/Objects/Units/UnitMapping.o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o ${OBJECTDIR}/Utilities/PcgRng/Rng.o ${OBJECTDIR}/Objects/Algo/Pso.o ${OBJECTDIR}/Objects/Algo/PsoParticle.o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o ${OBJECTDIR}/Objects/Algo/Characterization.o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o

# Source Files
SOURCEFILES=Objects/Algo/SteadyState.c Objects/Perturb/Perturb.c source/LedDriver.c source/Codec/Codec.c source/Codec/Protocol.c source/main.c source/StateMachine.c source/Interrupts.c source/Setup.c source/StateFunctions.c source/SkadiFunctions.c source/Potentiometer.c Objects/Units/Unit.c Objects/Units/UnitArray.c Objects/Units/UnitMapping.c Utilities/BipBuffer/BipBuffer.c Utilities/ByteBuffer/ByteBuffer.c Utilities/ChainedList/LinkedList.c Utilities/MathFunctions/MathFunctions.c Utilities/PcgRng/pcg_basic.c Utilities/PcgRng/Rng.c Objects/Algo/Pso.c Objects/Algo/PsoParticle.c Objects/Algo/PsoSwarm.c Objects/Algo/Characterization.c Objects/Algo/ExtremumSeeking.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/Objects/Algo/SteadyState.o: Objects/Algo/SteadyState.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyState.o Objects/Algo/SteadyState.c   
	
${OBJECTDIR}/Objects/Perturb/Perturb.o: Objects/Perturb/Perturb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Perturb" 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o.d 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" -o ${OBJECTDIR}/Objects/Perturb/Perturb.o Objects/Perturb/Perturb.c   
	
${OBJECTDIR}/source/LedDriver.o: source/LedDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/LedDriver.o.d 
	@${RM} ${OBJECTDIR}/source/LedDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/source/LedDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/LedDriver.o.d" -o ${OBJECTDIR}/source/LedDriver.o source/LedDriver.c   
	
${OBJECTDIR}/source/Codec/Codec.o: source/Codec/Codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Codec.o.d" -o ${OBJECTDIR}/source/Codec/Codec.o source/Codec/Codec.c   
	
${OBJECTDIR}/source/Codec/Protocol.o: source/Codec/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Protocol.o.d" -o ${OBJECTDIR}/source/Codec/Protocol.o source/Codec/Protocol.c   
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c   
	
${OBJECTDIR}/source/StateMachine.o: source/StateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/StateMachine.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateMachine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateMachine.o.d" -o ${OBJECTDIR}/source/StateMachine.o source/StateMachine.c   
	
${OBJECTDIR}/source/Interrupts.o: source/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/source/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Interrupts.o.d" -o ${OBJECTDIR}/source/Interrupts.o source/Interrupts.c   
	
${OBJECTDIR}/source/Setup.o: source/Setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Setup.o.d 
	@${RM} ${OBJECTDIR}/source/Setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Setup.o.d" -o ${OBJECTDIR}/source/Setup.o source/Setup.c   
	
${OBJECTDIR}/source/StateFunctions.o: source/StateFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateFunctions.o.d" -o ${OBJECTDIR}/source/StateFunctions.o source/StateFunctions.c   
	
${OBJECTDIR}/source/SkadiFunctions.o: source/SkadiFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/SkadiFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/SkadiFunctions.o.d" -o ${OBJECTDIR}/source/SkadiFunctions.o source/SkadiFunctions.c   
	
${OBJECTDIR}/source/Potentiometer.o: source/Potentiometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o.d 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Potentiometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Potentiometer.o.d" -o ${OBJECTDIR}/source/Potentiometer.o source/Potentiometer.c   
	
${OBJECTDIR}/Objects/Units/Unit.o: Objects/Units/Unit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/Unit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/Unit.o.d" -o ${OBJECTDIR}/Objects/Units/Unit.o Objects/Units/Unit.c   
	
${OBJECTDIR}/Objects/Units/UnitArray.o: Objects/Units/UnitArray.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitArray.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitArray.o.d" -o ${OBJECTDIR}/Objects/Units/UnitArray.o Objects/Units/UnitArray.c   
	
${OBJECTDIR}/Objects/Units/UnitMapping.o: Objects/Units/UnitMapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" -o ${OBJECTDIR}/Objects/Units/UnitMapping.o Objects/Units/UnitMapping.c   
	
${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o: Utilities/BipBuffer/BipBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/BipBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" -o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o Utilities/BipBuffer/BipBuffer.c   
	
${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o: Utilities/ByteBuffer/ByteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ByteBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" -o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o Utilities/ByteBuffer/ByteBuffer.c   
	
${OBJECTDIR}/Utilities/ChainedList/LinkedList.o: Utilities/ChainedList/LinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ChainedList" 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" -o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o Utilities/ChainedList/LinkedList.c   
	
${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o: Utilities/MathFunctions/MathFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/MathFunctions" 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" -o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o Utilities/MathFunctions/MathFunctions.c   
	
${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o: Utilities/PcgRng/pcg_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o Utilities/PcgRng/pcg_basic.c   
	
${OBJECTDIR}/Utilities/PcgRng/Rng.o: Utilities/PcgRng/Rng.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/Rng.o Utilities/PcgRng/Rng.c   
	
${OBJECTDIR}/Objects/Algo/Pso.o: Objects/Algo/Pso.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pso.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pso.o.d" -o ${OBJECTDIR}/Objects/Algo/Pso.o Objects/Algo/Pso.c   
	
${OBJECTDIR}/Objects/Algo/PsoParticle.o: Objects/Algo/PsoParticle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoParticle.o Objects/Algo/PsoParticle.c   
	
${OBJECTDIR}/Objects/Algo/PsoSwarm.o: Objects/Algo/PsoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o Objects/Algo/PsoSwarm.c   
	
${OBJECTDIR}/Objects/Algo/Characterization.o: Objects/Algo/Characterization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Characterization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Characterization.o.d" -o ${OBJECTDIR}/Objects/Algo/Characterization.o Objects/Algo/Characterization.c   
	
${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o: Objects/Algo/ExtremumSeeking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" -o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o Objects/Algo/ExtremumSeeking.c   
	
else
${OBJECTDIR}/Objects/Algo/SteadyState.o: Objects/Algo/SteadyState.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyState.o Objects/Algo/SteadyState.c   
	
${OBJECTDIR}/Objects/Perturb/Perturb.o: Objects/Perturb/Perturb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Perturb" 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o.d 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" -o ${OBJECTDIR}/Objects/Perturb/Perturb.o Objects/Perturb/Perturb.c   
	
${OBJECTDIR}/source/LedDriver.o: source/LedDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/LedDriver.o.d 
	@${RM} ${OBJECTDIR}/source/LedDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/source/LedDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/LedDriver.o.d" -o ${OBJECTDIR}/source/LedDriver.o source/LedDriver.c   
	
${OBJECTDIR}/source/Codec/Codec.o: source/Codec/Codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Codec.o.d" -o ${OBJECTDIR}/source/Codec/Codec.o source/Codec/Codec.c   
	
${OBJECTDIR}/source/Codec/Protocol.o: source/Codec/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Protocol.o.d" -o ${OBJECTDIR}/source/Codec/Protocol.o source/Codec/Protocol.c   
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c   
	
${OBJECTDIR}/source/StateMachine.o: source/StateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/StateMachine.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateMachine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateMachine.o.d" -o ${OBJECTDIR}/source/StateMachine.o source/StateMachine.c   
	
${OBJECTDIR}/source/Interrupts.o: source/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/source/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Interrupts.o.d" -o ${OBJECTDIR}/source/Interrupts.o source/Interrupts.c   
	
${OBJECTDIR}/source/Setup.o: source/Setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Setup.o.d 
	@${RM} ${OBJECTDIR}/source/Setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Setup.o.d" -o ${OBJECTDIR}/source/Setup.o source/Setup.c   
	
${OBJECTDIR}/source/StateFunctions.o: source/StateFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateFunctions.o.d" -o ${OBJECTDIR}/source/StateFunctions.o source/StateFunctions.c   
	
${OBJECTDIR}/source/SkadiFunctions.o: source/SkadiFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/SkadiFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/SkadiFunctions.o.d" -o ${OBJECTDIR}/source/SkadiFunctions.o source/SkadiFunctions.c   
	
${OBJECTDIR}/source/Potentiometer.o: source/Potentiometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o.d 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Potentiometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Potentiometer.o.d" -o ${OBJECTDIR}/source/Potentiometer.o source/Potentiometer.c   
	
${OBJECTDIR}/Objects/Units/Unit.o: Objects/Units/Unit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/Unit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/Unit.o.d" -o ${OBJECTDIR}/Objects/Units/Unit.o Objects/Units/Unit.c   
	
${OBJECTDIR}/Objects/Units/UnitArray.o: Objects/Units/UnitArray.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitArray.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitArray.o.d" -o ${OBJECTDIR}/Objects/Units/UnitArray.o Objects/Units/UnitArray.c   
	
${OBJECTDIR}/Objects/Units/UnitMapping.o: Objects/Units/UnitMapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" -o ${OBJECTDIR}/Objects/Units/UnitMapping.o Objects/Units/UnitMapping.c   
	
${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o: Utilities/BipBuffer/BipBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/BipBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" -o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o Utilities/BipBuffer/BipBuffer.c   
	
${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o: Utilities/ByteBuffer/ByteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ByteBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" -o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o Utilities/ByteBuffer/ByteBuffer.c   
	
${OBJECTDIR}/Utilities/ChainedList/LinkedList.o: Utilities/ChainedList/LinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ChainedList" 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" -o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o Utilities/ChainedList/LinkedList.c   
	
${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o: Utilities/MathFunctions/MathFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/MathFunctions" 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" -o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o Utilities/MathFunctions/MathFunctions.c   
	
${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o: Utilities/PcgRng/pcg_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o Utilities/PcgRng/pcg_basic.c   
	
${OBJECTDIR}/Utilities/PcgRng/Rng.o: Utilities/PcgRng/Rng.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/Rng.o Utilities/PcgRng/Rng.c   
	
${OBJECTDIR}/Objects/Algo/Pso.o: Objects/Algo/Pso.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pso.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pso.o.d" -o ${OBJECTDIR}/Objects/Algo/Pso.o Objects/Algo/Pso.c   
	
${OBJECTDIR}/Objects/Algo/PsoParticle.o: Objects/Algo/PsoParticle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoParticle.o Objects/Algo/PsoParticle.c   
	
${OBJECTDIR}/Objects/Algo/PsoSwarm.o: Objects/Algo/PsoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o Objects/Algo/PsoSwarm.c   
	
${OBJECTDIR}/Objects/Algo/Characterization.o: Objects/Algo/Characterization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Characterization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Characterization.o.d" -o ${OBJECTDIR}/Objects/Algo/Characterization.o Objects/Algo/Characterization.c   
	
${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o: Objects/Algo/ExtremumSeeking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" -o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o Objects/Algo/ExtremumSeeking.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../ChinookLib/ChinookLib.X/dist/default/debug/ChinookLib.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\ChinookLib\ChinookLib.X\dist\default\debug\ChinookLib.X.a       -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=1024,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../ChinookLib/ChinookLib.X/dist/default/production/ChinookLib.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\ChinookLib\ChinookLib.X\dist\default\production\ChinookLib.X.a      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=1024,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:
	cd /D ../../ChinookLib/ChinookLib.X && ${MAKE}  -f Makefile CONF=default


# Subprojects
.clean-subprojects:
	cd /D ../../ChinookLib/ChinookLib.X && rm -rf "build/default" "dist/default"

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
