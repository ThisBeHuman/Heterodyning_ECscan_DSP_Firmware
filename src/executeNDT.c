/***************************************************************
	Filename:	executeNDT.c 
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	
	Dependecies:	executeNDT.h
					
	Purpose:	Non-Destructive Testing execution algorithms.
		Includes functions to execute multiple tests:
			Single Frequency
			Swept Frequency
			XY Motion Control
			
	Usage:	

***************************************************************/


#include "../h/executeNDT.h"

/**************************************************************
			EXTERNAL Execution GLOBAL VARIABLES
***************************************************************/



/**************************************************************
			LOCAL Execution GLOBAL VARIABLES
***************************************************************/




/************************************************************
	Function:	int NDT_SingleFrequency( int frequency, char phase, short gain, char current_scale,
							int sample_size)

	Argument:	unsigned int buffer_size - Size of buffer with samples to process
				unsigned int * samples_buffer - Buffer with samples to process
	
	Return:		Updates the processed_buffer buffer with the processed
			samples
			
			
	Description: Executes a single frequency ND Testing.
	
	Extra:	
		1. Configure the DDS according to the frequency and phase
		2. Set Gain
		3. Set Current Scale
		4. Enable Driver
		5. Acquire "sample_size" samples
		
		6. Calculate Phase
	
		Amplitude = sqrt(I^2+Q^2)
		Phase = arctan(Q/I)
		
************************************************************/
int NDT_SingleFrequency( int driving_frequency, int intermediate_frequency, short gain, char current_scale,
					 int sample_size)
{
	
	
	
	// 1. Configure DDS
	DDS_init();
	DDS_init();

	DDS_WriteData(driving_frequency, DDS_PHASE_0, 0, DDS_ch1);
	DDS_WriteData(driving_frequency-intermediate_frequency, DDS_PHASE_0, 0, DDS_ch2);
	DDS_WriteData(driving_frequency-intermediate_frequency, DDS_PHASE_90, 0, DDS_ch3);
	
	DDS_update_frequency();	
	
	// 2. Set Gain
	GAIN_set_voltage(gain&0x0fff,GAIN_PD_ON);
	
	// 3. Set Current Scale
	DDS_current_scale(current_scale);
	
	// 4. Enable Driver
//	DRIVER_ENABLE;
	
	
	// 5. Acquire Sample Size 
	
	
	// 6. Disable Driver
//	DRIVER_DISABLE;
	
	
	return TRUE;	
}
