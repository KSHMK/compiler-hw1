#ifndef __TABLE_DATA_H__
#define __TABLE_DATA_H__

// data structure field

enum TOKEN_TYPE {
    TOKEN_TYPE_ID,
    TOKEN_TYPE_NUM,
    TOKEN_TYPE_REAL,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MUL,
    TOKEN_TYPE_DIV ,
    TOKEN_TYPE_ASSIGN,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_SPACE,
    TOKEN_TYPE_TAB,
    TOKEN_TYPE_ENTER,
    TOKEN_TYPE_ERROR,
};


typedef struct token {
    enum TOKEN_TYPE type;
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
    int list_len;
    CHECK_FP *check_list;
    struct automata **next_list;
} AUTOMATA, *PAUTOMATA;


typedef struct regex {
    enum TOKEN_TYPE token_type;
    struct automata* start;
} REGEX, *PREGEX;

typedef struct regex_list {
    int len;
    int size;
    PREGEX* list;
} REGEX_LIST, *PREGEX_LIST;

typedef struct pre_token {
    enum TOKEN_TYPE token_candidate;
    int data_sp, data_ep;
    struct pre_token* next;
} PRE_TOKEN, *PPRE_TOKEN;


// function field
// token list
PTOKEN_LIST token_list_init(void);
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
PREGEX regex_init(void);
PREGEX_LIST regex_list_init(void);

#endif