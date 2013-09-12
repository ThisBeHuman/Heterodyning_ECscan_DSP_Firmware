/***************************************************************
	Filename:	configXY.h 
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		August 2013
	Version:	v1.1
	Revisions:

	Purpose:	
	Usage:
	
	Extra:		



***************************************************************/

#ifndef _CONFIGXY_H
#define _CONFIGXY_H


#include "..\h\general.h"


// IO Definitions


#define XY_MOTION_OUTPUT	SRU(HIGH,DPI_PBEN09_I)
#define XY_MOTION_DISABLE 	SRU(HIGH,DPI_PB09_I)
#define XY_MOTION_ENABLE 	SRU(LOW,DPI_PB09_I)

#define XY_SRST_OUTPUT		SRU(HIGH,DPI_PBEN02_I)
#define XY_SRST_OFF 		SRU(HIGH,  DPI_PB02_I)
#define XY_SRST_ON 			SRU(LOW,   DPI_PB02_I)

#define X_EN_OUTPUT			SRU(HIGH,DAI_PBEN07_I)
#define X_ENABLE		 	SRU(HIGH,  DAI_PB07_I)
#define X_DISABLE	 		SRU(LOW,   DAI_PB07_I)

#define X_HALFFULL_OUTPUT	SRU(HIGH,DPI_PBEN01_I)
#define X_HALF		 		SRU(HIGH,  DPI_PB01_I)
#define X_FULL			 	SRU(LOW,   DPI_PB01_I)

#define X_CWCCW_OUTPUT		SRU(HIGH,DPI_PBEN07_I)
#define X_CW		 		SRU(HIGH,  DPI_PB07_I)
#define X_CCW			 	SRU(LOW,   DPI_PB07_I)

#define X_STEP_OUTPUT		SRU(HIGH,DPI_PBEN03_I)
#define X_STEP_HIGH		 	SRU(HIGH,  DPI_PB03_I)
#define X_STEP_LOW			SRU(LOW,   DPI_PB03_I)

#define MOVE_X 	0

#define Y_EN_OUTPUT			SRU(HIGH,DAI_PBEN13_I)
#define Y_ENABLE		 	SRU(HIGH,  DAI_PB13_I)
#define Y_DISABLE	 		SRU(LOW,   DAI_PB13_I)

#define Y_HALFFULL_OUTPUT	SRU(HIGH,DPI_PBEN04_I)
#define Y_HALF		 		SRU(HIGH,  DPI_PB04_I)
#define Y_FULL			 	SRU(LOW,   DPI_PB04_I)

#define Y_CWCCW_OUTPUT		SRU(HIGH,DPI_PBEN08_I)
#define Y_CW		 		SRU(HIGH,  DPI_PB08_I)
#define Y_CCW			 	SRU(LOW,   DPI_PB08_I)

#define Y_STEP_OUTPUT		SRU(HIGH,DPI_PBEN06_I)
#define Y_STEP_HIGH		 	SRU(HIGH,  DPI_PB06_I)
#define Y_STEP_LOW			SRU(LOW,   DPI_PB06_I)

#define MOVE_Y	1


#define MOVE_XY_CLK_DELAY 	500
#define MOVE_X_DELAY 	1200000
#define MOVE_Y_DELAY 	400000

// Modes of operation definition

#define MODE_HALF_STEP	1
#define MODE_FULL_STEP	0
#define MODE_CW			1
#define MODE_CCW		0


#endif