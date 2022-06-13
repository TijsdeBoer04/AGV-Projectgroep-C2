/*
 * h_bridge.h - XvR 2020
 */

#ifndef _H_BRIDGE_H_B
#define _H_BRIDGE_H_B

// These pins are available on the shield via the header:
//
//		Mega	Uno
// digital 5	PE3	PD5
// digital 6	PH3	PD6
// digital 9	PH6	PB1
// analog 5	PF5	PC5

// The settings below are for the Mega, modify
// in case you want to use other pins
#define PORT_RPWM_B	PORTH
#define PIN_RPWM_B	PH6
#define DDR_RPWM_B	DDRA

#define PORT_LPWM_B	PORTB
#define PIN_LPWM_B	PB4
#define DDR_LPWM_B	DDRB

void init_h_bridge_b(void);
void h_bridge_set_percentage_b(signed char percentage_b);

#endif /* _H_BRIDGE_H_ */
