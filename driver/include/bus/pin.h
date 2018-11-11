/**
 * This is the difinition of iic
 * 
 * Author: Arno
 *
 * Created 2018/07/23
 */
#ifndef __PIN_H__
#define __PIN_H__
#include <common/pidef.h>

#ifdef __cplusplus
extern "C"{
#endif

struct pin{
    const struct pin_operations *ops;
    void* private_data;
};

typedef enum{
    PIN_DIR_OUTPUT,
    PIN_DIR_INPUT,
}pin_dir_t;

typedef enum{
    PIN_LEVEL_LOW,
    PIN_LEVEL_HIGH,
}pin_level_t;

struct pin_operations{
    int (*init)(struct pin* pin);
    int (*set_direction)(struct pin* pin, int pin_id, pin_dir_t dir);
    int (*get_direction)(struct pin* pin, int pin_id, pin_dir_t *dir);
    int (*get_output)(struct pin* pin, int pin_id, pin_level_t *level);
    int (*set_level)(struct pin* pin, int pin_id, pin_level_t level);
    int (*get_level)(struct pin* pin, int pin_id, pin_level_t *level);
};

int pin_init(struct pin* pin);

int pin_get_direction(struct pin* pin, int pin_id);

int pin_get_output(struct pin* pin, int pin_id);

int pin_set_level(struct pin* pin, int pin_id, pin_level_t level);

int pin_get_level(struct pin* pin, int pin_id);

#ifdef __cplusplus
}
#endif

#endif