#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>
#include <stdint.h>

#include "ll.h"

#define DATA_PACKET_MAX_SIZE  4096

#define DATA_CTRL 1
#define START_CTRL 2
#define END_CTRL 3

#define NUM_FIELDS_CTRL 5
#define NUM_FIELDS_DATA 4

#define FILE_SIZE_FLAG 0
#define FILE_NAME_FLAG 1

#define OFFSET 0xFF

#define CTRL 0
#define T1_SIZE 1
#define L1_SIZE 2
#define V1_SIZE 3
#define T2_NAME 7
#define L2_NAME 8
#define V2_NAME 9

#define N_DATA 1
#define L2_DATA 2
#define L1_DATA 3
#define P_DATA 4

int fileDescriptor; // Descriptor corresponding to the serial port

int application (int fd, status_t status, char* path);

int app_send_data_packet(char* data, uint32_t data_size, unsigned int sequence_number);

int app_send_control_packet(int ctrl_flag, uint32_t file_size, const char* filename);

int app_send_file(int fd, char* path);

int app_receive_data_packet(char * data, int sequence_number);

int app_receive_control_packet(int ctrl_flag, unsigned int * file_size, char* filename);

int app_receive_file();

#endif // _APP_H