#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

u8 width = 10;
u8 height = 10;

enum FOLDS
{
    FOLD_X = 0,
    FOLD_Y = 1,
};

struct fold_instruction
{
    FOLDS fold;
    u32 coord;
};

b32
collides(v2 *dots, u16 dot_count, v2 *dot)
{
    b32 result = false;
    for(u16 i = 0;
        i < dot_count;
        ++i)
    {
        v2 *test = dots + i;

        if(test != dot && (dot->x == test->x && dot->y == test->y) )
        {
            result = true;
            break;
        }
    }

    return result;
}

int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");
        
        if (file)
        {
            v2 dots[1024] = {};
            u16 dot_count = 0;

            fold_instruction folds[256];
            u8 fold_count = 0;
            
            u32 width = 0;
            u32 height = 0;
            
            char buffer[4096];
            
            while (fgets(buffer, array_len(buffer), file))
            {
                token tok = get_next_token(buffer);

                if(tok.type == TOKEN_NUMBER)
                {
                    assert(dot_count < array_len(dots));

                    v2 *dot = dots + dot_count++;
                    dot->x = atoi(tok.at);

                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_COMMA);

                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_NUMBER);
                    dot->y = atoi(tok.at);

                    width = max(width, dot->x);
                    height = max(height, dot->y);
                }
                else if(tok.type == TOKEN_NAME)
                {
                    assert(fold_count < array_len(folds));
                    assert(tok.type == TOKEN_NAME);
                    
                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_NAME);

                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_NAME);

                    fold_instruction *fold = folds + fold_count++;
                    if(*tok.at == 'x')
                    {
                        fold->fold = FOLD_X;
                    }
                    else if(*tok.at == 'y')
                    {
                        fold->fold = FOLD_Y;
                    }
                    
                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_EQUALS);

                    tok = get_next_token(&tok);
                    assert(tok.type == TOKEN_NUMBER);

                    fold->coord = atoi(tok.at);
                }

            }

            for(u8 i = 0;
                i < fold_count;
                ++i)
            {
                fold_instruction *fold = folds + i;

                if(fold->fold == FOLD_X)
                {
                    i32 x_coord = fold->coord;

                    for(u16 j = 0;
                        j < dot_count;
                        ++j)
                    {
                        v2 *dot = dots + j;

                        if(dot->x > x_coord)
                        {
                            i32 dist = dot->x - x_coord;

                            dot->x = x_coord - dist;

                            if(collides(dots, dot_count, dot))
                            {
                                dot->x = dots[dot_count-1].x;
                                dot->y = dots[dot_count-1].y;
                                --dot_count;
                                --j; //since we copied a dot over, gotta check it next time through
                            }
                        }
                    }

                    width = x_coord - 1;
                }
                else if(fold->fold == FOLD_Y)
                {
                    i32 y_coord = fold->coord;

                    for(u16 j = 0;
                        j < dot_count;
                        ++j)
                    {
                        v2 *dot = dots + j;
                        
                        if(dot->y > y_coord)
                        {
                            i32 dist = dot->y - y_coord;
                            
                            dot->y = y_coord - dist;
                            
                            if(collides(dots, dot_count, dot))
                            {
                                dot->x = dots[dot_count-1].x;
                                dot->y = dots[dot_count-1].y;
                                --dot_count;
                                --j; //since we copied a dot over, gotta check it next time through
                            }
                        }
                    }

                    height = y_coord;
                }
                else
                {
                    assert(false);
                }
                printf("Visible dots: %d\n", dot_count);
            }

            char *display = new char[height*width];

            for(u32 i = 0;
                i < width*height;
                ++i)
            {
                display[i] = '.';
            }

            for(u32 i = 0;
                i < dot_count;
                ++i)
            {
                v2 *dot = dots + i;
                display[dot->y*width + dot->x] = '#';
            }

            for(u32 y = 0;
                y < height;
                ++y)
            {
                for(u32 x = 0;
                    x < width;
                    ++x)
                {
                    printf("%c", display[y*width + x]);
                }
                printf("\n");
            }

        }
    }
    
    return result;
}