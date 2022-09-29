#ifndef __PARSE_H__
#define __PARSE_H__



typedef int (*CHECK_FP)(char);


// struct field
typedef struct automata {
    int is_end;
    int list_len;
    CHECK_FP *check_list;
    struct automata **next_list;
} AUTOMATA, *PAUTOMATA;


typedef struct regex {
    int token;
    struct automata* start;
} REGEX, *PREGEX;

void token_init();
void token_parse();
void syntex_check();
void syntex_save();

// function field
int parse_data(unsigned char* data, 
                size_t data_size, 
                PTOKEN_LIST token_list, 
                PUNIQUE_LIST symbol_list, 
                PUNIQUE_LIST string_list,
                PUNIQUE_LIST etc_list);
#endif