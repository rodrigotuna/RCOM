#include "../header/utils.h"


#include "../header/flag.h"


int destuff_frame(uint8_t * destuff_buf, uint8_t * buf, int sz){
    int destuff_sz = 0;
    for(int i = 0; i < sz; i++){
        if(buf[i] == MSG_ESC){
            destuff_buf[destuff_sz++] = MSG_STUFF(buf[++i]);
        } else destuff_buf[destuff_sz++] = buf[i];
    }
    return destuff_sz;

}

int stuff_frame(uint8_t * stuff_buf, uint8_t * buf, int sz){
    int stuff_sz = 0;
    for(int i = 0; i < sz; i++){
        if(buf[i] == MSG_FLAG || buf[i] == MSG_ESC){
            stuff_buf[stuff_sz++] = MSG_ESC;
            stuff_buf[stuff_sz++] = MSG_STUFF(buf[i]);
        } else stuff_buf[stuff_sz++] = buf[i];
    }
    return stuff_sz;
}

uint8_t bcc_buf(uint8_t * buf, int sz){
    uint8_t res = 0;
    for(int i = 0; i < sz; i++){
        res ^= buf[i];
    }
    return res;
}