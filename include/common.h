#pragma once

#include <arpa/inet.h>
#include <endian.h>

/* universal logging interface, use different font colors to display different types of information */
#define TTV_PRINT        printf

// Error information  --- red
#define TTV_LOGE(...)   TTV_PRINT("\033[31;22m[%s:%d] ", __FUNCTION__, __LINE__);  \
                        TTV_PRINT(__VA_ARGS__);                                    \
                        TTV_PRINT("\033[0m\n")
// Normal information --- green
#define TTV_LOGI(...)   TTV_PRINT("\033[32;22m[%s:%d] ", __FUNCTION__, __LINE__);  \
                        TTV_PRINT(__VA_ARGS__);                                    \
                        TTV_PRINT("\033[0m\n")
// Debug information  --- blue 
#define TTV_LOGD(...)   TTV_PRINT("\033[34;22m[%s:%d] ", __FUNCTION__, __LINE__);  \
                        TTV_PRINT(__VA_ARGS__);                                    \
                        TTV_PRINT("\033[0m\n")

#define TTV_ASSERT(x)                                            \
    do {                                                         \
        int res = (x);                                           \
        if (!res) {                                              \
            TTV_LOGE("Error occurred when executing %s!", #x);   \
        }                                                        \
    } while(0)

#if defined(_MSC_VER)
#ifdef BUILDING_DLL
#define TTV_PUBLIC __declspec(dllexport)
#else
#define TTV_PUBLIC __declspec(dllimport)
#endif
#else
#define TTV_PUBLIC __attribute__((visibility("default")))
#endif

// assumes sender & receiver use same float format, such as IEEE-754
static float swapFloat(float f) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    union {
        float f;
        uint32_t i;
    } u32;
    u32.f = f;
    u32.i = htonl(u32.i);
    return u32.f;
#else
    return f;
#endif
}

// assumes sender & receiver use same double format, such as IEEE-754
static double swapDouble(double d) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    union {
        double d;
        uint32_t i[2];
    } u64; // used i[2] instead of long long since some compilers don't support long long
    u64.d         = d;
    uint32_t temp = htonl(u64.i[1]);
    u64.i[1]      = htonl(u64.i[0]);
    u64.i[0]      = temp;
    return u64.d;
#else
    return d;
#endif
}

/* the definition of supported data types */
enum TlvTypeDefinition {
    START_TYPE          = 0x00,  // reserved, the definition of start type
    BOOL_T              = 0x01,  // boot
    UINT8_T,                     // uint8_t
    INT8_T,                      // int8_t
    UINT16_T,                    // uint16_t
    INT16_T,                     // int16_t
    UINT32_T,                    // uint32_t
    INT32_T,                     // int32_t
    UINT64_T,                    // uint64_t
    INT64_T,                     // int64_t
    FLOAT_T,                     // float
    DOUBLE_T,                    // double
    STRING_T         = 0x20,     // string
    BYTES_T,                     // char* str
    TTV_T,                       // ttv object

    BASIC_TYPE_MAX   = DOUBLE_T, // uplimit of the basic type
    COMPLEX_TYPE_MAX = TTV_T,    // uplimit of the complex type

    END_TYPE  = 0xFF,            // reserved, the definition of end type
    START_TAG = START_TYPE,      // the definition of start tag
    END_TAG   = END_TYPE         // the definition of end tag
};
