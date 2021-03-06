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

#define kilobytes(n) ((n)*1024)
#define megabytes(n) (kilobytes(n)*1024)
#define gigabytes(n) (megabytes(n)*1024)


#define max(a, b) ( (a) > (b) ? (a) : (b) )

union v2
{
    struct
    {
        i32 x;
        i32 y;
    };

    i32 e[2];
};

enum token_type
{
    TOKEN_EOF = 0x0,
    TOKEN_NEWLINE = 0x1,

    TOKEN_NUMBER = 0x2,
    TOKEN_NAME = 0x3,

    TOKEN_COMMA = 0x4,
    TOKEN_MINUS = 0x5,
    TOKEN_RIGHT_ARROW = 0x6,
    TOKEN_LEFT_ARROW = 0x7,
    TOKEN_RIGHT_SQUARE_BRACKET = 0x8,
    TOKEN_LEFT_SQUARE_BRACKET = 0x9,
    TOKEN_RIGHT_BRACKET = 0xA,
    TOKEN_LEFT_BRACKET = 0xB,
    TOKEN_OPEN_PAREN = 0xC,
    TOKEN_CLOSE_PAREN = 0xD,
    TOKEN_PIPE = 0xE,
    TOKEN_EQUALS = 0xF,

};

struct token
{
    char *at;
    i32 length;

    token_type type;
};

struct token_collection
{
    token* tokens;

    u32 size;
    u32 capacity;
};

inline char*
eat_whitespace(char *c)
{
    while(*c == ' ' || *c == '\t' )
        ++c;

    return c;
}

inline b32
is_alpha(char *c)
{
    return ( (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') );
}

inline b32
is_number(char *c)
{
    return (*c >= '0' && *c <= '9');
}

inline b32
is_alphanumeric(char *c)
{
    return ( is_alpha(c) || is_number(c) );
}

// This is hacky af and if I cared I'd make it more robust, 
// but for now it'll do
token get_next_token(char *str)
{
    token result = {};

    char *c = eat_whitespace(str);

    result.at = c;

    if(*c == 0)
    {
        result.type = TOKEN_EOF;
        result.length = 1;
    }
    else if(*c == '\n')
    {
        result.type = TOKEN_NEWLINE;
        result.length = 1;
    }
    else if(*c == ',')
    {
        result.type = TOKEN_COMMA;
        result.length = 1;
    }
    else if(*c == '-')
    {
        result.type = TOKEN_MINUS;
        result.length = 1;
    }
    else if(*c == '<')
    {
        result.type = TOKEN_LEFT_ARROW;
        result.length = 1;
    }
    else if(*c == '>')
    {
        result.type = TOKEN_RIGHT_ARROW;
        result.length = 1;
    }
    else if(*c == '[')
    {
        result.type = TOKEN_LEFT_SQUARE_BRACKET;
        result.length = 1;
    }
    else if(*c == ']')
    {
        result.type = TOKEN_RIGHT_SQUARE_BRACKET;
        result.length = 1;
    }
    else if (*c == '{')
    {
        result.type = TOKEN_LEFT_BRACKET;
        result.length = 1;
    }
    else if (*c == '}')
    {
        result.type = TOKEN_RIGHT_BRACKET;
        result.length = 1;
    }
    else if(*c == '(')
    {
        result.type = TOKEN_OPEN_PAREN;
        result.length = 1;
    }
    else if(*c == ')')
    {
        result.type = TOKEN_CLOSE_PAREN;
        result.length = 1;
    }
    else if (*c == '|')
    {
        result.type = TOKEN_PIPE;
        result.length = 1;
    }
    else if (*c == '=')
    {
        result.type = TOKEN_EQUALS;
        result.length = 1;
    }

    else if(is_alpha(c))
    {
        result.type = TOKEN_NAME;
        while(is_alphanumeric(c))
        {
            ++c;
        }

        result.length = c - result.at;
    }
    else if(is_number(c))
    {
        result.type = TOKEN_NUMBER;
        while(is_number(c))
        {
            ++c;
        }

        result.length = c - result.at;
    }

    return result;
}


token
get_next_token(token *prev)
{
    return get_next_token(prev->at + prev->length);
}
