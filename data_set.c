#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_set.h"
#include "utils.h"

// ==================================
// token list function

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

PPRE_TOKEN pre_token_init(TOKEN_TYPE token_type, int sp, int ep, int can_neighbor)
{
    PPRE_TOKEN pre_token;
    pre_token = (PPRE_TOKEN)malloc(sizeof(PRE_TOKEN));
    if(!pre_token)
        ERROR("malloc failed");

    pre_token->candidate_type = token_type;
    pre_token->can_neighber = can_neighbor;
    pre_token->next = NULL;
    pre_token->data_sp = sp;
    pre_token->data_ep = ep;
    return pre_token;
}

void pre_token_free_all(PPRE_TOKEN pre_token)
{
    PPRE_TOKEN cur, next;
    cur = pre_token;
    while(cur != NULL)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }

}

PTOKEN token_init(TOKEN_TYPE token_type, int id)
{
    PTOKEN token;
    token = (PTOKEN)malloc(sizeof(TOKEN));
    if(!token)
        ERROR("malloc failed");

    token->type = token_type;
    token->id = id;
    return token;
}

PTOKEN_LIST token_list_init(void)
{
    PTOKEN_LIST token_list;
    token_list = (PTOKEN_LIST)malloc(sizeof(TOKEN_LIST));
    if(!token_list)
        ERROR("malloc failed");

    token_list->len = 0;
    token_list->size = 0x10;
    token_list->list = (PTOKEN*)malloc(sizeof(PTOKEN)*token_list->size);
    if(!token_list->list)
        ERROR("malloc failed");

    return token_list;
}

void token_list_append(PTOKEN_LIST token_list, TOKEN_TYPE token_type, int id)
{
    PTOKEN token;
    
    token = token_init(token_type, id);

    if(token_list->len == token_list->size)
    {
        token_list->size *= 2;
        token_list->list = (PTOKEN*)realloc(token_list->list, sizeof(PTOKEN)*token_list->size);
        if(!token_list->list)
            ERROR("realloc failed");
    }
    
    token_list->list[token_list->len] = token;
    token_list->len++;
}

void token_list_free(PTOKEN_LIST token_list)
{
    int i;
    for(i=0;i<token_list->len;i++)
        free(token_list->list[i]);
    free(token_list->list);
    free(token_list);
}

void token_list_print(PTOKEN_LIST token_list, 
                        PUNIQUE_LIST symbol_list, 
                        PUNIQUE_LIST string_list,
                        PUNIQUE_LIST etc_list)
{
    int i, j, id;
    int need_print;
    char* value;
    int value_len;
    int line;
    PTOKEN cur;
    
    line = 1;
    for(i=0;i<token_list->len;i++)
    {
        need_print = 1;
        id = -1;
        value = NULL;
        cur = token_list->list[i];

        switch (cur->type)
        {
        case TOKEN_TYPE_ID:
            id = cur->id;
            value = symbol_list->list[cur->id]->data;
            value_len = symbol_list->list[cur->id]->data_size;
            break;
        case TOKEN_TYPE_STRING:
            id = cur->id;
            value = string_list->list[cur->id]->data;
            value_len = string_list->list[cur->id]->data_size;
            break;
        case TOKEN_TYPE_NUM:
        case TOKEN_TYPE_REAL:
            id = -2;
        case TOKEN_TYPE_PLUS:
        case TOKEN_TYPE_MINUS:
        case TOKEN_TYPE_MUL:
        case TOKEN_TYPE_DIV :
        case TOKEN_TYPE_ASSIGN:
        case TOKEN_TYPE_COLON:
        case TOKEN_TYPE_SEMICOLON:
            value = etc_list->list[cur->id]->data;
            value_len = etc_list->list[cur->id]->data_size;
            break;
        case TOKEN_TYPE_SPACE:
        case TOKEN_TYPE_TAB:
            need_print = 0;
            break;
        case TOKEN_TYPE_ENTER:
            need_print = 0;
            line++;
            break;
        case TOKEN_TYPE_ERROR:
            need_print = 0;
            printf("Error: line %d\t", line);
            for(j=0;j<etc_list->list[cur->id]->data_size;j++)
                printf("%c", etc_list->list[cur->id]->data[j]);
            printf("\n");
            break;
        default:
            need_print = 0;
            break;
        }
        if(need_print)
        {
            printf("<%s, ", TOKEN_NAME[token_list->list[i]->type]);
            if(id == -2)
            {
                for(j=0;j<value_len;j++)
                    printf("%c", value[j]);
            }
            else if(id != -1)
                printf("%d", id);
            printf(">\t");
            for(j=0;j<value_len;j++)
                printf("%c", value[j]);
            printf("\n");
        }
    }

}

// ==================================
// unique list function

PUNIQUE_LIST unique_list_init(void)
{
    PUNIQUE_LIST unique_list;
    unique_list = (PUNIQUE_LIST)malloc(sizeof(UNIQUE_LIST));
    if(!unique_list)
        ERROR("malloc failed");

    unique_list->len = 0;
    unique_list->size = 0x10;
    unique_list->list = (PUNIQUE*)malloc(sizeof(PUNIQUE)*unique_list->size);
    if(!unique_list->list)
        ERROR("malloc failed");

    return unique_list;
}

void unique_list_free(PUNIQUE_LIST unique_list)
{
    int i;
    for(i=0;i<unique_list->len;i++)
    {
        free(unique_list->list[i]->data);
        free(unique_list->list[i]);
    }
    free(unique_list->list);
    free(unique_list);
}

int unique_list_append(PUNIQUE_LIST unique_list, char* data, int data_size, int limit_cmp)
{
    
    int i, found;
    unsigned int hash;
    PUNIQUE cur, new;
    
    if(limit_cmp == 1 && data_size > 10)
        data_size = 10;
    
    found = -1;
    hash = CRC32(data, data_size);
    // is data exist?
    for(i=0;i<unique_list->len;i++)
    {
        cur = unique_list->list[i];
        if(hash == cur->hash && data_size <= cur->data_size && !memcmp(cur->data, data, data_size))
        {
            found = i;
            break;
        }
    }
    if(found != -1)
        return found;
    
    new = (PUNIQUE)malloc(sizeof(UNIQUE));
    if(!new)
        ERROR("malloc failed");

    new->data = (char*)malloc(data_size);
    if(!new->data)
        ERROR("malloc failed");

    memcpy(new->data,data,data_size);
    new->hash = hash;
    new->data_size = data_size;

    if(unique_list->len == unique_list->size)
    {
        unique_list->size *= 2;
        unique_list->list = (PUNIQUE*)realloc(unique_list->list, sizeof(PUNIQUE)*unique_list->size);
        if(!unique_list->list)
            ERROR("realloc failed");
    }
    
    unique_list->list[unique_list->len] = new;
    unique_list->len++;
    
    return unique_list->len - 1;
}

void unique_list_print(PUNIQUE_LIST unique_list)
{
    int i, j;
    for(i=0;i<unique_list->len;i++)
    {
        printf("%4d\t",i);
        for(j=0;j<unique_list->list[i]->data_size;j++)
            printf("%c", unique_list->list[i]->data[j]);
        printf("\n");
    }
        
}

// ==================================
// regex function

PAUTOMATA automata_init(int is_end)
{
    PAUTOMATA automata;
    automata = (PAUTOMATA)malloc(sizeof(AUTOMATA));
    if(!automata)
        ERROR("malloc failed");
    
    automata->is_end = is_end;
    automata->len = 0;
    automata->size = 0x10;
    automata->check_list = (CHECK_FP*)malloc(sizeof(CHECK_FP)*automata->size);
    if(!automata->check_list)
        ERROR("malloc failed");
    automata->next_list = (PAUTOMATA*)malloc(sizeof(PAUTOMATA)*automata->size);
    if(!automata->next_list)
        ERROR("malloc failed");

    return automata;
}

void automata_connect_node_to_node(PAUTOMATA from, PAUTOMATA to, CHECK_FP check_func)
{
    if(from->len == from->size)
    {
        from->size *= 2;
        from->check_list = (CHECK_FP*)realloc(from->check_list, sizeof(CHECK_FP)*from->size);
        if(!from->check_list)
            ERROR("realloc failed");
            
        from->next_list = (PAUTOMATA*)realloc(from->next_list, sizeof(PAUTOMATA)*from->size);
        if(!from->next_list)
            ERROR("realloc failed");
    }

    from->check_list[from->len] = check_func;
    from->next_list[from->len] = to;
    from->len++;
}

void regex_automata_connection(PREGEX regex, 
                            int from_idx, 
                            int to_idx, 
                            CHECK_FP check_func)
{
    PAUTOMATA from = NULL, to = NULL;

    // start node handle
    if(from_idx == -1 && regex->len == to_idx)
    {
        to = automata_init(0);
        regex_append(regex, to);
        regex->start = to;
        return;
    }

    // end node handle
    if(to_idx == -1 && regex->len > from_idx)
    {
        from = regex->list[from_idx];
        from->is_end = 1;
        return;
    }

    // other handle
    if(regex->len > from_idx && regex->len >= to_idx)
    {
        from = regex->list[from_idx];
        if(regex->len == to_idx)
        {
            to = automata_init(0);
            regex_append(regex, to);
        }
        to = regex->list[to_idx];
        automata_connect_node_to_node(from, to, check_func);
        return;
    }
   
    ERROR("Wrong Index");
    return;
}

PREGEX regex_init(TOKEN_TYPE token_type, int can_neighbor)
{
    PREGEX regex;
    regex = (PREGEX)malloc(sizeof(REGEX));
    if(!regex)
        ERROR("malloc failed");
    
    regex->start = NULL;
    regex->token_type = token_type;
    regex->can_neighbor = can_neighbor;

    regex->len = 0;
    regex->size = 0x10;
    regex->list = (PAUTOMATA*)malloc(sizeof(PAUTOMATA)*regex->size);
    if(!regex->list)
        ERROR("malloc failed");

    return regex;
}

void regex_append(PREGEX regex, PAUTOMATA automata)
{
    if(regex->len == regex->size)
    {
        regex->size *= 2;
        regex->list = (PAUTOMATA*)realloc(regex->list, sizeof(PAUTOMATA)*regex->size);
        if(!regex->list)
            ERROR("realloc failed");
    }
    
    regex->list[regex->len] = automata;
    regex->len++;
}

int regex_check(PREGEX regex, char* data, size_t data_size, size_t* data_checked_len)
{
    int i;
    size_t data_i;
    PAUTOMATA cur_state, next_state;

    cur_state = regex->start;
    for(data_i=0;data_i<data_size;data_i++)
    {
        next_state = NULL;
        if(data[data_i] == '\\')
            data_i+=2;
        for(i=0;i<cur_state->len;i++)
        {
            if(cur_state->check_list[i](data[data_i]))
            {
                next_state = cur_state->next_list[i];
                break;
            }
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


PREGEX_LIST regex_list_init(void)
{
    PREGEX_LIST regex_list;
    regex_list = (PREGEX_LIST)malloc(sizeof(REGEX_LIST));
    if(!regex_list)
        ERROR("malloc failed");
    
    regex_list->len = 0;
    regex_list->size = 0x10;
    regex_list->list = (PREGEX*)malloc(sizeof(PREGEX)*regex_list->size);
    if(!regex_list->list)
        ERROR("malloc failed");

    return regex_list;
}

void regex_list_append(PREGEX_LIST regex_list, PREGEX regex)
{
    if(regex_list->len == regex_list->size)
    {
        regex_list->size *= 2;
        regex_list->list = (PREGEX*)realloc(regex_list->list, sizeof(PREGEX)*regex_list->size);
        if(!regex_list->list)
            ERROR("realloc failed");
    }

    regex_list->list[regex_list->len] = regex;
    regex_list->len++;

}

void regex_list_free_all(PREGEX_LIST regex_list)
{
    PREGEX regex;
    PAUTOMATA automata;
    int i, j;
    for(i=0;i<regex_list->len;i++)
    {
        regex = regex_list->list[i];
        for(j=0;j<regex->len;j++)
        {
            free(regex->list[j]->check_list);
            free(regex->list[j]->next_list);
            free(regex->list[j]);
        }
        free(regex->list);
        free(regex);
    }
    free(regex_list->list);
    free(regex_list);
}