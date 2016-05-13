#include "__cf_pvModel.h"
#ifndef RTW_HEADER_pvModel_acc_private_h_
#define RTW_HEADER_pvModel_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)   if(!(ptr)) {\
  ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
  }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((ptr));\
  (ptr) = (NULL);\
  }
#else
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((void *)(ptr));\
  (ptr) = (NULL);\
  }
#endif
#endif
void fnplb0kurc ( SimStruct * const S ) ; void bzpiibtxxj ( SimStruct * const
S ) ; void mx1fiirz0r ( SimStruct * const S ) ; void d3tq5ywgdg ( SimStruct *
const S ) ; void oekp0dakeh ( SimStruct * const S ) ; void m13kbqcs0g (
SimStruct * const S ) ; void kjlnbl0dyn ( SimStruct * const S ) ;
#endif
