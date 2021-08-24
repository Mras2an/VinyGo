#include "Debug.h"

eVrError_t Debug_printf(eVrError_t error, const char * format, ...)
{
    return error;
}

void Debug_checkFailed(eVrError_t error, const char *function, int line, const char *functionCall)
{
    OsPrintf("ERROR CHECK failed with error code (%d) in %s(%d) when call \"%s\"\n", error, function, line, functionCall);
}

