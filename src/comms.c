#include "srvr.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

static comms_addr_t **c_addrs; 
static int size;
/**
 * initializes comms
 * @conns: Max connections allowed
*/
void comms_init(int conns)
{
    int i;
    comms_addr_t *head;

    size = (conns) ? conns : CONN_MAX;
    c_addrs = malloc(sizeof(comms_addr_t) * size);
    if (c_addrs == NULL)
        err_exit("malloc");
    for (i = 0; i < size; i++)
        c_addrs[i] = NULL;
}

/**
 * comms_add - Adds a mapping to c_addrs
 * @cid: Client's id
 * @addr: Address associated
*/
void comms_add(int cid, struct sockaddr_un *addr)
{
    comms_addr_t *ele;
    int i;

    ele = malloc(sizeof(comms_addr_t));
    ele->addr = addr;
    ele->cid = cid;

    for (i = 0; i < size; i++)
    {
        if (c_addrs[i] == NULL)
            c_addrs[i] = ele;
    }
}
/**
 * _read_comms - reads stream of input from socket into a req struct
 * @sfd: Socket's File descriptor
 * @buff: Buffer to read from
 * Return: returns size read 
 * 
 * Author: Michael Kerrisk
* The Linux Programming Interface
*/
ssize_t _read_comms(int sfd, req_t *req)
{
    ssize_t count, n;
    char c;

    if (req == NULL || sfd < 0)
    { 
        printf("%d\n", sfd);
        printf("Null or Empty\n");   
        return (-1);
    }

    /**
     * assumption: requests are null terminated strings
     * recepient's address (if present) is placed in subsequent line
     * TODO: Retrieve receipient's add in next line
    */
    
    for (count = 0;;)
    {
        if ((n = read(sfd, &c, 1)) == -1)
        {
            if (errno == EINTR) /**interrupted;restart*/
                continue;
            else
                return (-1);
        }
        else if (n == 0)
        {
            if (count == 0)
                return (0);
            else
                break;
        }
        else
        {
            if (c == '\n' || c == '\r')/**check next line*/
            {   
                break;
            }
            else
            {
                req->request[count] = c;
                count++;
            }
        }
    }
    req->request[count] = '\0';
    req->size = ++count;
    return (count);
}  

/**
 * comms_distribute - accepts request, and determinees client to send to 
 * depending on client's position in the conn's list
 * @req: Request to send
 * @cid: 'Master' client's connection id
*/
void comms_distribute(req_t *req, int cid)
{
    /**
     * Looks up for 'subordinate' connected client
     * finds a client whose pair add is open
     * sends out to said client
    */
    conn_list_t *list;
    comms_addr_t *head;
    

   for (list = get_conns_struct()->list; list; list = list->next)
        if (list->cid == cid)
            break;
    list = list->next; /**list of 'subordinate' clients*/
    if (list == NULL)/**add to buffer or fail*/
        comms_buff_add(req, cid);
    else
    {
        for (; list; list->next)
        {
            for (head = (*c_addrs); head; head++)
            {
                if (list->cid == head->cid)
                {   
                    write(head->cid, req->request, req->size); /**send out request to 'subordinate'*/
                } 
            }
        }
    }

}


/**
 * comms_eval - Reads, packages and excutes appropriate commands
 * request from given fd
 * @cfd: Client's communication socket
 * @addr: Pair Sockets address for client
*/
void comms_eval(int cfd, struct sockaddr_un *addr)
{
    
    ssize_t size;
    req_t req;

    if (!conn_connected(cfd)) /**connection fd not in list*/
        err_exit("Connection not found! Try CONNECTING again!");
    else
    {
        comms_add(cfd, addr);
        if ((size = _read_comms(cfd, &req)) < 1)  /**failed to read/ empty*/
        {    
            err_msg("Evaluation Error. Incomplete Message, Try Again");
            return;
        }
        else
            comms_distribute(&req, cfd); /**submit req to sender*/
        
    }
}



/**
 * comms_destruct - Destroys comms structure
 * Return: 0 if successful
*/
void comms_destruct(void)
{
    free(*c_addrs);
}