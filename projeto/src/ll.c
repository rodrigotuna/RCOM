#include "../header/ll.h"

#include "../header/com.h"
#include "../header/flag.h"

status_t status;

int ll_open(int fd, status_t st){
    status = st;
    if(status == TRANSMITTER){
        int tries;
        for(tries = 0; tries < 3; tries++){
            if (send_command(fd,status, MSG_C_SET) < 0){
                return -1;
            }

            char a_rcv, c_rcv;
            receive_U(fd, &a_rcv, &c_rcv); 

            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_UA){
                break;
            }   
        }
        if(tries == 3) return -1;

    } else if (status == RECEIVER){
        int tries;
        for(tries = 0; tries < 3; tries++){
            char a_rcv, c_rcv;
            receive_S(fd, &a_rcv, &c_rcv);

            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_SET){
                    if (send_response(fd, status, MSG_C_UA) < 0){
                        return -1;
                    };
                    break;
            }
        }
        if(tries == 3) return -1;
    }
    Ns = 1; Nr = 0;
    return 0;
}

int ll_read(int fd, char * buffer) {
    Ns = (Ns + 1)%2;
    Nr = (Nr + 1)%2;

    int tries,res;
    for(tries = 0; tries < 3; tries++){
        res = receive_I(fd, buffer);

        if(res <= 0){
            send_response(fd, status, MSG_C_REJ(Nr));
        } else {
            send_response(fd, status, MSG_C_RR(Nr));
            break;
        }
    }
    if(tries == 3) return -1;
    return res;
}
int ll_write(int fd, char * buffer, int length){
    Ns = (Ns + 1)%2;
    Nr = (Nr + 1)%2;
 
    int tries,res;
    for(tries = 0; tries < 3; tries++){
        res = send_I_FRAME(fd, buffer, length);

        if(res != length){
            continue;
        }

        char a_rcv, c_rcv;
        res = receive_S(fd, &a_rcv, &c_rcv);

        if(res < 0) continue;
        if(a_rcv == MSG_A_SEND){
            if(c_rcv == MSG_C_RR(Nr)) break;
            else if(c_rcv == MSG_C_REJ(Nr)) continue;
        }
    }
    if(tries == 3) return -1;
    return res;
}

int ll_close(int fd){
    int tries,res;
    if(status == TRANSMITTER){
        for(tries = 0; tries < 3; tries++){
            if (send_command(fd,status, MSG_C_DISC) < 0){
                return -1;
            }

            char a_rcv, c_rcv;
            res = receive_S(fd, &a_rcv, &c_rcv); 
            if(res < 0) continue;
            else if(a_rcv == MSG_A_RECV && c_rcv == MSG_C_DISC){
                break;
            }   
        }
        if(tries == 3) return -1;

        res = send_response(fd, status, MSG_C_UA);
    } else if (status == RECEIVER){
        char a_rcv, c_rcv;
        res = receive_S(fd, &a_rcv, &c_rcv);

        if(res < 0) return -1;
        if(a_rcv == MSG_A_RECV && c_rcv == MSG_C_DISC){
        
        } else {
            return -1;
        }

        res = send_command(fd, status, MSG_C_DISC);

        if(res < 0 ) return -1;

        res = receive_U(fd, &a_rcv, &c_rcv);

        if(res < 0) return -1;

        if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_UA){
            
        } else {
            return -1;
        }
    }

    return res;
}