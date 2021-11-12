/*
 * MT_pid_control.c
 *
 * Created: 5/05/2021 12:22:17
 *  Author: mtorres
 */ 


/* File inclusion */
#include "stdint.h"
#include "MT_pid_control.h"


/* Parameters */

static double kp_01 = 0.0 , ki_01 = 0.0 ,  kd_01 = 0.0;
static double kp_02 = 0.0 , ki_02 = 0.0 ,  kd_02 = 0.0;


/* Function definition */

void PID_01_Set_Parameters(double my_kp, double my_ki, double my_kd){
	kp_01 = my_kp;
	ki_01 = my_ki;
	kd_01 = my_kd;
}


double PID_Controller_01(double pid_setpoint, double pid_feedback, uint8_t pid_reset){

    static double pid_error[3];
    static double pid_output = 0.0;
    double delta_area = 0.0;
    double p_comp, d_comp;
    static double i_comp = 0;

    if(pid_reset){
	    
	    pid_error[2] = 0.0;
	    pid_error[1] = 0.0;
	    pid_error[0] = 0.0;
	    pid_output = 0.0;
	    i_comp = 0.0;
	    
	 }else{
	    
	    pid_error[2] = pid_error[1];
	    pid_error[1] = pid_error[0];
	    pid_error[0] = pid_setpoint - pid_feedback;
	    delta_area = ((double)PID_01_SAMPLE_PERIOD_MS) * ((pid_error[0] + pid_error[1])) / 2000.0;

	    p_comp = pid_error[0];
	    i_comp = i_comp + delta_area;
	    d_comp = ((pid_error[0] - pid_error[1]) * 1000) / (double)PID_01_SAMPLE_PERIOD_MS;
	    
		if(i_comp > I1_COMPONENT_POSITIVE_LIMIT){
			i_comp = I1_COMPONENT_POSITIVE_LIMIT;
		}else if(i_comp < I1_COMPONENT_NEGATIVE_LIMIT){
			i_comp = I1_COMPONENT_NEGATIVE_LIMIT;
		}else{
			//Does nothing
		}
		
	    pid_output = (kp_01 * p_comp) + (ki_01 * i_comp) + (kd_01 * d_comp);
	    

	    if(pid_output > PID_01_MAX_OUTPUT){
		    pid_output = PID_01_MAX_OUTPUT;
		}else if (pid_output < PID_01_MIN_OUTPUT){
		    pid_output = PID_01_MIN_OUTPUT;
		}else{
		    // pid_output doesn't change
	    }
	    
    }

    return pid_output;
	
}

void PID_02_Set_Parameters(double my_kp, double my_ki, double my_kd){
	kp_02 = my_kp;
	ki_02 = my_ki;
	kd_02 = my_kd;
}


void PID_Controllers_Initialize(void){
	
	PID_01_Set_Parameters(KP1_BY_DEFAULT, KI1_BY_DEFAULT, KD1_BY_DEFAULT);
	PID_Controller_01(0.0, 0.0, 1);
	
	PID_02_Set_Parameters(KP2_BY_DEFAULT, KI2_BY_DEFAULT, KD2_BY_DEFAULT);
	PID_Controller_02(0.0, 0.0, 1);	
	
}


double PID_Controller_02(double pid_setpoint, double pid_feedback, uint8_t pid_reset){

    static double pid_error[3];
    static double pid_output = 0.0;
    double delta_area = 0.0;
    double p_comp, d_comp;
    static double i_comp = 0;

    if(pid_reset){
	    
	    pid_error[2] = 0.0;
	    pid_error[1] = 0.0;
	    pid_error[0] = 0.0;
	    pid_output = 0.0;
	    i_comp = 0.0;
	    
	    }else{
	    
	    pid_error[2] = pid_error[1];
	    pid_error[1] = pid_error[0];
	    pid_error[0] = pid_setpoint - pid_feedback;
	    delta_area = ((double)PID_02_SAMPLE_PERIOD_MS) * ((pid_error[0] + pid_error[1])) / 2000.0;

	    p_comp = pid_error[0];
	    i_comp = i_comp + delta_area;
	    d_comp = ((pid_error[0] - pid_error[1]) * 1000) / (double)PID_02_SAMPLE_PERIOD_MS;

		if(i_comp > I2_COMPONENT_POSITIVE_LIMIT){
			i_comp = I2_COMPONENT_POSITIVE_LIMIT;
		}else if(i_comp < I2_COMPONENT_NEGATIVE_LIMIT){
			i_comp = I2_COMPONENT_NEGATIVE_LIMIT;
		}else{
			//Does nothing
		}
	    	    
	    pid_output = (kp_02 * p_comp) + (ki_02 * i_comp) + (kd_02 * d_comp);
	    
	    if(pid_output > PID_02_MAX_OUTPUT){
		    pid_output = PID_02_MAX_OUTPUT;
		}else if (pid_output < PID_02_MIN_OUTPUT){
		    pid_output = PID_02_MIN_OUTPUT;
		}else{
		    // pid_output doesn't change
	    }
	    
    }

    return pid_output;
	
}

