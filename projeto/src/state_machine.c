#include "../header/state_machine.h"

#include "../header/flag.h"
#include "../header/com.h"

int u_state_trans(u_states_t *state, char byte, char * a_rcv, char * c_rcv){
    switch(*state){
              case U_START:
                  if(byte == MSG_FLAG) *state = U_FLAG_RCV;
                  break;
              case U_FLAG_RCV:
                  if(byte == MSG_A_RECV) {
                      *state = U_A_RCV;
                      *a_rcv = MSG_A_RECV;
                  } else if (byte == MSG_FLAG) *state = U_FLAG_RCV;
                  else state = U_START;
                  break;
              case U_A_RCV:
                  if(byte == MSG_C_SET) {
                      *state = U_C_RCV;
                      *c_rcv = MSG_C_SET;
                  } else if (byte == MSG_FLAG) *state = U_FLAG_RCV;
                  else state = U_START;
                  break;
              case U_C_RCV:
                  if(byte == (*a_rcv ^ *c_rcv)) *state = U_BCC_OK;
                  else if (byte == MSG_FLAG) *state = U_FLAG_RCV;
                  else *state = U_START;
                  break;
              case U_BCC_OK:
                  if(byte == MSG_FLAG) *state = U_END;
                  else *state = U_START;
                  break;
             case U_END:
                  return -1;
          }
          return 0;
}     

int i_state_trans(i_states_t *state, char byte, int *sz, char* buf){

    switch(*state){
              case I_START:
                  if(byte == MSG_FLAG) *state = I_FLAG_RCV;
                  break;
              case I_FLAG_RCV:
                  if(byte == MSG_A_SEND) *state = I_A_RCV;
                  else if (byte == MSG_FLAG) *state = I_FLAG_RCV;
                  else state = I_START;
                  break;
              case I_A_RCV:
                  if(byte == MSG_C_I(Ns))  *state = I_C_RCV;
                  else if (byte == MSG_FLAG) *state = I_FLAG_RCV;
                  else state = I_START;
                  break;
              case I_C_RCV:
                  if(byte == (MSG_A_SEND ^ MSG_C_I(Ns))) *state = I_DATA;
                  else if (byte == MSG_FLAG) *state = I_FLAG_RCV;
                  else *state = I_START;
                  break;
              case I_DATA:
                  if(byte == MSG_FLAG) *state = I_END;
                  else {
                      buf[(*sz)++] = byte;
                  }
                  break;
             case I_END:
                  return -1;
          }
          return 0;
}