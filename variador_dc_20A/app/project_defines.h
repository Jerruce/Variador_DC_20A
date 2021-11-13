/*
 * project_defines.h
 *
 * Created: 11/05/2021 10:19:12
 *  Author: mtorres
 */ 


#ifndef PROJECT_DEFINES_H_
#define PROJECT_DEFINES_H_

//------------ System clock frequency -------------
#define F_CPU	16000000UL	

/***********************************************
**************** PIN DEFINITION ****************
************************************************/

// ---------- Pin definition for LEDs --------------

#define DDR_ON_LED					DDRD
#define PORT_ON_LED					PORTD
#define ON_LED						5

#define DDR_OVERCURRENT_LED			DDRD
#define PORT_OVERCURRENT_LED		PORTD
#define OVERCURRENT_LED				6

#define DDR_RUN_LED					DDRD
#define PORT_RUN_LED				PORTD
#define RUN_LED						7

// --------- Pin definition for pulse output-------------- 

#define DDR_TRIGGER_PULSE_POS		DDRB
#define PORT_TRIGGER_PULSE_POS		PORTB
#define TRIGGER_PULSE_POS			1

#define DDR_TRIGGER_PULSE_NEG		DDRB
#define PORT_TRIGGER_PULSE_NEG		PORTB
#define TRIGGER_PULSE_NEG			2

// --------- Pin definition for Zero Cross Detector -------

#define DDR_ZCD_SYNC_POS				DDRD
#define PIN_ZCD_SYNC_POS				PIND
#define PORT_ZCD_SYNC_POS				PORTD
#define ZCD_SYNC_POS					2

#define DDR_ZCD_SYNC_NEG				DDRD
#define PIN_ZCD_SYNC_NEG				PIND
#define PORT_ZCD_SYNC_NEG				PORTD
#define ZCD_SYNC_NEG					3

// -------- Pin definition for ENABLE jumper -------------

#define DDR_POWER_ENABLE			DDRC			
#define PIN_POWER_ENABLE			PINC
#define PORT_POWER_ENABLE			PORTC	
#define POWER_ENABLE				1


/*****************************************************
**************** CONSTANTS AND MACROS ****************
******************************************************/

// ------------------ Defines for ADC ------------------------

#define SPEED_ADJ_CONTROL_ADC_CHANNEL			ADC_CH0_Single
#define OFFSET_CONTROL_ADC_CHANNEL				ADC_CH3_Single
#define SPAN_CONTROL_ADC_CHANNEL				ADC_CH4_Single
#define CURRENT_LIMIT_CONTROL_ADC_CHANNEL		ADC_CH5_Single

#define CURRENT_SENSE_ADC_CHANNEL				ADC_CH2_Single
#define SPEED_SENSE_ADC_CHANNEL					ADC_CH6_Single

// --------------- Potentiometer measurement sample period ---------------------

#define CONTROL_INPUTS_SAMPLE_PERIOD_MS					10

// --------------------- Defines for low-pass filters ------------------------

#define POWER_PERCENT_CONTROL_LPF_FC_HZ					1.0
#define POWER_PERCENT_CONTROL_LPF_RC_CONSTANT_S			(1 / (6.28 * POWER_PERCENT_CONTROL_LPF_FC_HZ))
#define	POWER_PERCENT_CONTROL_SAMPLE_PERIOD_MS			CONTROL_INPUTS_SAMPLE_PERIOD_MS

#define CURRENT_SENSE_FEEDBACK_LPF_FC_HZ				0.5
#define CURRENT_SENSE_FEEDBACK_LPF_RC_CONSTANT_S		(1 / (6.28 * CURRENT_SENSE_FEEDBACK_LPF_FC_HZ))
#define	CURRENT_SENSE_FEEDBACK_SAMPLE_PERIOD_MS			1

#define SPEED_CONTROL_LPF_FC_HZ							0.1
#define SPEED_CONTROL_LPF_RC_CONSTANT_S					(1 / (6.28 * SPEED_CONTROL_LPF_FC_HZ))
#define	SPEED_CONTROL_SAMPLE_PERIOD_MS					CONTROL_INPUTS_SAMPLE_PERIOD_MS

#define	CURRENT_SENSE_FEDDBACK_LPF_N					3

// -------------- Defines for PID controller ----------------

#define DIMMING_ACTION_MAX_VALUE				200.0
#define DIMMING_ACTION_MIN_VALUE				0.0

#define SPEED_SETPOINT_MAX_VALUE				1800.0
#define SPEED_SETPOINT_MIN_VALUE				0.0

#define PID_01_SAMPLE_PERIOD_MS					CONTROL_INPUTS_SAMPLE_PERIOD_MS
#define PID_01_MAX_OUTPUT						0.0
#define PID_01_MIN_OUTPUT						(-DIMMING_ACTION_MAX_VALUE)

#define PID_02_SAMPLE_PERIOD_MS					CONTROL_INPUTS_SAMPLE_PERIOD_MS
#define PID_02_MAX_OUTPUT						DIMMING_ACTION_MAX_VALUE
#define PID_02_MIN_OUTPUT						0

#define KP1_BY_DEFAULT							0.01
#define KI1_BY_DEFAULT							1.0
#define KD1_BY_DEFAULT							0.0

#define KP2_BY_DEFAULT							0.21
#define KI2_BY_DEFAULT							0.092
#define KD2_BY_DEFAULT							0.0

#define I1_COMPONENT_POSITIVE_LIMIT				10.0
#define I1_COMPONENT_NEGATIVE_LIMIT				-1000.0	

#define I2_COMPONENT_POSITIVE_LIMIT				1000.0
#define I2_COMPONENT_NEGATIVE_LIMIT				-1000.0

#define CURRENT_LIMITER_PERIOD_10MS				5

// -------------- Defines for PID controller ----------------

#define FUZZY_CONTROLLER_01_SAMPLE_PERIOD_MS		CONTROL_INPUTS_SAMPLE_PERIOD_MS
#define FUZZY_CONTROLLER_01_MAX_OUTPUT				DIMMING_ACTION_MAX_VALUE
#define FUZZY_CONTROLLER_01_MIN_OUTPUT				0.0

#define GE1_BY_DEFAULT								1.0
#define GCE1_BY_DEFAULT								0.0
#define GCU1_BY_DEFAULT								1.0

// -------------------- Defines for the trigger pulse -------------------------

#define TRIGGER_PULSE_WIDTH_US		4
#define TRIGGER_PULSE_WIDTH			(TRIGGER_PULSE_WIDTH_US * 2);		
#define TRIGGER_PULSE_PERIOD_US		(TRIGGER_PULSE_WIDTH_US * 10)
#define TRIGGER_PULSE_PERIOD		(TRIGGER_PULSE_PERIOD_US * 2)				

#define AC_SIGNAL_HALF_PERIOD_US	8333
#define AC_SIGNAL_HALF_PERIOD		(AC_SIGNAL_HALF_PERIOD_US * 2)

#define TRIGGER_PHASE_OFFSET_US		220
#define TRIGGER_PHASE_OFFSET		(TRIGGER_PHASE_OFFSET_US * 2)
#define TRIGGER_MAX_PHASE			(AC_SIGNAL_HALF_PERIOD - TRIGGER_PHASE_OFFSET)

// Trigger pulse generation states
#define WAIT_FOR_ZCD				0
#define WAIT_FOR_PHASE				1
#define GENERATE_TRIGGER_PULSE		2
#define WAIT_FOR_PULSE_COMPLETE		3

/* System flags */

#define PROCESS_INPUTS_FLAG			0
#define VARIATOR_ENABLE_FLAG		1
#define CURRENT_FEEDBACK_FLAG		2
#define CURRENT_LIMITER_FLAG		3

/* Max values for the control parameters */

#define	OFFSET_VOLTAGE_INPUT_MAX_VALUE_V			15.0
#define SPAN_INPUT_MIN_VALUE						0.25//0.75
#define SPAN_INPUT_MAX_VALUE						1.00//1.25
#define CURRENT_LIMIT_INPUT_MAX_VALUE_MA			10000.0
#define CURRENT_LIMIT_INPUT_MIN_VALUE_MA			1000.0

#define OFFSET_VOLTAGE_INPUT_MAX_VALUE_PERCENT_X2	((((OFFSET_VOLTAGE_INPUT_MAX_VALUE_V + 1) * 0.54) / 220.0) * 200)
#define CURRENT_LIMIT_INPUT_FULL_SCALE_MA			(CURRENT_LIMIT_INPUT_MAX_VALUE_MA - CURRENT_LIMIT_INPUT_MIN_VALUE_MA)
#define SPAN_INPUT_FULL_SCALE						(SPAN_INPUT_MAX_VALUE - SPAN_INPUT_MIN_VALUE)

#define POSITIVE_CYCLE								0
#define NEGATIVE_CYCLE								1

#endif /* PROJECT_DEFINES_H_ */