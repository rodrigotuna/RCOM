#include "../header/com.h"

#include <unistd.h>
#include <stdio.h>

#include "../header/flag.h"
#include "../header/state_machine.h"

int send_SET(int fd,status_t status) {
    char msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_RECV : MSG_A_SEND);
    msg_buf[2] = MSG_C_SET;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    return write(fd, msg_buf, 5);
}

int send_UA(int fd,status_t status) {
    int res;

    char msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_RECV : MSG_A_SEND);
    msg_buf[2] = MSG_C_UA;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    res = write(fd, msg_buf, 5);
    if(res < 0){
        perror("write");
        return -1;
    }
    return res;
}

int receive_UA(int fd, char *a_rcv, char *c_rcv){
    u_states_t state = START;
    char byte;

    while(state != END){
        read(fd, &byte, 1);
        u_state_trans(&state, byte);
    }

    *a_rcv = a_received;
    *c_rcv = c_received;

    return 0;
}