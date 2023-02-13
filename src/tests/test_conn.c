#include <stdio.h>
#include "../headers/conn.h"

void print_list(void)
{
    conn_table_t *conns;
    conn_list_t *head;

    int count;

    if ((conns = get_conns_struct()) == NULL)
    {
        printf("Struct not found");
        return;
    }
    for (head = (conns->list), count = 0; head && count < (int) conns->size; head = head->next, count++)
    {   
        if (head == NULL)
            printf("%d. (nil)\n", count);
        else
            printf("%d. %d\n", count, head->cid);
    }
}

int main(void)
{
    conn_init(3);
    print_list();
    printf("==\n");
    conn_add(3234);
    print_list();
    printf("==\n");
    conn_add(234);
    print_list();
    printf("==\n");
    conn_add(444);
    print_list();
    printf("==\n");
    conn_add(34567);
    print_list();
    conn_add(3457);
    print_list();

    printf("----\n");
    
    conn_drop(3457);
    print_list();
    printf("==\n");
    conn_drop(34567);
    print_list();
    printf("==\n");
    conn_drop(234);
    print_list();
    printf("==\n");
    conn_drop(444);
    print_list();
    printf("==\n");
    conn_drop(3234);
    print_list();
    printf("==\n");
    print_list();

   return (0);
}