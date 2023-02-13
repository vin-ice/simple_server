#include "srvr.h"
/**
 * Module to handle all connections matters
*/

static size_t conn_max;
static conn_table_t *conns = NULL;
/**
 * conn_init - initializes the conn structures
 * @size: max connection s allowed 
 * Return: Returns void
*/  
void conn_init(size_t size)
{  
    conn_max = (size) ? size : CONN_MAX;

    conns = malloc(sizeof (conn_table_t));
    if (conns == NULL)
    {    
        perror("malloc");
        return;
    }
    conns->list = NULL;
    conns->size = 0;
    return;
}

/**
 * conn_add - adds an entry into conns list
 * @cid - connection id to add
 * Return: number indicating status of function
 *      -2 - Max client connections reached hence connection not added
 *      -1 - General failure
 *      0 - Success
*/
int conn_add(int cid)
{
    conn_list_t *head, *node;
    int count;

    if (!cid)
        return (-1);
    if (conns->size >= conn_max)
        return (-2);

    head = conns->list;

    node = malloc(sizeof(conn_list_t));
    if (node == NULL)
    {
        err_msg("malloc");
        return (-1);
    }

    node->cid = cid;
    node->next = NULL;

    if (head == NULL)/**first conn*/
    {
        node->prev = NULL;
        conns->list = node;
        conns->size += 1;
    }
    else
    {
        for (count = 0; head->next && count < (int) conn_max; head = head->next, count++)
            ;
        if (count >= (int) conn_max)/**max conns*/
        {
            free(node);
            return (-2);
        }
        else
        {
            node->next = NULL;
            node->prev = head;
            head->next = node;
            conns->size += 1; 
        }
    }
    return (0);
}

/**
 * conn_drop - removes a connection from lsit
 * @cid: Connection id
 * Rt:
*/
int conn_drop(int cid)
{
    conn_list_t *conn, *nxt, *prev;

    if (!cid)
        return (-1);
    
    if ((conn = conns->list) == NULL)
    {
        printf("Empty list");
        return (-1);
    }

    if (cid == conn->cid)/**first conn*/
    {
        nxt = conn->next;
        if (nxt)
            nxt->prev = NULL;
        conns->list = nxt;
        free(conn);
        return (0);
    }
    else
    {
        for (; conn; conn = conn->next)
        {
            if (conn->cid == cid)
            {
                nxt = conn->next;
                prev = conn->prev;
                prev->next = nxt;
                if (nxt)
                    nxt->prev = prev;
                free(conn);
                return (0);
            }
        }

    }
    return (-1);
}

/**
 * conn_connnected - Checks if client is connected
 * @cfd: Clients id
 * Returns: True if connected else false
*/
boolean conn_connected(int cfd)
{
    conn_list_t *conn;

    if (!cfd)
        retrun (false);
    
    for (conn = conns->list; conn; conn = conn->next)
        if (conn->cid == cfd)
            return (true);
    return (false);
}


/**
 * get_conns_struct - returns structure containing connection list
 * Return: Structure with list
*/
conn_table_t *get_conns_struct(void)
{
    return (conns);
}