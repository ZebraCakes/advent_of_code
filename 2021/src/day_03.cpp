#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

inline u16
determine_gamma(u16 *arr, u16 len, u8 high_bit)
{
    int sums[16] = {};

    for(u16 i = 0;
        i < len;
        ++i)
    {
        u16 *val = arr + i;
        for(u16 j = 0;
            j < high_bit;
            ++j)
        {
            if(*val & (1 << j) )
            {
                ++sums[j];
            }
        }
    }

    u32 gamma = 0;
    for(u8 i = 0;
        i < high_bit;
        ++i)
    {
        u8 bit = (sums[i] < ( (len / 2) + (len%2) )) ? 0 : 1;
        gamma |= ( (bit) << i);
    }    

    return gamma;
}

inline u16 
determine_epsilon(u16 gamma, u8 high_bit)
{
    u32 not_gamma = ~gamma;
    u32 epsilon = 0; 

    for(u8 i = 0;
        i < high_bit;
        ++i)
    {
        epsilon |= (not_gamma & (1 << i));
    }

    return epsilon;
}

inline u16
determine_oxygen_rating(u16 *arr, u16 len, u8 high_bit)
{
    u16 rating;
    u16 *current = (u16*)malloc(sizeof(u16)*len);
    u16 *next = (u16*)malloc(sizeof(u16)*len);
    u16 count = len;
    memcpy(current, arr, count*sizeof(u16));
    i8 current_bit = high_bit-1;

    while(count > 1)
    {
        u16 gamma = determine_gamma(current, count, high_bit);

        u16 new_count = 0;
        u16 bit_val = (1 << current_bit);

        for(u16 i = 0;
            i < count;
            ++i)
        {
            if( (current[i] & bit_val ) == (gamma & bit_val) )
            {
                next[new_count++] = current[i];
            }
        }

        u16 *temp = current;
        current = next;
        next = temp;

        count = new_count;

        current_bit -= 1;

        if(current_bit < 0)
            current_bit = high_bit - 1;
    }

    u16 result = *current;

    free(next);
    free(current);

    return result;
}

inline u16
determine_co2_rating(u16 *arr, u16 len, u8 high_bit)
{
    u16 rating;
    u16 *current = (u16*)malloc(sizeof(u16)*len);
    u16 *next = (u16*)malloc(sizeof(u16)*len);
    u16 count = len;
    memcpy(current, arr, count*sizeof(u16));
    i8 current_bit = high_bit-1;

    while(count > 1)
    {
        u16 gamma = determine_gamma(current, count, high_bit);
        u16 epsilon = determine_epsilon(gamma, high_bit);

        u16 new_count = 0;
        u16 bit_val = (1 << current_bit);

        for(u16 i = 0;
            i < count;
            ++i)
        {
            if( (current[i] & bit_val ) == (epsilon & bit_val) )
            {
                next[new_count++] = current[i];
            }
        }

        u16 *temp = current;
        current = next;
        next = temp;

        count = new_count;

        current_bit -= 1;

        if(current_bit < 0)
            current_bit = high_bit - 1;
    }

    u16 result = *current;

    free(next);
    free(current);

    return result;
}

int main(int arg_count, char **args)
{
    int result = 0;
    if(arg_count > 1)
    {
        FILE *file = fopen(args[1], "r");

        if(file)
        {
            u16 inputs[1024] = {};
            u16 input_count = 0;
            u8 high_bit = 12;

            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                u8 pos = 0;
                char *c = buffer + strlen(buffer) - 2;
                while(c >= buffer)
                {
                    u16 bit = ( (*c-- - '0') << pos++);
                    inputs[input_count] |= bit; 
                }

                ++input_count;
            }

            u16 gamma = determine_gamma(inputs, input_count, high_bit);
            u16 epsilon = determine_epsilon(gamma, high_bit); 
            
            printf("gamma: %d, epsilon: %d - %d\n", gamma, epsilon, gamma*epsilon);

            u16 oxygen_rating = determine_oxygen_rating(inputs, input_count, high_bit);
            printf("oxygen_rating: %d\n", oxygen_rating);
            u16 co2_rating = determine_co2_rating(inputs, input_count, high_bit);

            printf("co2_rating: %d\n", co2_rating);
            printf("result: %d\n", oxygen_rating*co2_rating);
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
