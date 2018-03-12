#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "functions.h"
#include "confs.h"


//Inits all anodes low and cathodes low (=> no leds should be on)
void initPorts(void){
	//all are set as output
	DDRA = 0xFE; 
	DDRB = 0xFF;  

	//Set all anodes low
	PORTB = 0x00;

	//Set all cathodes high
	PORTA = 0xC2;
	PORTB |= (1<<6);

	//enable sleep
	SMCR = (1<<SM1) | (1<<SM0) | (1<<SE); //Enables power down mode 
	PRR = (1<<PRLIN) |(1<<PRSPI) |(1<<PRTIM1) |(1<<PRTIM0) |(1<<PRUSI) |(1<<PRADC); //parts to be shut down
	MCUCR = (1<<BODS) | (1<<BODSE);
	

	//Enable interrupts for button
	PCICR |= (1<<PCIE0);	//Enable pin change interrupt for PCINT[7:0]
	PCIFR |= (1<<PCIF0);	//Setting interrupt flag for PCINT0
	PCMSK0 |= (1<<PCINT0);	//Mask for enabling external interrupt PCINT[0]
	sei();				//Enables all interrupts


}


/////////////////////////////////////////////////////
//
//
//	ALL COTROLL FUNCTIONS FOR LEDS ARE BELOW
//
//
/////////////////////////////////////////////////////





//Macros for lighting and shutting led in a matrix
#define SET_LED_MATRIX(x, y) 		{*ledPorts[0][x] |= (1<<pinIndexPointer[0][x]), *ledPorts[1][y]  &= ~(1<<pinIndexPointer[1][y]);}
#define SHUT_LED_MATRIX(x, y)		{*ledPorts[0][x] &= ~(1<<pinIndexPointer[0][x]), *ledPorts[1][y] |= (1<<pinIndexPointer[1][y]);}


//Delay macro for functions 
#define DELAY_US(us)	{while(us--){_delay_us(1);}}


//Simple set pin high or low by changing state from 0 <-> 1 and pinNumber
//
//WARNING!! This may cause current overload if too many LEDs are ON
//
void setPin(bool state, uint8_t LEDNumber){
	uint8_t firstDec = 0, secDec = 0;
	uint8_t newNumber = LEDNumber;

	//Decimal controlling was desired, thus +6 in order to control 5x4 matrix
	if(newNumber >= 4) newNumber = newNumber+6;
	if(newNumber >= 14) newNumber = newNumber+6;
	if(newNumber >= 24) newNumber = newNumber+6;
	if(newNumber >= 34) newNumber = newNumber+6;
	
	//Checks tenth
	firstDec = newNumber%10;
	//Checks hundreds
	secDec = (newNumber/10)%10;	
	
	//Light LED if true, else LED is shut down 
	if(state == true) {
		SET_LED_MATRIX(secDec, firstDec);
	}else SHUT_LED_MATRIX(secDec, firstDec);
}


//Simple blink function for single LED.
//
//Safe to use if not modified.
//
void blinkLED(uint8_t LEDNumber, int period_us, uint8_t dutyCycle){
	int timeOn = period_us*dutyCycle/100;	
	//int timeOff = period_us*(100-dutyCycle)/100;
	int timeOff = period_us-timeOn;

	//Set pin high
	if(timeOn > 0) {
		setPin(true, LEDNumber);

		//waiting for defined time
		DELAY_US(timeOn);
	}
	
	
	//Set pin low
	setPin(false, LEDNumber);

	//waiting for defined time
	DELAY_US(timeOff);
	
}


void lightAll(uint8_t loops, int period_us, uint8_t dutyCycle){
	//Defines how many times this function will be run
	for(uint16_t x=0; x < loops; x++){

		//Blinks all 20 LEDs
		for(uint8_t led = 0; led < LEDS; led++){
			blinkLED(led, period_us, dutyCycle);
		}		
	}
}


/////////////////////////////////////////////////////
//
//
//	ALL ACTUAL IMAGES FOR LEDS ARE BELOW
//
//
/////////////////////////////////////////////////////




void lightWaveAll(uint8_t loops, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Defines how many times this function will be run			
	for(uint8_t x=0; x < loops; x++){

		//Loops from minimum brightness to maximum
		for(uint8_t percent = minBrightness; percent <= maxBrightness; percent++){
			lightAll(3, period_us, percent);
		}
		//Loops from maximun brightness to minimum
		for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
			lightAll(3, period_us, percent);
		}
	}
}



void lightWaveSingle(uint8_t loops, uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t loop = 0; loop < loops; loop++){	
	
		//Offset increase for moving image
		for(uint8_t offset = 0; offset < LEDS+5; offset++){
			
			//Defines how many times all LEDs will be run before offset increase
			for(uint8_t x=0; x < slowing; x++){

				//Runs all LEDs through with offset statements
				for(uint8_t led = 5; led < LEDS+5; led++){
					if(led == offset)		 blinkLED(led-5, period_us, minBrightness);
					else if (led == offset+1) blinkLED(led-5, period_us, (maxBrightness-minBrightness)/2);
					else if (led == offset+2) blinkLED(led-5, period_us, maxBrightness);
					else if (led == offset+3) blinkLED(led-5, period_us, (maxBrightness-minBrightness)/2);
					else if (led == offset+4) blinkLED(led-5, period_us, minBrightness);
					else blinkLED(led, period_us, 0);
					
				}	
			}		
		}
	}
}


void lightWaveBlocks(uint8_t loops, uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
		for(uint8_t loop = 0; loop < loops; loop++){
			//Loops blocks with different brightness from minimum to maximum
			for(uint8_t percent = 0; percent <= (maxBrightness - minBrightness); percent++){
			
				//Loops all LEDs few times before brightness change
				for(uint8_t x=0; x < slowing; x++){
					for(uint8_t led = 0; led < LEDS; led++){
						if(led <= 3) 		blinkLED(led, period_us, minBrightness+percent);
						else if(led <= 8) 	blinkLED(led, period_us, maxBrightness-percent);
						else if(led <= 13) 	blinkLED(led, period_us, minBrightness+percent);
						else if(led <= 15) 	blinkLED(led, period_us, maxBrightness-percent);
						else 			blinkLED(led, period_us, 0);
					}
				}
			}
			//Loops blocks with different brightness from maximum to minimum
			for(uint8_t percent = (maxBrightness - minBrightness); percent > 0; percent--){

				//Loops all LEDs few times before brightness change
				for(uint8_t x=0; x < slowing; x++){
					for(uint8_t led = 0; led < LEDS; led++){
						if(led <= 3) 		blinkLED(led, period_us, minBrightness+percent);
						else if(led <= 8) 	blinkLED(led, period_us, maxBrightness-percent);
						else if(led <= 13) 	blinkLED(led, period_us, minBrightness+percent);
						else if(led <= 15) 	blinkLED(led, period_us, maxBrightness-percent);
						else 			blinkLED(led, period_us, minBrightness+percent);
					}
				}
			}	
	}
}





























