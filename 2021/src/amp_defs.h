#pragma once

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  r32;
typedef double r64;

typedef i32 b32;

#define array_len(arr) (sizeof((arr))/sizeof((arr)[0]))

#define assert(expr) if(!(expr)){*(int*)0 = 0;}

#define IS_NUMERIC(c) ( ( (c) >= '0' && (c) <= '9' ) )

b32 is_number(char *num)
{
    b32 result = false;
    if(num)
    {
        result = true;
        char *c = num;
        while(c)
        {
            if(!IS_NUMERIC(*c))
            {
                result = false;
                break;
            }
            ++c;
        }
    }
}
