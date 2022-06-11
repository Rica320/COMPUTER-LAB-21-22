/**
 * @file handlers.h
 * @brief Util for dealing with return values
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _LCOM_HANDLERS_
#define _LCOM_HANDLERS_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

inline bool LogCall(const char* function, const char* file, int line, int ret)
{
    if (ret != EXIT_SUCCESS)
    {
      printf("%s : FAILED -> Return value different from EXIT_SUCCESS / LINE: %d / FILE: %s \n", function, line, file);
      return false;
    }
    
    return true;
}

#define Assert(x) if(!(x)) exit(EXIT_FAILURE);
// similar to the above macro -- TODO: TEST
#define NullSafety(x) if(x == NULL) {\
 printf("NULL POINTER (%s): LINE: %d / FILE: %s \n", #x, __LINE__, __FILE__);\
 exit(EXIT_FAILURE);};

/** closed between*/
#define Assert_cbet(x, a, b) {int ret = x; if( !(ret >= a && ret <= b)){\
    printf("Value out of bound : %d <= %d <= %d (F)\n", a, ret, b);\
    printf("LINE: %d / FILE: %s \n", __LINE__, __FILE__);};}




#ifdef _DEBUG_MACRO_
#define CHECKCall(x) {int ret = x;\
    Assert(LogCall(#x, __FILE__, __LINE__, ret));}
#else
#define CHECKCall(x) (x)
#endif

#endif
