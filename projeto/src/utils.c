#include "../header/utils.h"


int destuff_frame(){
    return 0;
}
int stuff_frame();

char bcc_buf(char * buf, int sz){
    char res = 0;
    for(int i = 0; i < sz; i++){
        res ^= buf[i];
    }
    return res;
}