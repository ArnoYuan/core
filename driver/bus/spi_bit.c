#include <common/include.h>
#include <bus/spi_bit.h>
#include <bus/spi.h>


#ifdef SPI_DEBUG
#define spi_dbg(fmt, ...)   kernel_printf(fmt, ##__VA_ARGS__)
#else
#define spi_dbg(fmt, ...)
#endif

#define sck_set(ops, state) (ops->sck_set(state))
#define mosi_set(ops, state) (ops->mosi_set(state))
#define miso_get(ops)        (ops->miso_get())

#define SCK_H(ops)          sck_set(ops, 1)
#define SCK_L(ops)          sck_set(ops, 0)
#define MOSI_H(ops)          mosi_set(ops, 1)
#define MOSI_L(ops)          mosi_set(ops, 0)
#define MISO_STATE(ops)      miso_get(ops)

pi_inline void spi_udelay(const struct spi_bit_operations *ops, int us)
{
    if(ops->udelay)
        ops->udelay(us);
}

pi_inline struct spi_bit *spi_to_spi_bit(struct spi *spi)
{
    return spi->private_data;
}

int spi_bit_config(struct spi* spi)
{
    struct spi_bit* spi_bit = spi_to_spi_bit(spi);
    if(spi_bit->ops->config)
        spi_bit->ops->config(spi->mode);
    spi_bit->delay_us = 1000000 / spi->speed / 2;
    return 0;
}

pi_inline int spi_read_writeb(struct spi_bit* spi_bit, pi_uint8_t wr_data, spi_mode_t mode)
{
    pi_uint8_t rd_data = 0;

    spi_udelay(spi_bit->ops, spi_bit->delay_us);
    if(mode == SPI_MODE_0)
    {
        // CPOL=0, CPHA=0
        for(int i=0; i < 8; i++)
        {
            if(wr_data & 0x80)
                MOSI_H(spi_bit->ops);
            else
                MOSI_L(spi_bit->ops);
            wr_data <<= 1;
            spi_udelay(spi_bit->ops, spi_bit->delay_us);
            SCK_H(spi_bit->ops);
            spi_udelay(spi_bit->ops, spi_bit->delay_us);
            rd_data <<= 1;
            if(MISO_STATE(spi_bit->ops))
                rd_data |= 1;
            SCK_L(spi_bit->ops);
        }
    }
    else if(mode == SPI_MODE_1)
    {
        //CPOL=0, CPHA=1
        for(int i=0; i < 8; i++)
        {
            SCK_H(spi_bit->ops);

            if(wr_data & 0x80)
                MOSI_H(spi_bit->ops);
            else
                MOSI_L(spi_bit->ops);
            wr_data <<= 1;

            spi_udelay(spi_bit->ops, spi_bit->delay_us);
            rd_data <<= 1;
            if(MISO_STATE(spi_bit->ops))
                rd_data |= 1;

            SCK_L(spi_bit->ops);
            spi_udelay(spi_bit->ops, spi_bit->delay_us);

        }
    }
    else if(mode == SPI_MODE_2)
    {
        //CPOL=1, CPHA=0
        for(int i=0; i < 8; i++)
        {
            if(wr_data & 0x80)
                MOSI_H(spi_bit->ops);
            else
                MOSI_L(spi_bit->ops);
            wr_data <<= 1;
            spi_udelay(spi_bit->ops, spi_bit->delay_us);
            SCK_L(spi_bit->ops);
            spi_udelay(spi_bit->ops, spi_bit->delay_us);
            rd_data <<= 1;
            if(MISO_STATE(spi_bit->ops))
                rd_data |= 1;
            SCK_H(spi_bit->ops);
        }
    }
    else if(mode == SPI_MODE_3)
    {
        //CPOL=1, CPHA=1
        for(int i=0; i < 8; i++)
        {
            SCK_L(spi_bit->ops);

            if(wr_data & 0x80)
                MOSI_H(spi_bit->ops);
            else
                MOSI_L(spi_bit->ops);
            wr_data <<= 1;

            spi_udelay(spi_bit->ops, spi_bit->delay_us);

            SCK_H(spi_bit->ops);

            spi_udelay(spi_bit->ops, spi_bit->delay_us);

            rd_data <<= 1;
            if(MISO_STATE(spi_bit->ops))
                rd_data |= 1;
        }
    }
    spi_udelay(spi_bit->ops, spi_bit->delay_us);
    return rd_data;        
}


int spi_bit_write(struct spi* spi, pi_uint8_t* wr_data, pi_size_t len)
{
    struct spi_bit* spi_bit = spi_to_spi_bit(spi);

    for(int i=0; i < len; i++)
    {
        spi_read_writeb(spi_bit, *wr_data++, spi->mode);
    }

    return len;
}

int spi_bit_read(struct spi* spi, pi_uint8_t* rd_data, pi_size_t len)
{
    struct spi_bit* spi_bit = spi_to_spi_bit(spi);

    for(int i=0; i < len; i++)
    {
        *rd_data++ = spi_read_writeb(spi_bit, 0xFF, spi->mode);
    }

    return len;
}

int spi_bit_transfer(struct spi* spi, pi_uint8_t *wr_data, pi_uint8_t *rd_data, pi_size_t len)
{
    struct spi_bit* spi_bit = spi_to_spi_bit(spi);

    for(int i=0; i < len; i++)
    {
        *rd_data++ = spi_read_writeb(spi_bit, *wr_data++, spi->mode);
    }

    return len;
}

const struct spi_operations spi_ops={
    .config = spi_bit_config,
    .write = spi_bit_write,
    .read = spi_bit_read,
    .transfer = spi_bit_transfer,
};

int spi_bit_init(struct spi_bit *spi_bit, struct spi* spi)
{
    spi->private_data = spi_bit;
    spi->ops = &spi_ops;
    return spi_init(spi);
}