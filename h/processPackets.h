/***************************************************************
	Filename:	processPackets.h
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		July 2013
	Version:	v1.0
	Revisions:
				1.0	July 2013
	Purpose:	
	Usage:
	
	Extra:		



***************************************************************/

#ifndef _PROCESSPACKETS_H
#define _PROCESSPACKETS_H


#include "..\h\general.h"





int processDDSChangeFreq(unsigned short msg_size, unsigned char * msg_buffer);

int processSetGain(unsigned short msg_size, unsigned char * msg_buffer);
int processSetCurrentScale(unsigned short msg_size, unsigned char * msg_buffer);
int processADCStartSampling(unsigned short msg_size, unsigned char * msg_buffer);
int processADCStopSampling(unsigned short msg_size, unsigned char * msg_buffer);
int USB_processPayload(unsigned short payload_size, unsigned char * payload_buffer);

int processMUXSet(unsigned short msg_size, unsigned char * msg_buffer);



#endif

