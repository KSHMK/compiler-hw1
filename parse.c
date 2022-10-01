#include <stdio.h>
#include <stdlib.h>
#include "data_set.h"
#include "parse.h"
#include "utils.h"
#include "lex.h"



void parse_init()
{
    PREGEX_LIST regex_list;

    regex_list = regex_list_init();
    

}

int automata_check(char* data, size_t data_size, PREGEX regex, size_t* data_checked_len)
{
    int i;
    size_t data_i;
    PAUTOMATA cur_state, next_state;

    cur_state = regex->start;
    for(data_i=0;data_i<data_size;i++)
    {
        next_state = NULL;
        for(i=0;i<cur_state->list_len;i++)
        {
            if(cur_state->check_list[i](data[data_i]))
                next_state = cur_state->next_list[i];
        }
        if(!next_state)
            break;
        cur_state = next_state;
    }
    if(cur_state->is_end)
    {
        *data_checked_len = data_i;
        return 1;
    }
    return 0;
}

int parse_data(unsigned char* data, 
                size_t data_size, 
                PTOKEN_LIST token_list, 
                PUNIQUE_LIST symbol_list, 
                PUNIQUE_LIST string_list,
                PUNIQUE_LIST etc_list)
{
    size_t data_i, data_checked_len;
    while(data_i < data_size)
    {

    }
    return 0;
}

