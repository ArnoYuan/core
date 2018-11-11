#ifndef __PI_ERRNO_H__
#define __PI_ERRNO_H__

#define PI_ENO_OK           (0)
#define PI_ENO_ERROR        (-1)
#define PI_ENO_TIMEOUT      (-2)
#define PI_ENO_FULL         (-3)
#define PI_ENO_EMPTY        (-4)
#define PI_ENO_NOMEM        (-5)
#define PI_ENO_BUSY         (-6)
#define PI_ENO_INVAL        (-7)
#define PI_ENO_INIT         (-8)


const char* errno_get_reason(int errno);

#endif