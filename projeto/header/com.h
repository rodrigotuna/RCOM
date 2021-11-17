#ifndef _COM_H_
#define _COM_H_

#include "ll.h"

int Ns;
int Nr;


int send_SET(int fd, status_t status);
int send_UA(int fd, status_t status);
int send_DISC(int fd);
int receive_U(int fd, char* a_rcv, char* c_rcv);
int receive_I(int fd, char* buffer);

#endif // _COM_H