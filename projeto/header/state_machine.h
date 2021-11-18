#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

typedef enum { 
    U_START, 
    U_FLAG_RCV, 
    U_A_RCV, 
    U_C_RCV, 
    U_BCC_OK, 
    U_END
} u_states_t;

int u_state_trans(u_states_t *state, char byte,char * a_rcv, char * c_rcv);

typedef enum { 
    I_START, 
    I_FLAG_RCV, 
    I_A_RCV, 
    I_C_RCV, 
    I_DATA, 
    I_END
} i_states_t;

int i_state_trans(i_states_t *state, char byte,int *sz, char* buf);

typedef enum { 
    S_START, 
    S_FLAG_RCV, 
    S_A_RCV, 
    S_C_RCV, 
    S_BCC_OK, 
    S_END
} s_states_t;

int s_state_trans(s_states_t *state, char byte, char * a_rcv, char * c_rcv);

#endif // _STATE_MACHINE_H

