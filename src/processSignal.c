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





/************************************************************
	Function:	int signalIIR_bandpassfilter (float* sampleA_ptr,float* sampleB_ptr)
	Argument:	Pointer to current samples in the sample buffer
	
	Return:	
	
	Description: Processes samples A and B through the band pass filter.
		
	Extra:	

************************************************************/
int signalIIR_bandpassfilter(float* sampleA_ptr,float* sampleB_ptr)
{

//	*sampleA_ptr = iir(*sampleA_ptr, BP_ACoeffs, BP_BCoeffs, IIR_BPstatesChA, TAPS_IIR);
//	*sampleB_ptr = iir(*sampleB_ptr, BP_ACoeffs, BP_BCoeffs, IIR_BPstatesChB, TAPS_IIR);

	*sampleA_ptr = fir(*sampleA_ptr, BP_FIR_coeffs, FIR_BPstatesChA, TAPS_FIR);
	*sampleB_ptr = fir(*sampleB_ptr, BP_FIR_coeffs, FIR_BPstatesChB, TAPS_FIR);
	
	return 0;	
}

/************************************************************
	Function:	int signalIIR_lowpassfilter (float* sampleA_ptr,float* sampleB_ptr)
	Argument:	Pointer to current samples in the sample buffer
	
	Return:	
	
	Description: Processes samples A and B through the low pass filter.
		
	Extra:	

************************************************************/
int signalIIR_lowpassfilter(float* sampleA_ptr,float* sampleB_ptr)
{

//	*sampleA_ptr = iir(*sampleA_ptr, LP_ACoeffs, LP_BCoeffs, IIR_LPstatesChA, TAPS_IIR);
//	*sampleB_ptr = iir(*sampleB_ptr, LP_ACoeffs, LP_BCoeffs, IIR_LPstatesChB, TAPS_IIR);

	*sampleA_ptr = fir(*sampleA_ptr, LP_FIR_coeffs, FIR_LPstatesChA, TAPS_FIR_LP);
	*sampleB_ptr = fir(*sampleB_ptr, LP_FIR_coeffs, FIR_LPstatesChB, TAPS_FIR_LP);

	
	return 0;	
}

/************************************************************
	Function:	int signal_QuadratureDemodulation_InternalLO (float* bufferA,float* bufferB, int total_samples)
	Argument:	
	
	Return:	
	
	Description: Processes the quadrature demodulation of the IF mode using an internal local oscillator
		and the samples from channel A corresponding to the probe response data.
		
	Extra:	

************************************************************/
int signal_QuadratureDemodulation_InternalLO (float* bufferA,float* bufferB, int total_samples)
{

	int index = 0;
	float sampleA;
	float sampleB;
	float sampleC;
//	static float aux[MAX_SAMPLES_BUFFER_SIZE];

	// internal local oscillator incrementation
	unsigned int inc_ilo = (DDS_inc_Fex - DDS_inc_Flo)*1200;// >> 20;
	
	unsigned int inc_ilo_accA = 0;
//	unsigned int inc_ilo_accB = SINE_VALUES_SIZE/4;
	
	for (index = 0; index<total_samples;index++){
		sampleA = bufferA[index];

		// Sample * Sine
		bufferB[index] = 4*sampleA*sine_values_lut[((inc_ilo_accA>>20))];

//		bufferB[index] = 1*sine_values_lut[((inc_ilo_accA>>20)%SINE_VALUES_SIZE)];


		// Sample * CoSine
		bufferA[index] = 4*sampleA*sine_values_lut[((inc_ilo_accA>>20)+SINE_VALUES_90_DELAY)%SINE_VALUES_SIZE];

		inc_ilo_accA = inc_ilo_accA + inc_ilo;

		//inc_ilo_accB = ((inc_ilo_accA + inc_ilo))+SINE_VALUES_SIZE/4)%SINE_VALUES_SIZE;
	//	printf("inc: %d\n", inc_ilo_accA>>20);	
	}
	
	for (index = 0; index < total_samples; index++){
		signalIIR_lowpassfilter(&bufferA[index], &bufferB[index]);

	}	
	
	return 0;	
}



/************************************************************
	Function:	int signal_QuadratureDemodulation (float* bufferA,float* bufferB, int total_samples)
	Argument:	
	
	Return:	
	
	Description: Processes the quadrature demodulation of the IF mode.
		
	Extra:	

************************************************************/
int signal_QuadratureDemodulation (float* bufferA,float* bufferB, int total_samples)
{

	int index = 0;
	float sampleA;
	float sampleB;
	float sampleC;
	static float aux[MAX_SAMPLES_BUFFER_SIZE];

	
//	Init_IIR_LPsoft();

	for (index = 0; index<total_samples;index++){
		aux[index]=bufferB[(total_samples+index-SWEEP_SAMPLES_90DEGREE)%total_samples];
		
	}
	for (index = 0; index < total_samples; index++){
		sampleA = bufferA[index];
		sampleB = bufferB[index];
		sampleC = aux[index];
		bufferA[index]=sampleA*sampleB;
		bufferB[index]=sampleA*sampleC;
		
		
		
		
	//	signalIIR_lowpassfilter(&bufferA[index],&bufferB[index]);
	//	bufferA[index]=iir(sampleA*sampleB, LP_ACoeffs, LP_BCoeffs, IIR_LPstatesChA, TAPS_IIR);		
	//	bufferB[index]=iir(sampleA*sampleC, LP_ACoeffs, LP_BCoeffs, IIR_LPstatesChB, TAPS_IIR);		
	//	bufferB[index]=sampleA*sampleC;
	}
	for (index = 0; index < total_samples; index++){
		signalIIR_lowpassfilter(&bufferB[index],&bufferA[index]);

//		bufferB[index]=fir(bufferA[index], LP_FIR_coeffs, FIR_LPstatesChB, TAPS_FIR_LP);
//		bufferA[index]=fir(bufferB[index], LP_FIR_coeffs, FIR_LPstatesChA, TAPS_FIR_LP);
	}
	return 0;	
}

/************************************************************
	Function:	int signal_Calibrate (float* bufferA,float* bufferB, int total_samples)
	Argument:	
	
	Return:	
	
	Description: Calibrates buffer A and buffer B with the calibration
			values stored in memory
		
	Extra:	

************************************************************/
int signal_Calibrate (float* bufferA,float* bufferB, int total_samples)
{
	int index=0;
	
	for(index=0;index<total_samples;index++){
		bufferA[index]=bufferA[index]-CAL_chA_calibration;
		bufferB[index]=bufferB[index]-CAL_chB_calibration;

	}

	
	return 0;
}

int Init_IIR_LPsoft(){
	
	int i;
	for (i=0; i<TAPS_IIR+1; i++) {
		IIR_LPstatesChA[i] = 0.0;
		IIR_LPstatesChB[i] = 0.0;
	}
}



int Init_IIR_BPsoft(){
	
	int i;
	for (i=0; i<TAPS_IIR+1; i++) {
		IIR_BPstatesChA[i] = 0.0;
		IIR_BPstatesChB[i] = 0.0;
	}
}

int Init_FIR_BPsoft(){
	
	int i;
	for (i=0; i<TAPS_FIR+1; i++) {
		FIR_BPstatesChA[i] = 0.0;
		FIR_BPstatesChB[i] = 0.0;
	}
}

/*Adding the TCB for FIR channels
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

*/

/*Adding the TCB for IIR channels
int IIR_TCB_CH2[13]={
				0,
				3,
				1,
				memIIRcoeff,
				memProcessedBufferChB,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memProcessedBufferChB+0,
				memSamplesBufferChB,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memSamplesBufferChB+0,
				(0)|((MAX_SAMPLES_BUFFER_SIZE-1)<<14)
			};



int IIR_TCB_CH1[13]={
				IIR_TCB_CH2+12,
				3,
				1,
				memIIRcoeff,
				memProcessedBufferChA,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memProcessedBufferChA+0,
				memSamplesBufferChA,
				MAX_SAMPLES_BUFFER_SIZE,
				1,
				memSamplesBufferChA+0,
				(0)|((MAX_SAMPLES_BUFFER_SIZE-1)<<14)
			};


			
		*/	
			
/* Adding the Initialization Code for FIR Accelerator Now */

void Init_FIR(int window_size)
{

	int temp;
	int timeout = 1000000;
	
	
	//while(DSP_processingFIR&&timeout--);
	while(DSP_processingFIR);

/*
	while(1){
		temp = *pFIRDMASTAT;
		if(temp & FIR_DMAACDONE){
			break;
		}
	}
*/

//$!	FIR_TCB_CH2[0]=FIR_TCB_CH1+12;
	
/*	FIR_TCB_CH1[5] = window_size;
	FIR_TCB_CH1[9] = window_size;
	FIR_TCB_CH1[12] = (window_size-1)|((window_size-1)<<14);
	FIR_TCB_CH2[5] = window_size;
	FIR_TCB_CH2[9] = window_size;
	FIR_TCB_CH2[12] = (window_size-1)|((window_size-1)<<14);
*/
	//Mapping the FIR DMA interrupt
	temp=*pPICR0;
	temp&=~(P5I0|P5I1|P5I2|P5I3|P5I4);
	temp|=P5I0|P5I1|P5I3|P5I4;
	*pPICR0=temp;

	interrupt(SIG_P5,IRQ_FIR);
	
	//Selecting the FIR Accelerator
	temp=*pPMCTL1;
	temp&=~(BIT_17|BIT_18);
	temp|=FIRACCSEL;
	*pPMCTL1=temp;
	
	//PMCTL1 effect latency
		asm("nop;nop;nop;nop;");

		//Initializing the chain pointer register
//$!	*pCPFIR=FIR_TCB_CH1+12-0x80000;

	// Confirms there is no FIR processing in accelerator
//	while(DSP_processingFIR==0);
	
	// Must clear DMAEN to reload new data on the buffers to the DMA
	*pFIRCTL1&=~(FIR_DMAEN);//|FIR_EN);

	//Now Enabling the FIR Accelerator
	*pFIRCTL1=FIR_EN|FIR_DMAEN|FIR_CH2|FIR_RND0;
	
	DSP_processingFIR = TRUE;
//	SIG_LED2_ON;

}

/* Adding the Initialization Code for FIR Accelerator Now */

void Init_IIR(int window_size)
{

	int temp;
	int timeout = 1000000;
	
	
	//while(DSP_processingFIR&&timeout--);
//	while(DSP_processingFIR);

/*
	while(1){
		temp = *pFIRDMASTAT;
		if(temp & FIR_DMAACDONE){
			break;
		}
	}
*/

//$!	IIR_TCB_CH2[0]=IIR_TCB_CH1+12;
	
/*	FIR_TCB_CH1[5] = window_size;
	FIR_TCB_CH1[9] = window_size;
	FIR_TCB_CH1[12] = (window_size-1)|((window_size-1)<<14);
	FIR_TCB_CH2[5] = window_size;
	FIR_TCB_CH2[9] = window_size;
	FIR_TCB_CH2[12] = (window_size-1)|((window_size-1)<<14);
*/
	//Mapping the FIR DMA interrupt
	temp=*pPICR0;
	temp&=~(P5I0|P5I1|P5I2|P5I3|P5I4);
	temp|=P5I0|P5I1|P5I3|P5I4;
	*pPICR0=temp;

	interrupt(SIG_P5,IRQ_FIR);
	
	//Selecting the FIR Accelerator
	temp=*pPMCTL1;
	temp&=~(BIT_17|BIT_18);
	temp|=IIRACCSEL;
	*pPMCTL1=temp;
	
	//PMCTL1 effect latency
		asm("nop;nop;nop;nop;");

		//Initializing the chain pointer register
//$!	*pCPFIR=IIR_TCB_CH1+12-0x80000;

	// Confirms there is no FIR processing in accelerator
//	while(DSP_processingFIR==0);
	
	// Must clear DMAEN to reload new data on the buffers to the DMA
	*pFIRCTL1&=~(IIR_DMAEN);//|FIR_EN);

	//Now Enabling the FIR Accelerator
	*pFIRCTL1=IIR_EN|FIR_DMAEN|IIR_CH2|IIR_RND0;
	
	DSP_processingFIR = TRUE;
//	SIG_LED2_ON;

}



/************************************************************
	Function:		IRQ_FIRConversion(int sig_int)
	Argument:		sig_int
	Description:	Called at FIR Accelerator completed
		
			
************************************************************/
void IRQ_FIR()
{
	

	
//	USB_sendADCData(AR_totalSamples,(unsigned int*)memProcessedBufferChA);
//	adc_buffer_to_send = (unsigned int*)memProcessedBufferChA;
//	adc_number_of_samples_to_send = AR_totalSamples;
//	adc_send_continuous_samples = 1;
//	SIG_LED2_OFF;
//	adc_sample_buffer_full = 1;
//	adc_send_continuous_samples = 1;
	DSP_processingFIR = FALSE;
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
