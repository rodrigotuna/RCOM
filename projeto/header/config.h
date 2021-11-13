#ifndef _CONFIG_H_
#define _CONFIG_H_

#define BAUDRATE B38400

static int fd;
void set_config(char * serial_port);
void reset_config();


#endif // _CONFIG_H