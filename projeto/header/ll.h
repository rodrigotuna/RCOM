#ifndef _LL_H_
#define _LL_H_

int ll_open(int fd, char role);
int ll_read(int fd, char * buffer);
int ll_write(int fd, char * buffer, int length);
int ll_close(int fd);

#endif // _LL_H