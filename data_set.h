#ifndef __TABLE_DATA_H__
#define __TABLE_DATA_H__

// data structure field

typedef enum token_type {
    TOKEN_TYPE_ID,
    TOKEN_TYPE_REAL,
    TOKEN_TYPE_NUM,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MUL,
    TOKEN_TYPE_DIV,
    TOKEN_TYPE_ASSIGN,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_SPACE,
    TOKEN_TYPE_TAB,
    TOKEN_TYPE_ENTER,
    TOKEN_TYPE_ERROR,
} TOKEN_TYPE;

typedef struct pre_token {
    TOKEN_TYPE candidate_type;
    int can_neighber;
    int data_sp, data_ep;
    struct pre_token* next;
} PRE_TOKEN, *PPRE_TOKEN;

typedef struct token {
    TOKEN_TYPE type;
    int id;
} TOKEN, *PTOKEN;

typedef struct token_list {
    int len;
    int size;
    struct token** list;
} TOKEN_LIST, *PTOKEN_LIST;

typedef struct unique {
    char* data;
    int data_size;
    unsigned int hash;
} UNIQUE, *PUNIQUE;

typedef struct unique_list {
    int len;
    int size;
    struct unique **list;
} UNIQUE_LIST, *PUNIQUE_LIST;


typedef int (*CHECK_FP)(char);

typedef struct automata {
    int is_end;
    int len;
    int size;
    CHECK_FP *check_list;
    struct automata **next_list;
} AUTOMATA, *PAUTOMATA;

typedef struct regex {
    int len;
    int size;
    int can_neighbor;
    TOKEN_TYPE token_type;
    struct automata* start;
    struct automata **list;
} REGEX, *PREGEX;

typedef struct regex_list {
    int len;
    int size;
    PREGEX* list;
} REGEX_LIST, *PREGEX_LIST;


// function field
// token list
PPRE_TOKEN pre_token_init(TOKEN_TYPE token_type, int sp, int ep, int can_neighbor);
void pre_token_free_all(PPRE_TOKEN pre_token);
PTOKEN token_init(TOKEN_TYPE token_type, int id);
PTOKEN_LIST token_list_init(void);
void token_list_append(PTOKEN_LIST token_list, TOKEN_TYPE token_type, int id);
void token_list_free(PTOKEN_LIST token_list);
void token_list_print(PTOKEN_LIST token_list, 
                        PUNIQUE_LIST symbol_list, 
                        PUNIQUE_LIST string_list,
                        PUNIQUE_LIST etc_list);

// unique list
PUNIQUE_LIST unique_list_init(void);
void unique_list_free(PUNIQUE_LIST unique_list);

int unique_list_append(PUNIQUE_LIST unique_list, char* data, int data_size, int limit_cmp);
void unique_list_print(PUNIQUE_LIST unique_list);

// regex list
PAUTOMATA automata_init(int is_end);
void automata_connect_node_to_node(PAUTOMATA from, PAUTOMATA to, CHECK_FP check_func);
void regex_automata_connection(PREGEX regex, int from_idx, int to_idx, CHECK_FP check_func);
PREGEX regex_init(TOKEN_TYPE token_type, int can_neighbor);
void regex_append(PREGEX regex, PAUTOMATA automata);
int regex_check(PREGEX regex, char* data, size_t data_size, size_t* data_checked_len);
PREGEX_LIST regex_list_init(void);
void regex_list_append(PREGEX_LIST regex_list, PREGEX regex);
void regex_list_free_all(PREGEX_LIST regex_list);
#endif