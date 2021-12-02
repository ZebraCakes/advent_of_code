#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

enum commands
{
    FORWARD,
    DOWN,
    UP,
};

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            v2 location = {};

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
                            location.x += move;
                        }
                        else if(strncmp(cmd.str, "up", cmd.length) == 0)
                        {
                            location.y -= move;
                        }
                        else if(strncmp(cmd.str, "down", cmd.length) == 0)
                        {
                            location.y += move;
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

            printf("(%d, %d): %d\n", location.x, location.y, location.x*location.y);
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
