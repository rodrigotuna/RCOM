#ifndef _LL_H_
#define _LL_H_

typedef enum {
    TRANSMITTER,
    RECEIVER
} status_t;

int ll_open(int fd, status_t status);
int ll_read(int fd, char * buffer);
int ll_write(int fd, char * buffer, int length);
int ll_close(int fd);

#endif // _LL_H