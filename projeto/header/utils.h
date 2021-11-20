#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

int destuff_frame(uint8_t * destuff_buf, uint8_t * buf, int sz);
int stuff_frame(uint8_t * stuf_buf, uint8_t * buf, int sz);
uint8_t bcc_buf(uint8_t * buf, int sz);

#endif // _UTILS_H