/***************************************************************
	Filename:	configXY.c 
	Author:		Diogo Aguiam - diogo.aguiam@ist.utl.pt
	Date:		August 2013
	Version:	v1.0
	
	Dependecies:	configXY.h
					
	Purpose:	Configuration and execution functions to contorl
		the XY table.
			
	Usage:	

***************************************************************/


#include "../h/configXY.h"

/**************************************************************
			EXTERNAL XY GLOBAL VARIABLES
***************************************************************/



/**************************************************************
			LOCAL XY GLOBAL VARIABLES
***************************************************************/



/************************************************************
	Function:		InitXY_IO (void)
	Argument:	
	Description:	Initializes XY IOs
	Action:		

					
************************************************************/
void InitXY_IO(void)
{	
	int i;
	X_DISABLE;
	Y_DISABLE;
	
	
	XY_SRST_OUTPUT;
	X_EN_OUTPUT;
	X_HALFFULL_OUTPUT;
	X_CWCCW_OUTPUT;
	X_STEP_OUTPUT;
	
	Y_EN_OUTPUT;
	Y_HALFFULL_OUTPUT;
	Y_CWCCW_OUTPUT;
	Y_STEP_OUTPUT;
	
	XY_SRST_ON;
	
	X_STEP_LOW;
	Y_STEP_LOW;
	for(i=0;i<1000;i++);
	
	XY_SRST_OFF;
	XY_MOTION_DISABLE;
	XY_MOTION_OUTPUT;
}


/************************************************************
	Function:		X_init 
	Argument:	char half_full - half=1;full=0;
				char cw_ccw		- cw=1;cc2=0;
				
	Description:
			Initializes the modes of operation for the X axis
			on the XY table.
			
	Action:		
				
************************************************************/
void X_init(char half_full, char cw_ccw)
{
	if(half_full == MODE_FULL_STEP){
		X_FULL;
	}else{
		X_HALF;
	}
	if(cw_ccw == MODE_CCW){
		X_CCW;
	}else{
		X_CW;
	}		
	X_STEP_LOW;
}

/************************************************************
	Function:		Y_init 
	Argument:	char half_full - half=1;full=0;
				char cw_ccw		- cw=1;cc2=0;
				
	Description:
			Initializes the modes of operation for the Y axis
			on the XY table.
			
	Action:		
				
************************************************************/
void Y_init(char half_full, char cw_ccw)
{
	if(half_full == MODE_FULL_STEP){
		Y_FULL;
	}else{
		Y_HALF;
	}
	if(cw_ccw == MODE_CCW){
		Y_CCW;
	}else{
		Y_CW;
	}
	Y_STEP_LOW;
	
}


/************************************************************
	Function:		X_move 
	Argument:	int steps;
				
	Description:
			Moves the probe head STEPS number of steps on the
			X axis.
			(Clock period > 0.5us)
	Action:		
				
************************************************************/
void X_move(int steps)
{
	int i,k;
	X_ENABLE;
	for(k=steps; k>0; k--){
		X_STEP_HIGH;
		for(i=0;i<MOVE_XY_CLK_DELAY;i++);
		X_STEP_LOW;
		for(i=0;i<MOVE_X_DELAY;i++);
	}
	X_DISABLE;
}

/************************************************************
	Function:		Y_move 
	Argument:	int steps;
				
	Description:
			Moves the probe head STEPS number of steps on the
			Y axis.
			(Clock period > 0.5us)
	Action:		
				
************************************************************/
void Y_move(int steps)
{
	int i,k;
	Y_ENABLE;
	for(k=steps; k>0; k--){
		Y_STEP_HIGH;
		for(i=0;i<MOVE_XY_CLK_DELAY;i++);
		Y_STEP_LOW;
		for(i=0;i<MOVE_Y_DELAY;i++);
	}
	Y_DISABLE;
		
}
