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
    PTOKEN_LIST token_list = NULL;
    PUNIQUE_LIST symbol_list = NULL, string_list = NULL, etc_list = NULL;
    
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

    token_list = token_list_init();
    symbol_list = unique_list_init();
    string_list = unique_list_init();
    etc_list = unique_list_init();
    if(!symbol_list || !string_list || !etc_list)
    {
        printf("list init failed\n");
        goto _END;
    }
    result = parse_data(data, data_size, token_list, symbol_list, string_list, etc_list);

    if(result < 0)
    {
        printf("parser failed\n");
        goto _END;
    }

    printf("Token List\n");
    token_list_print(token_list, symbol_list, string_list, etc_list);
    printf("Symbol Table\n");
    unique_list_print(symbol_list);
    printf("String Table\n");
    unique_list_print(string_list);

_END:
    if(fd)
        close(fd);
    if(token_list)
        token_list_free(token_list);
    if(symbol_list)
        unique_list_free(symbol_list);
    if(string_list)
        unique_list_free(string_list);
    if(etc_list)
        unique_list_free(etc_list);
    return 0;
}