#include "../header/url.h"

#include <stdlib.h>
#include <regex.h>

int valid_url(char * url){
    regex_t regex;
    if(regcomp(&regex, "^ftp://([[a-zA-Z0-9]+:[a-zA-Z0-9]+@])?([.a-z0-9-]+)/([./a-z0-9-]+)$", REG_EXTENDED)){
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
    return 0;
}