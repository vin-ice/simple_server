#include "srvr.h"
/**
 * _shutdown - graceful server shutdown
 * 
*/
void _shutdown(void)
{
    remove(SRVR_SOCK_PATH);
}
/**
 * main - driver function of the server program
 * @argc: argument count
 * @argv: arguments vector
 * 
 * Return: returns 0
*/
int main(void)
{
    int sfd, cfd; /*client and server "connections"*/
    struct sockaddr_un addr;
    struct stat sb;

    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)/**socket creation*/
        err_exit("socket");

    switch (stat(SRVR_SOCK_PATH, &sb))
    {
        case -1:
            if (errno == 2)
                break;
            err_exit("stat");
            break;
        default:
            remove(SRVR_SOCK_PATH);
    }
    /**
     * Author: Michael Kerrisk
     * The Linux Programming Interface
    */
    memset(&addr, 0, sizeof (struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SRVR_SOCK_PATH);
    
    /**bind socket to an address*/
    if (bind(sfd, (struct sockaddr *) &addr, sizeof (struct sockaddr_un)) == -1)
        err_exit("bind");

    conn_init(0);/**connections struct*/
    if (listen(sfd, 5) == -1)
        err_exit("listen");

    for (;;)
    {
        if ((cfd = accept(sfd, (struct sockaddr *) &addr, (socklen_t *) SRVR_SOCK_PATH)) == -1)
            err_exit("accept");

        switch (conn_add(cfd))/**add connection to list*/
        {
            case 0:
            switch (fork())/**handle each connection with its own process*/
            {
                case -1: /**failed fork instance: log to file*/
                    close(cfd);
                    break;
                case 0: /**Child*/
                    close(sfd);
                    comms_req(cfd, addr);
            }
                break;
            case -2:
           /**hold or close*/
                break;
            default:
                break;
        }
        
    }
    return (0);
}