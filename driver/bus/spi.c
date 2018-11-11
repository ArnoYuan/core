#include <bus/spi.h>
#include <common/include.h>


pi_inline int spi_config(struct spi *spi)
{
    if(spi->ops->config)
        return spi->ops->config(spi);
    return 0;
}

int spi_init(struct spi *spi)
{   
    if(spi->ops->init)
        return spi->ops->init(spi);
    return 0;
}

int spi_set_mode(struct spi *spi, spi_mode_t mode)
{
    pi_assert(spi);
    spi->mode = mode;
    return spi_config(spi);
}

int spi_set_speed(struct spi *spi, int speed)
{
    pi_assert(spi);
    spi->speed = speed;
    return spi_config(spi);
}

int spi_write(struct spi *spi, pi_uint8_t *wr_data, pi_size_t wr_len)
{
    pi_assert(spi);
    pi_assert(wr_data);
    pi_assert(spi->ops->transfer);

    return spi->ops->write(spi, wr_data, wr_len);
}

int spi_read(struct spi *spi, pi_uint8_t *rd_data, pi_size_t rd_len)
{
    pi_assert(spi);
    pi_assert(wr_data);
    pi_assert(spi->ops->read);

    return spi->ops->read(spi, rd_data, rd_len);
}

int spi_transfer(struct spi *spi, pi_uint8_t *wr_data, pi_uint8_t *rd_data, pi_size_t len)
{
    pi_assert(spi);
    pi_assert(wr_data);
    pi_assert(spi->ops->transfer);

    return spi->ops->transfer(spi, wr_data, rd_data, len);
}
