#include "srvr.h"

/**
 * comms_req - Reads and packages request from given fd
 * @cfd: Connection id; Used to tap into communications
 * @addr: Address to read comm/req 
*/
void comms_req(int cfd, struct sockaddr_un addr)
{
    char *c_buff[C_BUFF_SIZE];
    ssize_t size;
    req_t req;

    if (!conn_connected(cfd))
        err_exit("Connection not found! Try CONNECTING again!");

    if ((size = recvfrom(cfd, c_buff, C_BUFF_SIZE, 0, &addr, sizeof(struct sockaddr_un))) == -1)
    {    
        /**log and exit*/
        return;
    }
    else
    {
       strncpy(req.request, c_buff, size);
        req.size = size;
        req.res_id = 0;

        if (req.res_id == 0)/**try sending out to subordinate client*/
            comms_post();
    }
}