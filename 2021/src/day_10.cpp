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
            char buffer[4096];
            u32 score = 0;
            while (fgets(buffer, array_len(buffer), file))
            {
                token tokens[256] = {};
                u16 next_token_index = 0;

                token tok = get_next_token(buffer);

                while (tok.type != TOKEN_EOF && tok.type != TOKEN_NEWLINE)
                {
                    assert(next_token_index < 256);

                    if (tok.type == TOKEN_LEFT_ARROW ||
                        tok.type == TOKEN_LEFT_BRACKET ||
                        tok.type == TOKEN_LEFT_SQUARE_BRACKET ||
                        tok.type == TOKEN_OPEN_PAREN)
                    {
                        tokens[next_token_index++] = tok;
                    }

                    else if (tok.type == TOKEN_RIGHT_ARROW)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_LEFT_ARROW)
                        {
                            score += 25137;
                        }

                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_RIGHT_SQUARE_BRACKET)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_LEFT_SQUARE_BRACKET)
                        {
                            score += 57;
                        }
                        
                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_RIGHT_BRACKET)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_LEFT_BRACKET)
                        {
                            score += 1197;
                        }
                        
                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_CLOSE_PAREN)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_OPEN_PAREN)
                        {
                            score += 3;
                        }
                        
                        --next_token_index;
                    }

                    tok = get_next_token(&tok);
                }
            }

            printf("Score: %d\n", score);
        }
    }

    return result;
}