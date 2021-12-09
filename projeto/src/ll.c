#include "../header/ll.h"

#include <stdio.h>

#include "../header/com.h"
#include "../header/flag.h"

status_t status;

int ll_open(int fd, status_t st){
    status = st;
    if(status == TRANSMITTER){
        int tries;
        for(tries = 0; tries < NUM_RETRANS; tries++){
            if (send_command(fd,status, MSG_C_SET) < 0){
                fprintf(stderr, "Error: Sending SET.\n");
                continue;
            }

            uint8_t a_rcv, c_rcv;
            if (receive_U(fd, &a_rcv, &c_rcv) < 0){
                fprintf(stderr, "Error: Couldn't receive UA, retrying.\n");
                continue;
            } else  if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_UA){
                break;
            } else {
                fprintf(stderr, "Error: Received wrong UA.\n");
            }  
        }
        if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llopen.\n");
            return -1;
        }

    } else if (status == RECEIVER){
        int tries;
        for(tries = 0; tries < NUM_RETRANS; tries++){
            uint8_t a_rcv, c_rcv;
            if (receive_S(fd, &a_rcv, &c_rcv) < 0){
                fprintf(stderr, "Error: Couldn't receive SET, retrying.\n");
                continue;
            }

            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_SET){
                if (send_response(fd, status, MSG_C_UA) < 0){
                    fprintf(stderr, "Error: Sending UA.\n");
                    return -1;
                }
                break;
            } else {
                fprintf(stderr, "Error: Received wrong SET.\n");
            }
        }
        if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llopen.\n");
            return -1;
        }
    }
    Ns = 1; Nr = 0;
    return 0;
}

int ll_read(int fd, uint8_t * buffer) {
    Ns = (Ns + 1)%2;
    Nr = (Nr + 1)%2;

    int tries,res;
    for(tries = 0; tries < NUM_RETRANS; tries++){
        res = receive_I(fd, buffer);

        if(res <= 0){
            send_response(fd, status, MSG_C_REJ(Nr));
            fprintf(stderr, "Error: Couldn't receive Information Frame, retrying.\n");
            tries--;
        } else {
            send_response(fd, status, MSG_C_RR(Nr));
            break;
        }
    }
    if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llread.\n");
            return -1;
    }
    return res;
}
int ll_write(int fd, uint8_t * buffer, int length){
    Ns = (Ns + 1)%2;
    Nr = (Nr + 1)%2;
 
    int tries,res;
    for(tries = 0; tries < NUM_RETRANS; tries++){
        res = send_I_FRAME(fd, buffer, length);

        if(res != length){
            fprintf(stderr, "Error: Sending Information Frame, retrying.\n");
            continue;
        }

        uint8_t a_rcv, c_rcv;
        res = receive_S(fd, &a_rcv, &c_rcv);

        if(res < 0){
            fprintf(stderr, "Error: Couldn't receive answer, retrying.\n");
            continue;
        }
        if(a_rcv == MSG_A_SEND){
            if(c_rcv == MSG_C_RR(Nr)) break;
            else if(c_rcv == MSG_C_REJ(Nr)) {
                fprintf(stderr, "Error: Received REJ, retrying.\n");
                tries--;
                continue;
            }
        }
    }
    if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llwrite.\n");
            return -1;
    }
    return res;
}

int ll_close(int fd){
    int tries,res;
    uint8_t a_rcv, c_rcv;
    if(status == TRANSMITTER){
        for(tries = 0; tries < NUM_RETRANS; tries++){
            if (send_command(fd,status, MSG_C_DISC) < 0){
                fprintf(stderr, "Error: Sending DISC.\n");
                return -1;
            }
 
            if((res = receive_S(fd, &a_rcv, &c_rcv)) < 0) {
                fprintf(stderr, "Error: Couldn't receive DISC, retrying.\n");
                continue;
            }
            else if(a_rcv == MSG_A_RECV && c_rcv == MSG_C_DISC){
                break;
            }else {
                fprintf(stderr, "Error: Received wrong DISC, retrying.\n");
                continue;
            }   
        }
        if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llclose.\n");
            return -1;
        }

        res = send_response(fd, status, MSG_C_UA);
    } else if (status == RECEIVER){
        for(tries = 0; tries < NUM_RETRANS; tries++){
            res = receive_S(fd, &a_rcv, &c_rcv);

            if(res < 0) {
                fprintf(stderr, "Error: Couldn't receive DISC, retrying.\n");
                continue;
            }
            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_DISC){
                if((res = send_command(fd, status, MSG_C_DISC)) < 0){
                    fprintf(stderr, "Error: Sending DISC.\n");
                    return -1;
                }
                break;
            } else {
                fprintf(stderr, "Error: Received wrong DISC.\n");
                continue;
            }
        }
        if(tries == NUM_RETRANS) {
            fprintf(stderr, "Error: Exceeded number of tries in llclose.\n");
            return -1;
        }
        res = receive_U(fd, &a_rcv, &c_rcv);

        if(res < 0) {
            fprintf(stderr, "Error: Couldn't receive UA.\n");
            return -1;
        }

        if(a_rcv != MSG_A_RECV || c_rcv != MSG_C_UA){
            fprintf(stderr, "Error: Received wrong UA.\n");
            return -1;
        }
    }
    return res;
}