#ifndef __PI_ASSERT_H__
#define __PI_ASSERT_H__
#include <common/pidef.h>
#ifdef __cplusplus
extern "C"{
#endif

#ifdef CONFIG_PI_USING_ASSERT
#define pi_assert(expr)     ((expr)?(void)0:pi_assert_failed((pi_uint8_t *)__FILE__, __LINE__)
pi_weak void pi_assert_failed(pi_uint8_t *file, pi_uint32_t line);
#else
#define pi_assert(expr)     ((void)0) 
#endif

#ifdef __cplusplus
}
#endif
#endif