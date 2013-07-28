/***************************************************************
	Filename:	general.h (general project includes and defines)
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.1
	Revisions:
				1.0 April 2013 - Prototype Definitions
				1.1	July 2013 - Update to final board with adsp21489
	Purpose:	General and global definitions and project 
		specific includes to be shared by all source files.
			
	Usage:
	
	Extra:		
		
***************************************************************/

#ifndef _GENERAL_H_
#define _GENERAL_H_



#include <Cdef21489.h>		// This board!
#include <def21489.h>


#include <stdio.h>
#include <stdbool.h>

#include <sysreg.h>
#include <signal.h>
#include <sru.h>

//#include <initPLL.h> 


//#include <services/int/adi_int.h>

#include "configADC.h"

#include "configDDS.h"
#include "configUSB.h"
#include "processPackets.h"

// Signals

#define SIG_ALIVE_ON 	SRU(HIGH,DPI_PB14_I)
#define SIG_ALIVE_OFF 	SRU(LOW,DPI_PB14_I)

#define DRIVER_DISABLE 	SRU(HIGH,DAI_PB03_I)
#define DRIVER_ENABLE 	SRU(LOW,DAI_PB03_I)


#define TRUE 	1
#define FALSE	0


#endif


