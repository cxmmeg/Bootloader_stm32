
/********************************************************************/
//#include <stm32f10x_lib.h>
#include "dbug.h"
#include "main.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "flash.h"

/********************************************************************/
static const char PROMPT[] = "dBUG> ";
static const char SYNTAX[] = "Error: Invalid syntax for: %s\n";

static char cmdline1 [UIF_MAX_LINE];
static char cmdline2 [UIF_MAX_LINE];

int curbaud = 115200;
/********************************************************************/
int make_argv (char *cmdline, char *argv[])
{
	int argc, i, in_text;

    argc = 0;
    i = 0;
    in_text = FALSE;
    while (cmdline[i] != '\0')  /* getline() must place 0x00 on end */
    {
		if (((cmdline[i] == ' ') || (cmdline[i] == '\t')) )
        {
            if (in_text)
            {
                cmdline[i] = '\0';
                in_text = FALSE;
            }
            else
            {
                /* still looking for next argument */
            }
        }
        else
        {
            /* got non-whitespace character */
            if (in_text)
            {
            }
            else
            {
                /* start of an argument */
                in_text = TRUE;
                if (argc < UIF_MAX_ARGS)
                {
                    if (argv != NULL)
                        argv[argc] = &cmdline[i];
                    argc++;
                }
                else
                    /*return argc;*/
                    break;
            }

        }
        i++;    /* proceed to next character */
    }
    if (argv != NULL)
	{
        argv[argc] = NULL;
	}
    return argc;
}
/********************************************************************/
char *get_history_line (char *userline)
{
    char line[UIF_MAX_LINE];
    int pos, ch;
 
    pos = 0;

    ch = (int)board_getchar();
    while ( (ch != 0x0D /* 回车 */) &&
            (ch != 0x0A /* 换行 */) &&
            (pos < UIF_MAX_LINE))
    {
        switch (ch)
        {
            case 0x08:      /* Backspace */
            case 0x7F:      /* Delete */
                if (pos > 0)
                {
                    pos -= 1;
                    board_putchar(0x08);    /* backspace */
                    board_putchar(' ');
                    board_putchar(0x08);    /* backspace */
                }
                break;
              
            default:
                if ((pos+1) < UIF_MAX_LINE)
                {
                    /* only printable characters */
                    if ((ch > 0x1f) && (ch < 0x80))
                    {
                        line[pos++] = (char)ch;
                        board_putchar((char)ch);
                    }
                }
                break;
        } 
        ch = (int)board_getchar();
    }
    line[pos] = '\0';
    board_putchar(0x0D);    /* CR */
    board_putchar(0x0A);    /* LF */

    strcpy(userline,line);

    return userline;
}
/********************************************************************/
void run_cmd (void)
{
    int argc;
    char *argv[UIF_MAX_ARGS + 1];   /* one extra for null terminator */

    get_history_line(cmdline1);

    if (!(argc = make_argv(cmdline1,argv)))
    {
        /* no command entered, just a blank line */
        strcpy(cmdline1,cmdline2);
        argc = make_argv(cmdline1,argv);
    }
    cmdline2[0] = '\0';

    if (argc)
    {
		int i;
        /*
         * First try for an exact match on command name
         */
        for (i = 0; i < UIF_NUM_CMD; i++)
        {
            if (strcasecmp(UIF_CMDTAB[i].cmd, argv[0]) == 0)
            {
                if (((argc-1) >= UIF_CMDTAB[i].min_args) &&
                    ((argc-1) <= UIF_CMDTAB[i].max_args))
                {
                    if (UIF_CMDTAB[i].flags & UIF_CMD_FLAG_REPEAT)
                    {
                        strcpy(cmdline2,argv[0]);
                    }
                    UIF_CMDTAB[i].func(argc, argv);
                    return;
                }
                else
                {
                    printf(SYNTAX,argv[0]);
                    return;
                }
            }
        }
        printf("\n");
        printf(INVCMD,argv[0]);
        printf("\n");
        printf(HELPMSG);
    }
}
/********************************************************************/
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK  72
#endif
/* mdelay emulates a 1msec delay, it's not very accurate! */
void mdelay(int msec)
{
    unsigned int i;

    for(i = (SYSTEM_CLOCK * 100 * msec); i; i--);

}
/********************************************************************/
void auto_boot(void)
{
    int boot_timeout = 50;  /* Number of 100msec intervals to wait */
    char c;
    printf("按下's'键以启动 bootloader.\n");

    while(boot_timeout-- > 0)
    {
        printf("%1d\r",((boot_timeout/10)+1));
        /* check keyboard after 100 msec */
        mdelay(100); 
        if (board_getchar_present())
        {
            c = board_getchar();
            if('s' == c) 
            {
                return;
            }
        }
    }
    printf("\n");
    #if 0
    flash_read(0x800FFFC,program_update_flag,4);
    flag = get_le_val(program_update_flag,4);
    if( 0xAA55AA55 == flag)
    #endif
    cmd_gofrom_flash(NULL, NULL);
}
/********************************************************************/
void mainloop (void)
{
    while (TRUE)
    {
		printf("\n");
        printf("     1、输入 dn    下载升级包。      \n");
        printf("     2、输入 reset 重启设备。		 \n");
        printf(PROMPT);
        run_cmd();
    }
}
void init_uart_chn(int baud)
{
	USART_InitTypeDef USART_InitStructure;

	USART_Cmd(USART1, DISABLE);
	USART_InitStructure.USART_BaudRate 		= baud;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 , &USART_InitStructure);
	USART_Cmd(USART1 , ENABLE);
}
void sys_uart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,		ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, 	ENABLE); 

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	init_uart_chn(curbaud);
}
int main (void) 
{ 
    sys_uart();
    cmdline1[0] = '\0';
    cmdline2[0] = '\0'; 
    uif_cmd_ver(NULL, NULL); 
    auto_boot();
    mainloop();
    return 0;   /* useless, should never reach! */
}
/********************************************************************/



