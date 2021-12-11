/**
 * @brief Link Layer
 */

#ifndef _LL_H_
#define _LL_H_

#include <stdint.h>

/**
 * 
 * @brief Application Status structure
*/
typedef enum {
    TRANSMITTER, // Transfers the file
    RECEIVER // Receives the File
} status_t;

#define MAX_SIZE 5001 

/**
 * 
 * @brief Initializes the Communication Protocol
 * 
 * @param fd File descriptor corresponding to the serial port
 * @param status Status of the application, Transmitter or Receiver
 * 
 * @return 0 on success, -1 otherwise
*/
int ll_open(int fd, status_t status);

/**
 * 
 * @brief Reads from the Serial Port
 * 
 * @param fd File descriptor corresponding to the serial port
 * @param buffer Buffer where the data read will be stored
 * 
 * @return Number of bytes read on success, a negative number otherwise 
*/
int ll_read(int fd, uint8_t * buffer);

/**
 * 
 * @brief Writes to the Serial Port
 * 
 * @param fd File descriptor corresponding to the serial port
 * @param buffer Buffer where the data will be written 
 * @param length Size of the data to write
 * 
 * @return Number of bytes written on success, a negative number otherwise 
*/
int ll_write(int fd, uint8_t * buffer, int length);

/**
 * 
 * @brief
 * 
 * @param fd File descriptor corresponding to the serial port
 * 
 * @return 0 if the present status is RECEIVER and the number of bytes written 
 * sending UA if the status is TRANSMITTER, a negative number otherwise
*/
int ll_close(int fd);

#endif // _LL_H