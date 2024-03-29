
/* File inclusion */
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "MT_dimmer_leading_edge.h"

/* Variable definition */
const float sinewave_leading_edge_power_percent_table[200] PROGMEM = { 
															0.908, 0.884, 0.867, 0.853, 0.841, 0.831, 0.822, 0.813, 0.805, 0.798,
															0.791, 0.785, 0.778, 0.773, 0.767, 0.761, 0.756, 0.751, 0.746, 0.741,
															0.736, 0.732, 0.728, 0.723, 0.719, 0.715, 0.711, 0.707, 0.703, 0.699,
															0.695, 0.692, 0.688, 0.684, 0.681, 0.677, 0.674, 0.670, 0.667, 0.664,
															0.660, 0.657, 0.654, 0.651, 0.648, 0.645, 0.642, 0.638, 0.635, 0.632,
															0.629, 0.626, 0.624, 0.621, 0.618, 0.615, 0.612, 0.609, 0.606, 0.604, 
															0.601, 0.598, 0.595, 0.593, 0.590, 0.587, 0.584, 0.582, 0.579, 0.576,
															0.574, 0.571, 0.569, 0.566, 0.563, 0.561, 0.558, 0.556, 0.553, 0.550,
															0.548, 0.545, 0.543, 0.540, 0.538, 0.535, 0.533, 0.530, 0.528, 0.525,
															0.523, 0.520, 0.518, 0.515, 0.513, 0.510, 0.508, 0.505, 0.503, 0.500,
															0.497, 0.495, 0.492, 0.490, 0.487, 0.485, 0.482, 0.480, 0.477, 0.475,
															0.472, 0.470, 0.467, 0.465, 0.462, 0.460, 0.457, 0.455, 0.452, 0.450,
															0.447, 0.444, 0.442, 0.439, 0.437, 0.434, 0.431, 0.429, 0.426, 0.424,
															0.421, 0.418, 0.416, 0.413, 0.410, 0.407, 0.405, 0.402, 0.399, 0.396, 
															0.394, 0.391, 0.388, 0.385, 0.382, 0.379, 0.377, 0.374, 0.371, 0.368,
															0.365, 0.362, 0.359, 0.356, 0.352, 0.349, 0.346, 0.343, 0.340, 0.336,
															0.333, 0.330, 0.326, 0.323, 0.319, 0.316, 0.312, 0.309, 0.305, 0.301,
															0.297, 0.293, 0.289, 0.285, 0.281, 0.277, 0.273, 0.269, 0.264, 0.259,
															0.254, 0.249, 0.244, 0.239, 0.233, 0.228, 0.222, 0.216, 0.209, 0.202,
															0.195, 0.187, 0.178, 0.169, 0.159, 0.147, 0.133, 0.116, 0.092, 0.000   
														};

/*														
float sinewave_trailing_edge_power_percent_table[200] = {
															
															0.092, 0.116, 0.133, 0.147, 0.159, 0.169, 0.178, 0.187, 0.195, 0.202,
															0.209, 0.216, 0.222, 0.228, 0.233, 0.239, 0.244, 0.249, 0.254, 0.259,
															0.264, 0.269, 0.273, 0.277, 0.281, 0.285, 0.289, 0.293, 0.297, 0.301,
															0.305, 0.309, 0.312, 0.316, 0.319, 0.323, 0.326, 0.330, 0.333, 0.336,
															0.340, 0.343, 0.346, 0.349, 0.352, 0.356, 0.359, 0.362, 0.365, 0.368,
															0.371, 0.374, 0.377, 0.379, 0.382, 0.385, 0.388, 0.391, 0.394, 0.396,
															0.399, 0.402, 0.405, 0.407, 0.410, 0.413, 0.416, 0.418, 0.421, 0.424,
															0.426, 0.429, 0.431, 0.434, 0.437, 0.439, 0.442, 0.444, 0.447, 0.450, 
															0.452, 0.455, 0.457, 0.460, 0.462, 0.465, 0.467, 0.470, 0.472, 0.475,
															0.477, 0.480, 0.482, 0.485, 0.487, 0.490, 0.492, 0.495, 0.497, 0.500,
															0.503, 0.505, 0.508, 0.510, 0.513, 0.515, 0.518, 0.520, 0.523, 0.525,															
															0.528, 0.530, 0.533, 0.535, 0.538, 0.540, 0.543, 0.545, 0.548, 0.550,													
															0.553, 0.556, 0.558, 0.561, 0.563, 0.566, 0.569, 0.571, 0.574, 0.576,															
															0.579, 0.582, 0.584, 0.587, 0.590, 0.593, 0.595, 0.598, 0.601, 0.604,	
															0.606, 0.609, 0.612, 0.615, 0.618, 0.621, 0.624, 0.626, 0.629, 0.632,
															0.635, 0.638, 0.642, 0.645, 0.648, 0.651, 0.654, 0.657, 0.660, 0.664,
															0.667, 0.670, 0.674, 0.677, 0.681, 0.684, 0.688, 0.692, 0.695, 0.699,
															0.703, 0.707, 0.711, 0.715, 0.719, 0.723, 0.728, 0.732, 0.736, 0.741,							
															0.746, 0.751, 0.756, 0.761, 0.767, 0.773, 0.778, 0.785, 0.791, 0.798,
															0.805, 0.813, 0.822, 0.831, 0.841, 0.853, 0.867, 0.884, 0.908, 1.000		
															
														};														
*/

float sinewave_leading_edge_power_time_table[200];

uint16_t sinewave_leading_edge_power_time_integer_table[200];

/* Function definition */

void Initialize_Dimmer_Leading_Edge_Table(float period){
	
	uint8_t percent_index = 0;
	float normalized_value;
	
	for(percent_index = 0; percent_index < 200; percent_index++){
		normalized_value = pgm_read_float(&sinewave_leading_edge_power_percent_table[percent_index]);
		sinewave_leading_edge_power_time_table[percent_index] = period * normalized_value;
	}
	
	for(percent_index = 0; percent_index < 200; percent_index++){
		normalized_value = pgm_read_float(&sinewave_leading_edge_power_percent_table[percent_index]);
		sinewave_leading_edge_power_time_integer_table[percent_index] = (uint16_t)(period * normalized_value);
	}	
	
}


uint8_t Get_Dimmer_Leading_Edge_Phase_Time(uint8_t percent, float *phase){
	
	uint8_t trigger_on = 0;

	if(percent > 200){
		percent = 200;
	}
	
	if(percent > 0){
		trigger_on = 1;
		*phase = sinewave_leading_edge_power_time_table[percent - 1];
	}
	
	return trigger_on;
}


uint8_t Get_Dimmer_Leading_Edge_Integer_Phase_Time(uint8_t percent, uint16_t *phase){

	uint8_t trigger_on = 0;
	
	if(percent > 200){
		percent = 200;
	}
	
	if(percent > 0){
		trigger_on = 1;
		*phase = sinewave_leading_edge_power_time_integer_table[percent - 1];
	}
	
	return trigger_on;
}