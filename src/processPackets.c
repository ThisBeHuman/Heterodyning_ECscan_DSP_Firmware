/***************************************************************
	Filename:	processPackets.c USB Packet processing
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	
	Dependecies:	processPackets.h
					
	Purpose:	Processes the different payloads received by
		USB.
			
	Usage:	

***************************************************************/


#include "../h/processPackets.h"

/**************************************************************
			EXTERNAL PACKET GLOBAL VARIABLES
***************************************************************/



/**************************************************************
			LOCAL PACKET GLOBAL VARIABLES
***************************************************************/




/************************************************************
	Function:	short USB_processPayload (unsigned short payload_size, unsigned char * payload_buffer)
	Argument:	unsigned short payload_size - Payload message size for confirmation
 				unsigned char * payload_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
				USB_WRONG_CMD if the interpreted command was unrecognized
			
	Description:	Processes and executes the command from
		the read payload message received from the USB according 
		to its header byte.
		It confirms the payload_size with the appropriate command message size.
	Action:		
	
************************************************************/
int USB_processPayload(unsigned short payload_size, unsigned char * payload_buffer)
{
	int temp, index;	
//	printf("USB Header: %d\n",payload_buffer[0]);
	switch ( payload_buffer[0] ){
		case USB_MSG_CHANGE_FREQ:
			if(payload_size != USB_MSG_CHANGE_FREQ_SIZE) return USB_WRONG_CMD_SIZE;
			
			processDDSChangeFreq(payload_size, payload_buffer);
			break;
		case USB_MSG_SET_GAIN:
			if(payload_size != USB_MSG_SET_GAIN_SIZE) return USB_WRONG_CMD_SIZE;
			processSetGain(payload_size, payload_buffer);
			break;
		case USB_MSG_CURRENT_SCALE:
			if(payload_size != USB_MSG_CURRENT_SCALE_SIZE) return USB_WRONG_CMD_SIZE;
			
			processSetCurrentScale(payload_size, payload_buffer);
			break;
		case USB_MSG_ADC_SAMPLING:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_ADC_SAMPLING_SIZE) return USB_WRONG_CMD_SIZE;
			
			processADCStartSampling(payload_size, payload_buffer);
			break;
		case USB_MSG_ADC_STOP_SAMPLING:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_ADC_STOP_SAMPLING_SIZE) return USB_WRONG_CMD_SIZE;
			
			processADCStopSampling(payload_size, payload_buffer);
			break;
		case USB_MSG_CALIBRATE:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_CALIBRATE_SIZE) return USB_WRONG_CMD_SIZE;
			
			processCalibrate(payload_size, payload_buffer);
			break;
		case USB_MSG_MOVEXY:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_MOVEXY_SIZE) return USB_WRONG_CMD_SIZE;
			
			processMoveXY(payload_size, payload_buffer);
			break;
		case USB_MSG_DRIVER_EN:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_DRIVER_EN_SIZE) return USB_WRONG_CMD_SIZE;
			
			processDriverEn(payload_size, payload_buffer);
			break;
		case USB_MSG_OPMODE:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_OPMODE_SIZE) return USB_WRONG_CMD_SIZE;
			
			processOpMode(payload_size, payload_buffer);
			break;
		case USB_MSG_STEPPER_EN:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_STEPPER_EN_SIZE) return USB_WRONG_CMD_SIZE;
			
			processStepperEn(payload_size, payload_buffer);
			break;
		case USB_MSG_ADC_SINGLESAMPLE:
//			printf("payload size:%d\n",payload_size);
			if(payload_size != USB_MSG_ADC_SINGLESAMPLE_SIZE) return USB_WRONG_CMD_SIZE;
			
			processADCSingleSample(payload_size, payload_buffer);
			break;
		default:
			return USB_ERROR_FLAG;
		
	}
	
	return TRUE;	
}




/************************************************************
	Function:	short processDDSChangeFreq (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Processes a Change Frequency message.
		Verifies if the header byte corresponds to this function
		and if so updates the frequency of the DDS
	Extra:	The change frequency command message is as follows:
	[CHANGE_FREQ_HEADER - 1 byte (0)	|
	 DDS1	Freq - 4 bytes 	(1,2,3,4) 	| Phase	- 1 byte (5)
	 DDS2	Freq - 4 bytes 	(6,7,8,9)	| Phase	- 1 byte (10)
	 DDS3	Freq - 4 bytes (11,12,13,14)| Phase	- 1 byte (15)]
		
************************************************************/
int processDDSChangeFreq(unsigned short msg_size, unsigned char * msg_buffer)
{
	int temp, index;	
	
	int DDS1_Freq, DDS2_Freq, DDS3_Freq;
	char DDS1_Phase, DDS2_Phase, DDS3_Phase;
	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_CHANGE_FREQ_SIZE 
		&& msg_buffer[0] != USB_MSG_CHANGE_FREQ) {
			return USB_WRONG_CMD;
	}
	
//	printf("CHANGE FREQ\n");
	
	// DDS 1 Frequency
	DDS1_Freq = msg_buffer[1] <<24;
	DDS1_Freq |= msg_buffer[2] <<16;
	DDS1_Freq |= msg_buffer[3] <<8;
	DDS1_Freq |= msg_buffer[4];
//	DDS1_Freq = DDS1_Freq * DDS_FREQUENCY_MULTIPLIER;
// Converted to floats and rounded
	DDS1_frequency = DDS1_Freq = (int) DDS1_Freq * DDS_FREQUENCY_MULTIPLIER_FLOAT;
	DDS_inc_Fex = DDS1_Freq;
	
	DDS1_phase = DDS1_Phase = msg_buffer[5]&0x1f;

		// DDS 2 Frequency
	DDS2_Freq = msg_buffer[6] <<24;
	DDS2_Freq |= msg_buffer[7] <<16;
	DDS2_Freq |= msg_buffer[8] <<8;
	DDS2_Freq |= msg_buffer[9];
//	DDS2_Freq = DDS2_Freq * DDS_FREQUENCY_MULTIPLIER;
// Converted to floats and rounded
	DDS2_frequency = DDS2_Freq = (int) DDS2_Freq * DDS_FREQUENCY_MULTIPLIER_FLOAT;
	DDS_inc_Flo = DDS2_Freq;
	
	
	DDS2_phase = DDS2_Phase = msg_buffer[10]&0x1f;

		// DDS 3 Frequency
	DDS3_Freq = msg_buffer[11] <<24;
	DDS3_Freq |= msg_buffer[12] <<16;
	DDS3_Freq |= msg_buffer[13] <<8;
	DDS3_Freq |= msg_buffer[14];
//	DDS3_Freq = DDS3_Freq * DDS_FREQUENCY_MULTIPLIER;
// Converted to floats and rounded
	DDS3_frequency = DDS3_Freq = (int) DDS3_Freq * DDS_FREQUENCY_MULTIPLIER_FLOAT;

	
	DDS3_phase = DDS3_Phase = msg_buffer[15]&0x1f;
	
	// Reconfigure the DDS.

//		DDS_init();
		// Double Reset and INIT - Makes no sense but works...
		DDS_init();
		DDS_init();

		//printf("DDS %f 1: %d, 2: %d, 3: %d\n",DDS_FREQUENCY_MULTIPLIER_FLOAT, DDS1_Freq, DDS2_Freq, DDS3_Freq);
		
		DDS_WriteData(DDS1_Freq, DDS1_Phase, 0, DDS_ch1);
		DDS_WriteData(DDS2_Freq, DDS2_Phase, 0, DDS_ch2);
		DDS_WriteData(DDS3_Freq, DDS3_Phase, 0, DDS_ch3);
		
		DDS_update_frequency();	

//		DDS_update_frequency();	
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}


/************************************************************
	Function:	short processSetGain (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Processes a Set Gain message.
		Verifies if the header byte corresponds to this function
		and if so updates the Gain of the Amplifier chain
	Extra:			
************************************************************/
int processSetGain(unsigned short msg_size, unsigned char * msg_buffer)
{
	int temp, index;	
	
	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_SET_GAIN_SIZE 
		&& msg_buffer[0] != USB_MSG_SET_GAIN) {
			return USB_WRONG_CMD;
	}
	 
	temp = (msg_buffer[1]<<8 | msg_buffer[2])&0x0fff;
	printf("gain: %d\n", temp);
	GAIN_set_voltage(temp,GAIN_PD_ON);
	
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}


/************************************************************
	Function:	short processSetCurrentScale (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Processes a and sets the current scale
		
	Extra:	Scale Value		Output current
				0				100 mA
				1				200 mA
				2				500 mA
				4				 1	A
************************************************************/
int processSetCurrentScale(unsigned short msg_size, unsigned char * msg_buffer)
{
	//int temp, index;	
	int current_scale;
	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_CURRENT_SCALE_SIZE 
		&& msg_buffer[0] != USB_MSG_CURRENT_SCALE) {
			return USB_WRONG_CMD;
	}
	 
	current_scale = msg_buffer[1]& 0xff;
	printf("scale: %d\n", current_scale);

	DDS_current_scale(current_scale);
	
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}



/************************************************************
	Function:	short processADCStartSampling (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Processes an ADC Sampling message
		
	Extra:	#!

************************************************************/
int processADCStartSampling(unsigned short msg_size, unsigned char * msg_buffer)
{
	//int temp, index;	
	int current_scale;
	unsigned int sampling_period;
	char continuous_sampling;
	unsigned int number_of_samples;
		
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_ADC_SAMPLING_SIZE 
		&& msg_buffer[0] != USB_MSG_ADC_SAMPLING) {
			return USB_WRONG_CMD;
	}
	 
	sampling_period = msg_buffer[1] <<24;
	sampling_period |= msg_buffer[2] <<16;
	sampling_period |= msg_buffer[3] <<8;
	sampling_period |= msg_buffer[4];
	
	continuous_sampling = msg_buffer[5]&0x1;
	
	number_of_samples = msg_buffer[6] <<24;
	number_of_samples |= msg_buffer[7] <<16;
	number_of_samples |= msg_buffer[8] <<8;
	number_of_samples |= msg_buffer[9];
	
	SweepMode = msg_buffer[10]&0xff;
//	DRIVER_ENABLE;
	ADC_StartSampling(number_of_samples, sampling_period, continuous_sampling);
	
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}


/************************************************************
	Function:	short processADCStopSampling (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Stops an ocurring ADC Sampling and sending to USB.
		
	Extra:	

************************************************************/
int processADCStopSampling(unsigned short msg_size, unsigned char * msg_buffer)
{
	//int temp, index;	
	int current_scale;
	unsigned int sampling_period;
	char continuous_sampling;
	unsigned int number_of_samples;
		
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_ADC_STOP_SAMPLING_SIZE 
		&& msg_buffer[0] != USB_MSG_ADC_STOP_SAMPLING) {
			return USB_WRONG_CMD;
	}
	 
//	DRIVER_DISABLE;
	ADC_StopSampling();

	process_sendAcknowledge(msg_buffer[0]);
	
	return TRUE;
}



/************************************************************
	Function:	short processCalibrate (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Starts an acquisition run that, when finished,
		flags the calibration loop routine to update the calibration
		variables.
		
	Extra:	
			CAL_chA_calibration
			CAL_chB_calibration
			
************************************************************/
int processCalibrate(unsigned short msg_size, unsigned char * msg_buffer)
{
	int i;	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_CALIBRATE_SIZE 
		&& msg_buffer[0] != USB_MSG_CALIBRATE) {
			printf("error calibrate!\n");//#!
			return USB_WRONG_CMD;
	}
	 
	// Reset calibration variables
	CAL_chA_calibration=0;
	CAL_chB_calibration=0;
	// #! 3 samples 10us, single acquisition
	//Init_IIR_soft();
	ADC_StartSampling(4000, 10, 0);
//	while(~AR_finishedFlag);
	for(i=0;i<1;i++);
	CAL_calibrateFlag = TRUE;
	
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}


/************************************************************
	Function:	int process_sendAcknowledge (unsigned char header)
	Argument:	unsigned char header - Same header as received packet
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Stops an ocurring ADC Sampling and sending to USB.
		
	Extra:	

************************************************************/
int process_sendAcknowledge(unsigned char header)
{
	unsigned short acknowledge_packet_size=1;
	unsigned short acknowledge_payload_size=6;
	
	USB_ACK_BUFFER[0] = USB_START_OF_PACKET_TO_HOST;
	USB_ACK_BUFFER[1] = (acknowledge_packet_size>>24&0xff);
	USB_ACK_BUFFER[2] = (acknowledge_packet_size>>16&0xff);
	USB_ACK_BUFFER[3] = (acknowledge_packet_size>>8&0xff);
	USB_ACK_BUFFER[4] = acknowledge_packet_size&0xff;
	USB_ACK_BUFFER[5] = header;
	
//	printf("sent acknowledge: %x\n",header);

	if(USB_writeBuffer(acknowledge_payload_size, &USB_ACK_BUFFER[0]) == USB_ERROR_FLAG){
		return USB_ERROR_FLAG;	
	} 

	return TRUE;
}


/************************************************************
	Function:	int process_sendSampleData (unsigned char header)
	Argument:	unsigned char header - Same header as received packet
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Stops an ocurring ADC Sampling and sending to USB.
		
	Extra:	

************************************************************/
int process_sendSampleData(unsigned short sample_size, float * bufferChA, float * bufferChB)
{
	unsigned int packet_size, payload_size;
	unsigned short sendSampleData_header_size=6;
	
	float whatfloat[]={0,1,2,3,4};
	
	
	
	packet_size = sizeof(float);
	packet_size = 1 + sample_size*2*4;//sizeof(float);
	payload_size = sample_size+1;
	
	USB_ACK_BUFFER[0] = USB_START_OF_PACKET_TO_HOST;
	USB_ACK_BUFFER[1] = (packet_size>>24&0xff);
	USB_ACK_BUFFER[2] = (packet_size>>16&0xff);
	USB_ACK_BUFFER[3] = (packet_size>>8&0xff);
	USB_ACK_BUFFER[4] = packet_size&0xff;
	
	USB_ACK_BUFFER[5] = USB_MSG_SENDSAMPLEDATA; // header
	
	
	
	if(USB_writeBuffer(sendSampleData_header_size, &USB_ACK_BUFFER[0]) == USB_ERROR_FLAG){
		return USB_ERROR_FLAG;	
	} 
	if(USB_sendADCData(sample_size*sizeof(float), (unsigned char*)bufferChA) == USB_ERROR_FLAG){
		printf("error sending channel A\n");
		return USB_ERROR_FLAG;	
	} 
	if(USB_sendADCData(sample_size*sizeof(float), (unsigned char*)bufferChB) == USB_ERROR_FLAG){
		printf("error sending channel B\n");
		return USB_ERROR_FLAG;	
	} 
	
	return TRUE;
}


/************************************************************
	Function:	int processMoveXY (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Moves probe a number of steps on the X or Y axis.
		
	Extra:	
			byte XY
			int steps
************************************************************/
int processMoveXY(unsigned short msg_size, unsigned char * msg_buffer)
{
	int i;	
	int steps;
	char cw_ccw;
	char half_full;
	char XY;
	int speed;
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_MOVEXY_SIZE 
		&& msg_buffer[0] != USB_MSG_MOVEXY) {
			printf("error XY!\n");//#!
			return USB_WRONG_CMD;
	}
	 
	XY = msg_buffer[1];
	half_full = msg_buffer[2];
	cw_ccw = msg_buffer[3];
	steps = (msg_buffer[4]<<24|msg_buffer[5]<<16|msg_buffer[6]<<8 | msg_buffer[7])&0xffffffff;
	speed = (msg_buffer[8]<<24|msg_buffer[9]<<16|msg_buffer[10]<<8 | msg_buffer[11])&0xffffffff;
//	printf("move: %d \n", speed);

	if(XY == MOVE_X){
		X_init( half_full,  cw_ccw);
		move_x_speed = speed;
		X_move(steps);
	}else if(XY == MOVE_Y){
		Y_init( half_full,  cw_ccw);
		move_y_speed = speed;
		Y_move(steps);	
		move_y_speed = speed;
		
	}
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}



/************************************************************
	Function:	int processDriverEnable (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Enables or Disables the driver
		
	Extra:	
			byte ENABLE/DISABLE
			
************************************************************/
int processDriverEn(unsigned short msg_size, unsigned char * msg_buffer)
{
	int temp;	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_DRIVER_EN_SIZE 
		&& msg_buffer[0] != USB_MSG_DRIVER_EN) {
			printf("error Driver EN!\n");//#!
			return USB_WRONG_CMD;
	}
	temp = msg_buffer[1]& 0xff;
	printf("DRIVER %d\n", temp);

	if(temp == 0){
		DRIVER_DISABLE ;
	}else{
		DRIVER_ENABLE;
	}			
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}

/************************************************************
	Function:	int processStepperEnable (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Enables or Disables the stepper
		
	Extra:	
			byte ENABLE/DISABLE
			
************************************************************/
int processStepperEn(unsigned short msg_size, unsigned char * msg_buffer)
{
	int temp,stepper;	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_STEPPER_EN_SIZE 
		&& msg_buffer[0] != USB_MSG_STEPPER_EN) {
			printf("error Stepper EN!\n");//#!
			return USB_WRONG_CMD;
	}
	temp = msg_buffer[1]& 0xff;
	stepper = msg_buffer[2]& 0xff;


	if(temp == 0){
		if(stepper) Y_DISABLE; else X_DISABLE ;
	}else{
		if(stepper) Y_ENABLE ; else X_ENABLE ;
	}			
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}


/************************************************************
	Function:	int processOpMode (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Changes operation mode to IF or IQ
		
	Extra:	
			byte OpMode
			
************************************************************/
int processOpMode(unsigned short msg_size, unsigned char * msg_buffer)
{
	int temp;	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_OPMODE_SIZE 
		&& msg_buffer[0] != USB_MSG_OPMODE) {
			printf("error OpMode EN!\n");//#!
			return USB_WRONG_CMD;
	}
	temp = msg_buffer[1]& 0xff;
	printf("OpMode %s\n", temp? "IF":"IQ");

	if(temp == MODE_IF){
		OpMode = MODE_IF ;
	}else{
		OpMode = MODE_IQ ;
	}			
	process_sendAcknowledge(msg_buffer[0]);

	return TRUE;
}




/************************************************************
	Function:	short processADCSingleSample (unsigned short msg_size, unsigned char * msg_buffer)
	Argument:	unsigned short msg_size - Payload message size for confirmation
 				unsigned char * msg_buffer - Payload buffer with message to process
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Acknowledges and sends a Real and Imaginary Sample from
			the buffer
		
	Extra:	
			
************************************************************/
int processADCSingleSample(unsigned short msg_size, unsigned char * msg_buffer)
{
	int i;	
	// Checks if this message corresponds to a Change Frequency command
	if(msg_size != USB_MSG_ADC_SINGLESAMPLE_SIZE 
		&& msg_buffer[0] != USB_MSG_ADC_SINGLESAMPLE) {
			printf("error SingleSample!\n");//#!
			return USB_WRONG_CMD;
	}
	 
	// Reset calibration variables
//	printf(" SingleSample! %f %f\n",AR_bufferChA[AR_bufferIndex],AR_bufferChB[AR_bufferIndex]);//#!
	process_sendAcknowledge(msg_buffer[0]);
	process_sendSampleData(1,&AR_bufferChA[AR_bufferIndex],&AR_bufferChB[AR_bufferIndex]);

	return TRUE;
}


