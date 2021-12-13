#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue>

#include "amp_defs.h"


struct height_map
{
    u8* tiles;
    u8* visited;

    u8 width;
    u8 height;

    u32 capacity;
    u32 count;
};

int main(int arg_count, char** args)
{
    int result = 0;
    if (arg_count > 1)
    {
        FILE* file = fopen(args[1], "r");

        if (file)
        {
            height_map map = {};
            map.capacity = 16384;
            map.tiles = (u8*)malloc(sizeof(u8) * map.capacity);
            map.visited = (u8*)calloc(map.capacity, sizeof(u8));
            
            u8 width = 0;
            char buffer[4096];
            while (fgets(buffer, array_len(buffer), file))
            {
                token tok = get_next_token(buffer);

                assert(tok.length < 256);

                // initialize
                if (map.width == 0)
                {
                    map.width = tok.length;
                }

                ++map.height;

                for (u8 i = 0;
                    i < tok.length;
                    ++i)
                {
                    map.tiles[map.count++] = tok.at[i] - '0';
                }

                assert(map.count <= map.capacity);
            }

            u32 sum = 0;
            for (u32 y = 0;
                y < map.height;
                ++y)
            {
                for (u32 x = 0;
                    x < map.width;
                    ++x)
                {
                    u32 index = y * map.width + x;
                    u8 tile = map.tiles[index];
                    if ((x != 0 && map.tiles[index - 1] <= tile) ||
                        (x + 1 < map.width && map.tiles[index + 1] <= tile) ||
                        (y != 0 && map.tiles[index - map.width] <= tile) ||
                        (y + 1 < map.height && map.tiles[index + map.width] <= tile) )
                    {
                        continue;
                    }
                    sum += tile + 1;                    
                }
            }

            printf("Sum of risk levels: %d\n", sum);

            u32 basins[3] = { 0 };

            // BFS search
            for (u32 y = 0;
                y < map.height;
                ++y)
            {
                for (u32 x = 0;
                    x < map.width;
                    ++x)
                {
                    u32 index = y * map.width + x;
                    
                    if (!map.visited[index])
                    {                        
                        v2 tile = { x, y };
                        u32 size = 0;

                        std::queue<v2> q;

                        q.push(tile);

                        while (!q.empty())
                        {
                            v2 tile = q.front();
                            q.pop();

                            u32 tile_index = tile.y * map.width + tile.x;
                            if (map.tiles[tile_index] == 9 || map.visited[tile_index] == 1)
                            {
                                continue;
                            }

                            map.visited[tile_index] = 1;

                            
                            ++size;

                            if (tile.x > 0)
                            {
                                v2 new_tile = { tile.x - 1, tile.y };
                                q.push(new_tile);
                            }
                            if (tile.x + 1 < map.width)
                            {
                                v2 new_tile = { tile.x + 1, tile.y };
                                q.push(new_tile);
                            }
                            if (tile.y > 0 )
                            {
                                v2 new_tile = { tile.x, tile.y-1};
                                q.push(new_tile);
                            }
                            if (tile.y + 1 < map.height)
                            {
                                v2 new_tile = { tile.x, tile.y +1};
                                q.push(new_tile);
                            }
                        }

                        u32 min = INT32_MAX;
                        u8 mindex = 5;
                        for (u8 i = 0;
                            i < 3;
                            ++i)
                        {
                            if (basins[i] < min)
                            {
                                min = basins[i];
                                mindex = i;
                            }
                        }

                        if (min < size)
                            basins[mindex] = size;
                    }
                }
            }

            printf("Basins: ");
            for (u8 i = 0;
                i < 3;
                ++i)
            {
                printf("%d ", basins[i]);
            }

            printf("Result: %d", basins[0] * basins[1] * basins[2]);
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
