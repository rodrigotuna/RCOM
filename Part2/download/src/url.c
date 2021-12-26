#include "../header/url.h"

#define _GNU_SOURCE     
#include <libgen.h>   
#undef basename      
#include <string.h>     
#include <stdlib.h>
#include <regex.h>

int valid_url(char * url){
    regex_t regex;
    if(regcomp(&regex, "^ftp://([a-zA-Z0-9]+:[a-zA-Z0-9]+@)?([.a-zA-Z0-9-]+)/([./a-zA-Z0-9-]+)$", REG_EXTENDED)){
        return 0;
    }

    if(regexec(&regex, url , 0, NULL, 0) == 0) {
		regfree(&regex);
		return 1;
	}

    regfree(&regex);
    return 0;
}

int parse_url(char * url, url_data_t * url_data){

    strcpy(url_data->url, url);
    url = url + 6;

    if(strchr(url,'@') != NULL){
        memcpy(url_data->user, url, strcspn(url, ":"));
        url = url + strcspn(url, ":") + 1;
        memcpy(url_data->pwd, url, strcspn(url, "@"));
        url = url + strcspn(url, "@") + 1;
    } else {
        strcpy(url_data->user, "anonymous");
        strcpy(url_data->pwd, "guest");
    }

    memcpy(url_data->host, url, strcspn(url, "/"));
    url = url + strcspn(url, "/") + 1;

    strcpy(url_data->filename, basename(url));
    strcpy(url_data->path, url);

    return 0;
}