#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_set.h"
#include "parse.h"
#include "utils.h"
#include "lex.h"



PREGEX_LIST parse_build_regex_list(void)
{
    PREGEX_LIST regex_list;
    PREGEX regex;
    regex_list = regex_list_init();

    //ID
    regex = regex_init(TOKEN_TYPE_ID, 0);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_a_zA_Z);

    regex_automata_connection(regex, 1, 2, check_0_9);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_automata_connection(regex, 1, 1, check_a_zA_Z);
    
    regex_automata_connection(regex, 2, -1, NULL);
    regex_automata_connection(regex, 2, 2, check_0_9);
    
    regex_list_append(regex_list, regex);

    //REAL
    regex = regex_init(TOKEN_TYPE_REAL, 0);
    regex_automata_connection(regex, -1, 0, NULL);
    
    regex_automata_connection(regex, 0, 1, check_1_9);

    regex_automata_connection(regex, 1, 2, check_0_9);
    regex_automata_connection(regex, 1, 3, check_dot);

    regex_automata_connection(regex, 2, 3, check_dot);
    regex_automata_connection(regex, 2, 2, check_0_9);
    
    regex_automata_connection(regex, 0, 3, check_dot);

    regex_automata_connection(regex, 3, 4, check_0_9);

    regex_automata_connection(regex, 4, 4, check_0_9);
    regex_automata_connection(regex, 4, -1, NULL);
    regex_list_append(regex_list, regex);

    //NUM
    regex = regex_init(TOKEN_TYPE_NUM, 0);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_1_9);
    regex_automata_connection(regex, 1, -1, NULL);

    regex_automata_connection(regex, 1, 2, check_0_9);
    
    regex_automata_connection(regex, 2, 2, check_0_9);
    regex_automata_connection(regex, 2, -1, NULL);
    
    regex_automata_connection(regex, 0, 3, check_0);
    regex_automata_connection(regex, 3, -1, NULL);
    regex_list_append(regex_list, regex);
    

    
    //STRING
    regex = regex_init(TOKEN_TYPE_STRING, 0);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_double_quote);
    regex_automata_connection(regex, 1, 2, check_double_quote);
    regex_automata_connection(regex, 1, 1, check_nothing);
    regex_automata_connection(regex, 2, -1, NULL);
    regex_list_append(regex_list, regex);

    //PLUS
    regex = regex_init(TOKEN_TYPE_PLUS, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_plus);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //MINUS
    regex = regex_init(TOKEN_TYPE_MINUS, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_minus);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //MUL
    regex = regex_init(TOKEN_TYPE_MUL, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_asterisk);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //DIV 
    regex = regex_init(TOKEN_TYPE_DIV, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_slash);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //ASSIGN
    regex = regex_init(TOKEN_TYPE_ASSIGN, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_equal);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //COLON
    regex = regex_init(TOKEN_TYPE_COLON, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_colon);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //SEMICOLON
    regex = regex_init(TOKEN_TYPE_SEMICOLON, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_semicolon);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //SPACE
    regex = regex_init(TOKEN_TYPE_SPACE, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_space);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //TAB
    regex = regex_init(TOKEN_TYPE_TAB, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_tab);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    //ENTER
    regex = regex_init(TOKEN_TYPE_ENTER, 1);
    regex_automata_connection(regex, -1, 0, NULL);
    regex_automata_connection(regex, 0, 1, check_enter);
    regex_automata_connection(regex, 1, -1, NULL);
    regex_list_append(regex_list, regex);

    return regex_list;
}


PPRE_TOKEN parse_data_prev(unsigned char* data, size_t data_size)
{
    PPRE_TOKEN pre_token_sp,pre_token_cur,pre_token_pre;
    PREGEX_LIST regex_list;
    size_t data_i, data_checked_len;
    int i;

    pre_token_sp = pre_token_init(TOKEN_TYPE_ERROR, 0,0,1);
    pre_token_cur = pre_token_sp;

    regex_list = parse_build_regex_list();
    data_i = 0;
    while(data_i < data_size)
    {
        for(i=0;i<regex_list->len;i++)
        {
            if(regex_check(regex_list->list[i], &data[data_i], data_size - data_i, &data_checked_len))
            {
                pre_token_cur->next = pre_token_init(regex_list->list[i]->token_type, data_i, data_i + data_checked_len, regex_list->list[i]->can_neighbor);
                pre_token_cur = pre_token_cur->next;
                data_i += data_checked_len;
                break;
            }
        }
        // regex find failed
        if(i == regex_list->len)
        {
            pre_token_cur->next = pre_token_init(TOKEN_TYPE_ERROR, data_i, data_i + 1, 0);
            pre_token_cur = pre_token_cur->next;
            data_i += 1;
        }
    }

    // syntax checker
    pre_token_pre = pre_token_sp;
    pre_token_cur = pre_token_pre->next;
    while(pre_token_cur != NULL)
    {
        if(pre_token_pre->can_neighber == 1 || pre_token_cur->can_neighber == 1)
        {
            pre_token_pre = pre_token_cur;
            pre_token_cur = pre_token_cur->next;
            continue;
        }

        pre_token_pre->candidate_type = TOKEN_TYPE_ERROR;
        pre_token_pre->data_ep = pre_token_cur->data_ep;
        pre_token_pre->next = pre_token_cur->next;
        pre_token_pre->can_neighber = 0;
        free(pre_token_cur);
        pre_token_cur = pre_token_pre->next;
    }
    regex_list_free_all(regex_list);
    return pre_token_sp;
}

int parse_data(unsigned char* data, 
                size_t data_size, 
                PTOKEN_LIST token_list, 
                PUNIQUE_LIST symbol_list, 
                PUNIQUE_LIST string_list,
                PUNIQUE_LIST etc_list)
{
    PPRE_TOKEN pre_token_sp,pre_token_cur;
    int i, data_slice_len, id;
    char* data_slice;
    pre_token_sp = parse_data_prev(data, data_size);

    
    char* TOKEN_NAME[]= {
        "ID",      
        "REAL",  
        "NUM",   
        "STRING",
        "PLUS",  
        "MINUS", 
        "MUL",   
        "DIV",   
        "ASSIGN",
        "COLON", 
        "SEMICOLON",
        "SPACE",
        "TAB",
        "ENTER",
        "ERROR",
    };
    pre_token_cur = pre_token_sp;
    while(pre_token_cur != NULL)
    {
        printf("%10s %d %3d %3d ",TOKEN_NAME[pre_token_cur->candidate_type],pre_token_cur->can_neighber,pre_token_cur->data_sp,pre_token_cur->data_ep);
        for(i=pre_token_cur->data_sp;i<pre_token_cur->data_ep;i++)
            printf("%c",data[i]);
        printf("\n");
        pre_token_cur = pre_token_cur->next;
    }

    pre_token_cur = pre_token_sp->next;
    while(pre_token_cur != NULL)
    {
        data_slice_len = pre_token_cur->data_ep - pre_token_cur->data_sp;
        data_slice = (char*)malloc(sizeof(char)*(data_slice_len+1));
        if(!data_slice)
            ERROR("malloc failed");
        memcpy(data_slice, &data[pre_token_cur->data_sp], data_slice_len);
        data_slice[data_slice_len] = 0;

        switch(pre_token_cur->candidate_type)
        {
        case TOKEN_TYPE_ID:
            id = unique_list_append(symbol_list, data_slice, data_slice_len, 1);
            break;
        case TOKEN_TYPE_STRING:
            id = unique_list_append(string_list, data_slice, data_slice_len, 0);
            break;
        default:
            id = unique_list_append(etc_list, data_slice, data_slice_len, 0);
            break;
        }
        token_list_append(token_list, pre_token_cur->candidate_type, id);
        free(data_slice);
        pre_token_cur = pre_token_cur->next;
    }

    


    pre_token_free_all(pre_token_sp);
    
    return 0;
}

