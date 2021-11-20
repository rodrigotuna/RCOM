#ifndef _LL_H_
#define _LL_H_

#include <stdint.h>

typedef enum {
    TRANSMITTER,
    RECEIVER
} status_t;

#define MAX_SIZE 4097 

int ll_open(int fd, status_t status);
int ll_read(int fd, uint8_t * buffer);
int ll_write(int fd, uint8_t * buffer, int length);
int ll_close(int fd);

#endif // _LL_H