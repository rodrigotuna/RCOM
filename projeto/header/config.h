/**
 * @brief Serial Port Configuration
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

//Baudrate value
#define BAUDRATE B38400

/**
 * 
 * @brief Sets the serial port configuration
 * 
 * @param serial_port Port name to be configured
 * 
 * @return The file descriptor corresponding to the serial port on success, -1 otherwise
*/
int set_config(char * serial_port);

/**
 *
 * @brief Resets the serial port's configuration
 * 
 * @param fd File descriptor of the serial port 
*/
void reset_config(int fd);


#endif // _CONFIG_H