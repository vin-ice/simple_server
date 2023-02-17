#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "../headers/comms.h"

int main(void)
{
    int fd;
    req_t req;
    char *path = "comms_file";

   
    
    
    if ((fd = open(path, O_RDONLY)) == -1)
        perror("open");
    if (_read_comms(fd, &req) < 1)
        printf("Not read anything\n");
    else
    {
        printf("%s\n",req.request);
        printf("%d\n", (int) req.size);
    }
    
    /**
    if ( < 1)
        printf("Read Nothing!\n");
    else
    {
        printf("%s", req.request);
        printf("%d", req.res_id);
    }
    */
    return (0);
}