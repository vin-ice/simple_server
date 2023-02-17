#include "srvr.h"
/**
 * _shutdown - graceful server shutdown
 * 
*/
void _shutdown(void)
{
    remove(SRVR_SOCK_PATH);
    conn_destruct();
    comms_destruct();
}
/**
 * main - driver function of the server program
 * @argc: argument count
 * @argv: arguments vector
 * 
 * Return: returns 0
*/
int main(int argc, char argv)
{
    int sfd, cfd; /*client and server "connections"*/
    struct sockaddr_un addr, svaddr;
    struct stat sb;
    int n = 0, opt;

    while ((getopt(argc, argv, "n")) != -1)
    {
        switch (opt)
        {
            case 'n' : 
                n = atoi(optarg) ? atoi(optarg) : 0;
                break;
            default:
                break;
        }
    }


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
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SRVR_SOCK_PATH, sizeof(svaddr.sun_path) - 1);
    
    /**bind socket to an address*/
    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
        err_exit("bind");

    conn_init(n);/**connections struct*/
    comms_init(n);
    if (listen(sfd, 5) == -1)
        err_exit("listen");

    for (;;)
    {
        /**TODO: send regular KEEPALIVE so as to update structures aaccordingly*/
        if ((cfd = accept(sfd, (struct sockaddr *) &addr, (socklen_t *) sizeof(struct sockaddr_un))) == -1)
            err_exit("accept");

        switch (conn_add(cfd))/**add connection to list*/
        {
            case 0:
                switch (fork())/**handle each connection with its own process*/
                {
                    case -1: /**failed fork instance: log to file*/
                        close(cfd);
                        break;
                    case 0: /**Child process*/
                        close(sfd);
                        comms_eval(cfd, &addr);
                    default:
                        close(cfd);
                        break; /**continues to listen to next*/
                }
                break;
            case -2:
           /**hold or close*/
                break;
            default:
                break;
        }
        
    }
    _shutdown();
    return (0);
}