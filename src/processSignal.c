/***************************************************************
	Filename:	processSignal.c Digital Signal processing
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	
	Dependecies:	processSignal.h
					
	Purpose:	Digital Signal Processing functions
			
	Usage:	

***************************************************************/


#include "../h/processSignal.h"

/**************************************************************
			EXTERNAL Signal GLOBAL VARIABLES
***************************************************************/



/**************************************************************
			LOCAL Signal GLOBAL VARIABLES
***************************************************************/




/************************************************************
	Function:	int DSP_ModeIQ_AmplitudePhase (unsigned int buffer_size, unsigned int * samples_buffer, unsigned int * processed_buffer)
	Argument:	unsigned int buffer_size - Size of buffer with samples to process
				unsigned int * samples_buffer - Buffer with samples to process
	
	Return:		Updates the processed_buffer buffer with the processed
			samples
			
			
	Description: Processes the data in the buffer assuming it is
		already in IQ format at DC frequency.
	
	Extra:	
		1. Separate ChA samples from ChB
		2. Convert to float
		3. Subtract corresponding calibration
		4. Calculate Amplitude
		5. Calculate Phase
	
		Amplitude = sqrt(I^2+Q^2)
		Phase = arctan(Q/I)
		
************************************************************/
int DSP_ModeIQ_AmplitudePhase(unsigned int buffer_size, unsigned int * samples_buffer,
								 float * buffer_amplitude, float * buffer_phase)
{
	int index;
	float sample_chA, sample_chB;
	
	float amplitude, phase;
	//unsigned short * samples_buffer16 = (unsigned short *) samples_buffer;
//	a1 = ((k>>16)&0xffff)*2.5/65536;
//	a2 = (k&0xffff)*2.5/65536;	
	for(index = 0; index < buffer_size; index++)
	{
		// 1 and 2
		sample_chA = (float) ((samples_buffer[index]&0xffff)-CAL_chA_calibration)*2.5/65536;
		sample_chB = (float) (((samples_buffer[index]>>16)&0xffff)- CAL_chB_calibration)*2.5/65536;
		
		// 3.
	//	sample_chA = sample_chA - CAL_chA_calibration;
	//	sample_chB = sample_chB - CAL_chB_calibration;
		
		// 4.
		amplitude = sample_chA*sample_chA+sample_chB*sample_chB;
		amplitude = sqrtf(amplitude);
		
		//5. 
		phase = sample_chB/sample_chA;
		phase = atanf(phase);
		
		buffer_amplitude[index] = amplitude;
		buffer_phase[index] = phase;
		
		
		
	}
	
	return TRUE;
	
	
}
