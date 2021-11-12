/*
 * dc_speed_variator.c
 *
 * Created: 11/05/2021 15:52:35
 *  Author: mtorres
 */ 


/* File inclusion */
#include "dimming_action_implementation.h"
#include "dc_speed_variator.h"


/* Variable definition */

volatile uint8_t system_flags = 0;

uint16_t speed_adj_control_adc_value = 0;
uint16_t offset_control_adc_value = 0;
uint16_t span_control_adc_value = 0;
uint16_t current_limit_control_adc_value = 0;
uint16_t current_sense_feedback_adc_value = 0;
uint16_t speed_sense_feedback_adc_value = 0;

float speed_adj_control_percent_value = 0.0;
float offset_control_value = 0.0;
float span_control_value = 0.0; 
float current_limit_control_value_ma = 0.0;
float current_sense_feedback_value_ma = 0.0;
float dc_current_sense_feedback_value_ma = 0.0;
float raw_power_percentage = 0;
float raw_speed_setpoint_rpm = 0;
float filtered_speed_setpoint_rpm = 0;

float speed_sense_feedback_value_rpm = 0.0;

volatile uint8_t cycle_indicator = POSITIVE_CYCLE;

/* Function definition */

void WDT_Off(void){
	/* Disable interrupts */
	cli();
	/* Watchdog reset */
	wdt_reset();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1 << WDRF);
	/* Write logical one to WDCE and  WDE */
	/* Keep old prescaler setting to prevent unintentional time-out */
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;
	/* Enable interrupts */
	//sei();
}


void Enable_Input_Initialize(void){
	DDR_POWER_ENABLE &= ~(1 << POWER_ENABLE);
}


void LEDs_Initialize(void){
	/* Configure Power On LED pin as OUTPUT and turn it on */ 
	DDR_ON_LED |= (1 << ON_LED);
	PORT_ON_LED |= (1 << ON_LED);
	/* Configure Run LED pin as OUTPUT and turn it off */
	DDR_RUN_LED |= (1 << RUN_LED);
	PORT_RUN_LED &= ~(1 << RUN_LED);
	/* Configure Overcurrent LED pin as OUTPUT and turn it off */
	DDR_OVERCURRENT_LED |= (1 << OVERCURRENT_LED);
	PORT_OVERCURRENT_LED &= ~(1 << OVERCURRENT_LED);	
}


void Timer0_Initialize(void){
	
	Timer_8b_t my_timer;
	
	my_timer.clock = Timer_Clk_PS64;
	my_timer.mode = Timer_8b_CTC_Mode;
	my_timer.OCRA = 249;
	my_timer.OCRB = 0;
	my_timer.OCA = OC_Disabled;
	my_timer.OCB = OC_Disabled;
	my_timer.interrupt_mask = Timer_CompA_IE;
	
	Timer0_Configurar(&my_timer);
}



void ADC_Initialize(void){
	
	ADC_t my_adc;

	my_adc.prescaler = ADC_Prescaler_64;
	my_adc.vref = AVCC;
	my_adc.channel = SPEED_ADJ_CONTROL_ADC_CHANNEL;
	my_adc.auto_trigger = ADC_Autotrigger_Disabled;
	my_adc.trigger_source = ADC_Free_Running_Mode;
	my_adc.interrupt_mask = ADC_Interrupt_Disabled;

	ADC_Configurar(&my_adc);
}




void System_Initialize(void){
	
	Enable_Input_Initialize();
	LEDs_Initialize();
	ADC_Initialize();
	Timer0_Initialize();
	Filters_Initialize();
	PID_Controllers_Initialize();
	Trigger_Implementation_Initialize();
	
}



void Current_Feedback_Measure(void){
	
	float current_sensor_volt;

	/* Measure the analog input */
	ADC_Seleccionar_Canal(CURRENT_SENSE_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	current_sense_feedback_adc_value = ADC;
	
	/* Calculate measured DC current (in miliAmperes) */
	current_sensor_volt = (current_sense_feedback_adc_value * 5.0) / 1023.0;
	current_sense_feedback_value_ma = (current_sensor_volt - 2.5) * 10000;
	Apply_LPF_Current_Sense_Feedback(current_sense_feedback_value_ma);
	dc_current_sense_feedback_value_ma = Get_LPF_Current_Sense_Feedback();
	
}

void Speed_Feedback_Measure(void){
	
	float speed_sensor_volt;
	
	/* Measure the analog input */
	ADC_Seleccionar_Canal(SPEED_SENSE_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	speed_sense_feedback_adc_value = ADC;	
	/* Calculate measured speed (in RPM) */
	speed_sensor_volt = (speed_sense_feedback_adc_value * 5.0) / 1023.0;
	speed_sense_feedback_value_rpm = speed_sensor_volt / 0.0667;
	
}

void Inputs_Read(void){
	
	int16_t regulated_speed = 0.0;
	
	/* Measure the analog inputs (potentiometers) */
	
	ADC_Seleccionar_Canal(SPEED_ADJ_CONTROL_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	speed_adj_control_adc_value = ADC;
	
	ADC_Seleccionar_Canal(OFFSET_CONTROL_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	offset_control_adc_value = ADC;
	
	ADC_Seleccionar_Canal(SPAN_CONTROL_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	span_control_adc_value = ADC;
	
	ADC_Seleccionar_Canal(CURRENT_LIMIT_CONTROL_ADC_CHANNEL);
	/* Llevar a cabo una conversión */
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
	current_limit_control_adc_value = ADC;
	
    Speed_Feedback_Measure();
	
	/* Calculate desired speed in percentage (0% to 100% --> expressed from 0 to 200) */
	speed_adj_control_percent_value = (speed_adj_control_adc_value * SPEED_SETPOINT_MAX_VALUE) / 1023.0;
	/* Calculate desired offset in percentage (Considering 220VAC input) */
	offset_control_value = (offset_control_adc_value * OFFSET_VOLTAGE_INPUT_MAX_VALUE_PERCENT_X2) / 1023.0;
	/* Calculate desired span in gain (from 0.75 to 1.25) */
	span_control_value = SPAN_INPUT_MIN_VALUE + ((span_control_adc_value * SPAN_INPUT_FULL_SCALE) / 1023.0);
	/* Calculate desired limit current value (from 1000 mA to 15000 mA) */
	current_limit_control_value_ma = CURRENT_LIMIT_INPUT_MIN_VALUE_MA + ((current_limit_control_adc_value * CURRENT_LIMIT_INPUT_FULL_SCALE_MA) / 1023.0);
	
	if(PIN_POWER_ENABLE & (1 << POWER_ENABLE)){
		raw_speed_setpoint_rpm = speed_adj_control_percent_value * span_control_value;
		Apply_LPF_Speed_Control(raw_speed_setpoint_rpm);
		filtered_speed_setpoint_rpm = Get_LPF_Speed_Control();
	}else{
		offset_control_value = 0;
		raw_speed_setpoint_rpm = 0;
		RC_LPF_Speed_Control(0.0, 1);
		filtered_speed_setpoint_rpm = raw_speed_setpoint_rpm;
	}	
	
	if(filtered_speed_setpoint_rpm > SPEED_SETPOINT_MAX_VALUE){
		filtered_speed_setpoint_rpm = SPEED_SETPOINT_MAX_VALUE;
	}else if(filtered_speed_setpoint_rpm < SPEED_SETPOINT_MIN_VALUE){
		filtered_speed_setpoint_rpm = SPEED_SETPOINT_MIN_VALUE;
	}else{
		// Does nothing
	}
	
	regulated_speed = (int16_t)PID_Controller_02(filtered_speed_setpoint_rpm, speed_sense_feedback_value_rpm, 0);

	raw_power_percentage = offset_control_value + regulated_speed;
	
	if(raw_power_percentage > DIMMING_ACTION_MAX_VALUE){
		raw_power_percentage = DIMMING_ACTION_MAX_VALUE;
	}else if(raw_power_percentage < offset_control_value){
		raw_power_percentage = offset_control_value;
	}else{
		// Does nothing
	}
	
}


void Update_Variator_Outputs(void){	
	
	//int16_t filtered_power_percentage = 0;
	int16_t current_compensation = 0;
	int16_t total_output = 0;
	
	
	if(dc_current_sense_feedback_value_ma >= current_limit_control_value_ma){
		PORT_OVERCURRENT_LED |= (1 << OVERCURRENT_LED);
	}else{
		PORT_OVERCURRENT_LED &= ~(1 << OVERCURRENT_LED);
	}
	
	
	if(PIN_POWER_ENABLE & (1 << POWER_ENABLE)){
		PORT_RUN_LED |= (1 << RUN_LED);
	}else{
		PORT_RUN_LED &= ~(1 << RUN_LED);
	}
	
	//Apply_LPF_Power_Percent_Control(raw_power_percentage);
	//filtered_power_percentage = (int16_t)Get_LPF_Power_Percent_Control();
	
	//current_compensation = (int16_t)PID_Controller_01(current_limit_control_value_ma, dc_current_sense_feedback_value_ma, 0);
	
	//total_output = filtered_power_percentage + current_compensation;
	total_output = raw_power_percentage;
	
	if(total_output < ((int16_t)DIMMING_ACTION_MIN_VALUE)){
		total_output = (int16_t)DIMMING_ACTION_MIN_VALUE;
	}else if(total_output > ((int16_t)DIMMING_ACTION_MAX_VALUE)){
		total_output = (int16_t)DIMMING_ACTION_MAX_VALUE;
	}else{
		//Does nothing
	}
	
	Set_Trigger_Phase_Binary((uint8_t)total_output);

}