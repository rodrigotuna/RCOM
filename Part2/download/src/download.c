#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../header/url.h"
#include "../header/macros.h"
#include "../header/client.h"

int main(int argc, char *argv[]) {
    struct hostent *h;
    url_data_t *url_data = malloc(sizeof(url_data_t));
    char *response = malloc(MAX_STR_LEN);
    int fd_request, fd_read, bytes_read;
    FILE * fp;
    char buffer[MAX_BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: ./download ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    if (valid_url(argv[1]) == 0){
        fprintf(stderr, "Url format: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    parse_url(argv[1], url_data);

    if ((h = gethostbyname(url_data->host)) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

     printf("Host name  : %s\n", h->h_name);
     printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    if ((fd_request = init_socket(inet_ntoa(*((struct in_addr *) h->h_addr)), SERVER_PORT)) < 0){
        fprintf(stderr, "Error initializing socket\n");
        exit(-1);
    }

    read_response(fd_request, response);

    if(response[0] != POS_COMPL){
        fprintf(stderr, "Not able to access website\n");
        exit(-1);
    }
    send_command(fd_request, "user ", url_data->user);
    read_response(fd_request, response);

    if(response[0] != POS_COMPL && response[0] != POS_INTER){
        fprintf(stderr, "Error: User\n");
        exit(-1);
    }

    send_command(fd_request, "pass ", url_data->pwd);
    read_response(fd_request, response);

    if(response[0] != POS_COMPL && response[0] != POS_INTER){
        fprintf(stderr, "Error: Passsword\n");
        exit(-1);
    }

    send_command(fd_request, "pasv ", "");
    read_response(fd_request, response);

    if(response[0] != POS_COMPL){
        fprintf(stderr, "Error: Entering Passive Mode\n");
        exit(-1);
    }

    fd_read = init_socket(inet_ntoa(*((struct in_addr *) h->h_addr)), parse_pasv(response));

    send_command(fd_request, "retr ", url_data->path);
    read_response(fd_request, response);

    if(response[0] != POS_PREL){
        fprintf(stderr, "Error: Retrieving File\n");
        exit(-1);
    }

    if((fp = fopen(url_data->filename, "wb")) == NULL){
        fprintf(stderr, "Error: Creating File\n");
        exit(-1);
    }

    while((bytes_read = read(fd_read, buffer, MAX_BUF_SIZE)) > 0){
		fwrite(buffer, bytes_read, 1, fp);
    }

    read_response(fd_request, response);
    if(response[0] != POS_COMPL){
        fprintf(stderr, "Error: File Transfer Unsuccessful\n");
        exit(-1);
    }


    free(url_data);
    free(response);
    fclose(fp);
    close(fd_read);
    close(fd_request);
    return 0;
}