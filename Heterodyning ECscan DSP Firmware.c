/*****************************************************************************
 * Heterodyning ECscan DSP Firmware.c
 *****************************************************************************/

#define LED6 DAI_PB16_I
#include "./h/general.h" 

/////////////////////////////////////////////////
// 			GLOBALS
/////////////////////////////////////////////////

int gStatus,gStatus2;
int gFreq;
int gChangeFreq;
char gPhase;





void dai_Interrupt(int sig_int);





//////////////////////////////////////////////////////////////////////////////
// void Setup_Ints(void)
//
// PURPOSE:		Configure the system to accept the push buttons as inputs
//
//////////////////////////////////////////////////////////////////////////////
void Setup_Ints(void)
{
	//Config_SRU_INTS();


    (*pDAI_IRPTL_PRI) = (SRU_EXTMISCA1_INT  | SRU_EXTMISCA2_INT | SRU_EXTMISCB0_INT);    //unmask individual interrupts
    (*pDAI_IRPTL_RE) = (SRU_EXTMISCA1_INT  | SRU_EXTMISCA2_INT | SRU_EXTMISCB0_INT);    //make sure interrupts latch on the rising edge

	//Set up interrupt priorities
    sysreg_bit_set(sysreg_IMASK, DAIHI); //make DAI interrupts high priority

    (*pSYSCTL) |= (IRQ0EN|IRQ1EN);

    sysreg_bit_set(sysreg_MODE2, (IRQ0E|IRQ1E) );
    sysreg_bit_clr(sysreg_IRPTL, (IRQ1I|IRQ0I) );
    sysreg_bit_set(sysreg_IMASK, (IRQ1I|IRQ0I) ); //enable IRQ interrupts
    sysreg_bit_set(sysreg_MODE1, IRPTEN ); 		  //enable global interrupts

  //  interrupt(SIG_DAIH,DAIroutine);
    interrupt (SIG_SP1,IRQ_DDS_SP1);


}




/************************************************************
	Function:		IRQ_aliveTimer 
	Argument:	int sigint;
				
	Description:
			Occurs when a specified timer for each the alive 
			signal was flagged.
	Action:		
				
************************************************************/

void IRQ_aliveTimer(int sigint)
{
	int i;
	static int onoff=0;
	static int alive_duty_cycle=0;
	// Clears Timer interrupt
	*pTMSTAT &= TIM1IRQ;
	
	alive_duty_cycle++;
	if(alive_duty_cycle%ALIVE_DUTYCYCLE==0){
		SIG_ALIVE_ON;
		alive_duty_cycle=0;
	}else{
		SIG_ALIVE_OFF;
	}
	
/*	if(onoff==0){
		SIG_ALIVE_OFF;
		onoff=1;
	}else{
		SIG_ALIVE_ON;
		onoff=0;
	}
*/

}

/************************************************************
	Function:		Alive_timer_init() 
	Argument:	
				
	Description:
			Initializes timer to allow stepping the steppers
	Action:		
				
************************************************************/

void Alive_timer_init (void)
{

	
    *pTM1CTL = (TIMODEPWM | PULSE | PRDCNT | IRQEN);
    *pTM1PRD = ALIVE_TIMER_PRD;
    *pTM1W = *pTM1PRD/2;//(CNV_uSEC * TICKS_PER_uSEC-3); // 10% pulse
	*pTM1STAT = TIM1EN;

	
    //*pDAI_IRPTL_PRI |= SRU_EXTMISCB0_INT;
    //*pDAI_IRPTL_RE |= SRU_EXTMISCB0_INT;
   	interrupt(SIG_GPTMR1, IRQ_aliveTimer);
}




void IRQ_timer(int sigint)
{
	int i;
	*pTMSTAT &= TIM0IRQ;
	SRU(LOW, DAI_PB17_I);	
	//for(i=0;i<10;i++);
	SRU(HIGH, DAI_PB17_I);
//	IRQ_ADC_SampleReady(0);

}

void initTimer0 (void)
{


    *pTM0CTL = (TIMODEPWM | PULSE | PRDCNT | IRQEN);
    *pTM0PRD = CNV_uSEC * TICKS_PER_uSEC;
    *pTM0W = (CNV_uSEC * TICKS_PER_uSEC-3); // 10% pulse
	*pTM0STAT = TIM0EN;

	
    *pDAI_IRPTL_PRI |= SRU_EXTMISCB0_INT;
    *pDAI_IRPTL_RE |= SRU_EXTMISCB0_INT;
   	interrupts(SIG_GPTMR0, IRQ_timer);
}



void main( void )
{
	/* Begin adding your custom code here */


		int i,z=0,k = 0,nr=0;
	int j=0;
	
	int temp;
	int timeout;
	char *aux_ptr;
	int usbdata,usbdata2,usbdata3,usbdata4;
	unsigned char w4 = 0x8E;// 0x8E;
	unsigned char w3 = 0xE3;
	unsigned char w2 = 0x38;//0x38;
	unsigned char w1 = 0x0E>>0;//0x0E;
	unsigned char w0 = 0x09; // phase, power down, REF Multiplier
	float somefloat = 300;
	unsigned short packetSize;
	
	char memaux[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
//
/*	union dds_freq freq;
	freq.freq_byte[0]=0x0E;
	freq.freq_byte[1]=0x38;
	freq.freq_byte[2]=0xE3;
	freq.freq_byte[3]=0x8E;
*/	
	gStatus = 0;
	gChangeFreq =0;
	gPhase = 0;
	adc_end_of_sampling = 0;
	adc_continuous_sampling = 1;
	
		// Enable pull-up resistors on unused DAI pins
//	* (volatile int *)DAI_PIN_PULLUP = 0x9ffff;//0x9ffff;

	// Enable pull-up resistors on unused DPI pins
//	* (volatile int *)DPI_PIN_PULLUP = 0x3fff;
	
	//initPLL_SDRAM();
	//InitPLL_SDRAM();
	
	somefloat = sqrtf(somefloat);
	
	//SRU(HIGH,PIN_SCALE_b0);	
	//SRU(HIGH,PIN_SCALE_b1);	
	//InitSRU();

	//InitSPORT();
	
	
	//DDS_init_io();

	//DDS_current_scale(DDS_CURRENT_100);
//	initPLL_21489(8,4,3,false);
	
	// Signal INIT
	SIG_ALIVE_EN;
	SIG_RUNNING_EN;
	SIG_HOST_EN;
	SIG_ERROR_EN;
	SIG_LED1_EN;
	SIG_LED2_EN;
	SIG_LED3_EN;
	SIG_LED4_EN;
	SIG_ALIVE_OFF;
	SIG_RUNNING_OFF;
	SIG_HOST_OFF;
	SIG_ERROR_OFF;
	SIG_LED1_OFF;
	SIG_LED2_OFF;
	SIG_LED3_OFF;
	SIG_LED4_OFF;
	
	
	

	InitPLL();
	InitDDS_IO();

	InitGAIN_IO();
	InitADC_IO();
	InitUSB_IO();
	//InitXY_IO();
	
	InitMUX_IO();
	
	// #! PCGD
/*
	#define CLKD_DIVIDER 1

#define FSD_DIVIDER (CLKD_DIVIDER)*25	
	
	*pPCG_PW2 = (FSD_DIVIDER-1)<<16;
	*pPCG_SYNC1 = 0;	
	*pPCG_CTLD1 = CLKD_DIVIDER; 
	*pPCG_CTLD0 =  FSD_DIVIDER | ENFSD | ENCLKD ;

	
		SRU (HIGH, DAI_PBEN18_I);
//	SRU (PCG_FSD_O, DAI_PB14_I);		
//	SRU (HIGH, DAI_PBEN14_I);
	SRU (PCG_FSD_O, DAI_PB18_I);		
	
	//#! END OF
*/	
	Setup_Ints();
	Alive_timer_init();
	DDS_init();
	// Double Reset and INIT - Makes no sense but works...
	DDS_init();
	GAIN_init();
	//ADC_init();
	USB_init();
	//Setup_AMI();
	
	
/* Disabled for MUX testing 	
	X_init(MODE_FULL_STEP, MODE_CW);
	Y_init(MODE_FULL_STEP, MODE_CW);
	
	XY_timer_init();
	XY_MOTION_ENABLE;
	
*/ 

	// DRIVER DISABLE OUTPUT
	DRIVER_DISABLE;
	SRU(HIGH,PBEN03_I);
	SRU(HIGH,DPI_PBEN14_I);
	
	XY_MOTION_ENABLE;
	//X_ENABLE;
//	Y_ENABLE;
	// Enable DAI interrupt on falling edge of PCG_FS
    //(*pDAI_IRPTL_PRI) = (SRU_EXTMISCA1_INT  | SRU_EXTMISCA2_INT | SRU_EXTMISCB0_INT);    //unmask individual interrupts
    //(*pDAI_IRPTL_RE) = (SRU_EXTMISCA1_INT  | SRU_EXTMISCA2_INT | SRU_EXTMISCB0_INT);    //make sure interrupts latch on the rising edge
	
//	*pDAI_IRPTL_PRI = SRU_EXTMISCB0_INT ;
//	*pDAI_IRPTL_RE = SRU_EXTMISCB0_INT ;
//	*pDAI_IRPTL_FE = 0; 
//	interrupt(SIG_P0,dai_Interrupt);
//	SRU (DAI_PB19_O, DAI_INT_22_I); 

//	interrupt(SIG_P5,IRQ_FIR);

//	initTimer0();	
	while(1){
		if(gChangeFreq ==1){
	/*     	 
			DDS1_frequency = gFreq;// 0x80FFFF01;//DDS_10kHz;
			DDS1_phase = DDS_PHASE_0;
			
			DDS_WriteData(DDS_100kHz, DDS_PHASE_0, 0, DDS_ch1);
	
			DDS2_frequency = gFreq;//DDS_100kHz;
			DDS2_phase = DDS_PHASE_0;
			DDS_WriteData(DDS_99kHz, DDS_PHASE_0, 0, DDS_ch2);

			DDS3_frequency = gFreq;
			DDS3_phase = DDS_PHASE_0;
		
			DDS_WriteData(DDS_99kHz, DDS_PHASE_90, 0, DDS_ch3);


			DDS_update_frequency();	
			GAIN_set_voltage(GAIN_10VV,GAIN_PD_ON);
			*/
			gChangeFreq = 0;
		}
		
/* USB	*/	
	/*		if(z==0){
				z=1;
				SIG_ALIVE_ON;
				//SIG_LED2_OFF;
			}else{
				z=0;
				SIG_ALIVE_OFF;
			//	SIG_LED2_ON;
			}
*/					for(i=0;i<1;i++);	

			
		// USB Polling
		//	usbdata = USB_access(USB_STATUS, USB_READ, USB_NULL);
			//usbdata = usb_access(0, STATUS);
		//	usbdata = usbdata& DATA_AVAI;
//				usb_access(1, j++);
			if(USB_pollDataAvailable()){
				if(USB_readStartOfPacket()){
			//		ADC_StartSampling(1024, CNV_uSEC,adc_continuous_sampling);
					packetSize = USB_readPacketSize();
	//#!				printf("size: %d\n",packetSize);
					if(packetSize > 0){
						if(USB_readPayload(packetSize, &USB_PAYLOAD_BUFFER[0]) == USB_ERROR_FLAG){
							printf("USB Error: Error reading payload.\n");
						}else{
							for(i=0; i<packetSize;i++){
							//	printf("%x ", USB_PAYLOAD_BUFFER[i]);
							}
							temp = USB_processPayload(packetSize, &USB_PAYLOAD_BUFFER[0]);
							if( temp == USB_ERROR_FLAG){
								printf("USB Error: Error processing payload.\n");
							}else if(temp == USB_WRONG_CMD_SIZE){
								printf("USB Error: Wrong Command Size %d\n", temp);
							}
						}	
					}
						
				}
				//while(!(usb_access(0, STATUS) & SPACE_AVAI));
				//USB_access(USB_DATA_PIPE, USB_WRITE, usbdata);
			//	usb_access(1, usbdata);
			//	printf("usb%d: %x \n",j++,usbdata);
			}

/*		if(adc_end_of_sampling){
		//	aux_ptr = (char*)SAMPLES_MEMORY;
			k = adc_number_of_samples*4;
			j=0;
			for (i = 0; i< adc_number_of_samples ; i++){
				for(k = 0; k < 4; k++){
//				printf("sample: %d %x\n",i,SAMPLES_MEMORY.Char[i]);
					while(!(USB_access(USB_STATUS, USB_READ, USB_NULL)&SPACE_AVAI));
			//		printf("usb samples %d: int: %x - %x\n",j++,SAMPLES_MEMORY[i], (SAMPLES_MEMORY[i]>>(k*8))&0xff);
				//	printf("usb samples %d: int: %x - %x\n",j++,SAMPLES_MEMORY.Int[i/4], SAMPLES_MEMORY.Char[i]);
					USB_access(USB_DATA_PIPE, USB_WRITE, (SAMPLES_MEMORY[i]>>(k*8))&0xff);
				}
			}	
			
		//	USB_write_memory(memaux, 10);
			adc_end_of_sampling = 0;
		}	
*/
		
		if(AR_finishedFlag){
	//		SIG_LED1_OFF;
	/*		DSP_ModeIQ_AmplitudePhase(adc_number_of_samples_to_send,adc_buffer_to_send,
					&memDSPBufferAmplitude[0],&memDSPBufferPhase[0]);
			
			USB_sendADCData(adc_number_of_samples_to_send,(unsigned short*)&memDSPBufferAmplitude[0]);				
	*/
//					for(i=0;i<1000;i++);


			if(CAL_calibrateFlag&&0){			
				CAL_chA_calibration = (AR_bufferChA[3310]+AR_bufferChA[3511]+AR_bufferChA[3102])/3;
				CAL_chB_calibration = (AR_bufferChB[3310]+AR_bufferChB[3511]+AR_bufferChB[3102])/3;
				CAL_calibrateFlag = FALSE;
				printf("cal chA: %f, chB: %f\n",CAL_chA_calibration,CAL_chB_calibration);
				//Init_IIR_soft();
				
			}else{
				timeout = 100000;
				while(DSP_processingFIR&&timeout--);

						
	//			USB_sendADCData(adc_number_of_samples_to_send,(unsigned int*)memProcessedBufferChA);
	//			USB_sendADCData(adc_number_of_samples_to_send,(unsigned int*)AR_bufferChA);
				//USB_sendADCData(adc_number_of_samples_to_send,adc_buffer_to_send);
			//	Init_IIR_soft();
				//iir(AR_bufferChB,memProcessedBufferChB, IIR_coeffs, IIR_states,adc_number_of_samples_to_send,0);				
				
				if(OpMode == MODE_IF){	
			//		signal_QuadratureDemodulation(AR_bufferChA,AR_bufferChB,AR_bufferIndex);
					//signal_QuadratureDemodulation_InternalLO(AR_bufferChA,AR_bufferChB,AR_bufferIndex);
					// #! Removed for run time demodulation
				}
				if(CAL_calibrateFlag){			

			//		CAL_chA_calibration = (AR_bufferChA[3310]+AR_bufferChA[3511]+AR_bufferChA[3102])/3;
			//		CAL_chB_calibration = (AR_bufferChB[3310]+AR_bufferChB[3511]+AR_bufferChB[3102])/3;
			//		CAL_calibrateFlag = FALSE;
			//		printf("cal chA: %f, chB: %f\n",CAL_chA_calibration,CAL_chB_calibration);
				}else{
				//	signal_Calibrate(AR_bufferChA,AR_bufferChB,AR_bufferIndex);
					if (SweepMode == TRUE){
						
						// If in sweep mode. Only send last sample of each channel.
						process_sendSampleData(1,(unsigned int*)&AR_bufferChA[AR_bufferIndex],(unsigned int*)&AR_bufferChB[AR_bufferIndex]);
					}else{
						process_sendSampleData(AR_bufferIndex,(unsigned int*)AR_bufferChA,(unsigned int*)AR_bufferChB);
					}
				}
//				process_sendSampleData(adc_number_of_samples_to_send,(unsigned int*)memProcessedBufferChA,(unsigned int*)AR_bufferChA);//memProcessedBufferChB);
			
			}
			AR_finishedFlag = FALSE;	
		}

		

	}
//	return 0;
}



