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
unsigned int *AR_buffer=memSamplesBuffer1;
float *AR_bufferChA=memSamplesBufferChA;
float *AR_bufferChB=memSamplesBufferChB;

unsigned char AR_continuousSampling=0;


bool DSP_processingFIR =0;


unsigned int * current_memSamplesBuffer = memSamplesBuffer1;
unsigned int current_memSamplesBuffer_index = 0;

float CAL_chA_calibration=0;
float CAL_chB_calibration=0;
bool 	CAL_calibrateFlag=0;

#pragma section("seg_pmda") 
unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

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
