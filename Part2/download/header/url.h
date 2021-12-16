#ifndef _URL_H_
#define _URL_H_

#include "macros.h"

typedef struct url_data{
    char user[MAX_STR_LEN];
    char pwd[MAX_STR_LEN];
    char host[MAX_STR_LEN];
    char path[MAX_STR_LEN];
} url_data_t;

int valid_url(char * url);

int parse_url(char * url, url_data_t * url_data);

#endif // _URL_H_