
#ifndef MT_FILTERS_H_
#define MT_FILTERS_H_

/* File inclusion */
#include "stdint.h"
#include "project_defines.h"


///* Constants and macros */
//
//#define OFFSET_CONTROL_LPF_FC_HZ					0.1
//#define OFFSET_CONTROL_LPF_RC_CONSTANT_S			(1 / (6.28 * OFFSET_CONTROL_LPF_FC_HZ))
//#define	OFFSET_CONTROL_SAMPLE_PERIOD_MS				10
//
//#define SPAN_CONTROL_LPF_FC_HZ						0.1
//#define SPAN_CONTROL_LPF_RC_CONSTANT_S				(1 / (6.28 * SPAN_CONTROL_LPF_FC_HZ))
//#define	SPAN_CONTROL_SAMPLE_PERIOD_MS				10
//
//#define SPEED_ADJ_CONTROL_LPF_FC_HZ					0.1
//#define SPEED_ADJ_CONTROL_LPF_RC_CONSTANT_S			(1 / (6.28 * SPEED_ADJ_CONTROL_LPF_FC_HZ))
//#define	SPEED_ADJ_CONTROL_SAMPLE_PERIOD_MS			10
//
//#define CURRENT_LIMIT_CONTROL_LPF_FC_HZ				0.1
//#define CURRENT_LIMIT_CONTROL_LPF_RC_CONSTANT_S		(1 / (6.28 * CURRENT_LIMIT_CONTROL_LPF_FC_HZ))
//#define	CURRENT_LIMIT_CONTROL_SAMPLE_PERIOD_MS		10
//
//#define OFFSET_CONTROL_LPF_FC_HZ					0.1
//#define OFFSET_CONTROL_LPF_RC_CONSTANT_S			(1 / (6.28 * OFFSET_CONTROL_LPF_FC_HZ))
//#define	OFFSET_CONTROL_SAMPLE_PERIOD_MS				10
//
//#define CURRENT_SENSE_FEEDBACK_LPF_FC_HZ			0.1
//#define CURRENT_SENSE_FEEDBACK_LPF_RC_CONSTANT_S	(1 / (6.28 * CURRENT_SENSE_FEEDBACK_LPF_FC_HZ))
//#define	CURRENT_SENSE_FEEDBACK_SAMPLE_PERIOD_MS		10


/* Function declaration */

void Filters_Initialize(void);

float RC_LPF_Power_Percent_Control(float raw_value, uint8_t restart);
float RC_LPF_Current_Sense_Feedback(float raw_value, uint8_t restart);
float Cheby2_LPF_Current_Sense_Feedback(float raw_value, uint8_t restart);

void Apply_LPF_Power_Percent_Control(float new_power_output);
void Apply_LPF_Current_Sense_Feedback(float new_current_feedback);

float Get_LPF_Power_Percent_Control(void);
float Get_LPF_Current_Sense_Feedback(void);

#endif