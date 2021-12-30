#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

u8 width = 10;
u8 height = 10;

struct octo
{
    u8 energy;
    b32 visited;
};

void flash(octo *octopi, u8 x, u8 y, u64 *flash_count);

inline void
flash_neighbor(octo *octopi, u8 x, u8 y, u64 *flash_count)
{
    if(x < width && y < height && !octopi[y*width + x].visited)
    {
        octopi[y*width + x].energy++;
        if(octopi[y*width + x].energy > 9)
            flash(octopi, x, y, flash_count);
    }
}

inline void
flash(octo *octopi, u8 x, u8 y, u64 *flash_count)
{
    if(!octopi[y*width + x].visited)
    {
        octopi[y*width + x].visited = true;
        flash_neighbor(octopi, x-1, y-1, flash_count);
        flash_neighbor(octopi, x, y-1, flash_count);
        flash_neighbor(octopi, x+1, y-1, flash_count);

        flash_neighbor(octopi, x-1, y, flash_count);
        flash_neighbor(octopi, x+1, y, flash_count);

        flash_neighbor(octopi, x-1, y+1, flash_count);
        flash_neighbor(octopi, x, y+1, flash_count);
        flash_neighbor(octopi, x+1, y+1, flash_count);

        octopi[y*width + x].energy = 0;
        (*flash_count)++;
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
            octo octopi[100] = {};
            u8 octopi_count = 0;

            char buffer[4096];
            
            while (fgets(buffer, array_len(buffer), file))
            {
                for (char *c = buffer;
                     *c != 0 && *c != '\n';
                     ++c)
                {
                    octopi[octopi_count++].energy = *c - '0';
                }
            }

            u64 flash_count = 0;
            for(u16 step = 0;
                step < 100;
                ++step)
            {
                for (int y = 0;
                     y < 10;
                     ++y)
                {
                    for (int x = 0;
                         x < 10;
                         ++x)
                    {
                        octopi[y*width + x].energy++;
                    }
                }


                for (int y = 0;
                     y < 10;
                     ++y)
                {
                    for (int x = 0;
                         x < 10;
                         ++x)
                    {
                        if (octopi[y*width+ x].energy > 9)
                        {
                            flash(octopi, x, y, &flash_count);
                        }
                    }
                }

                /*for (int y = 0;
                     y < 10;
                     ++y)
                {
                    for (int x = 0;
                         x < 10;
                         ++x)
                    {
                        printf("%d", octopi[y*width + x].energy);
                    }

                    printf("\n");
                }*/

                for(u8 i = 0; i < 100; ++i)
                {
                    octopi[i].visited = false;
                }
                //printf("\n");
            }

            printf("Flash count: %llu\n", flash_count);
        }
    }
    
    return result;
}