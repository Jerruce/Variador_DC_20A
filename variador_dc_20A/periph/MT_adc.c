/*
 * adc.c
 *
 *  Created on: 19 mar. 2018
 *      Author: MiguelAngel
 */

/* Inclusiones */
#include <stdint.h>
#include <avr/io.h>
#include "MT_adc.h"


/****************************************************************************
* Nombre de la función: ADC_Configurar
* retorna : Nada
* arg : adc_param
* Descripción : Recibe un puntero hacia una estructura de tipo ADC_t (la
* 				cual contiene los parámetros de configuración) y configura
* 				el módulo ADC.
* Notas : Esta función no está diseñada para configurar canales diferenciales
* 		  ni ganancias. Tampoco permite deshabilitar las entradas digitales
* 		  que no se van a utilizar.
*****************************************************************************/
void ADC_Configurar(ADC_t *adc_param){

	/* Inicializar los registros de configuración */
	ADMUX = 0;
	ADCSRB = 0;
	ADCSRA = 0;

	/* Habilitar el ADC */
	ADCSRA |= (1 << ADEN);
	/* Configurar el Pre-Scaler del ADC */
	ADCSRA |= (adc_param->prescaler & 0x07) << ADPS0;
	/* Configurar el voltaje de referencia */
	ADMUX |= (adc_param->vref & 0x03) << REFS0;
	/* Cargar los nuevos valores de los bits selectores de canal */
	ADMUX |= (adc_param->channel & 0x0F) << MUX0;
	/* Configurar el trigger automático */
	ADCSRA |= (adc_param->auto_trigger & 0x01) << ADATE;
	ADCSRB |= (adc_param->trigger_source & 0x07) << ADTS0;
	/* Configurar el habilitador de interrupción */
	ADCSRA |= adc_param->interrupt_mask;
	/* Llevar a cabo una primera conversión */
    ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF))){
		/* No hacer nada hasta que la conversión finalice */
	}
	/* Limpiar la bandera de conversión completa */
	ADCSRA |= (1 << ADIF);
}

/****************************************************************************
* Nombre de la función: ADC_Canal_Seleccionar
* retorna : Nada
* arg : canal
* Descripción : Recibe como parámetro el número de canal analógico que
* 				se desea muestrear (de 0 a 15) y configura el módulo
* 				ADC para leer el canal seleccionado.
* Notas : No llamar a esta función mientras una conversión se esté llevando
* 		  a cabo (esperar a que la conversión termine). Esta función
* 		  permite seleccionar tanto canales simples como canales
* 		  diferenciales con ganancia programable. La función cuenta con un
* 		  "filtro" interno en caso que el parámetro ingresado se encuentre 
*         fuera del rango de 0 a 31.
*****************************************************************************/
void ADC_Seleccionar_Canal(uint8_t canal){

	ADC_Channel_t selector;

	/* Tabla para la configuración de los canales */
	static ADC_Channel_t tabla_canales[] ={
										ADC_CH0_Single,			//0
										ADC_CH1_Single,			//1
										ADC_CH2_Single,			//2
										ADC_CH3_Single,			//3
										ADC_CH4_Single,			//4
										ADC_CH5_Single,			//5
										ADC_CH6_Single,			//6
										ADC_CH7_Single,			//7
										ADC_CH8_Temp,			//8
										9,						//9 (NO USAR)
										10,						//10 (NO USAR)
										11,						//11 (NO USAR)
										12,						//12 (NO USAR)
										13,						//13 (NO USAR) 
										ADC_1p1_Bandgap_Single,	//14
										ADC_GND_Single,			//15		
									};

	/* Filtrar el parámetro canal para que no apunte fuera de la tabla */
	canal &= 0x0F;
	/* Cargar el valor correspondiente al selector */
	selector = tabla_canales[canal];
	/* Limpiar todos los bits selectores de canal del ADC */
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	/* Cargar los nuevos valores de los bits selectores */
	ADMUX |= (selector & 0x0F) << MUX0;
}
