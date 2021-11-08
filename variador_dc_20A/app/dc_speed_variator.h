/*
 * dc_speed_variator.h
 *
 * Created: 11/05/2021 15:52:11
 *  Author: mtorres
 */ 


#ifndef DC_SPEED_VARIATOR_H_
#define DC_SPEED_VARIATOR_H_

/* File inclusion */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "project_defines.h"
#include "MT_timer.h"
#include "MT_adc.h"
#include "MT_filters.h"
#include "MT_pid_control.h"

/* Function declaration */
void WDT_Off(void);
void Enable_Input_Initialize(void);
void LEDs_Initialize(void);
void Timer0_Initialize(void);
void ADC_Initialize(void);
void System_Initialize(void);

void Inputs_Read(void);
void Current_Feedback_Measure(void);
void Update_Variator_Outputs(void);

#endif /* DC_SPEED_VARIATOR_H_ */