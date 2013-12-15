/***************************************************************
	Filename:	configMUX.h (Analog Multiplexer expansion board configuration)
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		December 2013
	Version:	v1.0	
	Purpose:	Initialize the IO pinouts for the analog multiplexer 
					A0-A3 bits
				
	Usage:	
			Sets the Analog Multiplexer selection bitss
				
		Board DPI SRU assignements:
		
			
		MUX_A0	-		DPI_PB02
		MUX_A1	-		DPI_PB01
		MUX_A2	-		DPI_PB04
		MUX_A3	-		DPI_PB03
			
	
	Extra:		

***************************************************************/



#ifndef _CONFIGADC_H
#define _CONFIGADC_H


#include "..\h\general.h"


#define MUX_A0_H		SRU(HIGH,DPI_PB02_I)
#define MUX_A0_L		SRU(LOW,DAI_PB02_I)
#define MUX_A1_H		SRU(HIGH,DPI_PB01_I)
#define MUX_A1_L		SRU(LOW,DAI_PB01_I)
#define MUX_A2_H		SRU(HIGH,DPI_PB04_I)
#define MUX_A2_L		SRU(LOW,DAI_PB04_I)
#define MUX_A3_H		SRU(HIGH,DPI_PB03_I)
#define MUX_A3_L		SRU(LOW,DAI_PB03_I)

#define MUX_A0_OUTPUT		SRU(HIGH,DPI_PBEN02_I)
#define MUX_A1_OUTPUT		SRU(HIGH,DPI_PBEN02_I)
#define MUX_A2_OUTPUT		SRU(HIGH,DPI_PBEN02_I)
#define MUX_A3_OUTPUT		SRU(HIGH,DPI_PBEN02_I)

void InitMUX_IO(void);
void MUX_SetMux(char selection);



#endif