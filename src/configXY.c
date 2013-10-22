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

int xy_allow_step;


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
	Function:		IRQ_stepperTimer 
	Argument:	int sigint;
				
	Description:
			Occurs when a specified timer for each step was flagged.
	Action:		
				
************************************************************/

void IRQ_stepperTimer(int sigint)
{
	int i;
	static int onoff=0;
	// Clears Timer interrupt
	*pTMSTAT &= TIM0IRQ;

/*	if(onoff==0){
		SIG_ERROR_OFF;
		onoff=1;
	}else{
		SIG_ERROR_ON;
		onoff=0;
	}
*/
	xy_allow_step =TRUE;

}

/************************************************************
	Function:		XY_timer_init 
	Argument:		char move_xy - MOVE_X or MOVE_Y
				
	Description:
			Initializes timer to allow stepping the steppers
	Action:		
				
************************************************************/

void XY_timer_init (char move_xy)
{

	*pTM0STAT = TIM0DIS;
    *pTM0CTL = (TIMODEPWM | PULSE | PRDCNT | IRQEN);
    
    *pTM0PRD = move_xy ? MOVE_Y_DELAY : MOVE_X_DELAY;
    *pTM0W = *pTM0PRD/2;//(CNV_uSEC * TICKS_PER_uSEC-3); // 10% pulse
	*pTM0STAT = TIM0EN;

	
    //*pDAI_IRPTL_PRI |= SRU_EXTMISCB0_INT;
    //*pDAI_IRPTL_RE |= SRU_EXTMISCB0_INT;
   	interrupt(SIG_GPTMR0, IRQ_stepperTimer);
}

/************************************************************
	Function:		XY_timer_set 
	Argument:		char move_xy - MOVE_X or MOVE_Y
				
	Description:
			Initializes timer to allow stepping the steppers
	Action:		
				
************************************************************/

void XY_timer_set (char move_xy)
{

	*pTM0STAT = TIM0DIS;
    *pTM0CTL = (TIMODEPWM | PULSE | PRDCNT | IRQEN);
    
    *pTM0PRD = move_xy ? move_y_speed : move_x_speed;
    *pTM0W = *pTM0PRD/2;//(CNV_uSEC * TICKS_PER_uSEC-3); // 10% pulse
	*pTM0STAT = TIM0EN;

	
    //*pDAI_IRPTL_PRI |= SRU_EXTMISCB0_INT;
    //*pDAI_IRPTL_RE |= SRU_EXTMISCB0_INT;
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
//	X_ENABLE;
	XY_timer_set(MOVE_X);
	for(k=steps; k>0; k--){
		X_STEP_HIGH;
		for(i=0;i<MOVE_XY_CLK_DELAY;i++);
		X_STEP_LOW;
		xy_allow_step=FALSE;
		asm("nop;");
		while(xy_allow_step==FALSE);
		xy_allow_step=FALSE;

		//for(i=0;i<MOVE_X_DELAY;i++);
	}
//	X_DISABLE;
	
	
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
//	Y_ENABLE;
	XY_timer_set(MOVE_Y);
	
	for(k=steps; k>0; k--){
		
		Y_STEP_HIGH;
		for(i=0;i<MOVE_XY_CLK_DELAY;i++);
		Y_STEP_LOW;
		//for(i=0;i<MOVE_Y_DELAY;i++);
		xy_allow_step=FALSE;
		asm("nop;");
		while(xy_allow_step==FALSE);
		xy_allow_step=FALSE;
	}
//	Y_DISABLE;
		
}
