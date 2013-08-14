readme.txt for 21489EzFlashDriver_Parallel.dpj

ADSP-21489 EZ-Board Flash Programmer Interface Application
for ST M29W320EB parallel flash.

Date Modified: November 17, 2009

_____________________________________________________________________

Contents

I.    Overview
II.   Required Hardware
III.  Operation Description
IV.   Deliverables

_____________________________________________________________________

I. Overview

The flash programmer interface application (FPIA) implements the interface
between the VDSP++ Flash Programmer utility and the flash driver.

* Note that the driver uses software protection if LOCK_CONFIG_FLASH_SECTOR is
defined.  This will protect sector 70 from being erased or programmed since
this is where configuration data for the EZ-Board is stored.  If you use
a custom driver or remove this protection you may lose the data that is
programmed in the configuration sector.

* Note that rev 0.1 was configured for 16-bit mode while rev 0.2 was changed
to 8-bit mode.  In both cases only 8-bit data is available.

_____________________________________________________________________

II. Required Hardware 

1) SHARC 21489 EZ-Board rev 0.1 or later
2) HPPCI-ICE, HPUSB-ICE, or USB stand-alone debug agent board
3) Standard-A to mini-B USB cable if using the USB stand-alone USB debug agent

_____________________________________________________________________

III. Operation Description

1)  Start the IDDE.
2)  Create and/or select a 21489 session.
3)  Open the FPIA project (dpj) file.
4)  Build and load the FPIA.  Note, FPIAs must be built with symbolic
    information and the symbols must be globally defined.
5)  Go to the "Tools" menu and choose "Flash Programmer...".
6)  On the "Driver" tab set "Driver file" to the FPIA executable (dxe).  You may
    use the "Browse..." button to locate the dxe on your machine.
7)  Click "Load Driver" to load the FPIA.
8)  You are now ready to perform flash operations.

_____________________________________________________________________

IV. Deliverables

All the flash driver examples ship pre-built but you can rebuild them
yourself.  The source files for both the FPIA and the flash driver
are included in the directory structure shown below:

+---214xx
    +---Examples
    ¦   +---ADSP-21489 EZ-Board
    ¦       +---Flash Programmer 	
    ¦           +---Parallel
    ¦		       21489EzFlashDriver_Parallel.dpj
    ¦		       21489EzFlashDriver_Parallel.dxe
    ¦		       main.c
    ¦		       readme.txt
    +---include
    |   +---drivers
    |       +---flash
    |           adi_flash.h
    |           m29w320.h
    |           Errors.h
    |           util.h
    +---lib
    	+---src
    	    +---drivers
    	        +---flash
    	            +---M29W320
                            m29w320.c
