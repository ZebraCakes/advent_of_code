#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            u16 stride = 1024;
            u16 *tiles = (u16*)calloc(stride*stride, sizeof(u16));
            
            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                // Would generalize this, but w/e we know how the input
                // is formed
                token tok = get_next_token(buffer);
                assert(tok.type == TOKEN_NUMBER);
                
                v2 start;
                start.x = atoi(tok.at);
                
                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_COMMA);
                
                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_NUMBER);

                start.y = atoi(tok.at);

                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_MINUS);

                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_RIGHT_ARROW);

                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_NUMBER);
                
                v2 end;
                end.x = atoi(tok.at);
                
                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_COMMA);
                
                tok = get_next_token(&tok);
                assert(tok.type == TOKEN_NUMBER);

                end.y = atoi(tok.at);

                v2 slope = {(end.x > start.x) ? 1 : ((end.x != start.x) ? -1 : 0), 
                            (end.y > start.y) ? 1 : ((end.y != start.y) ? -1 : 0)};

                if(start.x == end.x)
                {
                    i32 x = start.x;
                    i32 y_start = start.y < end.y ? start.y : end.y;
                    i32 y_end = start.y > end.y ? start.y : end.y;

                    for(i32 y = y_start;
                            y <= y_end;
                            y++)
                    {
                        tiles[y*stride + x]++; 
                    }
                }
                else if(start.y == end.y)
                {
                    i32 y = start.y;
                    i32 x_start = start.x < end.x ? start.x : end.x;
                    i32 x_end = start.x > end.x ? start.x : end.x;
                    for(i32 x = x_start;
                        x <= x_end;
                        x++)
                    {
                        tiles[y*stride + x]++; 
                    }
                }
                else
                {
                }

            }

            u16 sum = 0;
            for(i32 i = 0;
                    i < stride*stride;
                    ++i)
            {
                if(tiles[i] > 1)
                {
                    ++sum;
                }
            }

            printf("Overlaps > 2: %d\n", sum);
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
