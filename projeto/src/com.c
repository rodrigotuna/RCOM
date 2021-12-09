#include "../header/com.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../header/flag.h"
#include "../header/state_machine.h"
#include "../header/utils.h"

int error;
int t_prop;

void set_error(int n){
    error = n;
}

void set_t_prop(int n){
    t_prop = n;
}

int send_command(int fd, status_t status, uint8_t ctrl){
    int res;

    uint8_t msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_RECV : MSG_A_SEND);
    msg_buf[2] = ctrl;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    res = write(fd, msg_buf, 5);
    if(res < 0){
        fprintf(stderr, "Error: Write.\n");
        return -1;
    }
    return res;
}

int send_response(int fd, status_t status, uint8_t ctrl){
    int res;

    uint8_t msg_buf[5];
    msg_buf[0] = MSG_FLAG;
    msg_buf[1] = (status == RECEIVER ? MSG_A_SEND : MSG_A_RECV);
    msg_buf[2] = ctrl;
    msg_buf[3] = msg_buf[1] ^ msg_buf[2];
    msg_buf[4] = MSG_FLAG;

    res = write(fd, msg_buf, 5);
    if(res < 0){
        fprintf(stderr, "Error: Write.\n");
        return -1;
    }
    return res;
}


int send_I_FRAME(int fd, uint8_t * buf, int size){
    int res;

    uint8_t header[4];
    header[0] = MSG_FLAG;
    header[1] = MSG_A_SEND;
    header[2] = MSG_C_I(Ns);
    header[3] = header[2] ^ header[1];

    res = write(fd, header, 4);
    if(res < 0){
        fprintf(stderr, "Error: Write.\n");
        return -1;
    }

    uint8_t bcc2 = (rand()%100 >= error ? bcc_buf(buf, size) : ~bcc_buf(buf, size));
    buf[size] = bcc2;

    uint8_t stuff_buf[2*MAX_SIZE];
    int stuff_sz = stuff_frame(stuff_buf, buf, size+1);

    res = write(fd, stuff_buf, stuff_sz);
    if(res < 0){
        fprintf(stderr, "Error: Write.\n");
        return -1;
    }

    uint8_t tail[1];
    tail[0] = MSG_FLAG;

    res = write(fd, tail, 1);
    if(res < 0){
        fprintf(stderr, "Error: Write.\n");
        return -1;
    }

    return size;
}

int receive_U(int fd, uint8_t *a_rcv, uint8_t *c_rcv){
    usleep(t_prop);
    u_states_t state = U_START;
    uint8_t byte;

    while(state != U_END){
        if (read(fd, &byte, 1) != 1) {
            fprintf(stderr, "Error: Exceeded read time.\n");
            return -1;
        }
        if (u_state_trans(&state, byte, a_rcv, c_rcv) < 0) return -1;
    }

    return 0;
}

int receive_S(int fd, uint8_t* a_rcv, uint8_t* c_rcv) {
    usleep(t_prop);
    s_states_t state = S_START;
    uint8_t byte;

    while(state != S_END){
        if (read(fd, &byte, 1) != 1) {
            fprintf(stderr, "Error: Exceeded read time.\n");
            return -1;
        }
        if (s_state_trans(&state, byte, a_rcv, c_rcv) < 0) return -1;
    }

    return 0;
}

int receive_I(int fd, uint8_t* buffer) {
    usleep(t_prop);
    i_states_t state = I_START;
    uint8_t byte;
    uint8_t buf[2*MAX_SIZE];
    int sz = 0;
    while (state != I_END){
        while(state != I_END && state != I_REP_END){
            if (read(fd, &byte, 1) != 1) {
                fprintf(stderr, "Error: Exceeded read time.\n");
                return -1;
            }
            if (i_state_trans(&state, byte, &sz, buf) < 0) return -1;
        }
        if(state == I_REP_END){
            send_response(fd, RECEIVER, MSG_C_RR((Nr + 1)%2));
            fprintf(stderr, "Error: Received repeated Information Frame, sending RR.\n");
            state = I_START;
        }
    }

    uint8_t buf_destuff [MAX_SIZE];
    int destuff_sz = destuff_frame(buf_destuff, buf, sz);

    if(buf_destuff[destuff_sz - 1] != bcc_buf(buf_destuff, destuff_sz-1)){
        fprintf(stderr, "Error: Wrong BCC2.\n");
        return -1;
    }

    memcpy(buffer, buf_destuff, destuff_sz-1);

    return sz-1;
}