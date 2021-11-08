/*
 * adc.h
 *
 *  Created on: 19 mar. 2018
 *      Author: MiguelAngel
 */

#ifndef ADC_H_
#define ADC_H_

/* Inclusiones */
#include <stdint.h>
#include <avr/io.h>

/* Pre-Scaler para el clock del ADC */
typedef enum{
	ADC_Prescaler_2 = 1,
	ADC_Prescaler_4,
	ADC_Prescaler_8,
	ADC_Prescaler_16,
	ADC_Prescaler_32,
	ADC_Prescaler_64,
	ADC_Prescaler_128
}ADC_Clock_Prescaler_t;

/* Voltaje de referencia */
typedef enum{
	AREF = 0,
	AVCC = 1,
	Internal_1p1_Bandgap = 3
}ADC_Reference_t;

/* Canal del ADC que se va a adquirir */
typedef enum{
	ADC_CH0_Single = 0,
	ADC_CH1_Single = 1,
	ADC_CH2_Single = 2,
	ADC_CH3_Single = 3,
	ADC_CH4_Single = 4,
	ADC_CH5_Single = 5,
	ADC_CH6_Single = 6,
	ADC_CH7_Single = 7,
	ADC_CH8_Temp = 8,
	ADC_1p1_Bandgap_Single = 14,	
	ADC_GND_Single = 15,
}ADC_Channel_t;

/* Trigger automático para iniciar conversión (habilitado o deshabilitado) */
typedef enum{
	ADC_Autotrigger_Disabled = 0,
	ADC_Autotrigger_Enabled
}ADC_Autotrigger_t;


/* Fuentes de trigger automático */
typedef enum{
	ADC_Free_Running_Mode = 0,
	ADC_Trigger_Analog_Comparator,
	ADC_Trigger_External_IRQ_0,
	ADC_Trigger_Timer0_CompA,
	ADC_Trigger_Timer0_Overflow,
	ADC_Trigger_Timer1_CompB,
	ADC_Trigger_Timer1_Overflow,
	ADC_Trigger_Timer1_Capture_Event
}ADC_Trigger_Source_t;


/* Máscaras para habilitar interrupciones */
typedef enum{
	ADC_Interrupt_Disabled = 0,
	ADC_Interrupt_Enabled = 8
}ADC_Interrupt_Mask_t;


/* Estructura que representa un Timer de 8 bits (para fines de configuración) */
typedef struct{
	ADC_Clock_Prescaler_t prescaler;
	ADC_Reference_t vref;
	ADC_Channel_t channel;
	ADC_Autotrigger_t auto_trigger;
	ADC_Trigger_Source_t trigger_source;
	ADC_Interrupt_Mask_t interrupt_mask;
}ADC_t;

/* Prototipos de las funciones que llevarán a cabo la configuración de los temporizadores */
void ADC_Configurar(ADC_t *adc_param);
void ADC_Seleccionar_Canal(uint8_t canal);

#endif /* ADC_H_ */
