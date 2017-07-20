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
SOURCEFILES_QUOTED_IF_SPACED=Objects/Algo/Characterization.c Objects/Algo/ExtremumSeeking.c Objects/Algo/Pno.c Objects/Algo/PnoInstance.c Objects/Algo/SteadyStatePno.c Objects/Algo/PnoSwarm.c Objects/Algo/PpsoPno.c Objects/Algo/Pso.c Objects/Algo/PsoParticle.c Objects/Algo/PsoSwarm.c Objects/Algo/Classifier.c Objects/Algo/SteadyState.c Objects/Algo/Position.c Objects/Perturb/Perturb.c source/LedDriver.c source/Codec/Codec.c source/Codec/Protocol.c source/main.c source/StateMachine.c source/Interrupts.c source/Setup.c source/StateFunctions.c source/SkadiFunctions.c source/Potentiometer.c Objects/Units/Unit.c Objects/Units/UnitArray.c Objects/Units/UnitMapping.c Utilities/BipBuffer/BipBuffer.c Utilities/ByteBuffer/ByteBuffer.c Utilities/ChainedList/LinkedList.c Utilities/MathFunctions/MathFunctions.c Utilities/PcgRng/pcg_basic.c Utilities/PcgRng/Rng.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Objects/Algo/Characterization.o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o ${OBJECTDIR}/Objects/Algo/Pno.o ${OBJECTDIR}/Objects/Algo/PnoInstance.o ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o ${OBJECTDIR}/Objects/Algo/PnoSwarm.o ${OBJECTDIR}/Objects/Algo/PpsoPno.o ${OBJECTDIR}/Objects/Algo/Pso.o ${OBJECTDIR}/Objects/Algo/PsoParticle.o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o ${OBJECTDIR}/Objects/Algo/Classifier.o ${OBJECTDIR}/Objects/Algo/SteadyState.o ${OBJECTDIR}/Objects/Algo/Position.o ${OBJECTDIR}/Objects/Perturb/Perturb.o ${OBJECTDIR}/source/LedDriver.o ${OBJECTDIR}/source/Codec/Codec.o ${OBJECTDIR}/source/Codec/Protocol.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/StateMachine.o ${OBJECTDIR}/source/Interrupts.o ${OBJECTDIR}/source/Setup.o ${OBJECTDIR}/source/StateFunctions.o ${OBJECTDIR}/source/SkadiFunctions.o ${OBJECTDIR}/source/Potentiometer.o ${OBJECTDIR}/Objects/Units/Unit.o ${OBJECTDIR}/Objects/Units/UnitArray.o ${OBJECTDIR}/Objects/Units/UnitMapping.o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o ${OBJECTDIR}/Utilities/PcgRng/Rng.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Objects/Algo/Characterization.o.d ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d ${OBJECTDIR}/Objects/Algo/Pno.o.d ${OBJECTDIR}/Objects/Algo/PnoInstance.o.d ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d ${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d ${OBJECTDIR}/Objects/Algo/PpsoPno.o.d ${OBJECTDIR}/Objects/Algo/Pso.o.d ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d ${OBJECTDIR}/Objects/Algo/Classifier.o.d ${OBJECTDIR}/Objects/Algo/SteadyState.o.d ${OBJECTDIR}/Objects/Algo/Position.o.d ${OBJECTDIR}/Objects/Perturb/Perturb.o.d ${OBJECTDIR}/source/LedDriver.o.d ${OBJECTDIR}/source/Codec/Codec.o.d ${OBJECTDIR}/source/Codec/Protocol.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/StateMachine.o.d ${OBJECTDIR}/source/Interrupts.o.d ${OBJECTDIR}/source/Setup.o.d ${OBJECTDIR}/source/StateFunctions.o.d ${OBJECTDIR}/source/SkadiFunctions.o.d ${OBJECTDIR}/source/Potentiometer.o.d ${OBJECTDIR}/Objects/Units/Unit.o.d ${OBJECTDIR}/Objects/Units/UnitArray.o.d ${OBJECTDIR}/Objects/Units/UnitMapping.o.d ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Objects/Algo/Characterization.o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o ${OBJECTDIR}/Objects/Algo/Pno.o ${OBJECTDIR}/Objects/Algo/PnoInstance.o ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o ${OBJECTDIR}/Objects/Algo/PnoSwarm.o ${OBJECTDIR}/Objects/Algo/PpsoPno.o ${OBJECTDIR}/Objects/Algo/Pso.o ${OBJECTDIR}/Objects/Algo/PsoParticle.o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o ${OBJECTDIR}/Objects/Algo/Classifier.o ${OBJECTDIR}/Objects/Algo/SteadyState.o ${OBJECTDIR}/Objects/Algo/Position.o ${OBJECTDIR}/Objects/Perturb/Perturb.o ${OBJECTDIR}/source/LedDriver.o ${OBJECTDIR}/source/Codec/Codec.o ${OBJECTDIR}/source/Codec/Protocol.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/StateMachine.o ${OBJECTDIR}/source/Interrupts.o ${OBJECTDIR}/source/Setup.o ${OBJECTDIR}/source/StateFunctions.o ${OBJECTDIR}/source/SkadiFunctions.o ${OBJECTDIR}/source/Potentiometer.o ${OBJECTDIR}/Objects/Units/Unit.o ${OBJECTDIR}/Objects/Units/UnitArray.o ${OBJECTDIR}/Objects/Units/UnitMapping.o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o ${OBJECTDIR}/Utilities/PcgRng/Rng.o

# Source Files
SOURCEFILES=Objects/Algo/Characterization.c Objects/Algo/ExtremumSeeking.c Objects/Algo/Pno.c Objects/Algo/PnoInstance.c Objects/Algo/SteadyStatePno.c Objects/Algo/PnoSwarm.c Objects/Algo/PpsoPno.c Objects/Algo/Pso.c Objects/Algo/PsoParticle.c Objects/Algo/PsoSwarm.c Objects/Algo/Classifier.c Objects/Algo/SteadyState.c Objects/Algo/Position.c Objects/Perturb/Perturb.c source/LedDriver.c source/Codec/Codec.c source/Codec/Protocol.c source/main.c source/StateMachine.c source/Interrupts.c source/Setup.c source/StateFunctions.c source/SkadiFunctions.c source/Potentiometer.c Objects/Units/Unit.c Objects/Units/UnitArray.c Objects/Units/UnitMapping.c Utilities/BipBuffer/BipBuffer.c Utilities/ByteBuffer/ByteBuffer.c Utilities/ChainedList/LinkedList.c Utilities/MathFunctions/MathFunctions.c Utilities/PcgRng/pcg_basic.c Utilities/PcgRng/Rng.c


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
${OBJECTDIR}/Objects/Algo/Characterization.o: Objects/Algo/Characterization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Characterization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Characterization.o.d" -o ${OBJECTDIR}/Objects/Algo/Characterization.o Objects/Algo/Characterization.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o: Objects/Algo/ExtremumSeeking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" -o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o Objects/Algo/ExtremumSeeking.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Pno.o: Objects/Algo/Pno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pno.o.d" -o ${OBJECTDIR}/Objects/Algo/Pno.o Objects/Algo/Pno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PnoInstance.o: Objects/Algo/PnoInstance.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoInstance.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoInstance.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PnoInstance.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PnoInstance.o.d" -o ${OBJECTDIR}/Objects/Algo/PnoInstance.o Objects/Algo/PnoInstance.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/SteadyStatePno.o: Objects/Algo/SteadyStatePno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o Objects/Algo/SteadyStatePno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PnoSwarm.o: Objects/Algo/PnoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PnoSwarm.o Objects/Algo/PnoSwarm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PpsoPno.o: Objects/Algo/PpsoPno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PpsoPno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PpsoPno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PpsoPno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PpsoPno.o.d" -o ${OBJECTDIR}/Objects/Algo/PpsoPno.o Objects/Algo/PpsoPno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Pso.o: Objects/Algo/Pso.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pso.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pso.o.d" -o ${OBJECTDIR}/Objects/Algo/Pso.o Objects/Algo/Pso.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PsoParticle.o: Objects/Algo/PsoParticle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoParticle.o Objects/Algo/PsoParticle.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PsoSwarm.o: Objects/Algo/PsoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o Objects/Algo/PsoSwarm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Classifier.o: Objects/Algo/Classifier.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Classifier.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Classifier.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Classifier.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Classifier.o.d" -o ${OBJECTDIR}/Objects/Algo/Classifier.o Objects/Algo/Classifier.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/SteadyState.o: Objects/Algo/SteadyState.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyState.o Objects/Algo/SteadyState.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Position.o: Objects/Algo/Position.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Position.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Position.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Position.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Position.o.d" -o ${OBJECTDIR}/Objects/Algo/Position.o Objects/Algo/Position.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Perturb/Perturb.o: Objects/Perturb/Perturb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Perturb" 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o.d 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" -o ${OBJECTDIR}/Objects/Perturb/Perturb.o Objects/Perturb/Perturb.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/LedDriver.o: source/LedDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/LedDriver.o.d 
	@${RM} ${OBJECTDIR}/source/LedDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/source/LedDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/LedDriver.o.d" -o ${OBJECTDIR}/source/LedDriver.o source/LedDriver.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Codec/Codec.o: source/Codec/Codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Codec.o.d" -o ${OBJECTDIR}/source/Codec/Codec.o source/Codec/Codec.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Codec/Protocol.o: source/Codec/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Protocol.o.d" -o ${OBJECTDIR}/source/Codec/Protocol.o source/Codec/Protocol.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/StateMachine.o: source/StateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/StateMachine.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateMachine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateMachine.o.d" -o ${OBJECTDIR}/source/StateMachine.o source/StateMachine.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Interrupts.o: source/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/source/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Interrupts.o.d" -o ${OBJECTDIR}/source/Interrupts.o source/Interrupts.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Setup.o: source/Setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Setup.o.d 
	@${RM} ${OBJECTDIR}/source/Setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Setup.o.d" -o ${OBJECTDIR}/source/Setup.o source/Setup.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/StateFunctions.o: source/StateFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateFunctions.o.d" -o ${OBJECTDIR}/source/StateFunctions.o source/StateFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/SkadiFunctions.o: source/SkadiFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/SkadiFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/SkadiFunctions.o.d" -o ${OBJECTDIR}/source/SkadiFunctions.o source/SkadiFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Potentiometer.o: source/Potentiometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o.d 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Potentiometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Potentiometer.o.d" -o ${OBJECTDIR}/source/Potentiometer.o source/Potentiometer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/Unit.o: Objects/Units/Unit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/Unit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/Unit.o.d" -o ${OBJECTDIR}/Objects/Units/Unit.o Objects/Units/Unit.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/UnitArray.o: Objects/Units/UnitArray.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitArray.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitArray.o.d" -o ${OBJECTDIR}/Objects/Units/UnitArray.o Objects/Units/UnitArray.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/UnitMapping.o: Objects/Units/UnitMapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" -o ${OBJECTDIR}/Objects/Units/UnitMapping.o Objects/Units/UnitMapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o: Utilities/BipBuffer/BipBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/BipBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" -o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o Utilities/BipBuffer/BipBuffer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o: Utilities/ByteBuffer/ByteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ByteBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" -o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o Utilities/ByteBuffer/ByteBuffer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/ChainedList/LinkedList.o: Utilities/ChainedList/LinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ChainedList" 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" -o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o Utilities/ChainedList/LinkedList.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o: Utilities/MathFunctions/MathFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/MathFunctions" 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" -o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o Utilities/MathFunctions/MathFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o: Utilities/PcgRng/pcg_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o Utilities/PcgRng/pcg_basic.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/PcgRng/Rng.o: Utilities/PcgRng/Rng.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/Rng.o Utilities/PcgRng/Rng.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/Objects/Algo/Characterization.o: Objects/Algo/Characterization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Characterization.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Characterization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Characterization.o.d" -o ${OBJECTDIR}/Objects/Algo/Characterization.o Objects/Algo/Characterization.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o: Objects/Algo/ExtremumSeeking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o.d" -o ${OBJECTDIR}/Objects/Algo/ExtremumSeeking.o Objects/Algo/ExtremumSeeking.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Pno.o: Objects/Algo/Pno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pno.o.d" -o ${OBJECTDIR}/Objects/Algo/Pno.o Objects/Algo/Pno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PnoInstance.o: Objects/Algo/PnoInstance.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoInstance.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoInstance.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PnoInstance.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PnoInstance.o.d" -o ${OBJECTDIR}/Objects/Algo/PnoInstance.o Objects/Algo/PnoInstance.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/SteadyStatePno.o: Objects/Algo/SteadyStatePno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyStatePno.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyStatePno.o Objects/Algo/SteadyStatePno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PnoSwarm.o: Objects/Algo/PnoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PnoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PnoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PnoSwarm.o Objects/Algo/PnoSwarm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PpsoPno.o: Objects/Algo/PpsoPno.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PpsoPno.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PpsoPno.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PpsoPno.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PpsoPno.o.d" -o ${OBJECTDIR}/Objects/Algo/PpsoPno.o Objects/Algo/PpsoPno.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Pso.o: Objects/Algo/Pso.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Pso.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Pso.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Pso.o.d" -o ${OBJECTDIR}/Objects/Algo/Pso.o Objects/Algo/Pso.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PsoParticle.o: Objects/Algo/PsoParticle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoParticle.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoParticle.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoParticle.o Objects/Algo/PsoParticle.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/PsoSwarm.o: Objects/Algo/PsoSwarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/PsoSwarm.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/PsoSwarm.o.d" -o ${OBJECTDIR}/Objects/Algo/PsoSwarm.o Objects/Algo/PsoSwarm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Classifier.o: Objects/Algo/Classifier.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Classifier.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Classifier.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Classifier.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Classifier.o.d" -o ${OBJECTDIR}/Objects/Algo/Classifier.o Objects/Algo/Classifier.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/SteadyState.o: Objects/Algo/SteadyState.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/SteadyState.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/SteadyState.o.d" -o ${OBJECTDIR}/Objects/Algo/SteadyState.o Objects/Algo/SteadyState.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Algo/Position.o: Objects/Algo/Position.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Algo" 
	@${RM} ${OBJECTDIR}/Objects/Algo/Position.o.d 
	@${RM} ${OBJECTDIR}/Objects/Algo/Position.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Algo/Position.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Algo/Position.o.d" -o ${OBJECTDIR}/Objects/Algo/Position.o Objects/Algo/Position.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Perturb/Perturb.o: Objects/Perturb/Perturb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Perturb" 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o.d 
	@${RM} ${OBJECTDIR}/Objects/Perturb/Perturb.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Perturb/Perturb.o.d" -o ${OBJECTDIR}/Objects/Perturb/Perturb.o Objects/Perturb/Perturb.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/LedDriver.o: source/LedDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/LedDriver.o.d 
	@${RM} ${OBJECTDIR}/source/LedDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/source/LedDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/LedDriver.o.d" -o ${OBJECTDIR}/source/LedDriver.o source/LedDriver.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Codec/Codec.o: source/Codec/Codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Codec.o.d" -o ${OBJECTDIR}/source/Codec/Codec.o source/Codec/Codec.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Codec/Protocol.o: source/Codec/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source/Codec" 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o.d 
	@${RM} ${OBJECTDIR}/source/Codec/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Codec/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Codec/Protocol.o.d" -o ${OBJECTDIR}/source/Codec/Protocol.o source/Codec/Protocol.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/StateMachine.o: source/StateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/StateMachine.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateMachine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateMachine.o.d" -o ${OBJECTDIR}/source/StateMachine.o source/StateMachine.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Interrupts.o: source/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/source/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Interrupts.o.d" -o ${OBJECTDIR}/source/Interrupts.o source/Interrupts.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Setup.o: source/Setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Setup.o.d 
	@${RM} ${OBJECTDIR}/source/Setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Setup.o.d" -o ${OBJECTDIR}/source/Setup.o source/Setup.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/StateFunctions.o: source/StateFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/StateFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/StateFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/StateFunctions.o.d" -o ${OBJECTDIR}/source/StateFunctions.o source/StateFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/SkadiFunctions.o: source/SkadiFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o.d 
	@${RM} ${OBJECTDIR}/source/SkadiFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/source/SkadiFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/SkadiFunctions.o.d" -o ${OBJECTDIR}/source/SkadiFunctions.o source/SkadiFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/Potentiometer.o: source/Potentiometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o.d 
	@${RM} ${OBJECTDIR}/source/Potentiometer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Potentiometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/source/Potentiometer.o.d" -o ${OBJECTDIR}/source/Potentiometer.o source/Potentiometer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/Unit.o: Objects/Units/Unit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/Unit.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/Unit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/Unit.o.d" -o ${OBJECTDIR}/Objects/Units/Unit.o Objects/Units/Unit.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/UnitArray.o: Objects/Units/UnitArray.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitArray.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitArray.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitArray.o.d" -o ${OBJECTDIR}/Objects/Units/UnitArray.o Objects/Units/UnitArray.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Objects/Units/UnitMapping.o: Objects/Units/UnitMapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Objects/Units" 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o.d 
	@${RM} ${OBJECTDIR}/Objects/Units/UnitMapping.o 
	@${FIXDEPS} "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Objects/Units/UnitMapping.o.d" -o ${OBJECTDIR}/Objects/Units/UnitMapping.o Objects/Units/UnitMapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o: Utilities/BipBuffer/BipBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/BipBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o.d" -o ${OBJECTDIR}/Utilities/BipBuffer/BipBuffer.o Utilities/BipBuffer/BipBuffer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o: Utilities/ByteBuffer/ByteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ByteBuffer" 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o.d" -o ${OBJECTDIR}/Utilities/ByteBuffer/ByteBuffer.o Utilities/ByteBuffer/ByteBuffer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/ChainedList/LinkedList.o: Utilities/ChainedList/LinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/ChainedList" 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d 
	@${RM} ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/ChainedList/LinkedList.o.d" -o ${OBJECTDIR}/Utilities/ChainedList/LinkedList.o Utilities/ChainedList/LinkedList.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o: Utilities/MathFunctions/MathFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/MathFunctions" 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d 
	@${RM} ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o.d" -o ${OBJECTDIR}/Utilities/MathFunctions/MathFunctions.o Utilities/MathFunctions/MathFunctions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o: Utilities/PcgRng/pcg_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/pcg_basic.o Utilities/PcgRng/pcg_basic.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/Utilities/PcgRng/Rng.o: Utilities/PcgRng/Rng.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Utilities/PcgRng" 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o.d 
	@${RM} ${OBJECTDIR}/Utilities/PcgRng/Rng.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"source" -I"headers" -I"Objects/Algo" -I"Objects/Units" -I"Utilities/BipBuffer" -I"Utilities/ChainedList" -I"Utilities/PcgRng" -I"Utilities/ByteBuffer" -I"Objects/Perturb" -I"Utilities/MathFunctions" -I"source/Codec" -MMD -MF "${OBJECTDIR}/Utilities/PcgRng/Rng.o.d" -o ${OBJECTDIR}/Utilities/PcgRng/Rng.o Utilities/PcgRng/Rng.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\ChinookLib\ChinookLib.X\dist\default\debug\ChinookLib.X.a      -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=1024,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../ChinookLib/ChinookLib.X/dist/default/production/ChinookLib.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/PsoMultipleUnits.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\ChinookLib\ChinookLib.X\dist\default\production\ChinookLib.X.a      -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=1024,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
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
