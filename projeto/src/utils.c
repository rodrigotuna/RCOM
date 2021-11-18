#include "../header/utils.h"

#include "../header/flag.h"


int destuff_frame(char * destuff_buf, char * buf, int sz){
    int destuff_sz = 0;
    for(int i = 0; i < sz; i++){
        if(buf[i] == MSG_ESC){
            destuff_buf[destuff_sz++] = MSG_STUFF(buf[++i]);
        } else destuff_buf[destuff_sz++] = buf[i];
    }
    return destuff_sz;

}

int stuff_frame(char * stuff_buf, char * buf, int sz){
    int stuff_sz = 0;
    for(int i = 0; i < sz; i++){
        if(buf[i] == MSG_FLAG || buf[i] == MSG_ESC){
            stuff_buf[stuff_sz++] = MSG_ESC;
            stuff_buf[stuff_sz++] = MSG_STUFF(buf[i]);
        } else stuff_buf[stuff_sz++] = buf[i];
    }
    return stuff_sz;
}

char bcc_buf(char * buf, int sz){
    char res = 0;
    for(int i = 0; i < sz; i++){
        res ^= buf[i];
    }
    return res;
}