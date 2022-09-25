#ifndef __PARSE_H__
#define __PARSE_H__

// data structure field
typedef struct table {
    int idx;
    char* data;
    unsigned int hash;
    struct table *next;
} TABLE;

// function field
//int parse_file(FILE* fp,)

#endif