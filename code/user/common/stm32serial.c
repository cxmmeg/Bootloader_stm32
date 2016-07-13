/*
 * File:        mcfserial.c
 * Purpose:     Provide common ColdFire UART routines for dBUG serial IO
 *
 * Notes:       These functions are placed into a special section that is
 *              copied from ROM to RAM at starup so that the IO functions
 *              can be executed while the Flash is erased, programmed, etc.
 */
//#include <stm32f10x_lib.h>
#include "../dbug.h"
#include "stm32f10x.h"

/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 * Return Values:
 *  the received character
 */
char board_getchar (void)
{
    /* Wait until character has been received */
    while (! USART_GetFlagStatus(USART1,USART_FLAG_RXNE)); 
    return  USART_ReceiveData(USART1) ;  
}
/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 */ 
void board_putchar (char ch)
{
    while (!(USART_GetFlagStatus(USART1,USART_FLAG_TXE)));
	USART_SendData(USART1, ch);  
}
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int board_getchar_present (void)
{ 
	USART_GetFlagStatus(USART1, USART_FLAG_ORE);
	return (USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
}
/********************************************************************/
/*
 * Flush the UART FIFO - this function is empty because the UART is 
 * never paused so characters in the FIFO are always sent as soon as
 * possible
 */
void board_putchar_flush (void)
{
}
/********************************************************************/
int board_getchar_timeout(char *c, int timeout)
{
    while (timeout-- > 0)
    {
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
        {
            *c = (uint8_t) USART1->DR;
            return 0;
        }
        //IWDG_ReloadCounter();
    }
    return -1;
}    
