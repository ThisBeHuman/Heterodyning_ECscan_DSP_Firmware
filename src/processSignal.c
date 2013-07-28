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


void Init_FIR();

/* Declaring the external buffers needed for FIR Accelerator*/
extern float memSamplesBufferChA[];
extern float memProcessedBufferChA[];

extern float memFIRcoeff[];

extern float memSamplesBufferChB[];
extern float memProcessedBufferChB[];


/*Adding the TCB for FIR channels*/
int FIR_TCB_CH2[13]={
				0,
				51,
				1,
				memFIRcoeff,
				memProcessedBufferChB,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memProcessedBufferChB+0,
				memSamplesBufferChB,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memSamplesBufferChB+0,
				(MAX_SAMPLES_BUFFER_SIZE-1)|((MAX_SAMPLES_BUFFER_SIZE-1)<<14)
			};



int FIR_TCB_CH1[13]={
				FIR_TCB_CH2+12,
				51,
				1,
				memFIRcoeff,
				memProcessedBufferChA,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memProcessedBufferChA+0,
				memSamplesBufferChA,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memSamplesBufferChA+0,
				(MAX_SAMPLES_BUFFER_SIZE-1)|((MAX_SAMPLES_BUFFER_SIZE-1)<<14)
			};


/* Adding the Initialization Code for FIR Accelerator Now */

void Init_FIR()
{

	int temp;

	FIR_TCB_CH2[0]=FIR_TCB_CH1+12;

	//Mapping the FIR DMA interrupt
	temp=*pPICR0;
	temp&=~(P5I0|P5I1|P5I2|P5I3|P5I4);
	temp|=P5I0|P5I1|P5I3|P5I4;
	*pPICR0=temp;

	//Selecting the FIR Accelerator
	temp=*pPMCTL1;
	temp&=~(BIT_17|BIT_18);
	temp|=FIRACCSEL;
	*pPMCTL1=temp;

	//PMCTL1 effect latency
		asm("nop;nop;nop;nop;");

		//Initializing the chain pointer register
	*pCPFIR=FIR_TCB_CH1+12-0x80000;

	//Now Enabling the FIR Accelerator
	*pFIRCTL1=FIR_EN|FIR_DMAEN|FIR_CH2|FIR_RND0;

}



/************************************************************
	Function:		IRQ_FIRConversion(int sig_int)
	Argument:		sig_int
	Description:	Called at FIR Accelerator completed
		
			
************************************************************/
void IRQ_FIR(int sigint)
{
//	USB_sendADCData(AR_totalSamples,(unsigned int*)memProcessedBufferChA);
//	adc_buffer_to_send = (unsigned int*)memProcessedBufferChA;
//	adc_number_of_samples_to_send = AR_totalSamples;
//	adc_send_continuous_samples = 1;
	
//	adc_sample_buffer_full = 1;
//	adc_send_continuous_samples = 1;

}



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
