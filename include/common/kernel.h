#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <common/pidef.h>

#ifdef __cplusplus
extern "C"{
#endif

void pi_kernel_start(void);

void pi_kernel_periodic(void);

#ifdef __cplusplus
}
#endif
#endif