#include <se-lib-c/osal/IMutex.h>

#ifndef RTOSDRIVER
#define RTOSDRIVER
/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __RtosMutexPrivateData* RtosMutexHandle;

/******************************************************************************
 Public functions
******************************************************************************/

RtosMutexHandle RtosMutex_create(void);

IMutexHandle RtosMutex_getIMutex(RtosMutexHandle self);

void RtosMutex_destroy(RtosMutexHandle self);

#endif
