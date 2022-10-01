#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_set.h"
#include "utils.h"

// ==================================
// token list function

char* TOKEN_NAME[]= {
        "ID",      
        "NUM",   
        "REAL",  
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
    int i, id;
    int need_print;
    char* value;
    PTOKEN cur;
    
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
            break;
        case TOKEN_TYPE_STRING:
            id = cur->id;
            value = string_list->list[cur->id]->data;
        case TOKEN_TYPE_NUM:
        case TOKEN_TYPE_REAL:
        case TOKEN_TYPE_PLUS:
        case TOKEN_TYPE_MINUS:
        case TOKEN_TYPE_MUL:
        case TOKEN_TYPE_DIV :
        case TOKEN_TYPE_ASSIGN:
        case TOKEN_TYPE_COLON:
        case TOKEN_TYPE_SEMICOLON:
            value = etc_list->list[cur->id]->data;
            break;
        case TOKEN_TYPE_SPACE:
        case TOKEN_TYPE_TAB:
        case TOKEN_TYPE_ENTER:
        case TOKEN_TYPE_ERROR:
        default:
            need_print = 0;
            break;
        }
        if(need_print)
        {
            printf("<%s, ", TOKEN_NAME[token_list->list[i]->type]);
            if(id != -1)
                printf("%d", id);
            printf(">\t%s\n", value);
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
    // is data exist?
    int i, cmp_size, found;
    unsigned int hash;
    PUNIQUE cur, new;
    
    cmp_size = data_size;
    if(limit_cmp == 1 && cmp_size > 10)
        cmp_size = 10;
    
    found = -1;
    hash = CRC32(data, cmp_size);
    for(i=0;i<unique_list->len;i++)
    {
        cur = unique_list->list[i];
        if(hash == cur->hash && cmp_size <= cur->data_size && !memcmp(cur->data, data, cmp_size))
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

    if(unique_list->len + 1 == unique_list->size)
    {
        unique_list->size *= 2;
        unique_list->list = (PUNIQUE*)realloc(unique_list->list, sizeof(PUNIQUE*)*unique_list->size);
        if(!unique_list->list)
            ERROR("realloc failed");
    }
    
    unique_list->list[unique_list->len] = new;
    unique_list->len++;
    
    return unique_list->len - 1;
}

void unique_list_print(PUNIQUE_LIST unique_list)
{
    int i;
    for(i=0;i<unique_list->len;i++)
        printf("%4d\t%s\n", i, unique_list->list[i]->data);
}

// ==================================
// regex function
PREGEX regex_init(void)
{
    PREGEX regex;
    regex = (PREGEX)malloc(sizeof(REGEX));
    if(!regex)
        ERROR("malloc failed");
    
    regex->start = NULL;
    regex->token_type = TOKEN_TYPE_ERROR;
    return regex;
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