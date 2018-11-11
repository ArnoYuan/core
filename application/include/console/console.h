#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <common/include.h>

typedef struct{
    char* buffer;
    pi_size_t bufsz;
}console_buffer_t;

typedef struct{
    pi_stdio_t* stdio;
    int rxpos;
    console_buffer_t rxbuf;
    console_buffer_t txbuf;
}console_t;

int console_init(console_t* console, pi_size_t stacksz);

#endif