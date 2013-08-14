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
#include <math.h>


//#include <initPLL.h> 


//#include <services/int/adi_int.h>
#include "processSignal.h"
#include "configADC.h"

#include "configDDS.h"
#include "configUSB.h"
#include "configXY.h"
#include "processPackets.h"
#include "global_variables.h"


// Signals
#define SIG_ALIVE_EN 	SRU(HIGH,DPI_PBEN14_I)
#define SIG_ALIVE_ON 	SRU(LOW,  DPI_PB14_I)
#define SIG_ALIVE_OFF 	SRU(HIGH,   DPI_PB14_I)

#define SIG_LED1_EN 	SRU(HIGH,DPI_PBEN07_I)
#define SIG_LED1_ON 	SRU(LOW,  DPI_PB07_I)
#define SIG_LED1_OFF 	SRU(HIGH,   DPI_PB07_I)

#define SIG_LED2_EN 	SRU(HIGH,DPI_PBEN08_I)
#define SIG_LED2_ON 	SRU(LOW,  DPI_PB08_I)
#define SIG_LED2_OFF 	SRU(HIGH,   DPI_PB08_I)

#define SIG_LED3_EN 	SRU(HIGH,DPI_PBEN09_I)
#define SIG_LED3_ON 	SRU(LOW,  DPI_PB09_I)
#define SIG_LED3_OFF 	SRU(HIGH,   DPI_PB09_I)

#define SIG_LED4_EN 	SRU(HIGH,DPI_PBEN10_I)
#define SIG_LED4_ON 	SRU(LOW,  DPI_PB10_I)
#define SIG_LED4_OFF 	SRU(HIGH,   DPI_PB10_I)

#define SIG_RUNNING_EN 		SRU(HIGH,DPI_PBEN11_I)
#define SIG_RUNNING_ON 		SRU(LOW,  DPI_PB11_I)
#define SIG_RUNNING_OFF 	SRU(HIGH,   DPI_PB11_I)

#define SIG_HOST_EN 		SRU(HIGH,DPI_PBEN12_I)
#define SIG_HOST_ON		 	SRU(LOW,  DPI_PB12_I)
#define SIG_HOST_OFF 		SRU(HIGH,   DPI_PB12_I)

#define SIG_ERROR_EN 		SRU(HIGH,DPI_PBEN12_I)
#define SIG_ERROR_ON		SRU(LOW,  DPI_PB12_I)
#define SIG_ERROR_OFF 		SRU(HIGH,   DPI_PB12_I)



#define DRIVER_OUTPUT		SRU(HIGH,DAI_PBEN03_I)
#define DRIVER_DISABLE 	SRU(HIGH,DAI_PB03_I)
#define DRIVER_ENABLE 	SRU(LOW,DAI_PB03_I)


#define TRUE 	1
#define FALSE	0


#endif


