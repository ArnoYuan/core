/**
 * This is the difinition of iic
 * 
 * Author: Arno
 *
 * Created 2018/07/23
 */
#ifndef __IIC_H__
#define __IIC_H__
#include <common/include.h>

#ifdef __cplusplus
extern "C"{
#endif

#define IIC_BYTE_ADDRESS	0
#define IIC_WORD_ADDRESS	1


typedef enum{
	IIC_ACK = 0,
	IIC_NACK,
}iic_ack_t;

struct iic{
    void* private_data;
    const struct iic_operations *ops;
};

struct iic_operations{
  int (*init)(struct iic* iic);
	int (*start)(struct iic *iic);
	int (*stop)(struct iic *iic);
	int (*writeb)(struct iic *iic, pi_uint8_t data, iic_ack_t *ack);
	int (*readb)(struct iic *iic, pi_uint8_t *data, iic_ack_t ack);
};

int iic_init(struct iic* iic);

/**
  * @brief This function is used to write data to iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address.
  * @param data: data to be write.
  * @param len: length of data.
  * @retval 0 for success, otherwise -1.
  */
int iic_write(struct iic *iic, pi_uint8_t dev_addr, const pi_uint8_t *data, pi_size_t len);

/**
  * @brief This function is used to read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address.
  * @param data: data to be read.
  * @param len: length of data.
  * @retval 0 for success, otherwise -1.
  */
int iic_read(struct iic *iic, pi_uint8_t dev_addr, pi_uint8_t *data, pi_size_t len);

/**
  * @brief This function is used to write and read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address
  * @param address: slave device memory or register address.
  * @param flag: address option, 0 for 7 bit address, 1 for 10 bit address.
  * @param wr_data: data to be write.
  * @param wr_len: length of data to be write.
  * @retval 0 for success, otherwise -1.
  */
int iic_address_write(struct iic *iic, pi_uint8_t dev_addr, pi_uint16_t address, pi_uint8_t flag, pi_uint8_t *wr_data, pi_size_t wr_len);

/**
  * @brief This function is used to write and read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address
  * @param address: slave device memory or register address.
  * @param flag: address option, 0 for 7 bit address, 1 for 10 bit address.
  * @param wr_data: data to be write.
  * @param wr_len: length of data to be read.
  * @retval 0 for success, otherwise -1.
  */
int iic_address_read(struct iic *iic, pi_uint8_t dev_addr, pi_uint16_t address, pi_uint8_t flag, pi_uint8_t *rd_data, pi_size_t rd_len);

#ifdef __cplusplus
}
#endif
#endif
