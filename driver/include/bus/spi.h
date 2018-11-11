/**
 * This is the difinition of spi
 *
 * Author: Arno
 *
 * Created 2018/07/28
 */
#ifndef __SPI_H__
#define __SPI_H__
#include <common/include.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3,
}spi_mode_t;

struct spi{
    pi_uint8_t bus;
    int speed;
    spi_mode_t mode;
    void* private_data;
    const struct spi_operations *ops;
};

struct spi_operations{
    int (*init)(struct spi* spi);
    int (*config)(struct spi* spi);
    int (*write)(struct spi* spi, pi_uint8_t* wr_data, pi_size_t len);
    int (*read)(struct spi* spi, pi_uint8_t* rd_data, pi_size_t len);
    int (*transfer)(struct spi* spi, pi_uint8_t *wr_data, pi_uint8_t *rd_data, pi_size_t len);
    void (*udelay)(struct spi* spi, int us);
};


int spi_init(struct spi *spi);

/**
  * @brief This function is used to set spi mode.
  * @param spi: spi bus.
  * @param mode: spi mode.
  * @retval 0 for success, otherwise -1.
  */
int spi_set_mode(struct spi* spi, spi_mode_t mode);

/**
  * @brief This function is used to set spi speed.
  * @param spi: spi bus.
  * @param mode: spi speed.
  * @retval 0 for success, otherwise -1.
  */
int spi_set_speed(struct spi* spi, int speed);

/**
  * @brief This function is used to write data to spi.
  * @param spi: spi bus pointer.
  * @param wr_data: data to write.
  * @param wr_len: length of data to write.
  * @retval 0 for success, otherwise -1.
  */
int spi_write(struct spi* spi, pi_uint8_t *wr_data, pi_size_t wr_len);

/**
  * @brief This function is used to read data from spi.
  * @param spi: spi bus pointer.
  * @param rd_data: data read.
  * @param rd_len: length of data to read.
  * @retval 0 for success, otherwise -1.
  */
int spi_read(struct spi* spi, pi_uint8_t *rd_data, pi_size_t rd_len);

/**
  * @brief This function is used to read data from spi.
  * @param spi: spi bus pointer.
  * @param wr_data: data to write.
  * @param rd_data: data read.
  * @param len: length of data to write and read.
  * @retval 0 for success, otherwise -1.
  */
int spi_transfer(struct spi* spi, pi_uint8_t *wr_data, pi_uint8_t *rd_data, pi_size_t len);

#ifdef __cplusplus
}
#endif
#endif
