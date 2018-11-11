#include <common/include.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*Macro variables----------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_fputchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

static struct pi_stdio* pi_stdio = PI_NULL;

#ifndef CONFIG_PI_PRINT_BUF_SIZE
#define CONFIG_PI_PRINT_BUF_SIZE    100
#endif

static char pi_stdio_buffer[CONFIG_PI_PRINT_BUF_SIZE];

struct pi_stdio* pi_set_stdio(struct pi_stdio* stdio)
{
    struct pi_stdio* tstdio = pi_stdio;
    pi_stdio = stdio;
    return tstdio;
}

int pi_stdio_putc(struct pi_stdio* stdio, int ch)
{
    return pi_stdio_write(stdio, (const char*)&ch, 1);
}

int pi_stdio_getc(struct pi_stdio* stdio)
{
    int ch = 0;

    int ret = pi_stdio_read(stdio, (char*)&ch, 1);

    if(ret < 0)
        return ret;

    return ch;
}

int pi_stdio_write(struct pi_stdio* stdio, const char* wr_data, pi_size_t wr_len)
{
    if(stdio == PI_NULL || stdio->write == PI_NULL)
        return 0;

    return stdio->write(stdio, wr_data, wr_len);
}

int pi_stdio_read(struct pi_stdio* stdio, char* rd_data, pi_size_t rd_len)
{
    if(stdio == PI_NULL || stdio->read == PI_NULL)
        return 0;

    return stdio->read(stdio, rd_data, rd_len);
}

int pi_print(const char* fmt, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, fmt);
    ret = vsnprintf((char*)pi_stdio_buffer, sizeof(pi_stdio_buffer) - 1, fmt, args);
    va_end(args);

    return pi_stdio_write(pi_stdio, (const char*)pi_stdio_buffer, ret);
}


PUTCHAR_PROTOTYPE
{
    pi_stdio_putc(pi_stdio, ch);
    return ch;
}