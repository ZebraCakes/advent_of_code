#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

struct aquarium
{
    i8 *fish;
    u64 count;
    u64 capacity;
};

void
push_fish(aquarium *aq, i8 timer)
{
    assert(aq->count < aq->capacity);

    aq->fish[aq->count++] = timer;
    if(aq->count == aq->capacity)
    {
        i8 *fish = (i8*)malloc(sizeof(i8)*aq->capacity*2);
        memcpy(fish, aq->fish, aq->capacity);
        free(aq->fish);
        aq->fish = fish;
        aq->capacity *= 2;
    }
}

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            aquarium aq;
            aq.capacity = kilobytes(64);
            aq.fish = (i8*)malloc(sizeof(i8)*aq.capacity);
            aq.count = 0;
            
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
                        push_fish(&aq, atoi(tok.at));
                    }

                    tok = get_next_token(&tok);
                }
            }

            i32 days = 80;

            for(i32 day = 0;
                    day < days;
                    ++day)
            {
                for(i32 i = 0;
                        i < aq.count;
                        ++i)
                {
                    i8 *fish = aq.fish + i;
                    --(*fish);

                    if(*fish < 0)
                    {
                        *fish = 6;
                        push_fish(&aq, 9);
                    }
                }
            }

            printf("Total Lantern Fish: %d\n", aq.count);
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
