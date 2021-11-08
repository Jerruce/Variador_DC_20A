/*
 * MT_numeric_integration.c
 *
 * Created: 13/05/2021 08:51:13
 *  Author: mtorres
 */ 


/* File inclusion */
#include "MT_numeric_integration.h"

/* Function definition */

float Integration_Trapezoid_Method(float y_val, uint8_t reset){
	
	static float f_x[2] = {0.0, 0.0};
	static float total_area = 0.0;
	float delta_area = 0.0;
	
	if(reset == 0){
		
		/* Update values stored in y */
		f_x[1] = f_x[0];
		f_x[0] = y_val;
		
		/* Calculate delta area */
		delta_area = ((f_x[0] + f_x[1]) * DELTA_X) / 2.0;
		
		/* Update the integration output */
		total_area += delta_area;
		
	}else{
		
		/* Restart the static variables */	
		
		f_x[1] = 0.0;
		f_x[0] = 0.0;
		total_area = 0.0;
		
	}
	
	return total_area;
}


float Integration_Midpoint_Method(float y_val, uint8_t reset){
	
	static float f_x[3] = {0.0, 0.0,0.0};
	static float total_area = 0.0;
	float  delta_area = 0.0;
	static uint8_t integral_count = 0;
	
	if(reset == 0){
		
		/* Update values stored in y (ALWAYS) */
		f_x[2] = f_x[1];
		f_x[1] = f_x[0];
		f_x[0] = y_val;
		
		/* Calculate delta area and update integration output (only when the counter reaches the value 2) */
		integral_count++; 
		if(integral_count >= 2){
			integral_count = 0;
			delta_area = 2.0 * DELTA_X * f_x[1];
			total_area += delta_area;
		}
		
	}else{
		
		
		/* Restart the static variables */	
		
		f_x[2] = 0.0;
		f_x[1] = 0.0;
		f_x[0] = 0.0;
		total_area = 0.0;
		integral_count = 0;
		
	}
	
	return total_area;
}


float Integration_Simpson_Method(float y_val, uint8_t reset){
	
	static float f_x[3] = {0.0, 0.0,0.0};
	static float total_area = 0.0;
	float  delta_area = 0.0;
	static uint8_t integral_count = 0;
	
	if(reset == 0){
		
		/* Update values stored in y (ALWAYS) */
		f_x[2] = f_x[1];
		f_x[1] = f_x[0];
		f_x[0] = y_val;
		
		/* Calculate delta area and update integration output (only when the counter reaches the value 2) */
		integral_count++;
		if(integral_count >= 2){
			integral_count = 0;
			delta_area = (DELTA_X / 3.0) * (f_x[0] + 4*f_x[1] + f_x[2]);
			total_area += delta_area;
		}
		
	}else{
		
		/* Restart the static variables */	
		
		f_x[2] = 0.0;
		f_x[1] = 0.0;
		f_x[0] = 0.0;
		total_area = 0.0;
		integral_count = 0;
		
	}
	
	return total_area;
}