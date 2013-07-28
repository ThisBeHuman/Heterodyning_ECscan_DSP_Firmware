/* This file includes the initialization codes for FIR, IIR and FFT Accelerators */

#include "def21469.h"



#include "Cdef21469.h"

void Init_FIR();

/* Declaring the external buffers needed for FIR Accelerator*/
extern int memSamplesBufferChA[];
extern int memProcessedBufferC[];
extern int [];

extern int [];
extern int [];


/*Adding the TCB for FIR channels*/
int FIR_TCB_CH2[13]={
				0,
				512,
				1,
				,
				,
				512,
				1,
				+0,
				,
				512,
				1,
				+0,
				511|(511<<14)
			};



int FIR_TCB_CH1[13]={
				FIR_TCB_CH2+12,
				512,
				1,
				,
				memProcessedBufferC,
				512,
				1,
				memProcessedBufferC+0,
				memSamplesBufferChA,
				512,
				1,
				memSamplesBufferChA+0,
				511|(511<<14)
			};


/* Adding the Initialization Code for FIR Accelerator Now */

void Init_FIR()
{

	int temp;

	FIR_TCB_CH2[0]=FIR_TCB_CH1+12;

	//Mapping the FIR DMA interrupt
	temp=*pPICR0;
	temp&=~(P0I0|P0I1|P0I2|P0I3|P0I4);
	temp|=P0I0|P0I1|P0I3|P0I4;
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

