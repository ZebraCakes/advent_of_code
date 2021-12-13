#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"


struct digit
{
    token tok;
    u8 value;
};

b32
has_char(token* tok, char c)
{
    b32 found = false;
    char* a = tok->at;

    while (a - tok->at < tok->length)
    {
        if (*(a++) == c)
        {
            found = true;
            break;
        }
    }

    return found;
}

inline u8
sum_diffs(token *a, token *b)
{
    u8 diff_count = 0;
    for (u8 j = 0;
        j < b->length;
        ++j)
    {
        if (!has_char(a, b->at[j]))
        {
            ++diff_count;
        }
    }
    return diff_count;
}

int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");

        if (file)
        {
            u32 sum = 0;

            char buffer[4096];
            while (fgets(buffer, array_len(buffer), file))
            {
                // Would generalize this, but w/e we know how the input
                // is formed
                token tok = get_next_token(buffer);
                token digits[10] = {};

                token temps[6] = {};
                u32 temp_count = 0;

                token numbers[4] = {};
                u8 numbers_count = 0;

                b32 past_pipe = false;

                while (tok.type != TOKEN_NEWLINE && tok.type != TOKEN_EOF)
                {
                    if (!past_pipe)
                    {
                        if (tok.type == TOKEN_PIPE)
                            past_pipe = true;

                        else
                        {
                            if (tok.type == TOKEN_NAME)
                            {
                                switch (tok.length)
                                {
                                    case 2:
                                    {
                                        digits[1] = tok;
                                    }break;

                                    case 3:
                                    {
                                        digits[7] = tok;
                                    }break;

                                    case 4:
                                    {
                                        digits[4] = tok;
                                    }break;

                                    case 7:
                                    {
                                        digits[8] = tok;
                                    }break;

                                    default:
                                    {
                                        temps[temp_count++] = tok;
                                    }
                                }
                            }
                        }
                    }
                        
                    else
                    {
                        assert(numbers_count < 4);
                        numbers[numbers_count++] = tok;
                    }
                    tok = get_next_token(&tok);
                }

                // solve the remaining codes
                for (u8 i = 0;
                    i < temp_count;
                    ++i)
                {
                    token* tok = temps + i;
                    assert(tok->length == 5 || tok->length == 6);

                    if (tok->length == 5)
                    {
                        char first = digits[1].at[0]; 
                        char second = digits[1].at[1];
                        if (has_char(tok, first) && has_char(tok, second))
                        {
                            digits[3] = *tok;
                        }
                        else
                        {
                            u8 diff_count = sum_diffs(tok, digits + 4);

                            if (diff_count == 2)
                            {
                                digits[2] = *tok;
                            }
                            else
                            {
                                digits[5] = *tok;
                            }
                        }
                    }
                    else
                    {
                        char first = digits[1].at[0];
                        char second = digits[1].at[1];

                        if (!has_char(tok, first) || !has_char(tok, second))
                        {
                            digits[6] = *tok;
                        }
                        else
                        {
                            u8 diff_count = sum_diffs(tok, digits + 4);

                            if (diff_count == 1)
                            {
                                digits[0] = *tok;
                            }
                            else
                            {
                                digits[9] = *tok;
                            }
                        }
                    }
                }

                u32 value = 0;
                u32 power = 1000;
                for (u8 i = 0;
                    i < 4;
                    ++i)
                {

                    for (u8 j = 0;
                        j < 10;
                        ++j)
                    {
                        if (numbers[i].length != digits[j].length)
                            continue;

                        b32 found = true;
                        token* digit = digits + j;
                        for (u8 k = 0;
                            k < digit->length;
                            ++k)
                        {
                            if (!has_char(numbers + i, digit->at[k]))
                            {
                                found = false;
                                break;
                            }
                        }

                        if (found)
                        {
                            value += j * power;
                            power /= 10;
                            continue;
                        }
                    }
                }

                sum += value;
            }

            printf("Total: %d\n", sum);
        }
        else
        {
            printf("Invalid filename or does not exist\n");
            result = -1;
        }
    }
    else
    {
        printf("No filename\n");
        result = -2;
    }

    return result;
}
