/**
 * @brief Stuffing Utilitaries
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

/**
 * 
 * @brief Destuffs frame
 * 
 * @param destuff_buf Pointer that holds the buffer after destuffing
 * @param buf Buffer to destuff
 * @param sz Size of the buffer to destuff
 * 
 * @return Size of destuffed buffer
*/
int destuff_frame(uint8_t * destuff_buf, uint8_t * buf, int sz);

/**
 * 
 * @brief Stuffs frame
 * 
 * @param stuf_buf Pointer that holds the buffer after stuffing
 * @param buf Buffer to stuff
 * @param sz Size of the buffer to stuff
 * 
 * @return Size of stuffed buffer
*/
int stuff_frame(uint8_t * stuf_buf, uint8_t * buf, int sz);

/**
 * 
 * @brief Calculates the BCC2 of a data buffer 
 * 
 * @param buf Data buffer to calculate the BCC2 from
 * @param sz Size of the data buffer
 * 
 * @return Value of the BCC2
*/
uint8_t bcc_buf(uint8_t * buf, int sz);

#endif // _UTILS_H