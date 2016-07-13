#include "dbug.h"
#include "stm32f10x_flash.h"
#include "stm32f10x.h"
#include "comfunc.h"  
#include "ymodem.h"
#include "flash.h"

/********************************************************************/
extern void board_reset (void);

/********************************************************************/
UIF_CMD UIF_CMDTAB[] =
{
    {"dn",	 2,0,4,0,uif_cmd_dn,"Download Package",""},
	{"reset",5,0,0,0,uif_cmd_reset,"Reset",""},
};
const int UIF_NUM_CMD = UIF_CMDTAB_SIZE;
/********************************************************************/
void uif_cmd_dn (int argc, char **argv)
{
    int ret;
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    ret = Ymodem_receive(ApplicationAddress, IMAGE_SIZE);
    if (ret > 0)
    {
        //FLASH_ProgramWord(0x800FFFC, 0xAA55AA55);
        mdelay(100);
        board_reset();
    }
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
/********************************************************************/
typedef void(*pfunc)(void);
void cmd_gofrom_flash (int argc, char **argv)
{
	unsigned int user_pc,user_sp; 

	printf("跳转到主程序! \n");
    user_sp = *(volatile unsigned int*)(__USER_SPACE + 0x0000);
	user_pc = *(volatile unsigned int*)(__USER_SPACE + 0x0004);
	/* Set the Vector Table base location at 0x08004000 */ 
	printf("~\n\n");
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
	__set_MSP(user_sp);
   ((pfunc)user_pc)();
}

