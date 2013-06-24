/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "stm32f4_sdio.h"	/* Example: MMC/SDC contorl */
#include "stm32f4_sdio.h"

#define BlockSize            512 /* Block Size in Bytes [這裡的block含義是“扇區”，SD卡的每個扇區為512字節]*/
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	SD_Error Status;

	Status = SD_Init();
	if(Status == SD_OK)
		Status = SD_GetCardInfo(&SDCardInfo);
  if(Status == SD_OK)
		Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  if(Status == SD_OK)
    Status = SD_EnableWideBusOperation(SDIO_BusWide_1b);
	if(Status == SD_OK)
		return 0;
  else
		return STA_NOINIT;
}
/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	SD_Error Status;

	if(count==1)
		Status = SD_ReadBlock( buff ,sector<< 9 , SDCardInfo.CardBlockSize );                                                              
	else
		Status = SD_ReadMultiBlocks( buff ,sector<< 9 ,SDCardInfo.CardBlockSize,count);
/*
	if(count==1)
		SD_ReadBlock(sector << 9 ,(u32 *)(&buff[0]),BlockSize);
	else
		SD_ReadMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),BlockSize,count);
*/
	if(Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	SD_Error Status;

	if(count==1)
		Status = SD_WriteBlock((uint8_t *)buff ,sector << 9 , SDCardInfo.CardBlockSize);                                                
	else {
		Status = SD_WriteMultiBlocks((uint8_t *)buff ,sector << 9 ,SDCardInfo.CardBlockSize,count);	  
		Status = SD_WaitWriteOperation();
	}
/*
	if(count==1)
		SD_WriteBlock(sector << 9 ,(u32 *)(&buff[0]),BlockSize);
	else
		SD_WriteMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),BlockSize,count);
*/
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
	if(Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(drv)
		return RES_PARERR;  /* 僅支持單磁盤操作，否則返回參數錯誤 */

	switch(ctrl) {
	  case CTRL_SYNC:
			return RES_OK;
	  case GET_SECTOR_COUNT:
	    *(DWORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case GET_BLOCK_SIZE:
	    *(WORD*)buff = SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case CTRL_POWER:
			return RES_OK;
	  case CTRL_LOCK:
			return RES_OK;
	  case CTRL_EJECT:
			return RES_OK;
      /* MMC/SDC command */
	  case MMC_GET_TYPE:
			return RES_OK;
	  case MMC_GET_CSD:
			return RES_OK;
	  case MMC_GET_CID:
			return RES_OK;
	  case MMC_GET_OCR:
			return RES_OK;
	  case MMC_GET_SDSTAT:
			return RES_OK;
	}

	return RES_PARERR;

}
DWORD get_fattime (void)
{
	return 0;
}
