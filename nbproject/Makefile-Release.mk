#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-eabi-gcc
CCC=arm-none-eabi-c++
CXX=arm-none-eabi-c++
FC=gfortran
AS=arm-none-eabi-ld

# Macros
CND_PLATFORM=GNU_STM32-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/drivers/src/misc.o \
	${OBJECTDIR}/src/drivers/src/stm32f4xx_flash.o \
	${OBJECTDIR}/src/drivers/src/stm32f4xx_gpio.o \
	${OBJECTDIR}/src/drivers/src/stm32f4xx_rcc.o \
	${OBJECTDIR}/src/drivers/src/stm32f4xx_syscfg.o \
	${OBJECTDIR}/src/drivers/src/stm32f4xx_usart.o \
	${OBJECTDIR}/src/system/startup.o \
	${OBJECTDIR}/src/system/system.o \
	${OBJECTDIR}/src/user/main.o \
	${OBJECTDIR}/src/user/setup.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32f407vg_1.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32f407vg_1.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32f407vg_1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/drivers/src/misc.o: src/drivers/src/misc.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/misc.o src/drivers/src/misc.c

${OBJECTDIR}/src/drivers/src/stm32f4xx_flash.o: src/drivers/src/stm32f4xx_flash.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/stm32f4xx_flash.o src/drivers/src/stm32f4xx_flash.c

${OBJECTDIR}/src/drivers/src/stm32f4xx_gpio.o: src/drivers/src/stm32f4xx_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/stm32f4xx_gpio.o src/drivers/src/stm32f4xx_gpio.c

${OBJECTDIR}/src/drivers/src/stm32f4xx_rcc.o: src/drivers/src/stm32f4xx_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/stm32f4xx_rcc.o src/drivers/src/stm32f4xx_rcc.c

${OBJECTDIR}/src/drivers/src/stm32f4xx_syscfg.o: src/drivers/src/stm32f4xx_syscfg.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/stm32f4xx_syscfg.o src/drivers/src/stm32f4xx_syscfg.c

${OBJECTDIR}/src/drivers/src/stm32f4xx_usart.o: src/drivers/src/stm32f4xx_usart.c 
	${MKDIR} -p ${OBJECTDIR}/src/drivers/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/drivers/src/stm32f4xx_usart.o src/drivers/src/stm32f4xx_usart.c

${OBJECTDIR}/src/system/startup.o: src/system/startup.s 
	${MKDIR} -p ${OBJECTDIR}/src/system
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/system/startup.o src/system/startup.s

${OBJECTDIR}/src/system/system.o: src/system/system.c 
	${MKDIR} -p ${OBJECTDIR}/src/system
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/system/system.o src/system/system.c

${OBJECTDIR}/src/user/main.o: src/user/main.c 
	${MKDIR} -p ${OBJECTDIR}/src/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/user/main.o src/user/main.c

${OBJECTDIR}/src/user/setup.o: src/user/setup.c 
	${MKDIR} -p ${OBJECTDIR}/src/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/user/setup.o src/user/setup.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32f407vg_1.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
