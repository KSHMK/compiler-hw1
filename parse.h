#ifndef __PARSE_H__
#define __PARSE_H__




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