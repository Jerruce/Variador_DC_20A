/*
 * variador_dc_10a.c
 *
 * Created: 11/05/2021 10:10:30
 * Author : mtorres
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"
#include "project_defines.h"
#include "dc_speed_variator.h"
#include "dimming_action_implementation.h"

int main(void)
{
    /*Turn-off watchdog timer */
    WDT_Off();
	/* Initialize peripherals, drivers and libraries */
	System_Initialize();
	sei();
	
	/* Repeat indefinitely ... */
    while(1){
		
		/* Measure current sense feedback every 1ms */
		if(system_flags & (1 << CURRENT_FEEDBACK_FLAG)){
			cli();
			system_flags &= ~(1 << CURRENT_FEEDBACK_FLAG);
			sei();
			
			Current_Feedback_Measure();
			
			/* After measuring current feedback, measure the user inputs if required */
			if(system_flags & (1 << PROCESS_INPUTS_FLAG	)){
				
				cli();
				system_flags &= ~(1 << PROCESS_INPUTS_FLAG);
				sei();
				
				Inputs_Read();
				Update_Variator_Outputs();
			}
			
		}
		
    }
	
	return 0;
}


/* Interrupt Service Routines (ISRs) */


ISR(TIMER0_COMPA_vect){
	
	static uint8_t conta_1ms = 0;
	
	/* Measure the current feedback every 1ms */
	system_flags |= (1 << CURRENT_FEEDBACK_FLAG);
	
	conta_1ms++;
	/* Measure current feedback every 10ms */
	if(conta_1ms >= CONTROL_INPUTS_SAMPLE_PERIOD_MS){
		conta_1ms = 0;
		system_flags |= (1 << PROCESS_INPUTS_FLAG);	
	}
	
}

ISR(TIMER1_COMPA_vect){
	trigger_pulse_state = GENERATE_TRIGGER_PULSE;
	Trigger_Pulse_State_Machine();
}

ISR(TIMER1_CAPT_vect){
	trigger_pulse_state = WAIT_FOR_PULSE_COMPLETE;
	Trigger_Pulse_State_Machine();
}


ISR(INT0_vect){
	trigger_pulse_state = WAIT_FOR_PHASE;
	Trigger_Pulse_State_Machine();
}




