#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "functions.h"


volatile uint8_t test = 0;

//Interrupt vector
ISR(PCINT0_vect){
	//Disable interrupt flag from PCINT0		
	PCIFR &= ~(1<< PCIF0);

	//Set all anodes low
	PORTB = 0x00;

	//Set all cathodes high
	PORTA = 0xC2;
	PORTB |= (1<<6);
	
	//Usr should stop pushing buttom within 1sec
	//or double interrupt will be the case
	_delay_ms(1000); 

	if(test < 3) test++;
	else test = 0;
	
	//Enable interrupt flag from PCINT0
	PCIFR |= (1<< PCIF0);
}


void main (void) {
	//Init all ports
 	initPorts();
	
	int LEDsPeriod = 100;
	uint8_t percentMAX = 100;
	uint8_t percentMIN = 1;
	uint8_t runTimes = 1;
	uint8_t slowing1 = 20;
	uint8_t slowing2 = 2;
	

	sleep_cpu();

     while (1) {

		switch(test){

   			case 0:
      			//test1
				lightWaveAll(runTimes, LEDsPeriod, percentMIN, percentMAX);
				//_delay_ms(500);
				break;
	
   			case 1:
     			//Test2
				lightWaveSingle(runTimes, slowing1, LEDsPeriod, percentMIN, percentMAX);
				//_delay_ms(500);
				break;

			case 2:
  				//Test3
				lightWaveBlocks(runTimes, slowing2, LEDsPeriod, percentMIN, percentMAX);
				//_delay_ms(500);
				break;
			case 3:
  				//Test3
				sleep_cpu();
				//_delay_ms(500);
				break;
   
   		default: test = 0;
		}
  	
     }

}





