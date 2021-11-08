/*
 * dimming_action_implementation.h
 *
 * Created: 4/03/2021 09:49:42
 *  Author: mtorres
 */ 


#ifndef DIMMING_ACTION_IMPLEMENTATION_H_
#define DIMMING_ACTION_IMPLEMENTATION_H_

#include <stdint.h>

/* Function declaration */
void Timer1_Initialize(void);
void External_Interrupt_Initialize(void);
void Trigger_Implementation_Initialize(void);
void Pos_Trigger_Pulse_State_Machine(void);
void Neg_Trigger_Pulse_State_Machine(void);
void Set_Trigger_Phase_Binary(uint8_t percent_x2);
uint16_t Get_Trigger_Phase_Binary(void);

#endif /* DIMMING_ACTION_IMPLEMENTATION_H_ */