readme.txt for 21489EzFlashDriver_Serial.dpj

ADSP-21489 EZ-Board Flash Programmer Interface Application
for Atmel M25P16 serial flash.

Date Modified: November 17, 2009

____________________________________________________________________

Contents

I.    Overview
II.   Required Hardware
III.  Operation Description
IV.   Deliverables

_____________________________________________________________________

I. Overview

The flash programmer interface application (FPIA) implements the interface
between the VDSP++ Flash Programmer utility and the flash driver.

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
    ¦           +---Serial
    ¦		       21489EzFlashDriver_Serial.dpj
    ¦		       21489EzFlashDriver_Serial.dxe
    ¦		       main.c
    ¦		       readme.txt
    +---include
    |   +---drivers
    |       +---flash
    |           adi_flash.h
    |           m25p16.h
    |           Errors.h
    |           util.h
    +---lib
    	+---src
    	    +---drivers
    	        +---flash
    	            +---M25P16
                            m25p16.c

