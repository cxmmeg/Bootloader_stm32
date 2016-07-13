#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "flash.h"


//---------------------------------------------------------------------------------------------
int flash_erase(int addr, int len)
{
    int i, page_nbr;
    FLASH_Status FLASHStatus = FLASH_COMPLETE;

    if ((len % STM_PAGE_SIZE) != 0)
        page_nbr = (len / STM_PAGE_SIZE) + 1;
    else
        page_nbr = len / STM_PAGE_SIZE;

    for (i = 0; i < page_nbr; i++)
    {
        do
        {
            FLASHStatus = FLASH_ErasePage(addr + STM_PAGE_SIZE * i);
        }
        while (FLASHStatus != FLASH_COMPLETE);
    }
    return len;
}

//---------------------------------------------------------------------------------------------
int flash_prog(int addr, const void *data, int len)
{
    int i;
    const uint8_t *d = data;

    for (i = 0; i < len; i += 4)
    {
        FLASH_ProgramWord(addr + i, *(uint32_t *) (d + i));
    }
    return len;
}

//---------------------------------------------------------------------------------------------
int flash_read(int iAddress, u8 *buf, int len)
{
        int i = 0;
        while(i < len ) {
           *(buf + i) = *(u8*) iAddress++;
           i++;
        }
        return i;
} 

