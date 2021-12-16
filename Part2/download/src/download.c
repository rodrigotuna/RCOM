#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include<arpa/inet.h>

#include "../header/url.h"

int main(int argc, char *argv[]) {
    struct hostent *h;
    url_data_t url_data;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./download ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    if (valid_url(argv[1]) == 0){
        fprintf(stderr, "Url format: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    parse_url(argv[1], &url_data);

    if ((h = gethostbyname(url_data.host)) == NULL) {
        herror("gethostbyname()");
        exit(-1);
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    return 0;
}