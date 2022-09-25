#ifndef __TABLE_DATA_H__
#define __TABLE_DATA_H__

// data structure field

typedef struct token {
    int type;
    union {
        int i;
        double f;
    } value;
    struct token* next;
} TOKEN, *PTOKEN;

typedef struct token_r {
    int num;
    struct token* next;
} TOKEN_R, *PTOKEN_R;

typedef struct table {
    char* data;
    int data_size;
    unsigned int hash;
    struct table *next;
} TABLE, *PTABLE;

typedef struct table_root {
    int num;
    struct table *next;
} TABLE_R, *PTABLE_R;



// function field
PTOKEN_R token_init(void);
void token_free(PTOKEN_R token_r);
void token_print(PTOKEN_R token_r);

PTABLE_R table_init(void);
void table_free(PTABLE_R table_r);

int table_append(PTABLE_R table_r, char* data, int data_size, int limit_cmp);
void table_print(PTABLE_R table_r);

// crc32 hash function
unsigned int CRC32(const char* data, int data_length);

#endif