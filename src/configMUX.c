/***************************************************************
	Filename:	configMUX.c (Analog Multiplexer expansion board configuration)
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		December 2013
	Version:	v1.0
	
	Dependecies:	configMUX.h
					
	Purpose:	Initialize the IO pinouts for the analog multiplexer 
			A0-A3 bits
				
	Usage:	
			Sets the Analog Multiplexer selection bitss
				
		Board DPI SRU assignements:
		
			
		MUX_A0	-		DPI_PB02
		MUX_A1	-		DPI_PB01
		MUX_A2	-		DPI_PB04
		MUX_A3	-		DPI_PB03
			
	
	Extra:	
	
		CAUTION: The multiplexer adapter board must be connected with
		the provided expansion flat cable. The Add-on motion control board
		is not compatible with this multiplexer adapter board. The extra
		stepper control signal board must be used.	

***************************************************************/


#include "../h/configMUX.h"


/**************************************************************
			EXTERNAL MUX GLOBAL VARIABLES
***************************************************************/



/**************************************************************
			LOCAL MUX GLOBAL VARIABLES
***************************************************************/





/************************************************************
	Function:		InitMUX_IO (void)
	Argument:	
	Description:	Initializes MUX IO pins
	Action:		
				Defaults to 0x0
				
************************************************************/
void InitMUX_IO(void){	
// Starting value of the Output Pins.
	
	MUX_A0_L;
	MUX_A1_L;
	MUX_A2_L;
	MUX_A3_L;

//Enabling pins as Outputs. High -> Output, Low -> Input
	MUX_A0_OUTPUT;
	MUX_A1_OUTPUT;
	MUX_A2_OUTPUT;
	MUX_A3_OUTPUT;
	
}

/************************************************************
	Function:		MUX_SetMux (void)
	Argument:		char selection
	Description:	Sets the MUX selection pints
	Action:		
				Defaults to 0x0
				
************************************************************/
void MUX_SetMux(char selection){	
// Grabs only the lowest 4 bits of the selection input value

	selection = selection&0x0f;
	
	if(selection&0x01){
		MUX_A0_H;
	}else{
		MUX_A0_L;
	}
	
	if(selection&0x02){
		MUX_A1_H;
	}else{
		MUX_A1_L;
	}
	
	if(selection&0x04){
		MUX_A2_H;
	}else{
		MUX_A2_L;
	}
	
	if(selection&0x08){
		MUX_A3_H;
	}else{
		MUX_A3_L;
	}

	printf("asdasd %x\n",selection);
	return;
	
}


