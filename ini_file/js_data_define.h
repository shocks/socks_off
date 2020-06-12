#ifndef JS_DATA_DEFINE_FLAG
#define JS_DATA_DEFINE_FLAG

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <errno.h>

typedef signed char JS_SINT8;
typedef unsigned char JS_UINT8;
typedef signed short int JS_SINT16;
typedef unsigned short int JS_UINT16;
typedef signed int JS_SINT32;
typedef unsigned int JS_UINT32;
typedef signed long int JS_SINT64;
typedef unsigned long int JS_UINT64;
typedef FILE JS_FILE;
typedef char JS_CHAR;
typedef short int JS_INT16;
typedef int JS_INT32;
typedef long int JS_INT64;
typedef bool JS_BOOL;
typedef void JS_VOID;
typedef size_t JS_SIZE_T;
typedef time_t JS_TIME_T;
#define JS_TRUE								(1)
#define JS_FALSE							(0)
#define JS_MAX_PATH							(255)

#endif
