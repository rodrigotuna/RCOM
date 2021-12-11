/**
 * @brief Application Layer
 */

#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>
#include <stdint.h>

#include "ll.h"

#define DATA_PACKET_MAX_SIZE  4096 

// Flags
#define DATA_CTRL 1
#define START_CTRL 2
#define END_CTRL 3

//Number of Fields in App Packets
#define NUM_FIELDS_CTRL 5
#define NUM_FIELDS_DATA 4

//Shift Offset
#define OFFSET 0xFF

//Control Packet Fields Flags
#define CTRL 0
#define FILE_SIZE_FLAG 0
#define FILE_NAME_FLAG 1
#define T1_SIZE 1
#define L1_SIZE 2
#define V1_SIZE 3
#define T2_NAME 7
#define L2_NAME 8
#define V2_NAME 9

//Data Packet Fields Flags
#define N_DATA 1
#define L2_DATA 2
#define L1_DATA 3
#define P_DATA 4

// File descriptor corresponding to the serial port
int fileDescriptor;

/**
 * 
 * @brief Initializes the application
 * 
 * @param fd File descriptor corresponding to the serial port
 * @param status Status of the application, Transmitter or Receiver
 * @param path Path of the file to transfer
 * @return 0 on success, -1 otherwise
 * 
 * */
int application (int fd, status_t status, char* path);

/**
 * 
 * @brief Sends the data packet
 * 
 * @param data Data packet from the file to be transferred
 * @param data_size Size of the data to send in bytes
 * @param sequence_number Sequence Number of the data packet
 * @return 0 on success, -1 otherwise
 * 
 * */
int app_send_data_packet(char* data, uint32_t data_size, unsigned int sequence_number);

/**
 * 
 * @brief Sends the control packet
 * 
 * @param ctrl_flag Flag identifier of the control message to send, START or END
 * @param data_size Size of the file to be transferred
 * @param filename Name of the file to be transferred
 * @return 0 on success, -1 otherwise
 * 
 * */
int app_send_control_packet(int ctrl_flag, uint32_t file_size, const char* filename);

/**
 * 
 * @brief Sends file
 * 
 * @param path Path of the file to be transferred
 * @return 0 on success, -1 otherwise
 * 
 * */
int app_send_file(char* path);

/**
 * 
 * @brief Receives the data packet
 * 
 * @param data Pointer where the data will be stored
 * @param sequence_number Expected Sequence Number
 * @return Total size of the data packet received on success, -1 otherwise
 * 
 * */
int app_receive_data_packet(char * data, int sequence_number);

/**
 * 
 * @brief Sends the control packet
 * 
 * @param ctrl_flag Flag identifier of the control message expected to be received, START or END
 * @param file_size Pointer where the file size will be stored
 * @param filename Pointer where the file name will be stored
 * @return 0 on success, -1 otherwise
 * 
 * */
int app_receive_control_packet(int ctrl_flag, unsigned int * file_size, char* filename);

/**
 * 
 * @brief Receives file
 * 
 * @return 0 on success, -1 otherwise
*/
int app_receive_file();

#endif // _APP_H