#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include "data_set.h"
#include "parse.h"

int main(int argc, char* argv[])
{
    int fd;
    int result;
    unsigned char* data;
    size_t data_size, mem_size;
    struct stat sb;
    PTOKEN_R token_list = NULL;
    PTABLE_R symbol_table = NULL, string_table = NULL;
    
    if(argc == 1) 
    {
        printf("Usage: %s <File>\n",argv[0]);
        goto _END;
    }
    
    fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        printf("Cannot open %s\n",argv[1]);
        goto _END;
    }
    
    if (fstat(fd, &sb) == -1)
    {
        printf("fstat failed\n");
        goto _END;
    }
    data_size = sb.st_size;

    mem_size = (data_size & ~(sysconf(_SC_PAGE_SIZE) - 1)) + sysconf(_SC_PAGE_SIZE);
    data = (unsigned char*)mmap(NULL, mem_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data == MAP_FAILED)
    {
        printf("mmap failed\n");
        goto _END;
    }

    token_list = token_init();
    symbol_table = table_init();
    string_table = table_init();
    if(!symbol_table || !string_table)
    {
        printf("table init failed\n");
        goto _END;
    }
    result = parse_file(data, data_size, token_list, symbol_table, string_table);

    if(result < 0)
    {
        printf("parser failed\n");
        goto _END;
    }

    printf("Token List\n");
    token_print(token_list);
    printf("Symbol Table\n");
    table_print(symbol_table);
    printf("String Table\n");
    table_print(string_table);

_END:
    if(fd)
        close(fd);
    if(token_list)
        token_free(token_list);
    if(symbol_table)
        table_free(symbol_table);
    if(string_table)
        table_free(string_table);
    return 0;
}