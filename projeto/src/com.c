#include "../header/com.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../header/flag.h"
#include "../header/state_machine.h"
#include "../header/utils.h"

int send_command(int fd, status_t status, char ctrl){
    int res;

    char msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_RECV : MSG_A_SEND);
    msg_buf[2] = ctrl;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    res = write(fd, msg_buf, 5);
    if(res < 0){
        perror("write");
        return -1;
    }
    return res;
}

int send_response(int fd, status_t status, char ctrl){
    int res;

    char msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_SEND : MSG_A_RECV);
    msg_buf[2] = ctrl;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    res = write(fd, msg_buf, 5);
    if(res < 0){
        perror("write");
        return -1;
    }
    return res;
}


int send_I_FRAME(int fd, char * buf, int size){

}

int receive_U(int fd, char *a_rcv, char *c_rcv){
    u_states_t state = U_START;
    char byte;

    while(state != U_END){
        if (read(fd, &byte, 1) != 1) {
            perror("read");
            return -1;
        }
        if (u_state_trans(&state, byte, a_rcv, c_rcv) < 0) return -1;
    }

    return 0;
}

int receive_S(int fd, char* a_rcv, char* c_rcv) {
    s_states_t state = S_START;
    char byte;

    while(state != S_END){
        if (read(fd, &byte, 1) != 1) {
            perror("read");
            return -1;
        }
        if (s_state_trans(&state, byte, a_rcv, c_rcv) < 0) return -1;
    }

    return 0;
}

int receive_I(int fd, char* buffer) {
    i_states_t state = I_START;
    char byte;
    char buf[1000];
    int sz = 0;

    while(state != I_END){
        if (read(fd, &byte, 1) != 1) {
            perror("read");
            return -1;
        }
        if (i_state_trans(&state, byte, &sz, buf) < 0) return -1;
    }

    char buf_destuff [1000];
    int destuff_sz = destuff_frame();

    if(buf_destuff[destuff_sz - 1] != bcc_buf(buf_destuff, destuff_sz-1)){
        return -1;
    }

    memcpy(buffer, buf_destuff, destuff_sz-1);

    return sz-1;
}