#ifndef __PI_STDIO_H__
#define __PI_STDIO_H__
#include <common/pidef.h>
#ifdef __cplusplus
extern "C"{
#endif

#ifndef PI_NEWLINE
#define PI_NEWLINE  "\r\n"
#endif



typedef struct pi_stdio{
    void* data;
    int (*write)(struct pi_stdio* stdio, const char* wr_data, pi_size_t wr_len);
    int (*read)(struct pi_stdio* stdio, char* rd_data, pi_size_t rd_len);
}pi_stdio_t;

struct pi_stdio* pi_set_stdio(struct pi_stdio* stdio);

int pi_stdio_putc(struct pi_stdio* stdio, int ch);

int pi_stdio_getc(struct pi_stdio* stdio);

int pi_stdio_write(struct pi_stdio* stdio, const char* wr_data, pi_size_t wr_len);

int pi_stdio_read(struct pi_stdio* stdio, char* rd_data, pi_size_t rd_len);

int pi_print(const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif