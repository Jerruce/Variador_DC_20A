
/* File inclusion */
#include "stdint.h"
#include "project_defines.h"
#include "MT_filters.h"

/* Global variables */
static float power_percent_lpf_output = 0.0;
static float current_sense_feedback_lpf_output = 0.0;
static float speed_control_lpf_output = 0.0;


/* Function definition */

void Filters_Initialize(void){
	RC_LPF_Power_Percent_Control(0.0, 1);
	RC_LPF_Current_Sense_Feedback(0.0, 1);
	Cheby2_LPF_Current_Sense_Feedback(0.0, 1);
}


float RC_LPF_Power_Percent_Control(float raw_value, uint8_t restart){

	static float lpf_prev_out = 0.0;
	float lpf_out, sample_period_sec, div;
	
	if(restart){
		
		lpf_prev_out = 0.0;
		lpf_out = 0.0;
		
		}else{
		
		sample_period_sec = 0.001 * ((float)POWER_PERCENT_CONTROL_SAMPLE_PERIOD_MS);
		div = sample_period_sec + POWER_PERCENT_CONTROL_LPF_RC_CONSTANT_S;
		
		lpf_out = (raw_value * (sample_period_sec / div)) +  (lpf_prev_out * (POWER_PERCENT_CONTROL_LPF_RC_CONSTANT_S / div));
		lpf_prev_out = lpf_out;
		
	}
	
	return lpf_out;
}


float RC_LPF_Current_Sense_Feedback(float raw_value, uint8_t restart){
	
	static float lpf_prev_out = 0.0;
	float lpf_out, sample_period_sec, div;
	
	if(restart){
		
		lpf_prev_out = 0.0;
		lpf_out = 0.0;
		
		}else{
		
		sample_period_sec = 0.001 * ((float)CURRENT_SENSE_FEEDBACK_SAMPLE_PERIOD_MS);
		div = sample_period_sec + CURRENT_SENSE_FEEDBACK_LPF_RC_CONSTANT_S;
		
		lpf_out = (raw_value * (sample_period_sec / div)) +  (lpf_prev_out * (CURRENT_SENSE_FEEDBACK_LPF_RC_CONSTANT_S / div));
		lpf_prev_out = lpf_out;
		
	}
	
	return lpf_out;
}





float Cheby2_LPF_Current_Sense_Feedback(float raw_value, uint8_t restart){
	
	const float b[CURRENT_SENSE_FEDDBACK_LPF_N] = {0.0032, -0.0060, 0.0032};
	const float a[CURRENT_SENSE_FEDDBACK_LPF_N] = {1.0000, -1.9716, 0.9720};
		
	static float x[CURRENT_SENSE_FEDDBACK_LPF_N];
	static float y[CURRENT_SENSE_FEDDBACK_LPF_N];
	
	int16_t i;

	if(restart){
		for(i = 0; i < CURRENT_SENSE_FEDDBACK_LPF_N; i++){
			x[i] = 0.0;
			y[i] = 0.0;
		}
		}else{
		
		/* Update recorded input and output values */
		for(i = CURRENT_SENSE_FEDDBACK_LPF_N - 1; i > 0; i--){
			x[i] = x[i - 1];
			y[i] = y[i - 1];
		}
		
		/* Update current input and output values */
		x[0] = raw_value;
		y[0] = b[0] * x[0];
		for(i = 1; i < CURRENT_SENSE_FEDDBACK_LPF_N; i++){
			y[0] += (b[i] * x[i]) - (a[i] * y[i]);
		}
		
	}

	return y[0];
}


float RC_LPF_Speed_Control(float raw_value, uint8_t restart){
	
	static float lpf_prev_out = 0.0;
	float lpf_out, sample_period_sec, div;
	
	if(restart){
		
		lpf_prev_out = 0.0;
		lpf_out = 0.0;
		
	}else{
		
		sample_period_sec = 0.001 * ((float)SPEED_CONTROL_SAMPLE_PERIOD_MS);
		div = sample_period_sec + SPEED_CONTROL_LPF_RC_CONSTANT_S;
		
		lpf_out = (raw_value * (sample_period_sec / div)) +  (lpf_prev_out * (SPEED_CONTROL_LPF_RC_CONSTANT_S / div));
		lpf_prev_out = lpf_out;
		
	}
	
	return lpf_out;	
	
}



void Apply_LPF_Power_Percent_Control(float new_power_output){
	power_percent_lpf_output = RC_LPF_Power_Percent_Control(new_power_output, 0);
}


void Apply_LPF_Current_Sense_Feedback(float new_current_feedback){
	//current_sense_feedback_lpf_output = Cheby2_LPF_Current_Sense_Feedback(new_current_feedback, 0);
	current_sense_feedback_lpf_output = RC_LPF_Current_Sense_Feedback(new_current_feedback, 0);
}

void Apply_LPF_Speed_Control(float new_speed_input){
	speed_control_lpf_output = RC_LPF_Speed_Control(new_speed_input, 0);
}


float Get_LPF_Power_Percent_Control(void){
	return  power_percent_lpf_output;
}


float Get_LPF_Current_Sense_Feedback(void){
	return  current_sense_feedback_lpf_output;
}

float Get_LPF_Speed_Control(void){
	return speed_control_lpf_output;
}