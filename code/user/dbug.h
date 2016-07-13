/*
 * File:        src/include/dbug.h
 * Purpose:     Definitions and prototypes for dBUG.
 *
 * Notes:       Main header file for dBUG
 *
 */

#ifndef _DBUG_H_
#define _DBUG_H_

/********************************************************************/
#define ADDRESS   int
#define uint32    unsigned int
#define uint16   unsigned short
#define uint8    unsigned char
#define vuint32   volatile unsigned int
#define vuint8    volatile unsigned char
#define int32   int 
 
#define  __BOOT_START       0x08000000
#define  __BOOT_SIZE        0x00002000
#define  __USER_SPACE       (__BOOT_START + __BOOT_SIZE)
#define  __FLASH_SIZE       (0x20000)
#define  __USER_SIZE        (__FLASH_SIZE - __BOOT_SIZE)

#ifdef NULL
#undef NULL
#endif
#define NULL        (0)

char board_getchar (void);

void board_putchar (char);

int board_getchar_present (void);

void board_putchar_flush (void);

int board_getchar_timeout(char *c, int timeout);

void board_reset (void);

/*
 * Optional, needed only if used
 */
int board_get_baud (void);

void board_set_baud (int);

//void board_init (void);

void init_uart_chn(int baud);
/********************************************************************/
/*                                                                  */
/* Global messages/constant strings                                 */
/*                                                                  */
/********************************************************************/

extern const char COPYRIGHT[];
extern const char HELPMSG[];
extern const char INVARG[];
extern const char INVCMD[];
extern const char INVREG[];
extern const char INVALUE[];
/*
 * Maximum command line arguments
 */
#define UIF_MAX_ARGS    (50)

/*
 * Maximum length of the command line
 */
#define UIF_MAX_LINE    (256)

extern int BASE;

int get_value (char *, int *, int);
int make_argv (char *, char **);
/********************************************************************/
/*                                                                  */
/* Miscellaneous                                                    */
/*                                                                  */
/********************************************************************/
#define SIZE_8      (8)
#define SIZE_16     (16)
#define SIZE_32     (32)
#define SIZE_64     (64)

#define CTRL_XON    (0x11)
#define CTRL_XOFF   (0x13)
#define CTRL_BS     (0x08)
#define CTRL_LF     (0x0A)
#define CTRL_CR     (0x0D)
/********************************************************************/
/*                                                                  */
/* Standard library functions provided by dBUG                      */
/*                                                                  */
/********************************************************************/

int isspace (int);
int isalnum (int);
int isdigit (int);
int isupper (int);

int strcasecmp (const char *, const char *);
char *strcat (char *, const char *);
char *strcpy (char *, const char *);
int strcmp (const char *, const char *);
void *memcpy(void *, const void *, unsigned);
void *memset (void *, int, unsigned);
#define printf printD
#define sprintf sprintD

int printD (const char *, ... );
int sprintD (char *, const char *, ... );
void free (void *);
void *malloc (unsigned);

typedef const struct
{
    char *  cmd;                    /* command name user types, ie. GO  */
    int     unique;                 /* num chars to uniquely match      */
    int     min_args;               /* min num of args command accepts  */
    int     max_args;               /* max num of args command accepts  */
    int     flags;                  /* command flags (repeat, hidden)   */
    void    (*func)(int, char **);  /* actual function to call          */
    char *  description;            /* brief description of command     */
    char *  syntax;                 /* syntax of command                */
} UIF_CMD;

extern UIF_CMD UIF_CMDTAB[];
extern const int UIF_NUM_CMD;
#define UIF_CMDTAB_SIZE     (sizeof(UIF_CMDTAB)/sizeof(UIF_CMD))

#define UIF_CMD_FLAG_REPEAT     (0x0001)
#define UIF_CMD_FLAG_HIDDEN     (0x0002)

void uif_cmd_dn (int, char **);
void uif_cmd_reset (int, char **);
void uif_cmd_ver (int, char **);
void cmd_gofrom_flash (int, char **);

#ifndef UIF_CMD_DN
#define UIF_CMD_DN  \
    {"dn",2,0,4,0,uif_cmd_dn,"Download Network",\
    "<-c|e|i|s <-o offset>> <filename>"},
#endif

#ifndef UIF_CMD_RESET
#define UIF_CMD_RESET   \
    {"reset",5,0,0,0,uif_cmd_reset,"Reset",""},
#endif

/********************************************************************/
/*                                                                  */
/* Generic User InterFace Set/Show Commands                         */
/*                                                                  */
/********************************************************************/

typedef const struct
{
    char *  option;
    int     unique;
    int     min_args;
    int     max_args;
    int     flags;
    void    (*func)(int, char **);
    char *  description;            /* brief description of command     */
    char *  syntax;
} UIF_SETCMD;

#define UIF_SETCMD_FLAG_HIDDEN     (0x0001)

#endif

