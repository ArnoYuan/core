/**
 * This is the difinition of iic bit
 *
 * Author: Arno
 *
 * Created 2018/07/23
 */
#ifndef __IIC_BIT_H__
#define __IIC_BIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <common/pidef.h>
#include "iic.h"

struct iic_bit{
	int delay_us;
	const struct iic_bit_operations *ops;
};

struct iic_bit_operations{
	int (*scl_set)(int state);
	int (*sda_set)(int state);
	int (*sda_get)(void);
    void (*enable_output)(void);
    void (*enable_input)(void);
	void (*udelay)(int us);
};

int iic_bit_init(struct iic_bit* iic_bit, struct iic* iic);

#ifdef __cplusplus
}
#endif
#endif
