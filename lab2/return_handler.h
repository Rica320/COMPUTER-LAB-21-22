#ifndef _LCOM_HANDLER_
#define _LCOM_HANDLER_

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

#define CHECKCall(x) {int ret = x;\
    Assert(LogCall(#x, __FILE__, __LINE__, ret));}\


#endif
