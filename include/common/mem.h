#ifndef __PI_MEM_H__
#define __PI_MEM_H__
#include <common/pidef.h>

#ifdef __cplusplus
extern "C"{
#endif

void *pi_mem_alloc(pi_size_t size);

void *pi_mem_realloc(void *ptr,pi_size_t size);

void pi_mem_free(void *ptr);
#ifdef __cplusplus
}
#endif
#endif