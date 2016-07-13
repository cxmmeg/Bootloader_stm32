#ifndef _FLASH_H_
#define _FLASH_H_

#include "../common/comfunc.h"

#define STM_FLASH_SIZE		(64*SIZE_1K)
#define STM_PAGE_SIZE		(1*SIZE_1K)
#define BASE_SPACE			0x8000000
#define USER_SPACE			(0x8000000+8*SIZE_1K)
#define ApplicationAddress	USER_SPACE
#define IMAGE_SIZE	        (64*SIZE_1K)//可接收最大BIN文件


int flash_read(int iAddress, uint8_t *buf, int len);

#endif
