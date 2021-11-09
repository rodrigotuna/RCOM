/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

/** Message Bytes */
#define MES_FLAG 0x7E
#define MES_A 0x03
#define MES_C_SET 0x03

typedef enum { START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, END} states_t;

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];

    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) &&
          (strcmp("/dev/ttyS11", argv[1])!=0) &&
          (strcmp("/dev/ttyS10", argv[1])!=0)) ) {
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
    states_t state = START;
    int i = 0;
    
    while(state != END){
        if (read(fd,&buf[i],1) == 1){
          switch(state){
              case START:
                  if(buf[i] == MES_FLAG) state = FLAG_RCV;
                  break;
              case FLAG_RCV:
                  if(buf[i] == MES_A) state = A_RCV;
                  else if (buf[i] == MES_FLAG) state = FLAG_RCV;
                  else state = START;
                  break;
              case A_RCV:
                  if(buf[i] == MES_C_SET) state = C_RCV;
                  else if (buf[i] == MES_FLAG) state = FLAG_RCV;
                  else state = START;
                  break;
              case C_RCV:
                  if(buf[i] == buf[i-1] ^ buf[i-2]) state = BCC_OK;
                  else if (buf[i] == MES_FLAG) state = FLAG_RCV;
                  else state = START;
                  break;
              case BCC_OK:
                  if(buf[i] == MES_FLAG) state = END;
                  else state = START;
                  break;
          }
          i++;
        }
    }

    printf("String recebida %x %x %x %x %x\n", buf[i-5], buf[i-4], buf[i-3], buf[i-2], buf[i-1]);
    res = write(fd, buf+i-5, 5);
    printf("%d bytes written\n", res);

  /* 
    O ciclo WHILE deve ser alterado de modo a respeitar o indicado no gui�o 
  */
    
    sleep(2);

    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
