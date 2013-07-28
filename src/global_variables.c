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

unsigned int AR_totalSamples;
unsigned int AR_bufferIndex=0;
unsigned int *AR_buffer=memSamplesBuffer1;
unsigned char AR_continuousSampling=0;


unsigned int * current_memSamplesBuffer = memSamplesBuffer1;
unsigned int current_memSamplesBuffer_index = 0;

int CAL_chA_calibration=0x6B6B;
int CAL_chB_calibration=0x6D09;


#pragma section("seg_pmda") 
unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

int memSamplesBufferChA[MAX_SAMPLES_BUFFER_SIZE];
int memSamplesBufferChB[MAX_SAMPLES_BUFFER_SIZE];



float memDSPBufferAmplitude[MAX_SAMPLES_BUFFER_SIZE];
float memDSPBufferPhase[MAX_SAMPLES_BUFFER_SIZE];
