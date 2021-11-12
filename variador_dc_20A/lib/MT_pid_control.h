/*
 * MT_pid_control.h
 *
 * Created: 5/05/2021 12:21:54
 *  Author: mtorres
 */ 


#ifndef MT_PID_CONTROL_H_
#define MT_PID_CONTROL_H_

/* File inclusion */
#include "stdint.h"
#include "project_defines.h"

/* Constants */

//#define PID_01_SAMPLE_PERIOD_S					0.01
//#define PID_01_MAX_OUTPUT						100.0
//#define PID_01_MIN_OUTPUT						0.0
//


/* Function declaration */

void PID_01_Set_Parameters(double my_kp, double my_ki, double my_kd);
double PID_Controller_01(double pid_setpoint, double pid_feedback, uint8_t pid_reset);
void PID_02_Set_Parameters(double my_kp, double my_ki, double my_kd);
double PID_Controller_02(double pid_setpoint, double pid_feedback, uint8_t pid_reset);
void PID_Controllers_Initialize(void);

#endif /* MT_PID_CONTROL_H_ */