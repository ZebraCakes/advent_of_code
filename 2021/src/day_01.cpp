#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "amp_defs.h"

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            i32 last_depth = -1;
            i32 deeper_count = 0;

            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                if(is_number(buffer))
                {
                    i32 depth = atoi(buffer);
                    b32 changed = false;
                    if(last_depth == -1)
                    {
                        last_depth = depth;
                    }
                    else
                    {
                        if(depth > last_depth)
                        {
                            ++deeper_count;
                        }

                        last_depth = depth;
                    }
                }
            }

            printf("Total number of increases: %d\n", deeper_count);
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
