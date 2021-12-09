#ifndef _COM_H_
#define _COM_H_

#include "ll.h"

int Ns;
int Nr;


int send_command(int fd, status_t status, uint8_t ctrl);
int send_response(int fd, status_t status, uint8_t ctrl);
int send_I_FRAME(int fd, uint8_t * buf, int size);
int receive_U(int fd, uint8_t* a_rcv, uint8_t* c_rcv);
int receive_S(int fd, uint8_t* a_rcv, uint8_t* c_rcv);
int receive_I(int fd, uint8_t* buffer);

void set_error(int n);
void set_t_prop(int n);
    
#endif // _COM_H