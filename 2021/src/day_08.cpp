#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"


int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");

        if (file)
        {
            u32 count = 0;

            char buffer[4096];
            while (fgets(buffer, array_len(buffer), file))
            {
                // Would generalize this, but w/e we know how the input
                // is formed
                token tok = get_next_token(buffer);

                b32 past_pipe = false;
                u32 sum = 0;

                while (tok.type != TOKEN_NEWLINE && tok.type != TOKEN_EOF)
                {
                    if (!past_pipe)
                    {
                        if (tok.type == TOKEN_PIPE)
                            past_pipe = true;
                    }
                        
                    else
                    {
                        if (tok.type == TOKEN_NAME)
                        {
                            if (tok.length == 2 || tok.length == 3 ||
                                tok.length == 4 || tok.length == 7)
                            {
                                ++count;
                            }
                        }
                    }
                    tok = get_next_token(&tok);
                }
            }

            printf("Instances: %d\n", count);
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
