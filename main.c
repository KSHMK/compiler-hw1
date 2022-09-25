#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include "parse.h"

int main(int argc, char* argv[])
{
    int fd;
    int result;
    unsigned char* data;
    size_t data_size, mem_size;
    struct stat sb;
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
    //result = parse_file(fp,);

_END:
    if(fd)
        close(fd);

    return 0;
}