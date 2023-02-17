#ifndef CONN_H
#define CONN_H

#include <sys/types.h>
#include <stdlib.h>

#define CONN_MAX 4
/**
 * conn_list_s - structure of connections list elements
 * @prev: Previous connection
 * @next: next connection
 * @cid: Client's connection id
 * @addr: Pair socket address: For requests/responses
*/
typedef struct conn_list_s
{
    struct conn_list_s *next;
    struct conn_list_s *prev;
    int cid;
} conn_list_t;

/**
 * conn_table_s - structure containing list of connections
 * @list: List of connections
 * @size: size of list 
*/
typedef struct conn_table_s
{
    conn_list_t *list;
    size_t size;
} conn_table_t;

typedef enum { false, true } boolean;


void conn_init(size_t);
int conn_add(int cid);
int conn_drop(int cid);
boolean conn_connected(int cfd);
conn_table_t *get_conns_struct(void);
int conn_destruct(void);

#endif