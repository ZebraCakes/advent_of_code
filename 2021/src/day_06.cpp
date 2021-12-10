#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 2)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            u64 fish[9] = {};
            
            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                // Would generalize this, but w/e we know how the input
                // is formed
                token tok = get_next_token(buffer);

                while(tok.type != TOKEN_NEWLINE && tok.type != TOKEN_EOF)
                {
                    if(tok.type == TOKEN_NUMBER)
                    {
                        fish[atoi(tok.at)]++;
                    }

                    tok = get_next_token(&tok);
                }
            }

            i32 days = atoi(args[2]);

            for(i32 day = 0;
                    day < days;
                    ++day)
            {
                u64 new_fish[9] = {fish[1], fish[2], fish[3], 
                                   fish[4], fish[5], fish[6],
                                   fish[0] + fish[7], fish[8], fish[0]};
                memcpy(fish, new_fish, 9*sizeof(u64));
            }

            u64 count = 0;
            for(i32 i = 0;
                    i < 9;
                    ++i)
            {
                count += fish[i];
            }
            printf("Total Lantern Fish: %llu\n", count);
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
