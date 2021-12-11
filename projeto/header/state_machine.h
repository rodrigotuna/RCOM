/**
 * @brief State Machine Communication 
 */


#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include <stdint.h>

/**
 * 
 * @brief States to receive U-frame 
*/
typedef enum { 
    U_START, 
    U_FLAG_RCV, 
    U_A_RCV, 
    U_C_RCV, 
    U_BCC_OK, 
    U_END
} u_states_t;

/**
 * 
 * @brief Performs state transitions in U-frames
 * 
 * @param state Current state of the state machine
 * @param byte Byte value of the current state
 * @param a_rcv Pointer where the Address Field value will be stored
 * @param c_rcv Pointer where the Control Field value will be stored
 * 
 * @return 0 on success, -1 otherwise
*/
int u_state_trans(u_states_t *state, uint8_t byte, uint8_t * a_rcv, uint8_t * c_rcv);

/**
 * 
 * @brief States to receive I-frame 
*/
typedef enum { 
    I_START, 
    I_FLAG_RCV, 
    I_A_RCV, 
    I_C_RCV, 
    I_DATA, 
    I_END,
    I_REP,
    I_REP_END
} i_states_t;

/**
 * 
 * @brief Performs state transitions in I-frames
 * 
 * @param state Current state of the state machine
 * @param byte Byte value of the current state
 * @param sz Current buffer index to store the data byte value
 * @param buf Buffer where the data byte value will be written
 * 
 * @return 0 on success, -1 otherwise
*/
int i_state_trans(i_states_t *state, uint8_t byte, int *sz, uint8_t* buf);

/**
 * 
 * @brief States to receive S-frame 
*/
typedef enum { 
    S_START, 
    S_FLAG_RCV, 
    S_A_RCV, 
    S_C_RCV, 
    S_BCC_OK, 
    S_END
} s_states_t;

/**
 * 
 * @brief Performs state transitions in S-frames
 * 
 * @param state Current state of the state machine
 * @param byte Byte value of the current state
 * @param a_rcv Pointer where the Address Field value will be stored
 * @param c_rcv Pointer where the Control Field value will be stored
 * 
 * @return 0 on success, -1 otherwise
*/
int s_state_trans(s_states_t *state, uint8_t byte, uint8_t * a_rcv, uint8_t * c_rcv);

#endif // _STATE_MACHINE_H

