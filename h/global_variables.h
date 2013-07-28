/***************************************************************
	Filename:	global_variables.h
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	Revisions:
				1.0	July 2013
	Purpose:	
	Usage:
	
	Extra:		



***************************************************************/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_



#include "../h/general.h"



#define MAX_SAMPLES_BUFFER_SIZE 8192

// Acquisition Run
extern unsigned int AR_totalSamples;
extern unsigned int AR_bufferIndex;
extern unsigned int *AR_buffer;
extern unsigned char AR_continuousSampling;

extern unsigned int * current_memSamplesBuffer;
extern unsigned int current_memSamplesBuffer_index ;

extern unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
extern unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

extern int memSamplesBufferChA[MAX_SAMPLES_BUFFER_SIZE];
extern int memSamplesBufferChB[MAX_SAMPLES_BUFFER_SIZE];



// Digital Signal Processing
extern int CAL_chA_calibration;
extern int CAL_chB_calibration;

extern float memDSPBufferAmplitude[MAX_SAMPLES_BUFFER_SIZE];
extern float memDSPBufferPhase[MAX_SAMPLES_BUFFER_SIZE];


#endif

