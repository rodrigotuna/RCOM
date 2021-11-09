/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

/** Message Bytes **/
#define MES_FLAG 0x7E
#define MES_A 0X03
#define MES_C_SET 0x03

int flag=1, conta=1;

void atende()                   // atende alarme
{
	printf("alarme # %d\n", conta);
	flag=1;
	conta++;
}

int comp (char * a, char * b){
    for(int i = 0; i < 5; i++){
        if( a[i] != b[i]) return 0;
    }
    return 1;
}
   

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c,res;
    struct termios oldtio,newtio;
    char buf[255];
    int i = 0, t = 0, sum = 0, speed = 0;
    
    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) &&
          (strcmp("/dev/ttyS11", argv[1])!=0) &&
          (strcmp("/dev/ttyS10", argv[1])!=0) ) ) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */


    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 30;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) pr�ximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");
    
    
    buf[0] = MES_FLAG;
    buf[1] = MES_A;
    buf[2] = MES_C_SET;
    buf[3] = MES_A ^ MES_C_SET; 
    buf[4] = MES_FLAG;

    (void) signal(SIGALRM, atende);

    while (t < 3){
    
        res = write(fd,buf,5);   
        printf("%d bytes written\n", res);
        
        while(conta < 4 && i < 5){  
        
           if(flag){
              alarm(3);                 // activa alarme de 3s
              flag=0;
           }
           if(read(fd,&buf[i + 5],1)) i++; 
        }
        
        if( comp( buf, buf + 5) ) break;
        conta = 1;
        i = 0;
        
        t++;
    }

    printf("String recebida %x %x %x %x %x\n", buf[5], buf[6], buf[7], buf[8], buf[9]);

    sleep(2);

    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }




    close(fd);
    return 0;
}
