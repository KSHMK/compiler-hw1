#ifndef __TABLE_DATA_H__
#define __TABLE_DATA_H__

// data structure field

typedef struct token {
    int type;
    union {
        int i;
        double f;
    } value;
} TOKEN, *PTOKEN;

typedef struct token_list {
    int num;
    int size;
    struct token** list;
} TOKEN_LIST, *PTOKEN_LIST;

typedef struct unique {
    char* data;
    int data_size;
    unsigned int hash;
} UNIQUE, *PUNIQUE;

typedef struct unique_list {
    int num;
    int size;
    struct unique **list;
} UNIQUE_LIST, *PUNIQUE_LIST;



// function field
PTOKEN_LIST token_list_init(void);
void token_list_free(PTOKEN_LIST token_list);
void token_list_print(PTOKEN_LIST token_list);

PUNIQUE_LIST unique_list_init(void);
void unique_list_free(PUNIQUE_LIST unique_list);

int unique_list_append(PUNIQUE_LIST unique_list, char* data, int data_size, int limit_cmp);
void unique_list_print(PUNIQUE_LIST unique_list);

// crc32 hash function
unsigned int CRC32(const char* data, int data_length);

#endif