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
	DDS1_Freq = DDS1_Freq * DDS_FREQUENCY_MULTIPLIER;
	
	DDS1_Phase = msg_buffer[5]&0x1f;

		// DDS 2 Frequency
	DDS2_Freq = msg_buffer[6] <<24;
	DDS2_Freq |= msg_buffer[7] <<16;
	DDS2_Freq |= msg_buffer[8] <<8;
	DDS2_Freq |= msg_buffer[9];
	DDS2_Freq = DDS2_Freq * DDS_FREQUENCY_MULTIPLIER;
	
	DDS2_Phase = msg_buffer[10]&0x1f;

		// DDS 3 Frequency
	DDS3_Freq = msg_buffer[11] <<24;
	DDS3_Freq |= msg_buffer[12] <<16;
	DDS3_Freq |= msg_buffer[13] <<8;
	DDS3_Freq |= msg_buffer[14];
	DDS3_Freq = DDS3_Freq * DDS_FREQUENCY_MULTIPLIER;
	DDS3_Phase = msg_buffer[15]&0x1f;
	
	// Reconfigure the DDS.

//		DDS_init();
		// Double Reset and INIT - Makes no sense but works...
		DDS_init();
		DDS_init();

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
	
	DRIVER_ENABLE;
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
	 
	DRIVER_DISABLE;
	ADC_StopSampling();

	process_sendAcknowledge(msg_buffer[0]);
	
	return TRUE;
}



/************************************************************
	Function:	short process_sendAcknowledge (unsigned char header)
	Argument:	unsigned char header - Same header as received packet
	Return:		TRUE if message has been processed without errors.
				USB_ERROR_FLAG if there was an error
			
			
	Description: Stops an ocurring ADC Sampling and sending to USB.
		
	Extra:	

************************************************************/
int process_sendAcknowledge(unsigned char header)
{
	unsigned short acknowledge_packet_size=1;
	unsigned short acknowledge_payload_size=4;
	
	USB_ACK_BUFFER[0] = USB_START_OF_PACKET_TO_HOST;
	USB_ACK_BUFFER[1] = (acknowledge_packet_size>>8&0xff);
	USB_ACK_BUFFER[2] = acknowledge_packet_size&0xff;
	USB_ACK_BUFFER[3] = header;
	
	printf("sent acknowledge: %x\n",header);
	if(USB_writeBuffer(acknowledge_payload_size, &USB_ACK_BUFFER[0]) == USB_ERROR_FLAG){
		return USB_ERROR_FLAG;	
	} 
	return TRUE;
}



