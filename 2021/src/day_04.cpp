#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amp_defs.h"

i32 stride = 5;

struct board
{
    int data[25];
    b32 hits[25];

    board *next;
};

board*
get_last_board(board *root)
{
    board *last = root;

    while(last && last->next)
    {
        last = last->next;
    }

    return last;
}

board*
push_board(board **root)
{
    if(*root == nullptr)
    {
        *root = (board*)calloc(1, sizeof(board));
        return *root;
    }
            
    board *last = *root;

    while(last->next != nullptr)
    {
        last = last->next;
    }

    last->next = (board*)calloc(1, sizeof(board));


    return last->next;
}

void
push_winner(board **head, board *to_push)
{
    if(*head == nullptr)
    {
        *head = (board*)malloc(sizeof(board));
        memcpy(*head, to_push, sizeof(board));
        (*head)->next = nullptr;
    }
    else
    {
        board *cur = *head;
        while(cur->next != nullptr)
        {
            cur = cur->next;
        }
        
        board *winner = (board*)malloc(sizeof(board));
        memcpy(winner, to_push, sizeof(board));
        winner->next = nullptr;

        cur->next = winner;
    }
}

bool
process_board(board *candidate, i32 draw)
{
    for(i32 i = 0;
            i < 25;
            ++i)
    {
        if(candidate->data[i] == draw)
        {
            candidate->hits[i] = 1;

            i32 x = i % stride;
            i32 y = i / stride;

            b32 winner = true;

            //check row
            for(i32 j = 0;
                    j < 5;
                    ++j)
            {
                if(candidate->hits[y*stride + j] == 0)
                {
                    winner = false;
                    break;
                }
            }


            if(!winner)
            {
                // check column
                winner = true;
                for(i32 k = 0;
                        k < 5;
                        ++k)
                {
                    if(candidate->hits[k*stride + x] == 0)
                    {
                        winner = false;
                        break;
                    }
                }

            }
            
            return winner;
        }
    }

    return false;
}

board*
process_boards(board *head, i32 draw)
{
    board *winners = nullptr;
    board *current = head;

    while(current)
    {
        if(process_board(current, draw))
        {
            push_winner(&winners, current);
        }

        current = current->next;
    }

    return winners;
}

i32
calculate_score(board *winner, int draw)
{
    i32 score = 0;
    for(int i = 0;
            i < 25;
            ++i)
    {
        score += winner->data[i]*(winner->hits[i] ? 0 : 1);
    }

    return score*draw;
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
            u8 board_count = 0;


            board *head = nullptr;
            
            char buffer[512];
            while(fgets(buffer, array_len(buffer), file))
            {
                token next = get_next_token(buffer);
                if(input_count == 0)
                {
                    while(next.type != TOKEN_EOF && next.type != TOKEN_NEWLINE )
                    {
                        if(next.type != TOKEN_NUMBER)
                        {
                            next = get_next_token(&next);
                            continue;
                        }

                        inputs[input_count++] = atoi(next.at);

                        next = get_next_token(&next); 
                    }
                }
                else if(next.type == TOKEN_NUMBER)
                {
                    board *current = get_last_board(head);
                    if(!current || board_count == 25)
                    {
                        current = push_board(&head);
                        board_count = 0;
                    }

                    while(next.type == TOKEN_NUMBER )
                    {
                        assert(board_count < 25);
                        current->data[board_count++] = atoi(next.at);

                        next = get_next_token(&next);
                    }
                }
            }

            for(i32 i = 0;
                    i < input_count;
                    ++i)
            {
                printf("ROUND %d(%d): ", i, inputs[i]);

                board *winners = process_boards(head, inputs[i]);
                if(winners)
                {
                    printf("Winners!\n");
                    board *cur = winners;

                    while(cur)
                    {
                        printf("\n SCORE: %d\n\n", calculate_score(cur, inputs[i]));
                        cur = cur->next;
                    }
                    break;
                }
                else
                {
                    printf("No Winner :(\n\n");
                }
            }
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
