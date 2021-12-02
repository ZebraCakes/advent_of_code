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
            v2 location_p1 = {};
            v2 location_p2 = {};
            i32 aim = 0;

            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                token cmd = get_next_token(buffer);
                if(cmd.length > 0)
                {
                    token dist = get_next_token(cmd.str+cmd.length+1);
                    if(dist.length > 0 )
                    {
                        i32 move = atoi(dist.str);
                        if(strncmp(cmd.str, "forward", cmd.length) == 0)
                        {
                            location_p1.x += move;

                            location_p2.y += aim*move;
                            location_p2.x += move;
                        }
                        else if(strncmp(cmd.str, "up", cmd.length) == 0)
                        {
                            location_p1.y -= move;
                            aim -= move;
                        }
                        else if(strncmp(cmd.str, "down", cmd.length) == 0)
                        {
                            location_p1.y += move;
                            aim += move;
                        }
                    }
                    else
                    {
                        printf("Invalid distance\n");
                    }
                }
                else
                {
                    printf("Empty command\n");
                }
            }

            printf("(%d, %d): %d\n", location_p1.x, location_p1.y, location_p1.x*location_p1.y);
            printf("(%d, %d): %d\n", location_p2.x, location_p2.y, location_p2.x*location_p2.y);
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
