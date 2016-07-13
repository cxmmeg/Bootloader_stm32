/*
 * File:        proj/common/setget.c
 * Purpose:     Board support functions for the global set/show routines
 *
 * Notes:       
 *
 */

#include "../dbug.h"
#include "setget.h"


/********************************************************************/
 

/********************************************************************/
void
board_set_baud (int baud)
{ 
    printf("serial baud change to:%d,make sure to change the pc software!!!\n",baud);
    curbaud = baud; 
    init_uart_chn(curbaud);
}

/********************************************************************/
int
board_get_baud (void)
{
    return(curbaud);
}

/********************************************************************/

#ifdef DBUG_NETWORK

/********************************************************************/
void
board_set_ethaddr (uint8 ethaddr[])
{
    PARAM *tparam = (PARAM *)NULL;

    update_board_params(ethaddr, 6, (uint32)&tparam->ethaddr);
}

/********************************************************************/
uint8 *
board_get_ethaddr (uint8 ethaddr[])
{
    int i;

    for (i = 0; i < 6; i++)
    {
        ethaddr[i] = ((PARAM *)PARAMS_ADDRESS)->ethaddr[i];
    }
    return ethaddr;
}

/********************************************************************/
static int
parse_ethaddr (char *ethstr, uint8 *ethaddr)
{
    int i, j, l, ti;
    char *m;
    char tstr[3];
    unsigned long ul;

    l = strlen(ethstr) + 1;
    j = 0;
    ti = 0;

    for (i = 0; i < l; i++)
    {
        if ((ethstr[i] != ':') && (ethstr[i] != '\0'))
        {
            if (ti >= 2)
            {
                return 0;
            }
            else
            {
                tstr[ti++] = ethstr[i];
            }
        }
        else
        {
            /*
             * convert string portion started at 'p'
             */
            tstr[ti] = '\0';
            ti = 0;
            ul = strtoul(tstr,&m,16);
            if ((ul == 0) && (m == tstr))
            {
                return 0;
            }
            ethaddr[j++] = (uint8)ul;
        }
    }
    if (j == 6)
        return 1;
    else
        return 0;
}
 
#endif /* DBUG_NETWORK */

/********************************************************************/
