/**
 * @brief Link Layer Flag Macros
 */

#ifndef _FLAG_H_
#define _FLAG_H_

#define MSG_FLAG         0x7E                      
#define MSG_A_SEND       0x03                       
#define MSG_A_RECV       0x01                         
#define MSG_C_SET        0x03                        
#define MSG_C_DISC       0x0B                        
#define MSG_C_UA         0x07
#define MSG_C_I(N)       ((N) << 6)
#define MSG_C_RR(N)      (0x05  | ((N) << 7)) 
#define MSG_C_REJ(N)     (0x01 | ((N) << 7))
#define MSG_ESC          0x7D
#define MSG_STUFF(N)     ((N) ^  0x20)


#define NUM_RETRANS 3

#endif // _FLAG_H