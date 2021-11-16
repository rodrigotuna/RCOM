#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

typedef enum { START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, END} u_states_t;

int a_received, c_received;

int u_state_trans(u_states_t *state, char byte);

#endif // _STATE_MACHINE_H

