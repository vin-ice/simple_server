#ifndef COMM_H
#define COMM_H

#include <sys/types.h>


#define C_BUFF_SIZE 1500
/**
 * struct req_s - Represents communications request internally
 * @request: Actual request as read from comm pipeline
 * @size: Size of request read
 * @res_id: Has a client id if request is in response to a clients req
*/
typedef struct req_s
{
    char request[C_BUFF_SIZE + 1];
    ssize_t size;
    int res_id;
} req_t;

void comms_req(int cfd, struct sockaddr_un addr);

#endif