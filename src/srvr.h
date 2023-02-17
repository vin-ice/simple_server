#ifndef SRVR_H
#define SRVR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>

#include "headers/conn.h"
#include "headers/err.h"
#include "headers/comms.h"

/**Server's socket binding address*/
#define SRVR_SOCK_PATH "/tmp/simp_srvr"
#define BUFF_SIZE 10

/** typedef enum { false, true} boolean;*/
#endif