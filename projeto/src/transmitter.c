#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/config.h"
#include "../header/app.h"

int main(int argc, char ** argv){
    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) &&
          (strcmp("/dev/ttyS11", argv[1])!=0) &&
          (strcmp("/dev/ttyS10", argv[1])!=0) ) ) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    set_config(argv[1]);

    if(application(fd, TRANSMITTER, argv[2]) != 0){
        fprintf(stderr, "Error transmitting File");
        exit(-1);
    }

    reset_config();
}