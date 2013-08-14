/******************************************************************************
*                                                                             *
*   (C) Copyright 2009 - Analog Devices, Inc.  All rights reserved.           *
*                                                                             *
*    FILE:     main.c                                                         *
*                                                                             *
*    CHANGES:  1.00.0  - initial release    								  *
*																			  *
*    PURPOSE:  VisualDSP++ "Flash Programmer" flash driver for use with the   *
*              ADSP-21469 EZ-Board containing the ST Micro M25P16 flash       *
*              device.                                                        *
*                                                                             *
*******************************************************************************/


#ifdef __ADSP21489__
	#include <cdef21489.h>
	#include <def21489.h>
#elif __ADSP21479__
	#include <cdef21479.h>
	#include <def21479.h>
#endif

#include <stdlib.h>							/* malloc */
#include <drivers\flash\util.h>
#include <drivers\flash\Errors.h>
#include <drivers\flash\m25p16.h>
#include <sru.h>
#include <sysreg.h>

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define FLASH_START_ADDR	0x000000
#define BUFFER_SIZE			0x400
//#define BAUD_RATE_DIVISOR 	100

/* Flash Programmer commands */
typedef enum
{
	FLASH_NO_COMMAND,		// 0
	FLASH_GET_CODES,		// 1
	FLASH_RESET,			// 2
	FLASH_WRITE,			// 3
	FLASH_FILL,				// 4
	FLASH_ERASE_ALL,		// 5
	FLASH_ERASE_SECT,		// 6
	FLASH_READ,				// 7
	FLASH_GET_SECTNUM,		// 8
	FLASH_GET_SECSTARTEND,	// 9
}enProgCmds;

//----- g l o b a l s -----//

char 			*AFP_Title ;						// EzKit info
char 			*AFP_Description;					// Device Description
char			*AFP_DeviceCompany;					// Device Company
char 			*AFP_DrvVersion		= "1.00.0";		// Driver Version
char			*AFP_BuildDate		= __DATE__;		// Driver Build Date
enProgCmds 		AFP_Command 		= FLASH_NO_COMMAND;	// command sent down from the GUI
int 			AFP_ManCode 		= -1;			// manufacturer code
int 			AFP_DevCode 		= -1;			// device code
unsigned long 	AFP_Offset 			= 0x0;			// offset into flash
int 			*AFP_Buffer;						// buffer used to read and write flash
long 			AFP_Size 			= BUFFER_SIZE;	// buffer size
long 			AFP_Count 			= -1;			// count of locations to be read or written
long 			AFP_Stride 			= -1;			// stride used when reading or writing
int 			AFP_NumSectors 		= -1;			// number of sectors in the flash device
int 			AFP_Sector 			= -1;			// sector number
int 			AFP_Error 			= NO_ERR;		// contains last error encountered
bool 			AFP_Verify 			= FALSE;		// verify writes or not
unsigned long 	AFP_StartOff 		= 0x0;			// sector start offset
unsigned long 	AFP_EndOff 			= 0x0;			// sector end offset
int				AFP_FlashWidth		= 0x8;			// width of the flash device
int 			*AFP_SectorInfo;

bool bExit = FALSE; 								//exit flag

#ifdef __ADSP21489__
static char *pEzKitTitle = "ADSP-21489 Heterodyning ECscan";
#elif __ADSP21479__
static char *pEzKitTitle = "ADSP-21479 EZ-Board";
#else
	#error "Error: Unknown EZ-Board"
#endif


//----- c o n s t a n t   d e f i n i t i o n s -----//

// structure for flash sector information

typedef struct _SECTORLOCATION
{
 	unsigned long ulStartOff;
	unsigned long ulEndOff;
}SECTORLOCATION;


//----- f u n c t i o n   p r o t o t y p e s -----//

ERROR_CODE OpenFlashDevice(void);
ERROR_CODE GetNumSectors(void);
ERROR_CODE AllocateAFPBuffer(void);
ERROR_CODE GetSectorMap(SECTORLOCATION *pSectInfo);
ERROR_CODE GetFlashInfo(void);
ERROR_CODE ProcessCommand(void);
ERROR_CODE FillData( unsigned long ulStart, long lCount, long lStride, int *pnData );
ERROR_CODE ReadData( unsigned long ulStart, long lCount, long lStride, int *pnData );
ERROR_CODE WriteData( unsigned long ulStart, long lCount, long lStride, int *pnData );
ERROR_CODE SetupForFlash(void);
void 	   FreeAFPBuffer(void);
void 	   InitPLL_SDRAM(void);


         
//------------- m a i n ( ) ----------------//

int main(void)
{	
	
	SECTORLOCATION *pSectorInfo;
    ERROR_CODE Result;							// result
	
    /* open flash driver */
	AFP_Error = m25p16_Open();
	
	// setup the device so the DSP can access it
	if (SetupForFlash() != NO_ERR)
		return FALSE;

	// get flash manufacturer & device codes, title & desc
	if( AFP_Error == NO_ERR )
	{
		AFP_Error = GetFlashInfo();
	}

	// get the number of sectors for this device
	if( AFP_Error == NO_ERR )
	{
		AFP_Error = GetNumSectors();
	}

	if( AFP_Error == NO_ERR )
	{
		// malloc enough space to hold our start and end offsets
		pSectorInfo = (SECTORLOCATION *)malloc(AFP_NumSectors * sizeof(SECTORLOCATION));
	}

	// allocate AFP_Buffer
	if( AFP_Error == NO_ERR )
	{
		AFP_Error = AllocateAFPBuffer();
	}

	// get sector map
	if( AFP_Error == NO_ERR )
	{
		AFP_Error = GetSectorMap(pSectorInfo);
	}

	// point AFP_SectorInfo to our sector info structure
	if( AFP_Error == NO_ERR )
	{
		AFP_SectorInfo = (int*)pSectorInfo;
	}

	// command processing loop
	while ( !bExit )
	{
		// the plug-in will set a breakpoint at "AFP_BreakReady" so it knows
		// when we are ready for a new command because the DSP will halt
		//
		// the jump is used so that the label will be part of the debug
		// information in the driver image otherwise it may be left out
 		// since the label is not referenced anywhere
		asm("AFP_BreakReady:");
       		asm("nop;");
			if ( FALSE )
				asm("jump AFP_BreakReady;");

		// Make a call to the ProcessCommand
		   AFP_Error = ProcessCommand();
	} 

	// Clear the AFP_Buffer
	FreeAFPBuffer();
	
	if( pSectorInfo )
	{
		free(pSectorInfo);
		pSectorInfo = NULL;
	}

	// Close the Device
	AFP_Error = m25p16_Close();

	if (AFP_Error != NO_ERR)
		return FALSE;

	return TRUE;
}


//----------- P r o c e s s   C o m m a n d  ( ) ----------//
//
//  PURPOSE
//  	Process each command sent by the GUI based on the value in
//  	the AFP_Command.
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs during Opcode scan
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE ProcessCommand()
{
	ERROR_CODE ErrorCode = 	NO_ERR; 		//return error code

	COMMAND_STRUCT CmdStruct;

	// switch on the command and fill command structure.
	switch ( AFP_Command )
	{

		// erase all
		case FLASH_ERASE_ALL:
			CmdStruct.SEraseAll.ulFlashStartAddr 	= FLASH_START_ADDR;	//FlashStartAddress
			ErrorCode = m25p16_Control( CNTRL_ERASE_ALL, &CmdStruct );
			break;

		// erase sector
		case FLASH_ERASE_SECT:
			CmdStruct.SEraseSect.nSectorNum  		= AFP_Sector;		// Sector Number to erase
			CmdStruct.SEraseSect.ulFlashStartAddr 	= FLASH_START_ADDR;	// FlashStartAddress
			ErrorCode = m25p16_Control( CNTRL_ERASE_SECT, &CmdStruct);
			break;

		// fill
		case FLASH_FILL:
			ErrorCode = FillData( AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer );
			break;

		// get manufacturer and device codes
		case FLASH_GET_CODES:
			CmdStruct.SGetCodes.pManCode 			= (unsigned long *)&AFP_ManCode;	// Manufacturer Code
			CmdStruct.SGetCodes.pDevCode 			= (unsigned long *)&AFP_DevCode;	// Device Code
			CmdStruct.SGetCodes.ulFlashStartAddr 	= FLASH_START_ADDR;
			ErrorCode = m25p16_Control( CNTRL_GET_CODES, &CmdStruct);
			break;

		// get sector number based on address
		case FLASH_GET_SECTNUM:
			CmdStruct.SGetSectNum.ulOffset 			= AFP_Offset;	// offset from the base address
			CmdStruct.SGetSectNum.pSectorNum 		= (unsigned long *)&AFP_Sector;	//Sector Number
			ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct);
			break;

		// get sector number start and end offset
		case FLASH_GET_SECSTARTEND:
			CmdStruct.SSectStartEnd.nSectorNum 		= AFP_Sector;	// Sector Number
			CmdStruct.SSectStartEnd.pStartOffset 	= &AFP_StartOff;// sector start address
			CmdStruct.SSectStartEnd.pEndOffset	 	= &AFP_EndOff;	// sector end address
			ErrorCode = m25p16_Control( CNTRL_GET_SECSTARTEND, &CmdStruct );
			break;

		// read
		case FLASH_READ:
			ErrorCode = ReadData( AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer );
			break;

		// reset
		case FLASH_RESET:
			CmdStruct.SGetCodes.ulFlashStartAddr 	= FLASH_START_ADDR; //Flash start address
			ErrorCode = m25p16_Control( CNTRL_RESET, &CmdStruct);
			break;

		// write
		case FLASH_WRITE:
			ErrorCode = WriteData( AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer );
			break;

		// no command or unknown command do nothing
		case FLASH_NO_COMMAND:
		default:
			// set our error
			ErrorCode = UNKNOWN_COMMAND;
			break;
	}

	// clear the command
	AFP_Command = FLASH_NO_COMMAND;

	return(ErrorCode);
}


//----------- S e t u p F o r F l a s h ( ) ----------//
//
//  PURPOSE
// 		Perform necessary setup for the processor to talk to the
//		flash such as external memory interface registers, etc.
//
//  RETURN VALUE
//  	ERROR_CODE - value if any error occurs during Opcode scan
//  	NO_ERR     - otherwise

ERROR_CODE SetupForFlash()
{

#if ( defined(__ADSP21375__) || defined(__ADSP21369__) || defined(__ADSP21469__)  || defined(__ADSP21479__)  || defined(__ADSP21489__))
	SRU(SPI_CLK_O,DPI_PB03_I);
	SRU(HIGH,DPI_PBEN03_I);

	// for the flag pins to act as chip select
	SRU(FLAG4_O, DPI_PB05_I);
	SRU(HIGH, DPI_PBEN05_I);


	//First set flag 4 as an output
	sysreg_bit_set( sysreg_FLAGS, FLG4O ); //asm("bit set flags FLG4O;");
	sysreg_bit_set( sysreg_FLAGS, FLG4 ); //asm("bit set flags FLG4;"); //Logic high
   
#elif (__ADSP21364__) || (__ADSP21262__)

	//First set flag 0 as an output
	sysreg_bit_set( sysreg_FLAGS, FLG0O ); //asm("bit set flags FLG0O;");
	sysreg_bit_set( sysreg_FLAGS, FLG0 ); //asm("bit set flags FLG0;"); //Logic high
#endif

	*pSPIDMAC = 0;
	*pSPIBAUD = 0;
	*pSPIFLG = 0xF80;
	*pSPICTL = 0x400;

	return NO_ERR;
}

		
//----------- A l l o c a t e A F P B u f f e r  ( ) ----------//
//
//  PURPOSE
//  	Allocate memory for the AFP_Buffer
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE AllocateAFPBuffer()
{

	ERROR_CODE ErrorCode = NO_ERR;	//return error code

	// by making AFP_Buffer as big as possible the plug-in can send and
	// receive more data at a time making the data transfer quicker
	//
	// by allocating it on the heap the compiler does not create an
	// initialized array therefore making the driver image smaller
	// and faster to load
	//
	// The linker description file (LDF) could be modified so that
	// the heap is larger, therefore allowing the BUFFER_SIZE to increase.
	
	// the data type of the data being sent from the flash programmer GUI
	// is in bytes but we store the data as integers to make data
	// manipulation easier when actually programming the data.  This is why
	// BUFFER_SIZE bytes are being allocated rather than BUFFER_SIZE * sizeof(int).
	AFP_Buffer = (int *)malloc(BUFFER_SIZE);

	// AFP_Buffer will be NULL if we could not allocate storage for the
	// buffer
	if ( AFP_Buffer == NULL )
	{
		// tell GUI that our buffer was not initialized
		ErrorCode = BUFFER_IS_NULL;
	}

	return(ErrorCode);
}


//----------- F r e e A F P B u f f e r  ( ) ----------//
//
//  PURPOSE
//  	Free the AFP_Buffer
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

void FreeAFPBuffer()
{
	// free the buffer if we were able to allocate one
	if ( AFP_Buffer )
		free( AFP_Buffer );

}

//----------- G e t N u m S e c t o r s  ( ) ----------//
//
//  PURPOSE
//  	Get the number of sectors for this device.
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE GetNumSectors(void)
{

	ERROR_CODE ErrorCode = NO_ERR;			//return error code

	GET_NUM_SECTORS_STRUCT	SGetNumSectors;	//structure for GetNumSectors
	SGetNumSectors.pnNumSectors = &AFP_NumSectors;

	ErrorCode = m25p16_Control( CNTRL_GETNUM_SECTORS, (COMMAND_STRUCT *)&SGetNumSectors  );

	return(ErrorCode);
}


//----------- G e t S e c t o r M a p  ( ) ----------//
//
//  PURPOSE
//  	Get the start and end offset for each sector in the flash.
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE GetSectorMap(SECTORLOCATION *pSectInfo)
{

	ERROR_CODE ErrorCode = NO_ERR;			//return error code
	GET_SECTSTARTEND_STRUCT	SSectStartEnd;	//structure for GetSectStartEnd
	int i;									//index

	//initiate sector information structures
	for( i=0;i<AFP_NumSectors; i++)
	{
		SSectStartEnd.nSectorNum = i;
		SSectStartEnd.pStartOffset = &pSectInfo[i].ulStartOff;
		SSectStartEnd.pEndOffset = &pSectInfo[i].ulEndOff;

		ErrorCode = m25p16_Control( CNTRL_GET_SECSTARTEND, (COMMAND_STRUCT *)&SSectStartEnd  );
	}

	return(ErrorCode);
}


//----------- G e t F l a s h I n f o  ( ) ----------//
//
//  PURPOSE
//  	Get the manufacturer code and device code
//
// 	RETURN VALUE
//  	ERROR_CODE - value if any error occurs
//  	NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE GetFlashInfo()
{

	ERROR_CODE ErrorCode = NO_ERR;		//return error code
	static GET_CODES_STRUCT  SGetCodes;	//structure for GetCodes
	COMMAND_STRUCT CmdStruct;

	//setup code so that flash programmer can just read memory instead of call GetCodes().
	CmdStruct.SGetCodes.pManCode = (unsigned long *)&AFP_ManCode;
	CmdStruct.SGetCodes.pDevCode = (unsigned long *)&AFP_DevCode;
	CmdStruct.SGetCodes.ulFlashStartAddr = FLASH_START_ADDR;

	ErrorCode = m25p16_Control( CNTRL_GET_CODES, &CmdStruct );

	if(!ErrorCode)
	{
		ErrorCode = m25p16_Control( CNTRL_GET_DESC, &CmdStruct );
		AFP_Title = pEzKitTitle;
		AFP_Description = CmdStruct.SGetDesc.pDesc;
		AFP_DeviceCompany = CmdStruct.SGetDesc.pFlashCompany;
	}
	return(ErrorCode);

}


//----------- F i l l D a t a  ( ) ----------//
//
//  PURPOSE
//  	Fill flash device with a value.
//
// 	INPUTS
//	 	unsigned long ulStart - address in flash to start the writes at
// 		long lCount - number of elements to write, in this case bytes
// 		long lStride - number of locations to skip between writes
// 		int *pnData - pointer to data buffer
//
// 	RETURN VALUE
// 		ERROR_CODE - value if any error occurs during fill
// 		NO_ERR     - otherwise
//
// 	CHANGES
//  	9-28-2005 Created

ERROR_CODE FillData( unsigned long ulStart, long lCount, long lStride, int* pnData )
{
	long i = 0;							// loop counter
	ERROR_CODE ErrorCode = NO_ERR;		// tells whether we had an error while filling
	bool bVerifyError = FALSE;			// lets us know if there was a verify error
	unsigned long ulStartAddr;   		// current address to fill
	unsigned long ulSector = 0;			// sector number to verify address
	int nCompare = 0;					// value that we use to verify flash

	ulStartAddr = FLASH_START_ADDR + ulStart;
	COMMAND_STRUCT	CmdStruct;	//structure for GetSectStartEnd

	// verify writes if the user wants to
	if( AFP_Verify == TRUE )
	{
		// fill the value
		for (i = 0; ( ( i < lCount ) && ( ErrorCode == NO_ERR ) ); i++, ulStartAddr += ( lStride ) )
		{

			// check to see that the address is within a valid sector
			CmdStruct.SGetSectNum.ulOffset = ulStartAddr;
			CmdStruct.SGetSectNum.pSectorNum = &ulSector;
			ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct  );

			if( NO_ERR == ErrorCode )
			{
				// unlock the flash, do the write, and wait for completion
				ErrorCode = m25p16_Write( (unsigned short*)&pnData[0], ulStartAddr, 0x1 );
				ErrorCode = m25p16_Read( (unsigned short*)&nCompare, ulStartAddr, 0x1 );

			if( nCompare != ( pnData[0] & 0x0000FFFF ) )
				{
					bVerifyError = TRUE;
					break;
				}
			}
			else
			{
				return ErrorCode;
			}

		}

		// return appropriate error code if there was a verification error
		if( bVerifyError == TRUE )
			return VERIFY_WRITE;
	}
	// user did not want to verify writes
	else
	{
		// fill the value
		for (i = 0; ( ( i < lCount ) && ( ErrorCode == NO_ERR ) ); i++, ulStartAddr += ( lStride ))
		{

			// check to see that the address is within a valid sector
			CmdStruct.SGetSectNum.ulOffset = ulStartAddr;
			CmdStruct.SGetSectNum.pSectorNum = &ulSector;
			ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct  );

			if( NO_ERR == ErrorCode )
			{
				// unlock the flash, do the write, and wait for completion
				ErrorCode = m25p16_Write( (unsigned short*)&pnData[0], ulStartAddr, 0x1 );
			}
			else
			{
				return ErrorCode;
			}
		}
	}

	// return the appropriate error code
	return ErrorCode;
}

//----------- W r i t e D a t a  ( ) ----------//
//
//  PURPOSE
//  	Write a buffer to flash device.
//
// 	INPUTS
// 		unsigned long ulStart - address in flash to start the writes at
//		long lCount - number of elements to write, in this case bytes
//		long lStride - number of locations to skip between writes
//		int *pnData - pointer to data buffer
//
//  RETURN VALUE
//  	ERROR_CODE - value if any error occurs during writing
//  	NO_ERR     - otherwise
//
//  CHANGES
//  	9-28-2005 Created

ERROR_CODE WriteData( unsigned long ulStart, long lCount, long lStride, int *pnData )
{
	long i = 0;						// loop counter
	ERROR_CODE ErrorCode = NO_ERR;	// tells whether there was an error trying to write
	int nCompare = 0;				// value that we use to verify flash
	bool bVerifyError = FALSE;		// lets us know if there was a verify error
	unsigned long ulAbsoluteAddr;   // current address to write
	unsigned long ulSector = 0;		// sector number to verify address
	COMMAND_STRUCT	CmdStruct;	//structure for GetSectStartEnd

	
	ulAbsoluteAddr = FLASH_START_ADDR + ulStart;
	
	// if the user wants to verify then do it
	if( AFP_Verify == TRUE )
	{
		// write the buffer up to BUFFER_SIZE items
		for (i = 0; ( i < lCount ) && ( ErrorCode == NO_ERR ); i++, ulAbsoluteAddr += lStride)
		{

			// check to see that the address is within a valid sector
			CmdStruct.SGetSectNum.ulOffset = ulAbsoluteAddr;
			CmdStruct.SGetSectNum.pSectorNum = &ulSector;
			ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct );

			if( NO_ERR == ErrorCode )
		 	{
			// unlock the flash, do the write, increase shift, and wait for completion
			ErrorCode = m25p16_Write( (unsigned short*)&pnData[i], ulAbsoluteAddr, 0x1 );

			ErrorCode = m25p16_Read( (unsigned short*)&nCompare, ulAbsoluteAddr, 0x1 );

				if( ( nCompare ) != (pnData[i] & 0xFF) )
				{
					bVerifyError = TRUE;
						break;
				}
			}
			else
			{
				return ErrorCode;
			}
		}

		// return appropriate error code if there was a verification error
		if( bVerifyError == TRUE )
			return VERIFY_WRITE;
	}
	// the user does not want to verify
	else
	{
		// write the buffer up to BUFFER_SIZE items
		for (i = 0; ( i < lCount ) && ( ErrorCode == NO_ERR ); i++, ulAbsoluteAddr += lStride)
				{

			// check to see that the address is within a valid sector
			CmdStruct.SGetSectNum.ulOffset = ulAbsoluteAddr;
			CmdStruct.SGetSectNum.pSectorNum = &ulSector;
			ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct );

			if( NO_ERR == ErrorCode )
			{
				// unlock the flash, do the write, increase shift, and wait for completion
				ErrorCode = m25p16_Write( (unsigned short*)&pnData[i], ulAbsoluteAddr, 0x1 );
			}
			else
			{
				return ErrorCode;
			}
		}
	}
	
	// return the appropriate error code
	return ErrorCode;
}


//----------- R e a d D a t a  ( ) ----------//
//
//  PURPOSE
//  	Read a buffer from flash device.
//
// 	INPUTS
//		unsigned long ulStart - address in flash to start the reads at
//		long lCount - number of elements to read, in this case bytes
//		long lStride - number of locations to skip between reads
//		int *pnData - pointer to data buffer to fill
//
//	RETURN VALUE
//  	ERROR_CODE - value if any error occurs during reading
//  	NO_ERR     - otherwise
//
//  CHANGES
//  	9-28-2005 Created

ERROR_CODE ReadData( unsigned long ulStart, long lCount, long lStride, int *pnData )
{

	long i = 0;						// loop counter
	ERROR_CODE ErrorCode = NO_ERR;	// tells whether there was an error trying to read
	unsigned long ulAbsoluteAddr;   // current address to read
	unsigned long ulSector = 0;		// sector number to verify address
	unsigned long ulMask =0xff;
	COMMAND_STRUCT	CmdStruct;		//structure for GetSectStartEnd

	ulAbsoluteAddr = FLASH_START_ADDR + ulStart;

	// read the buffer up to BUFFER_SIZE items
	for (i = 0; (i < lCount) && (i < BUFFER_SIZE); i++, ulAbsoluteAddr += lStride)
	{
		// check to see that the address is within a valid sector
		CmdStruct.SGetSectNum.ulOffset = ulAbsoluteAddr;
		CmdStruct.SGetSectNum.pSectorNum = &ulSector;
		ErrorCode = m25p16_Control( CNTRL_GET_SECTNUM, &CmdStruct );

		if( NO_ERR == ErrorCode )
		{
			ErrorCode = m25p16_Read( (unsigned short*)&pnData[i], ulAbsoluteAddr, 0x1 );
		}
		else
		{
			return ErrorCode;
		}
	}

	// return the appropriate error code
	return ErrorCode;
}
