#!/usr/bin/env bin/linux-x86_64/voltageLogger

#cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/voltageLogger.dbd"
voltageLogger_registerRecordDeviceDriver pdbbase


## Load record instances
#ld < db/calcRecord.o
dbLoadRecords("db/voltageLoggerrecords.db","SubDet=BEAST:DAQ:HE3:VOLTAGELOGGER,user=samHost")

#cd ${TOP}/iocBoot/${IOC}

#ld < db/calcRecord.o
#dbLoadDatabase("cRecord.dbd")


iocInit

## Start any sequence programs
dbpf BEAST:DAQ:HE3:VOLTAGELOGGER:INIT.A 1
dbpf BEAST:DAQ:HE3:VOLTAGELOGGER:SETPATH.A 0
dbgrep "*"
#seq sncxxx,"user=samHost"