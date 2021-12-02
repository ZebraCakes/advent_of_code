#include <stdio.h>
#include <stdlib.h>

#include "amp_defs.h"

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            i32 deeper_count = 0;
            i32 last_sum = -1;
            i32 depths[3] = {-1, -1, -1};
            i32 last = 0;
            i32 sum_count = 0;
            b32 first_sum = true;

            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                if(is_number(buffer))
                {
                    i32 depth = atoi(buffer);
                    i32 next = (last +1) % 3;
                    depths[next] = depth;

                    if(depths[last] > -1)
                    {
                        if(depths[last] < depths[next])
                        {
                            ++deeper_count;
                            last = next;
                        }
                    }

                    if(depths[0] != -1 &&
                       depths[1] != -1 &&
                       depths[2] != -1)
                    {
                        i32 sum = depths[0] + depths[1] + depths[2];
                        
                        if(first_sum)
                        {
                            last_sum = sum;
                            first_sum = false;
                        }
                        else
                        {
                            if(!first_sum && last_sum < sum)
                            {
                                sum_count++;
                            }
                            last_sum = sum;
                        }
                    }

                    last = next;
                }
            }

            printf("Part1: Total number of depth increases: %d\n", deeper_count);
            printf("Part2: Total number of sum increases: %d\n", sum_count);
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
