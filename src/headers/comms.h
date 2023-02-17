#ifndef COMM_H
#define COMM_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define C_BUFF_SIZE 1500

/**
 * comms_addr - Represents a mapping of a client
 * to their pair socket's address
 * @cid: Client's connection id
 * @addr: Address of pair socket
*/
typedef struct comms_addr_s
{
    int cid;
    struct sockaddr_un *addr;
} comms_addr_t;
/**
 * struct req_s - Represents communications request
 * @request: Actual request as read from comm pipeline
 * @size: Size of request read
 * @res_id: Has a client id if request is in response to a clients req
*/
typedef struct req_s
{
    char request[C_BUFF_SIZE + 1];
    int res_id;
    ssize_t *size;
} req_t;

void comms_init(int conns);
void comms_eval(int cfd, struct sockaddr_un *addr);
void comms_destruct(void);

#endif