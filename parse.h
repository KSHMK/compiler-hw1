#ifndef __PARSE_H__
#define __PARSE_H__






// function field
PREGEX_LIST parse_build_regex_list(void);
PPRE_TOKEN parse_data_prev(unsigned char* data, size_t data_size);
int parse_data(unsigned char* data, 
                size_t data_size, 
                PTOKEN_LIST token_list, 
                PUNIQUE_LIST symbol_list, 
                PUNIQUE_LIST string_list,
                PUNIQUE_LIST etc_list);
#endif