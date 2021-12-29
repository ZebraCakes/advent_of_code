#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>

#include "amp_defs.h"

int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");

        if (file)
        {
            char incomplete[100][4096];
            char buffer[4096];
            u64 score = 0;

            std::vector<u64> scores;
            scores.reserve(128);

            u64 next_incomplete = 0;
            while (fgets(buffer, array_len(buffer), file))
            {
                u64 sum = 0;
                token tokens[256] = {};
                u64 next_token_index = 0;

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
                            sum += 25137;
                        }

                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_RIGHT_SQUARE_BRACKET)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_LEFT_SQUARE_BRACKET)
                        {
                            sum += 57;
                        }
                        
                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_RIGHT_BRACKET)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_LEFT_BRACKET)
                        {
                            sum += 1197;
                        }
                        
                        --next_token_index;
                    }

                    else if (tok.type == TOKEN_CLOSE_PAREN)
                    {
                        if (tokens[next_token_index-1].type != TOKEN_OPEN_PAREN)
                        {
                            sum += 3;
                        }
                        
                        --next_token_index;
                    }

                    tok = get_next_token(&tok);
                }

                if (sum > 0)
                {
                    score += sum;
                }

                else
                {
                    --next_token_index;
                    u64 sum = 0;
                    while (next_token_index < 0xFFFF)
                    {
                        u64 points = 0;

                        switch (tokens[next_token_index--].type)
                        {
                            case TOKEN_OPEN_PAREN:
                            {
                                printf(")");
                                points = 1;
                            } break;
                            case TOKEN_LEFT_SQUARE_BRACKET:
                            {
                                printf("]");
                                points = 2;
                            } break;
                            case TOKEN_LEFT_BRACKET:
                            {
                                printf("}");
                                points = 3;
                            } break;
                            case TOKEN_LEFT_ARROW:
                            {
                                printf(">");
                                points = 4;
                            } break;
                        }

                        sum = sum*5 + points;
                    }

                    printf("\nline \"%s\": %lu\n", buffer, sum);
                    scores.push_back(sum);
                }

            }

            std::sort(scores.begin(), scores.end());

            printf("Error Score: %d\n", score);
            printf("Unfinished Mid Score: %lu\n", scores[scores.size() / 2]);
        }
    }

    return result;
}