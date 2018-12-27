#include <mt_types.h>
//#ifdef CONFIG_AT_CMD_FUNC

#define isnum(c)    (c>=48 && c<=57) //ascii 0~9
#define upcase(c)   (c>=65 && c<=90) //ascii A~Z
#define lowcase(c)  (c>=97 && c<=122)//ascii a~z
#define AT_MAX_ARGV      15

enum
{
    NUL   = 0x00,
    BS    = 0x08,
    TAB   = 0x09,
    LF    = 0x0a,
    CR    = 0x0d,
    ESC   = 0x1b,
    SPACE = 0X20,
    UP    = 0x41,//ESC[A
    DOWN  = 0x42,//ESC[B
    RIGHT = 0x43,//ESC[C
    LEFT  = 0x44,//ESC[D
    DEL   = 0x7f,
};

enum
{
    STATE_ESC_NONE  = 0,
    STATE_ESC_START = 1,
    STATE_ESC_CODE  = 2,
};

enum
{
    at_ERR_TRANS       =  2,
    at_ERR_HELP        =  1,
    at_ERR_OK          =  0,
    at_ERR_OP          = -1,
    at_ERR_PARM        = -2,
    at_ERR_FAIL        = -3,
    at_ERR_LAST        = -4,
};

typedef struct
{
    const char *atcmd;
    int (*func) (int argc, char *argv[], char *op[]);
    const char *msg;
} atcmdt;


int uart_recv_sem_give(int ev_num);
int uart_recv_sem_wait(void);
void at_cmd_start(void);
int at_cmd_loop(char *at_cmd_buf, char *at_cmd_len, char *at_cmd_state);
int at_cmd_proc(int argc, char **argv, char *op[]);
int get_at_args(const char *string, char *argv[], char *op[]);
void at_cmd_thread(void *arg);
void at_cmd_init(void);
int at_printf(char *fmt, ...);
//#endif
