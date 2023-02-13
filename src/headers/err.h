#ifndef ERR_H
#define ERR_H

void _shutdown(void);
void err_msg(const char *fmt, ...);
void err_exit(const char *fmt, ...);

#endif