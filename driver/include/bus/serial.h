#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <common/include.h>

#ifdef __cplusplus
extern "C"{
#endif


#define SERIAL_FLAG_IRQ_TX           0x01
#define SERIAL_FLAG_IRQ_RX           0x02
#define SERIAL_FLAG_DMA_TX           0x04
#define SERIAL_FLAG_DMA_RX           0x08
#define SERIAL_FLAG_INIT             0x10

#define SERIAL_EVENT_IRQ_TX           1
#define SERIAL_EVENT_IRQ_RX           2
#define SERIAL_EVENT_DMA_TX           3
#define SERIAL_EVENT_DMA_RX           4

#define SERIAL_RB_BUFSZ               100

typedef enum{
    SERIAL_PARITY_NONE,
    SERIAL_PARITY_EVEN,
    SERIAL_PARITY_ODD,
}serial_parity_t;

typedef enum{
    SERIAL_DATABITS_7,
    SERIAL_DATABITS_8,
    SERIAL_DATABITS_9,
}serial_databits_t;

typedef enum{
    SERIAL_STOPBITS_1,
    SERIAL_STOPBITS_1_5,
    SERIAL_STOPBITS_2,
}serial_stopbits_t;


#define SERIAL_CONFIG_DEFAULT       \
{                                   \
    115200,                         \
    SERIAL_DATABITS_8,              \
    SERIAL_PARITY_NONE,             \
    SERIAL_STOPBITS_1,              \
}

struct serial_config{
    int baudrate;
    serial_databits_t databits;
    serial_parity_t parity;
    serial_stopbits_t stopbits;
};

struct serial_fifo{
    pi_uint8_t *buffer;
    int put_index,get_index;
    int bufsz;
};

struct serial{
    pi_uint8_t flags;
    struct serial_config config;
    void *serial_rx;
    void *serial_tx;
    void *private_data;
    const struct serial_operations* ops;
};

struct serial_operations{
    int (*init)(struct serial* serial);
    int (*putc)(struct serial* serial, int ch);
    int (*getc)(struct serial* serial);
    int (*config)(struct serial* serial, int cmd, void* arg);
};

int serial_init(struct serial* serial);

int serial_write(struct serial* serial, const pi_uint8_t* wr_data, pi_size_t wr_len);

int serial_read(struct serial* serial, pi_uint8_t* rd_data, pi_size_t rd_len);

void serial_hw_isr(struct serial* serial, pi_uint8_t event);

#ifdef __cplusplus
}
#endif
#endif