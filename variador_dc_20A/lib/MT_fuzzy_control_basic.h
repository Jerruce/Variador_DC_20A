/*
 * MT_fuzzy_control_basic.h
 *
 * Created: 12/11/2021 20:26:46
 *  Author: mtorres
 */ 


#ifndef MT_FUZZY_CONTROL_BASIC_H_
#define MT_FUZZY_CONTROL_BASIC_H_

/* File inclusion */
#include <stdint.h>

/* Function declaration */
void Fuzzy_Linear_PD_Controller_01(double error, double error_change, double *controller_output);
float Flow_Fuzzy_Incremental_Controller_01(float control_setpoint, float control_feedback, uint8_t control_reset);
void Fuzzy_Controller_01_Set_Parameters(double my_ge, double my_gce, double my_gcu);
void Fuzzy_Controllers_Initialize(void);


#endif /* MT_FUZZY_CONTROL_BASIC_H_ */