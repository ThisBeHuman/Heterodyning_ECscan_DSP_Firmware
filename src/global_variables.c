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
char OpMode = MODE_IF;

bool DSP_processingFIR =0;

// Move XY speed global variables
int move_x_speed = 300000;
int move_y_speed = 300000;

// Sweep mode to flag sweeping operation and allow samples to be sent through usb while stepping
char SweepMode = FALSE;
char SweepMode_Flag_StepDone = FALSE;

//unsigned int * current_memSamplesBuffer = memSamplesBuffer1;
unsigned int current_memSamplesBuffer_index = 0;

float CAL_chA_calibration=0;
float CAL_chB_calibration=0;
bool 	CAL_calibrateFlag=0;


// Digital Filter Variables and memory allocation
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



float BIQUAD_stateChA[NSTATE];
float BIQUAD_stateChB[NSTATE];
float  pm BIQUAD_coeffs[BIQUAD_TAPS*NSECTIONS] =
{
	#include "iir_bw_lp100hz.dat"
};




#pragma section("seg_pmda") 
//unsigned int memSamplesBuffer1[MAX_SAMPLES_BUFFER_SIZE];
//unsigned int memSamplesBuffer2[MAX_SAMPLES_BUFFER_SIZE];

// Sampling buffers memory allocation

float memSamplesBufferChA[MAX_SAMPLES_BUFFER_SIZE];
float memSamplesBufferChB[MAX_SAMPLES_BUFFER_SIZE];


float memFIRcoeff[] = { 
	#include "fir_coeff1s.dat"
	};

float memIIRcoeff[] = { 
	#include "iir_coeff.dat"
	};
	
	
// Internal DDS Variables and memory allocation

unsigned int DDS_inc_Fex;
unsigned int DDS_inc_Flo;

// internal DDS look up table increment and accumulator for the running frequency. Updated by ADC start sampling and read by ADC_sampleDone
unsigned int iDDS_lut_inc;
unsigned int iDDS_lut_acc;
float sine_values_lut[] = {
	#include "sine4096.txt"	
	
};
	
	
//float memProcessedBufferChA[MAX_SAMPLES_BUFFER_SIZE];
//float memProcessedBufferChB[MAX_SAMPLES_BUFFER_SIZE];

// USB Buffers memory allocation

unsigned char USB_PAYLOAD_BUFFER[USB_MAX_PAYLOAD_SIZE];
unsigned char USB_ACK_BUFFER[USB_MAX_ACK_SIZE];





//float memDSPBufferAmplitude[MAX_SAMPLES_BUFFER_SIZE];
//float memDSPBufferPhase[MAX_SAMPLES_BUFFER_SIZE];
