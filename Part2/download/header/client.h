#ifndef _CLIENT_H_
#define _CLIENT_H_

int init_socket(char * ip, int port);
void send_command(int sockfd, char * cmd, char * arg);
void read_response(int sockfd, char * response);
int parse_pasv(char * response);

#endif // _CLIENT_H_