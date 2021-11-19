#ifndef _CONFIG_H_
#define _CONFIG_H_

#define BAUDRATE B38400

int set_config(char * serial_port);
void reset_config(int fd);


#endif // _CONFIG_H