#ifndef _APP_H_
#define _APP_H_

int app_send_file(int fd, char* file_path);
int app_recieve_file(int fd, char * buffer);

#endif // _APP_H