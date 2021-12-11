#include "../header/app.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

int application (int fd, status_t status, char* path) {

    if(ll_open(fd, status) < 0) return -1;

    fileDescriptor = fd;

    if(status == TRANSMITTER){
        if(app_send_file(path) < 0){
            fprintf(stderr, "Error: Couldn't send file to Receiver.\n");
            return -1;
        }
    }
    else {
        if(app_receive_file(fd) < 0){
            fprintf(stderr, "Error: Couldn't receive file from Transmitter.\n");
            return -1;
        }
    }

    if(ll_close(fd) < 0) return -1;
    fprintf(stdout, "File transmitted successfully.\n");
    return 0;
}

int app_send_data_packet(char* data, uint32_t data_size, unsigned int sequence_number){

    if(data_size > DATA_PACKET_MAX_SIZE){
        fprintf(stderr, "Error: Data packet has a larger size than the maximum allowed.\n");
        return -1;
    }

    uint8_t * data_packet = (uint8_t *) malloc(data_size + NUM_FIELDS_DATA);

    data_packet[CTRL] = DATA_CTRL;
    data_packet[N_DATA] = sequence_number % 255; 

    data_packet[L2_DATA] = (data_size >> 8) & OFFSET;
    data_packet[L1_DATA] = data_size & OFFSET;

    memcpy(data_packet + P_DATA, data, data_size);

    if(ll_write(fileDescriptor, data_packet, data_size + NUM_FIELDS_DATA) < 0){
        fprintf(stderr, "Error: Couldn't write data packet.\n");
        free(data_packet);
        return -1;
    }

    free(data_packet);

    return 0;
}

int app_send_control_packet(int ctrl_flag, uint32_t file_size, const char* filename){

    uint32_t ctrl_packet_size = sizeof(uint32_t) + strlen(filename) + NUM_FIELDS_CTRL;
    uint8_t * ctrl_packet = (uint8_t *) malloc(ctrl_packet_size);

    ctrl_packet[CTRL] = ctrl_flag;
    ctrl_packet[T1_SIZE] = FILE_SIZE_FLAG; 
    ctrl_packet[L1_SIZE] = sizeof(uint32_t);

    ctrl_packet[V1_SIZE] = file_size & OFFSET;
    ctrl_packet[V1_SIZE + 1] = (file_size >> 8) & OFFSET;
    ctrl_packet[V1_SIZE + 2] = (file_size >> 16) & OFFSET;
    ctrl_packet[V1_SIZE + 3] = (file_size >> 24) & OFFSET;

    ctrl_packet[T2_NAME] = FILE_NAME_FLAG;
    ctrl_packet[L2_NAME] = strlen(filename);

    memcpy(ctrl_packet + V2_NAME, filename, strlen(filename));

    if(ll_write(fileDescriptor, ctrl_packet, ctrl_packet_size) < 0){
        fprintf(stderr, "Error: Couldn't write control packet.\n");
        free(ctrl_packet);
        return -1;
    }

    free(ctrl_packet);

    return 0;
}

int app_send_file(char* path){

    FILE* file;
    unsigned int size = 0, sequence_number = 0;

    if((file = fopen(path, "rb")) == NULL){
        fprintf(stderr, "Error: Couldn't open file.\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    uint32_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *filename = basename(path) + '\0';

    if(app_send_control_packet(START_CTRL, file_size, path) < 0) return -1;

    char *data_buffer = (char*)malloc(DATA_PACKET_MAX_SIZE);

    while ((size = fread(data_buffer, sizeof(char),DATA_PACKET_MAX_SIZE,file)) > 0){

        if(app_send_data_packet(data_buffer, size,sequence_number)) return -1;

        sequence_number++;
    }

    if(app_send_control_packet(END_CTRL, file_size, filename) < 0) return -1;

    return 0;
}

int app_receive_data_packet(char * data, int sequence_number){

    uint8_t * data_packet = (uint8_t*) malloc(DATA_PACKET_MAX_SIZE + NUM_FIELDS_DATA);

    if(ll_read(fileDescriptor, data_packet) < 0){
        fprintf(stderr, "Error: Couldn't read data.\n");
        free(data_packet);
        return -1;
    }

    if(data_packet[CTRL] != DATA_CTRL){ 
        fprintf(stderr, "Error: Control Field doesn't match.");
        free(data_packet);
        return -1;
    }

    if(data_packet[N_DATA] != (sequence_number % 255)){
        fprintf(stderr, "Error: Sequence Number doesn't match.");
        free(data_packet);
        return -1;
    }

    int K = 256 * data_packet[L2_DATA] + data_packet[L1_DATA];

    if(K > DATA_PACKET_MAX_SIZE){
        fprintf(stderr, "Error: Data Recieved is larger than the maximum data size.");
        free(data_packet);
        return -1;
    }

    memcpy(data, data_packet + 4, K);

    free(data_packet);

    return K;
}

int app_receive_control_packet(int ctrl_flag, unsigned int * file_size, char* filename){

    uint8_t ctrl_packet[NUM_FIELDS_CTRL + 255 + sizeof(unsigned int)];

    if(ll_read(fileDescriptor, ctrl_packet) < 0){
        fprintf(stderr, "Error: Couldn't read control packet.\n");
        return -1;
    }

    if(ctrl_packet[CTRL] != ctrl_flag){
        fprintf(stderr, "Error: Control Field doesn't match.\n");
        return -1;
    } 

    if(ctrl_packet[T1_SIZE] != FILE_SIZE_FLAG){
        fprintf(stderr, "Error: Type of parameter doesn't match.\n");
        return -1;
    }

    if(ctrl_packet[L1_SIZE] > sizeof(uint32_t)){
        fprintf(stderr, "Error: File size in octates doesn't match (> 4).\n");
        return -1;
    }

    if(ctrl_packet[T2_NAME] != FILE_NAME_FLAG){
        fprintf(stderr, "Error: Type of parameter doesn't match.\n");
        return -1;
    }

    memcpy(file_size, ctrl_packet + V1_SIZE, ctrl_packet[L1_SIZE]);
    
    for(int i = 0; i < ctrl_packet[L2_NAME]; i++){
        filename[i] = (char) ctrl_packet[V2_NAME + i];
    }

    filename[(int) ctrl_packet[L2_NAME]] = '\0';

    return 0;
}

int app_receive_file(){
    FILE * file; 

    unsigned int file_size_start;
    char filename_start[255];

    unsigned int file_size_end;
    char filename_end[255];

    int K = 0;
    unsigned int sequence_number = 0, bytes_read = 0;

    if(app_receive_control_packet(START_CTRL, &file_size_start, filename_start) < 0) return -1;

    if((file = fopen(filename_start, "wb")) == NULL){
        fprintf(stderr, "Error: Couldn't open file.\n");
        return -1;
    }

    char *data_buffer = (char*)malloc(DATA_PACKET_MAX_SIZE);

    while(file_size_start > bytes_read){

        if((K = app_receive_data_packet(data_buffer,sequence_number)) < 0) {
            fprintf(stderr, "Error: Couldn't receive data packet.\n");
            return -1;
        };

        fwrite(data_buffer, sizeof(uint8_t), K, file);

        bytes_read += K;
        sequence_number++;

    }

    fclose(file);

    free(data_buffer);

    if(app_receive_control_packet(END_CTRL, &file_size_end, filename_end) < 0) return -1;

    if(file_size_start != file_size_end || strcmp(filename_start, filename_end) != 0){
        fprintf(stderr, "Error: Start and End Control packets don't match.");
    }

    return 0;
}