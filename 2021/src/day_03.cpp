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
            u32 sums[32] = {};
            u32 input_count = 0;
            u32 high_bit = 0;

            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                int index = 0;
                char *bitc = buffer + strlen(buffer) - 2;
                while(bitc >= buffer)
                {
                    if((u32)( *(bitc--) - '0' ))
                    {
                        sums[index]++;
                    }

                    ++index;
                }

                high_bit = index; //it is assumed all inputs are the same width
                ++input_count;
            }

            u32 gamma = 0;
            for(u8 i = 0;
                i < 32;
                ++i)
            {
                u8 bit = (sums[i] <= (input_count / 2) ) ? 0 : 1;
                gamma |= ( (bit) << i);
            }    

            u32 not_gamma = ~gamma;
            u32 epsilon = 0; 

            for(u8 i = 0;
                i < high_bit;
                ++i)
            {
                epsilon |= (not_gamma & (1 << i));
            }

            printf("gamma: %d, epsilon: %d - %d\n", gamma, epsilon, gamma*epsilon);
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
