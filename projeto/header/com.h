#ifndef _COM_H_
#define _COM_H_

#include "ll.h"

int Ns;
int Nr;


int send_command(int fd, status_t status, char ctrl);
int send_response(int fd, status_t status, char ctrl);
int send_I_FRAME(int fd, char * buf, int size);
int receive_U(int fd, char* a_rcv, char* c_rcv);
int receive_S(int fd, char* a_rcv, char* c_rcv);
int receive_I(int fd, char* buffer);

#endif // _COM_H