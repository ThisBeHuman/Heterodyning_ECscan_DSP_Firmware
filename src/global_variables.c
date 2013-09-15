/***************************************************************
	Filename:	global_variables.c
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	Revisions:
				1.0	July 2013
	Purpose:	
	Usage:
	
	Extra:		



***************************************************************/

#include "../h/global_variables.h"

//  Acquisition Run - AR

bool AR_finishedFlag = 0;
bool AR_finishedFIR = 0;
unsigned int AR_totalSamples;
unsigned int AR_bufferIndex=0;
//unsigned int *AR_buffer=memSamplesBuffer1;
float *AR_bufferChA=memSamplesBufferChA;
float *AR_bufferChB=memSamplesBufferChB;

unsigned char AR_continuousSampling=0;
char OpMode = MODE_IQ;

bool DSP_processingFIR =0;


//unsigned int * current_memSamplesBuffer = memSamplesBuffer1;
unsigned int current_memSamplesBuffer_index = 0;

float CAL_chA_calibration=0;
float CAL_chB_calibration=0;
bool 	CAL_calibrateFlag=0;

float dm IIR_BPstatesChA[TAPS_IIR+1];
float dm IIR_BPstatesChB[TAPS_IIR+1];
float dm IIR_LPstatesChA[TAPS_IIR+1];
float dm IIR_LPstatesChB[TAPS_IIR+1];

float pm BP_ACoeffs[TAPS_IIR] =
{
	#include "iir_bp1khz_acoeffs.dat"
};
float  pm BP_BCoeffs[TAPS_IIR+1] =
{
	#include "iir_bp1khz_bcoeffs.dat"
};


float pm LP_ACoeffs[TAPS_IIR] =
{
	#include "iir_lp1hz_acoeffs.dat"
};
float  pm LP_BCoeffs[TAPS_IIR+1] =
{
	#include "iir_lp1hz_bcoeffs.dat"
};

float pm BP_FIR_coeffs[TAPS_FIR] =
{
	#include "fir_coeff.dat"
};
	
float dm FIR_BPstatesChA[TAPS_FIR];
float dm FIR_BPstatesChB[TAPS_FIR];

float pm LP_FIR_coeffs[TAPS_FIR_LP] =
{
	#include "fir_coeff_LP.dat"
};
	
float dm FIR_LPstatesChA[TAPS_FIR_LP];
float dm FIR_LPstatesChB[TAPS_FIR_LP];


float  pm IIR_coeffs[2*TAPS_IIR] =
{
	#include "iir_coeffs.dat"
};


#pragma section("seg_pmda") 
//unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
//unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

float memSamplesBufferChA[MAX_SAMPLES_BUFFER_SIZE];
float memSamplesBufferChB[MAX_SAMPLES_BUFFER_SIZE];


float memFIRcoeff[] = { 
	#include "fir_coeff1s.dat"
	};

float memIIRcoeff[] = { 
	#include "iir_coeff.dat"
	};
	
	


	
	
float memProcessedBufferChA[MAX_SAMPLES_BUFFER_SIZE];
float memProcessedBufferChB[MAX_SAMPLES_BUFFER_SIZE];

unsigned char USB_PAYLOAD_BUFFER[USB_MAX_PAYLOAD_SIZE];
unsigned char USB_ACK_BUFFER[USB_MAX_ACK_SIZE];





//float memDSPBufferAmplitude[MAX_SAMPLES_BUFFER_SIZE];
//float memDSPBufferPhase[MAX_SAMPLES_BUFFER_SIZE];
