/*
 * File:        proj/common/setget.h
 * Purpose:     Definitions for common parameters
 *
 * Notes:
 */

#ifndef _SETGET_H_
#define _SETGET_H_

/********************************************************************/

#define FILENAME_SIZE   (40)
#define KCL_SIZE        (256)

/* 
 * Structure definition for the Parameters 
 */
typedef struct
{
#ifdef KERNEL_COMMAND_LINE
    char    kcl[KCL_SIZE];
#endif /* KCL */
    uint32  baud;
#ifdef CPU_MCF5208    
    uint8   wdt_enable;
#endif    
    uint8   client[4];
    uint8   server[4];
    uint8   gateway[4];
    uint8   netmask[4];
#ifdef CPU_MCF5208    
    uint8   dns[4];
#endif    
    uint8   netcfg[4];
    uint8   ethaddr[6];
    char    filename[FILENAME_SIZE];
} PARAM;

/********************************************************************/

#endif /* _SETGET_H_ */

