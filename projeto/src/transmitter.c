#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../header/config.h"
#include "../header/app.h"
#include "../header/com.h"

int main(int argc, char ** argv){
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    if ( (argc < 3) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) &&
          (strcmp("/dev/ttyS11", argv[1])!=0) &&
          (strcmp("/dev/ttyS10", argv[1])!=0) ) ) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    set_error(atoi(argv[3]));
    set_t_prop(atoi(argv[4])*1000);

    int fd = set_config(argv[1]);

    if(application(fd, TRANSMITTER, argv[2]) != 0){
        fprintf(stderr, "Error transmitting File");
        exit(-1);
    }

    reset_config(fd);
    gettimeofday(&tv2, NULL);
    printf ("Total time = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));
}