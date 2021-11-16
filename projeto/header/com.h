#ifndef _COM_H_
#define _COM_H_

#include "ll.h"

int send_SET(int fd, status_t status);
int send_UA(int fd, status_t status);
int receive_UA(int fd, char* a_rcv, char* c_rcv);
int send_DISC(int fd);

#endif // _COM_H