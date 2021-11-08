/*
 * global.h
 *
 * Created: 11/05/2021 11:48:20
 *  Author: mtorres
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

/* File inclusion */
#include <stdint.h>

/* Global variables declaration */
extern volatile uint8_t system_flags;
extern volatile uint8_t trigger_pulse_state;
extern volatile uint8_t cycle_indicator;

#endif /* GLOBAL_H_ */