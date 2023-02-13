#include "srvr.h"
/**
 * print_errors - outputs errors to stdout
 * @use_err: use errno
 * @fmt: error message
 * @ap: list of arguments
 * Return: void
*/
void print_errors(boolean use_err, const char *fmt, va_list pa)
{
    vprintf(fmt, pa);
    if (use_err)
        printf(": %s", strerror(errno));
    printf("\n"); 
}
/**
 * err_msg - prints error message to stdout
 * @fmt: Initial message string to print
 * Return: void
*/
void err_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    print_errors(false, fmt, ap);
    va_end(ap);  
}
/**
 * err_exit - prints error and exits app
 * @fmt: initial error text
*/
void err_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    print_errors(true, fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}