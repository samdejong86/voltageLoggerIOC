TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = voltageLogger
# voltageLogger.dbd will be created and installed
DBD += voltageLogger.dbd

# voltageLogger.dbd will be made up from these files:
voltageLogger_DBD += base.dbd
voltageLogger_DBD += voltageLoggercroutines.dbd

# Include dbd files from all support applications:
#voltageLogger_DBD += xxx.dbd

#add root libraries   					srdejong mods here

ROOTCONFIG   := root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLIBDIR   := $(shell $(ROOTCONFIG) --libdir)
ROOTLIBS     := $(shell $(ROOTCONFIG) --noldflags --noauxlibs --libs | sed s/-l//g)

USR_CXXFLAGS += -Wall -O2 $(ROOTCFLAGS)

# Add all the support libraries needed by this IOC	pof mods here
SHRLIB_SEARCH_DIRS += /usr/lib
SHRLIB_SEARCH_DIRS += $(ROOTLIBDIR)

#voltageLogger_LIBS += CAENDigitizer
voltageLogger_LIBS += $(ROOTLIBS)


voltageLogger_SYS_LIBS += usb-1.0


# voltageLogger_registerRecordDeviceDriver.cpp derives from voltageLogger.dbd
voltageLogger_SRCS += voltageLogger_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.	pof mods here
voltageLogger_SRCS_DEFAULT += voltageLoggerMain.cpp
#voltageLogger_SRCS_DEFAULT += Functions.c
#voltageLogger_SRCS_DEFAULT += keyb.c
voltageLogger_SRCS_vxWorks += -nil-

# Finally link to the EPICS Base libraries
voltageLogger_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

