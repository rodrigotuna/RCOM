#include "../header/ll.h"

#include "../header/com.h"
#include "../header/flag.h"

int ll_open(int fd, status_t status){
    if(status == TRANSMITTER){
        int tries;
        for(tries = 0; tries < 3; tries++){
            if (send_SET(fd,status) < 0){
                return -1;
            }

            char a_rcv, c_rcv;
            receive_UA(fd, &a_rcv, &c_rcv); 

            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_UA){
                break;
            }   
        }
        if(tries == 3) return -1;

    } else if (status == RECEIVER){
        int tries;
        for(tries = 0; tries < 3; tries++){
            char a_rcv, c_rcv;
            receive_UA(fd, &a_rcv, &c_rcv);

            if(a_rcv == MSG_A_SEND && c_rcv == MSG_C_SET){
                    if (send_UA(fd, status) < 0){
                        return -1;
                    };
                    break;
            }
        }
        if(tries == 3) return -1;
    }
    return 0;
}

int ll_read(int fd, char * buffer);
int ll_write(int fd, char * buffer, int length);
int ll_close(int fd);