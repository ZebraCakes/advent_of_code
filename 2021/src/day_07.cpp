#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

struct armada
{
    u16* ships;

    u16 min;
    u16 max;

    u32 count;
    u32 capacity;
};

void
push_crab(armada *arm, u16 fuel)
{
    assert(arm->count <= arm->capacity);

    arm->ships[arm->count++] = fuel;

    if (arm->min > fuel)
    {
        arm->min = fuel;
    }

    if (arm->max < fuel)
    {
        arm->max = fuel;
    }

    if (arm->count >= arm->capacity)
    {
        u32 new_capacity = arm->capacity * 2;
        u16* new_ships = (u16*)malloc(sizeof(u16) * new_capacity);
        memcpy(new_ships, arm->ships, sizeof(u16)*arm->capacity);
        free(arm->ships);
        arm->capacity = new_capacity;
        arm->ships = new_ships;
    }
}

int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");

        if (file)
        {
            armada arm = {};
            arm.capacity = 16;
            arm.ships = (u16*)malloc(sizeof(u16) * arm.capacity);
            arm.min = 65535;

            char buffer[4096];
            while (fgets(buffer, array_len(buffer), file))
            {
                // Would generalize this, but w/e we know how the input
                // is formed
                token tok = get_next_token(buffer);

                u32 count = 0;
                while (tok.type != TOKEN_NEWLINE && tok.type != TOKEN_EOF)
                {
                    if (tok.type == TOKEN_NUMBER)
                    {
                        push_crab(&arm, (u16)atoi(tok.at));
                        ++count;
                    }

                    tok = get_next_token(&tok);
                }
            }

            size_t window = arm.max - arm.min;
            u64* costs = (u64*)calloc(window, sizeof(u64));
            u32 offset = arm.min;

            for (u32 pos = arm.min;
                pos <= arm.max;
                ++pos)
            {
                for (u32 j = 0;
                    j < arm.count;
                    ++j)
                {
                    costs[pos - offset] += abs( i32(arm.ships[j] - pos) );
                }
            }


            u64 min = UINT64_MAX;
            u32 min_pos = 0;

            for (u32 i = 0;
                i < window;
                ++i)
            {
                if (costs[i] < min)
                {
                    min_pos = i;
                    min = costs[i];
                }
            }

            printf("\nMin: %d\nMax: %d\n", arm.min, arm.max);
            printf("Min cost: %lld Min pos: %d\n", min, min_pos);
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
