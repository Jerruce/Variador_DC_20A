/*
 * MT_fuzzy_control_basic.c
 *
 * Created: 12/11/2021 20:25:53
 *  Author: mtorres
 */ 

/* File inclusion */
#include <stdint.h>
#include "project_defines.h"
#include "MT_fuzzy_control_basic.h"


/* Variabledefinition */
static double ge_01 = 0.0 , gce_01 = 0.0 ,  gcu_01 = 0.0;

/* Function definition */
void Fuzzy_Linear_PD_Controller_01(double error, double error_change, double *controller_output){

	/* Set boundaries for error input */
	if(error < -1.0){
		error = -1.0;
	}else if(error > 1.0){
		error = 1.0;
	}else{
		/* Does nothing */
	}

	/* Set boundaries for error change input */
	if(error_change < -1.0){
		error_change = -1.0;
	}else if(error_change > 1.0){
		error_change = 1.0;
	}else{
		/* Does nothing */
	}

	*controller_output = (error + error_change) / 2.0;

}


float Flow_Fuzzy_Incremental_Controller_01(float control_setpoint, float control_feedback, uint8_t control_reset){

	static double error[2];
	double fuzzy_error;
	double error_change, fuzzy_error_change;
	static double fuzzy_output[2] = {0.0, 0.0};
	static double controller_output = 0.0;
	double delta_area;

	if(control_reset){
		error[0] = 0.0;
		error[1] = 0.0;
		fuzzy_output[0] = 0.0;
		fuzzy_output[1] = 0.0;
		controller_output = 0.0;
		}else{
		error[1] = error[0];
		error[0] = control_setpoint - control_feedback;

		error_change = (1000.0 * (error[0] - error[1])) / FUZZY_CONTROLLER_01_SAMPLE_PERIOD_MS;

		fuzzy_error = ge_01 * error[0];
		fuzzy_error_change = gce_01 * error_change;

		fuzzy_output[1] = fuzzy_output[0];
		Fuzzy_Linear_PD_Controller_01(fuzzy_error, fuzzy_error_change, &fuzzy_output[0]);
		fuzzy_output[0] *= gcu_01;

		delta_area = FUZZY_CONTROLLER_01_SAMPLE_PERIOD_MS * (fuzzy_output[0] + fuzzy_output[1]) / 2000.0;
		controller_output += delta_area;

		if(controller_output < FUZZY_CONTROLLER_01_MIN_OUTPUT){
			controller_output = FUZZY_CONTROLLER_01_MIN_OUTPUT;
		}else if(controller_output > FUZZY_CONTROLLER_01_MAX_OUTPUT){
			controller_output = FUZZY_CONTROLLER_01_MAX_OUTPUT;
		}else{
			/* Does nothing */
		}

	}

	return (float)controller_output;

}

void Fuzzy_Controller_01_Set_Parameters(double my_ge, double my_gce, double my_gcu){
	ge_01 = my_ge; 
	gce_01 = my_gce;  
	gcu_01 = my_gcu;
}


void Fuzzy_Controllers_Initialize(void){
	
	Fuzzy_Controller_01_Set_Parameters(GE1_BY_DEFAULT, GCE1_BY_DEFAULT, GCU1_BY_DEFAULT);
	Flow_Fuzzy_Incremental_Controller_01(0.0, 0.0, 1);
	
}