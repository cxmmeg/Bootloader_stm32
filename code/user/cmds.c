/*
 * File:        cmds.c
 * Purpose:     Core dBUG commands for general purpose debugging.
 *
 * Notes:
 *
 */

#include <string.h>
#include <stdlib.h>
#include "dbug.h"
#include "misc.h"
#include "stm32f10x.h"
/********************************************************************/

#ifndef DEFAULT_MD_LINES
#define DEFAULT_MD_LINES    8
#endif

/********************************************************************/
uint32  (*cpu_read_data) (int address,int sz);
/*
 * Default address and size for memory display.
 */
ADDRESS md_last_address;

/********************************************************************/
/********************************************************************/
int get_value (char *s, int *success, int base)
{
    uint32 value;
    char *p;
    
    value = strtoul(s,&p,base);
    if ((value == 0) && (p == s))
    {
        *success = FALSE;
        return 0;
    }
    else
    {
        *success = TRUE;
        return value;
    }
}

/********************************************************************/
#define AIRCR_VECTKEY_MASK    ((u32)0x05FA0000)
void board_reset (void)
{
	SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x04;
}
void uif_cmd_reset (int argc, char **argv)
{
	printf("\n\n");
    board_reset(); 
}
/********************************************************************/

static const char BRD_TIME[]        = __TIME__ ;
static const char BRD_DATE[]        = __DATE__ ;
void uif_cmd_ver (int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("\r\n");
    printf("      bootloader process\r\n");
    printf("\r\n");
	printf("       Firmware V1.0 \n"); 
    printf("\r\n");
    printf("Built on:  %s %s \r\n", BRD_DATE, BRD_TIME);
    printf("\r\n");
}


