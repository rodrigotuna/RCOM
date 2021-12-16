/**
 * @brief Link Layer Communication
 */

#ifndef _COM_H_
#define _COM_H_

#include "ll.h"

// Sequence Numbers of the Receiver and Transmitter
int Ns; 
int Nr;


/**
@brief Sends commands
 * 
 * @param fd File Descriptor of the Serial Port to write to
 * @param status Status of the application, Transmitter or Receiver
 * @param ctrl Control Field to be sent, SET and DISC
 * 
 * @return Number of bytes written to the serial port on success, -1 otherwise
*/
int send_command(int fd, status_t status, uint8_t ctrl);

/**
 * 
 * @brief Sends responses
 * 
 * @param fd File Descriptor of the Serial Port to write to
 * @param status Status of the application, Transmitter or Receiver
 * @param ctrl Control Field to be sent, RR, REJ and UA
 * 
 * @return Number of bytes written to the serial port on success, -1 otherwise
*/
int send_response(int fd, status_t status, uint8_t ctrl);

/**
 * 
 * @brief Sends I frame
 * 
 * @param fd File Descriptor of the Serial Port to write to
 * @param buf Data Buffer with the data to be transferred
 * @param size Size of the data to be transferred in the frame
 * 
 * @return The size of the data field sent on success, -1 otherwise
*/
int send_I_FRAME(int fd, uint8_t * buf, int size);

/**
 * @brief Receives U frame
 * 
 * @param fd File Descriptor of the Serial Port to read from
 * @param a_rcv Pointer where the Address Field value will be stored
 * @param c_rcv Pointer where the Control Field value will be stored
 * 
 * @return 0 on success, -1 otherwise
*/
int receive_U(int fd, uint8_t* a_rcv, uint8_t* c_rcv);

/**
 * @brief Receives S frame
 * 
 * @param fd File Descriptor of the Serial Port to read from
 * @param a_rcv Pointer where the Address Field value will be stored
 * @param c_rcv Pointer where the Control Field value will be stored
 * 
 * @return 0 on success, -1 otherwise
*/
int receive_S(int fd, uint8_t* a_rcv, uint8_t* c_rcv);

/**
 * @brief Receives I frame
 * 
 * @param fd File Descriptor of the Serial Port to read from
 * @param a_rcv Pointer where the Address Field value will be stored
 * @param c_rcv Pointer where the Control Field value will be stored
 * 
 * @return The size of the data field received on success, -1 otherwise
*/
int receive_I(int fd, uint8_t* buffer);

#endif // _COM_H