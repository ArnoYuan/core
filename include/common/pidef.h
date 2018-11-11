#ifndef __PI_DEF_H__
#define __PI_DEF_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef signed   char                   pi_int8_t;      /**<  8bit integer type */
typedef signed   short                  pi_int16_t;     /**< 16bit integer type */
typedef signed   long                   pi_int32_t;     /**< 32bit integer type */
typedef unsigned char                   pi_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  pi_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   pi_uint32_t;    /**< 32bit unsigned integer type */
typedef int                             pi_bool_t;      /**< boolean type */

/* 32bit CPU */
typedef long                            pi_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   pi_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef pi_base_t                       pi_err_t;       /**< Type for error number */
typedef pi_uint32_t                     pi_time_t;      /**< Type for time stamp */
typedef pi_uint32_t                     pi_tick_t;      /**< Type for tick count */
typedef pi_base_t                       pi_flag_t;      /**< Type for flags */
typedef pi_ubase_t                      pi_size_t;      /**< Type for size number */
typedef pi_ubase_t                      pi_dev_t;       /**< Type for device */
typedef pi_base_t                       pi_off_t;       /**< Type for offset */
typedef pi_uint32_t                      pi_usec_t;
typedef pi_uint32_t                      pi_millis_t;

/* boolean type definitions */
#define PI_TRUE                         1               /**< boolean true  */
#define PI_FALSE                        0               /**< boolean fails */

/* maximum value of base type */
#define PI_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define PI_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define PI_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */
#define PI_TICK_MAX                     PI_UINT32_MAX   /**< Maxium number of tick */


#if defined(__ICCARM__)
#define pi_volatile       volatile
#define pi_inline          static inline
#define pi_weak            __weak
#elif defined(__GNUC__)
#define pi_volatile       volatile
#define pi_inline          static __inline
#define pi_weak            __attribute__((weak))
#elif defined(__CC_ARM)
#define __kernell_volatile        __volatile
#define pi_inline           __inline
#define pi_weak            __weak
#endif


#define PI_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

#define PI_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define PI_NULL                         (0)


#define pi_container_of(node, type, member)\
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

#define PI_BIT(b)    (1<<(b))

#define PI_BIT_SET(x,b)            ((x)|=PI_BIT(b))
#define PI_BIT_RESET(x,b)          ((x)&=~(PI_BIT(b)))
#define PI_BITS_SET(x,bits)        ((x)|=bits)
#define PI_BITS_RESET(x,bits)      ((x)&=~(bi/bits))
#define PI_BIT_TEST(x,b)           (((x)&(PI_BIT(b)))?1:0)
#define PI_BITS_TEST(x,bits)       (((x)&(bits))?1:0)

/**
 * Double List structure
 */
struct pi_list_node
{
    struct pi_list_node *next;                          /**< point to next node. */
    struct pi_list_node *prev;                          /**< point to prev node. */
};
typedef struct pi_list_node pi_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct pi_slist_node
{
    struct pi_slist_node *next;                         /**< point to next node. */
};
typedef struct pi_slist_node pi_slist_t;                /**< Type for single list. */

typedef struct{
    pi_slist_t node;
    pi_err_t (*entry)(void* arg);
    void* arg;
}pi_co_thread_t;

#ifdef __cplusplus
}
#endif
#endif
