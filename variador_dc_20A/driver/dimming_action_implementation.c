
/* File inclusion */
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "dimming_action_implementation.h"
#include "MT_timer.h"
#include "MT_dimmer_leading_edge.h"
#include "project_defines.h"


/* Variable definition */
volatile uint8_t trigger_pulse_state = WAIT_FOR_ZCD;
volatile uint16_t trigger_phase_binary = 0;
volatile uint8_t trigger_required = 0;

/* Function definition */

void Timer1_Initialize(void){
	
	Timer_16b_t my_timer;
	
	/* Initialize the timer stopped */
	my_timer.clock = Timer_Clk_Disabled;
	/* Set the timer to compare mode */	
	my_timer.mode = Timer_16b_CTC_OCRA_Mode;
	/* Initialize OCRA, OCRB and ICR registers */
	my_timer.OCRA = 0;
	my_timer.OCRB = 0;
	my_timer.ICR = 0;
	/* Disable the compare output pins */ 
	my_timer.OCA = OC_Disabled;
	my_timer.OCB = OC_Disabled;
	/* Disable timer interrupts */
	my_timer.interrupt_mask = Timer_Interrupts_Disabled;
	/* Complete the other parameters */
	my_timer.ic_edge_selector = Timer_IC_Rising_Edge;
	my_timer.ic_noise_canceler = Timer_ICNC_Disabled;
	
	TCNT1 = 0x0000;
	Timer1_Configurar(&my_timer);

}


void External_Interrupt_Initialize(void){
	/* Configure ZCD_SYNC_POS and ZCD_SYNC_NEG as digital input */
	DDR_ZCD_SYNC_POS &= ~(1 << ZCD_SYNC_POS);
	DDR_ZCD_SYNC_NEG &= ~(1 << ZCD_SYNC_NEG);
	/* Generate an external interrupt when a FALLING EDGE is detected at INT0 or INT1 */
	EICRA |= (1 << ISC01);
	EICRA |= (1 << ISC11);
	/* Enable the interrupt generated by INT0 and INT1 */
	EIMSK |= (1 << INT0) | (1 << INT1);
}


void Trigger_Implementation_Initialize(void){
	/* Set trigger pulse pin as OUTPUT and initialize as LOW */
	DDR_TRIGGER_PULSE_POS |= (1 << TRIGGER_PULSE_POS);
	PORT_TRIGGER_PULSE_POS &= ~(1 << TRIGGER_PULSE_POS);
	DDR_TRIGGER_PULSE_NEG |= (1 << TRIGGER_PULSE_NEG);
	PORT_TRIGGER_PULSE_NEG &= ~(1 << TRIGGER_PULSE_NEG);
	/* Prepare the table for phase control */
	Initialize_Dimmer_Leading_Edge_Table((float)AC_SIGNAL_HALF_PERIOD);
	/* Inititalize Timer1 */
	Timer1_Initialize();
	/* Initialize external interupt */
	External_Interrupt_Initialize();
}



void Pos_Trigger_Pulse_State_Machine(void){
	
	static uint16_t pulse_counter;
	static uint16_t pulse_count_setpoint;
	static uint16_t trigger_phase_binary_buffer = 0;
	static uint16_t pulse_stream_duration = 0;
		
	switch(trigger_pulse_state){
		
		case WAIT_FOR_ZCD:
			// Does nothing
			break;
			
		case WAIT_FOR_PHASE:

			/* Disable the timer count */
			TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
			/* Reset the counter */
			TCNT1 = 0;		
			/* Force trigger output to LOW */				
			TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
			PORT_TRIGGER_PULSE_POS &= ~(1 << TRIGGER_PULSE_POS);			
			/* Configure Timer1 in CTC COMPA Mode */
			TCCR1A &= ~((1 << WGM10) | (1<< WGM11));
			TCCR1B &= ~(1 << WGM13);
			TCCR1B |= (1 << WGM12);
			/* Clear the interrupt flag */
			TIFR1 |= (1 << OCF1A);
			/* Capture the value of the desired phase */
			trigger_phase_binary_buffer = trigger_phase_binary;
			pulse_stream_duration = AC_SIGNAL_HALF_PERIOD - trigger_phase_binary_buffer;
			
			if(pulse_stream_duration <= TRIGGER_PHASE_OFFSET){
				pulse_stream_duration = 0;
			}else{
				pulse_stream_duration -= TRIGGER_PHASE_OFFSET;
			}
			
			/* Calculate the number of pulses that will be transmitted */
			pulse_count_setpoint = pulse_stream_duration / TRIGGER_PULSE_PERIOD;
			pulse_counter = 0;

			if(trigger_required){

				/* Store the phase value in OCRA */
				OCR1A = trigger_phase_binary_buffer;
				/* Enable Timer1 Compare A interrupt */
				TIMSK1 |= (1 << OCIE1A);
				/* Start count */
				TCCR1B |= (1 << CS11);
				/* Update state */
				trigger_pulse_state = GENERATE_TRIGGER_PULSE;
			
			}else{
				/* Update state */
				trigger_pulse_state = WAIT_FOR_ZCD;
			}
			
			break;	
		
		case GENERATE_TRIGGER_PULSE:
		
			/* Disable the timer count */
			TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
			/* Reset the counter */
			TCNT1 = 0;								
			/* Store the PWM period and Duty Cycle in ICR and OCRA, respectively */
			ICR1 = TRIGGER_PULSE_PERIOD - 1;
			OCR1A = TRIGGER_PULSE_WIDTH - 1;
			/* Clear the interrupt flag */
			TIFR1 |= (1 << OCF1A);			
			/* Configure Timer1 in PWM Mode */
			TCCR1A &= ~(1 << WGM10);
			TCCR1A |= (1<< WGM11);
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
			TCCR1A &= ~(1 << COM1A0);
			TCCR1A |= (1 << COM1A1);
			/* Disable Timer1 Compare A interrupt */
			TIMSK1 &= ~(1 << OCIE1A);
			/* Enable Timer1 Compare ICR interrupt */
			TIMSK1 |= (1 << ICIE1);			
			/* Start count */
			TCCR1B |= (1 << CS11);		
			/* Update state */
			trigger_pulse_state = WAIT_FOR_PULSE_COMPLETE;	

			break;

		case WAIT_FOR_PULSE_COMPLETE:
		
			pulse_counter++;
			
			if(pulse_counter >= pulse_count_setpoint){
				
				pulse_counter = 0;
				
				/* Disable the timer count */
				TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
				/* Reset the counter */
				TCNT1 = 0;
				/* Force trigger output to LOW */
				PORT_TRIGGER_PULSE_POS &= ~(1 << TRIGGER_PULSE_POS);
				TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
				/* Configure Timer1 in CTC COMPA Mode */
				TCCR1A &= ~((1 << WGM10) | (1<< WGM11));
				TCCR1B &= ~(1 << WGM13);
				TCCR1B |= (1 << WGM12);
				/* Clear the interrupt flag */
				TIFR1 |= (1 << OCF1A);
				/* Disable Timer1 Compare ICR interrupt */
				TIMSK1 &= ~(1 << ICIE1);
				/* Update state */
				trigger_pulse_state = WAIT_FOR_ZCD;
				/* Enable INT1 (negative synchronous signal) interrupt */
				EIMSK |= (1 << INT1);
			}
		
		break;

		default:
			break;
		
	}
}


void Neg_Trigger_Pulse_State_Machine(void){
	
	static uint16_t pulse_counter;
	static uint16_t pulse_count_setpoint;
	static uint16_t trigger_phase_binary_buffer = 0;
	static uint16_t pulse_stream_duration = 0;
	
	switch(trigger_pulse_state){
		
		case WAIT_FOR_ZCD:
		// Does nothing
		break;
		
		case WAIT_FOR_PHASE:

			/* Disable the timer count */
			TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
			/* Reset the counter */
			TCNT1 = 0;
			/* Force trigger output to LOW */
			TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
			PORT_TRIGGER_PULSE_NEG &= ~(1 << TRIGGER_PULSE_NEG);
			/* Configure Timer1 in CTC COMPA Mode */
			TCCR1A &= ~((1 << WGM10) | (1<< WGM11));
			TCCR1B &= ~(1 << WGM13);
			TCCR1B |= (1 << WGM12);
			/* Clear the interrupt flag */
			TIFR1 |= (1 << OCF1A);
			/* Capture the value of the desired phase */
			trigger_phase_binary_buffer = trigger_phase_binary;
			pulse_stream_duration = AC_SIGNAL_HALF_PERIOD - trigger_phase_binary_buffer;
		
			if(pulse_stream_duration <= TRIGGER_PHASE_OFFSET){
				pulse_stream_duration = 0;
			}else{
				pulse_stream_duration -= TRIGGER_PHASE_OFFSET;
			}
		
			/* Calculate the number of pulses that will be transmitted */
			pulse_count_setpoint = pulse_stream_duration / TRIGGER_PULSE_PERIOD;
			pulse_counter = 0;

			if(trigger_required){

				/* Store the phase value in OCRA */
				OCR1A = trigger_phase_binary_buffer;
				/* Enable Timer1 Compare A interrupt */
				TIMSK1 |= (1 << OCIE1A);
				/* Start count */
				TCCR1B |= (1 << CS11);
				/* Update state */
				trigger_pulse_state = GENERATE_TRIGGER_PULSE;	
			}else{
				/* Update state */
				trigger_pulse_state = WAIT_FOR_ZCD;
			}
		
			break;
		
		case GENERATE_TRIGGER_PULSE:
		
			/* Disable the timer count */
			TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
			/* Reset the counter */
			TCNT1 = 0;
			/* Store the PWM period and Duty Cycle in ICR and OCRB, respectively */
			ICR1 = TRIGGER_PULSE_PERIOD - 1;
			OCR1B = TRIGGER_PULSE_WIDTH - 1;
			/* Clear the interrupt flag */
			TIFR1 |= (1 << OCF1A);
			/* Configure Timer1 in PWM Mode */
			TCCR1A &= ~(1 << WGM10);
			TCCR1A |= (1<< WGM11);
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
			TCCR1A &= ~(1 << COM1B0);
			TCCR1A |= (1 << COM1B1);
			/* Disable Timer1 Compare A interrupt */
			TIMSK1 &= ~(1 << OCIE1A);
			/* Enable Timer1 Compare ICR interrupt */
			TIMSK1 |= (1 << ICIE1);
			/* Start count */
			TCCR1B |= (1 << CS11);
			/* Update state */
			trigger_pulse_state = WAIT_FOR_PULSE_COMPLETE;

			break;

		case WAIT_FOR_PULSE_COMPLETE:
		
		pulse_counter++;
		
		if(pulse_counter >= pulse_count_setpoint){
			
			pulse_counter = 0;
			
			/* Disable the timer count */
			TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
			/* Reset the counter */
			TCNT1 = 0;
			/* Force trigger output to LOW */
			PORT_TRIGGER_PULSE_NEG &= ~(1 << TRIGGER_PULSE_NEG);
			TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
			/* Configure Timer1 in CTC COMPA Mode */
			TCCR1A &= ~((1 << WGM10) | (1<< WGM11));
			TCCR1B &= ~(1 << WGM13);
			TCCR1B |= (1 << WGM12);
			/* Clear the interrupt flag */
			TIFR1 |= (1 << OCF1A);
			/* Disable Timer1 Compare ICR interrupt */
			TIMSK1 &= ~(1 << ICIE1);
			/* Update state */
			trigger_pulse_state = WAIT_FOR_ZCD;
			/* Enable INT0 (positive synchronous signal) interrupt */
			EIMSK |= (1 << INT0);			
		}
		
		break;

		
		default:
		break;
		
	}
}


void Set_Trigger_Phase_Binary(uint8_t percent_x2){
	cli();
	trigger_required = Get_Dimmer_Leading_Edge_Integer_Phase_Time(percent_x2, (uint16_t *)(&trigger_phase_binary));
	sei();
	
	if(trigger_phase_binary <= TRIGGER_PHASE_OFFSET){
		cli();
		trigger_phase_binary = 2;
		sei();
	}else{
		cli();
		trigger_phase_binary -= TRIGGER_PHASE_OFFSET;
		sei();
		
		//if(trigger_phase_binary > TRIGGER_MAX_PHASE){
			//cli();
			//trigger_phase_binary = TRIGGER_MAX_PHASE;
			//sei();
		//}
	}
	
	
}


uint16_t Get_Trigger_Phase_Binary(void){
	return trigger_phase_binary;
}

