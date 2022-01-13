#include "../header/client.h"

#include <netinet/ip.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/macros.h"

int init_socket(char * ip, int port){
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((void*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);      /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);               /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    return sockfd;
}

void send_command(int sockfd, char * cmd, char* arg){
    write(sockfd, cmd, strlen(cmd));
    write(sockfd, arg, strlen(arg));
    write(sockfd, "\n", 1);
}

void read_response(int sockfd, char * response){
    FILE* fp = fdopen(dup(sockfd), "r");
    size_t len = 0;
    ssize_t read;
    char *line = malloc(MAX_STR_LEN);

    while ((read = getline(&line, &len, fp)) != -1){
        if(line[3] == ' ') break;
    }

    strcpy(response, line);
    fclose(fp);
    free(line);
}

int parse_pasv(char * response){
    char first_byte[4];
    char second_byte[4];
    char * token;

    token = strtok(response, ",");

    while(token != NULL){
        memcpy(first_byte, second_byte, 4);
        memcpy(second_byte, token, 4);

        token = strtok(NULL, ",");
    }

    first_byte[3] = '\0';
    for(int i = 0; i < 4; i++){
        second_byte[i] = (isdigit(second_byte[i]) ? second_byte[i] : '\0');
    }

    return atoi(first_byte)*256 + atoi(second_byte);
}


