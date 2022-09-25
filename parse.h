#ifndef __PARSE_H__
#define __PARSE_H__


// function field
int parse_file(unsigned char* data, size_t data_size, PTOKEN_R token_list, PTABLE_R symbol_table, PTABLE_R string_table);

#endif