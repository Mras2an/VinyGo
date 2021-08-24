#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "BspConfig.h"
#include "Bsp.h"

typedef enum eVrError {
    VR_NO_ERROR          = 0,    // No errors.
    VR_UNKNOWN_ERROR     = -100,
    VR_INVALID_PARAMETER,
} eVrError_t;

eVrError_t Debug_printf(eVrError_t error, const char * format, ...);
void Debug_checkFailed(eVrError_t error, const char *function, int line, const char *functionCall);

#define VR_DEBUG

#ifdef VR_DEBUG
    #define Debug_info(msg, ...) OsPrintf("[INFO: %s, L%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define Debug_err  OsDebug_err
#else
    #define Debug_info(msg, ...)
    #define Debug_err  OsDebug_err
#endif

#define VR_ERROR(x)  Debug_printf(x, "[ERR : %s, L%d] 0x%08X \n",x ,__FUNCTION__ ,__LINE__ , x)

#ifdef NDEBUG
#define VR_ERROR_CHECK(x) do {                                         \
        eVrError_t rc = (x);                                               \
        (void) sizeof(rc);                                                 \
    } while(0);
#else
#define VR_ERROR_CHECK(x) do {                                         \
        eVrError_t rc = (x);                                               \
        if (rc != VR_NO_ERROR) {                                           \
            Debug_checkFailed(rc, __FUNCTION__, __LINE__, #x);               \
        }                                                                  \
    } while(0);
#endif

#endif /* _DEBUG_H_ */
