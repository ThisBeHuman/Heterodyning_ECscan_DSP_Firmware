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

#define SWEEP_SAMPLE_RATE 100000
#define SWEEP_SIGNAL_FREQUENCY 1000
#define SWEEP_SAMPLES_90DEGREE	(SWEEP_SAMPLE_RATE/SWEEP_SIGNAL_FREQUENCY)/4

#define TAPS_IIR 2
#define TAPS_FIR 159
#define TAPS_FIR_LP 273

#define MAX_SAMPLES_BUFFER_SIZE 8192
#define USB_MAX_PAYLOAD_SIZE	300
#define USB_MAX_ACK_SIZE	10
#define MODE_IQ 0
#define MODE_IF 1


// Acquisition Run
extern bool AR_finishedFlag;
extern bool AR_finishedFIR;
extern unsigned int AR_totalSamples;
extern unsigned int AR_bufferIndex;
//extern unsigned int *AR_buffer;
extern float *AR_bufferChA;
extern float *AR_bufferChB;

extern char OpMode;


extern unsigned char AR_continuousSampling;

extern unsigned int * current_memSamplesBuffer;
extern unsigned int current_memSamplesBuffer_index ;

//extern unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
//extern unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

extern float memSamplesBufferChA[MAX_SAMPLES_BUFFER_SIZE];
extern float memSamplesBufferChB[MAX_SAMPLES_BUFFER_SIZE];

extern float memProcessedBufferChA[MAX_SAMPLES_BUFFER_SIZE];
extern float memProcessedBufferChB[MAX_SAMPLES_BUFFER_SIZE];
extern float memFIRcoeff[];
extern float memIIRcoeff[];

extern float dm IIR_BPstatesChA[];
extern float dm IIR_BPstatesChB[];
extern float pm BP_ACoeffs[];
extern float pm BP_BCoeffs[];
extern float dm IIR_LPstatesChA[];
extern float dm IIR_LPstatesChB[];
extern float pm LP_ACoeffs[];
extern float pm LP_BCoeffs[];
extern float pm IIR_coeffs[];


extern float pm BP_FIR_coeffs[TAPS_FIR];
extern float dm FIR_BPstatesChA[TAPS_FIR];
extern float dm FIR_BPstatesChB[TAPS_FIR];

extern float pm LP_FIR_coeffs[TAPS_FIR_LP];
extern float dm FIR_LPstatesChA[TAPS_FIR_LP];
extern float dm FIR_LPstatesChB[TAPS_FIR_LP];



// Digital Signal Processing
extern float CAL_chA_calibration;
extern float CAL_chB_calibration;
extern bool CAL_calibrateFlag;

extern float memDSPBufferAmplitude[MAX_SAMPLES_BUFFER_SIZE];
extern float memDSPBufferPhase[MAX_SAMPLES_BUFFER_SIZE];
extern bool DSP_processingFIR;
extern unsigned char USB_PAYLOAD_BUFFER[USB_MAX_PAYLOAD_SIZE];
extern unsigned char USB_ACK_BUFFER[USB_MAX_ACK_SIZE];




#endif

