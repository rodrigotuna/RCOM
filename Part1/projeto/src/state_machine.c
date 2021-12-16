#include "../header/state_machine.h"

#include <stdint.h>

#include "../header/flag.h"
#include "../header/com.h"

int u_state_trans(u_states_t *state, uint8_t byte, uint8_t * a_rcv, uint8_t * c_rcv){
    switch(*state){
              case U_START:
                  if(byte == MSG_FLAG) *state = U_FLAG_RCV;
                  break;
              case U_FLAG_RCV:
                  if(byte == MSG_A_RECV || byte == MSG_A_SEND) {
                      *state = U_A_RCV;
                      *a_rcv = byte;
                  } else if (byte == MSG_FLAG) *state = U_FLAG_RCV;
                  else state = U_START;
                  break;
              case U_A_RCV:
                  if(byte == MSG_C_UA) {
                      *state = U_C_RCV;
                      *c_rcv = byte;
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

int i_state_trans(i_states_t *state, uint8_t byte, int *sz, uint8_t * buf){

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
                  else if (byte == MSG_C_I((Ns + 1)%2)) *state = I_REP;
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
              case I_REP:
                  if(byte == MSG_FLAG) *state = I_REP_END;
              case I_END:
                  return -1;
              case I_REP_END:
                  return -1;
          }
          return 0;
}

int s_state_trans(s_states_t *state, uint8_t byte, uint8_t * a_rcv, uint8_t * c_rcv){
    switch(*state){
              case S_START:
                  if(byte == MSG_FLAG) *state = S_FLAG_RCV;
                  break;
              case S_FLAG_RCV:
                  if(byte == MSG_A_RECV || byte == MSG_A_SEND) {
                      *state = S_A_RCV;
                      *a_rcv = byte;
                  } else if (byte == MSG_FLAG) *state = S_FLAG_RCV;
                  else state = S_START;
                  break;
              case S_A_RCV:
                  if(byte == MSG_C_SET || byte == MSG_C_DISC || byte == MSG_C_REJ(Nr) || byte == MSG_C_RR(Nr)) {
                      *state = S_C_RCV;
                      *c_rcv = byte;
                  } else if (byte == MSG_FLAG) *state = S_FLAG_RCV;
                  else state = S_START;
                  break;
              case S_C_RCV:
                  if(byte == (*a_rcv ^ *c_rcv)) *state = S_BCC_OK;
                  else if (byte == MSG_FLAG) *state = S_FLAG_RCV;
                  else *state = S_START;
                  break;
              case S_BCC_OK:
                  if(byte == MSG_FLAG) *state = S_END;
                  else *state = S_START;
                  break;
             case S_END:
                  return -1;
          }
          return 0;
}