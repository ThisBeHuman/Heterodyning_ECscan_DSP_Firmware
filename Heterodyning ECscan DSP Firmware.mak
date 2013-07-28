# Generated by the VisualDSP++ IDDE

# Note:  Any changes made to this Makefile will be lost the next time the
# matching project file is loaded into the IDDE.  If you wish to preserve
# changes, rename this file and run it externally to the IDDE.

# The syntax of this Makefile is such that GNU Make v3.77 or higher is
# required.

# The current working directory should be the directory in which this
# Makefile resides.

# Supported targets:
#     Heterodyning ECscan DSP Firmware_Debug
#     Heterodyning ECscan DSP Firmware_Debug_clean

# Define this variable if you wish to run this Makefile on a host
# other than the host that created it and VisualDSP++ may be installed
# in a different directory.

ADI_DSP=C:\Program Files (x86)\Analog Devices\VisualDSP 5.0


# $VDSP is a gmake-friendly version of ADI_DIR

empty:=
space:= $(empty) $(empty)
VDSP_INTERMEDIATE=$(subst \,/,$(ADI_DSP))
VDSP=$(subst $(space),\$(space),$(VDSP_INTERMEDIATE))

RM=cmd /C del /F /Q

#
# Begin "Heterodyning ECscan DSP Firmware_Debug" configuration
#

ifeq ($(MAKECMDGOALS),Heterodyning ECscan DSP Firmware_Debug)

Heterodyning\ ECscan\ DSP\ Firmware_Debug : ./Debug/Heterodyning\ ECscan\ DSP\ Firmware.dxe 

Debug/configADC.doj :src/configADC.c h/configADC.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configDDS.h h/configUSB.h h/processPackets.h h/global_variables.h h/processSignal.h 
	@echo ".\src\configADC.c"
	$(VDSP)/cc21k.exe -c .\src\configADC.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\configADC.doj -MM

Debug/configDDS.doj :src/configDDS.c h/configDDS.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configUSB.h h/processPackets.h h/global_variables.h h/processSignal.h 
	@echo ".\src\configDDS.c"
	$(VDSP)/cc21k.exe -c .\src\configDDS.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\configDDS.doj -MM

Debug/configUSB.doj :src/configUSB.c h/configUSB.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configDDS.h h/processPackets.h h/global_variables.h h/processSignal.h 
	@echo ".\src\configUSB.c"
	$(VDSP)/cc21k.exe -c .\src\configUSB.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\configUSB.doj -MM

Debug/global_variables.doj :src/global_variables.c h/global_variables.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configDDS.h h/configUSB.h h/processPackets.h h/processSignal.h 
	@echo ".\src\global_variables.c"
	$(VDSP)/cc21k.exe -c .\src\global_variables.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\global_variables.doj -MM

Debug/Heterodyning\ ECscan\ DSP\ Firmware.doj :Heterodyning\ ECscan\ DSP\ Firmware.c h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configDDS.h h/configUSB.h h/processPackets.h h/global_variables.h h/processSignal.h 
	@echo ".\Heterodyning ECscan DSP Firmware.c"
	$(VDSP)/cc21k.exe -c .\Heterodyning\ ECscan\ DSP\ Firmware.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\Heterodyning\ ECscan\ DSP\ Firmware.doj -MM

Debug/initPLL_SDRAM.doj :src/initPLL_SDRAM.c $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/cdef21489.h 
	@echo ".\src\initPLL_SDRAM.c"
	$(VDSP)/cc21k.exe -c .\src\initPLL_SDRAM.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\initPLL_SDRAM.doj -MM

Debug/processPackets.doj :src/processPackets.c h/processPackets.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configDDS.h h/configUSB.h h/global_variables.h h/processSignal.h 
	@echo ".\src\processPackets.c"
	$(VDSP)/cc21k.exe -c .\src\processPackets.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\processPackets.doj -MM

Debug/processSignal.doj :src/processSignal.c h/processSignal.h h/general.h $(VDSP)/214xx/include/Cdef21489.h $(VDSP)/214xx/include/def21489.h $(VDSP)/214xx/include/stdio.h $(VDSP)/214xx/include/stdio_21xxx.h $(VDSP)/214xx/include/stdbool.h $(VDSP)/214xx/include/yvals.h $(VDSP)/214xx/include/sysreg.h $(VDSP)/214xx/include/signal.h $(VDSP)/214xx/include/sru.h $(VDSP)/214xx/include/sru21489.h $(VDSP)/214xx/include/math.h $(VDSP)/214xx/include/math_21xxx.h h/configADC.h h/configDDS.h h/configUSB.h h/processPackets.h h/global_variables.h 
	@echo ".\src\processSignal.c"
	$(VDSP)/cc21k.exe -c .\src\processSignal.c -file-attr ProjectName=Heterodyning\ ECscan\ DSP\ Firmware -g -structs-do-not-overlap -no-multiline -double-size-32 -swc -warn-protos -si-revision 0.2 -proc ADSP-21489 -o .\Debug\processSignal.doj -MM

./Debug/Heterodyning\ ECscan\ DSP\ Firmware.dxe :./Heterodyning\ ECscan\ DSP\ Firmware.ldf $(VDSP)/214xx/lib/21479_rev_any/21489_hdr.doj ./Debug/configADC.doj ./Debug/configDDS.doj ./Debug/configUSB.doj ./Debug/global_variables.doj ./Debug/Heterodyning\ ECscan\ DSP\ Firmware.doj ./Debug/initPLL_SDRAM.doj ./Debug/processPackets.doj ./Debug/processSignal.doj $(VDSP)/214xx/lib/21479_rev_any/libc.dlb $(VDSP)/214xx/lib/21479_rev_any/libio.dlb $(VDSP)/214xx/lib/21479_rev_any/libcpp.dlb $(VDSP)/214xx/lib/21479_rev_any/libdsp.dlb 
	@echo "Linking..."
	$(VDSP)/cc21k.exe .\Debug\configADC.doj .\Debug\configDDS.doj .\Debug\configUSB.doj .\Debug\global_variables.doj .\Debug\Heterodyning\ ECscan\ DSP\ Firmware.doj .\Debug\initPLL_SDRAM.doj .\Debug\processPackets.doj .\Debug\processSignal.doj -T .\Heterodyning\ ECscan\ DSP\ Firmware.ldf -map .\Debug\Heterodyning\ ECscan\ DSP\ Firmware.map.xml -flags-link -ip -L .\Debug -add-debug-libpaths -swc -flags-link -od,.\Debug -o .\Debug\Heterodyning\ ECscan\ DSP\ Firmware.dxe -proc ADSP-21489 -si-revision 0.2 -flags-link -MM

endif

ifeq ($(MAKECMDGOALS),Heterodyning ECscan DSP Firmware_Debug_clean)

Heterodyning\ ECscan\ DSP\ Firmware_Debug_clean:
	-$(RM) "Debug\configADC.doj"
	-$(RM) "Debug\configDDS.doj"
	-$(RM) "Debug\configUSB.doj"
	-$(RM) "Debug\global_variables.doj"
	-$(RM) "Debug\Heterodyning ECscan DSP Firmware.doj"
	-$(RM) "Debug\initPLL_SDRAM.doj"
	-$(RM) "Debug\processPackets.doj"
	-$(RM) "Debug\processSignal.doj"
	-$(RM) ".\Debug\Heterodyning ECscan DSP Firmware.dxe"
	-$(RM) ".\Debug\*.ipa"
	-$(RM) ".\Debug\*.opa"
	-$(RM) ".\Debug\*.ti"
	-$(RM) ".\Debug\*.pgi"
	-$(RM) ".\*.rbld"

endif


