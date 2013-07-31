/* This file includes the initialization codes for FIR, IIR and FFT Accelerators */

#include "def21489.h"



#include "Cdef21489.h"

void Init_FIR();

/* Declaring the external buffers needed for FIR Accelerator*/
extern int FIR_IP_buff1[];
extern int FIR_OP_buff1[];
extern int FIR_CF_buff1[];


/*Adding the TCB for FIR channels*/
int FIR_TCB_CH1[13]={
				0,
				512,
				1,
				FIR_CF_buff1,
				FIR_OP_buff1,
				512,
				1,
				FIR_OP_buff1+0,
				FIR_IP_buff1,
				512,
				1,
				FIR_IP_buff1+0,
				511|(511<<14)
			};


/* Adding the Initialization Code for FIR Accelerator Now */

void Init_FIR()
{

	int temp;

	FIR_TCB_CH1[0]=FIR_TCB_CH1+12;

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
	*pFIRCTL1=FIR_EN|FIR_DMAEN|FIR_CH1|FIR_RND0;

}

