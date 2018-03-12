#include <avr/io.h>

#ifndef CONFS
#define CONFS


//Pin led number order
//
//	       17
//	16 15  18  14 13	
//         19     12
//	0      20     11
//	1             10
//	2           
//	3    4  5 6 7 8            


//Order in schematic with ports
//
//	  Va1  Va2  Va3  Va4  Va5
//     R100 R100 R100 R100 R100		
//Vk1  1     5    9    13   17
//Vk2  2     6    10   14   18
//Vk3  3     7    11   15   19
//Vk4  4     8    12   16   20


//#define F_CPU 8000000UL

#define LEDS 20

//Versio 1.0 attiny167 pin confs
#define Va1	4
#define Va2	3
#define Va3	2
#define Va4	1
#define Va5	0

#define Vk1	1
#define Vk2	6
#define Vk3	7
#define Vk4	6


#define port_Va1	&PORTB
#define port_Va2	&PORTB
#define port_Va3	&PORTB
#define port_Va4	&PORTB
#define port_Va5 	&PORTB


#define port_Vk1	&PORTA
#define port_Vk2	&PORTA
#define port_Vk3 	&PORTA
#define port_Vk4	&PORTB



//Pointers from prot adresses; PORTC, PORTB, PORTE etc
volatile uint8_t *ledPorts[2][10] = {
	{port_Va1, port_Va2, port_Va3, port_Va4, port_Va5},
	{port_Vk1, port_Vk2, port_Vk3, port_Vk4}
}; 

//indexes for selecting correct port from PORT*
uint8_t pinIndexPointer[2][10] = {
	{Va1, Va2, Va3, Va4, Va5}, 
	{Vk1, Vk2, Vk3, Vk4}
};



#endif //CONFS
