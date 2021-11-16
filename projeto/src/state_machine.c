#include "../header/state_machine.h"

#include "../header/flag.h"

int u_state_trans(u_states_t *state, char byte){
    switch(*state){
              case START:
                  if(byte == MSG_FLAG) *state = FLAG_RCV;
                  break;
              case FLAG_RCV:
                  if(byte == MSG_A_RECV) {
                      *state = A_RCV;
                      a_received = MSG_A_RECV;
                  } else if (byte == MSG_FLAG) *state = FLAG_RCV;
                  else state = START;
                  break;
              case A_RCV:
                  if(byte == MSG_C_SET) {
                      *state = MSG_C_SET;
                      c_received = C_RCV;
                  } else if (byte == MSG_FLAG) *state = FLAG_RCV;
                  else state = START;
                  break;
              case C_RCV:
                  if(byte == (a_received ^ c_received)) *state = BCC_OK;
                  else if (byte == MSG_FLAG) *state = FLAG_RCV;
                  else *state = START;
                  break;
              case BCC_OK:
                  if(byte == MSG_FLAG) *state = END;
                  else *state = START;
                  break;
             case END:
                  return -1;
          }
          return 0;
}     