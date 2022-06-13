/*
 * h_bridge.c - XvR 2020
 *
 * Use 8-bit timer. Uses interrupts in order to be able
 * to use the pins on the multifunction shield
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "h_bridge_b.h"

ISR(TIMER2_OVF_vect)
{
	if (OCR2A == 0 && OCR2B == 0)
	{
		PORT_RPWM_B &= ~(1<<PIN_RPWM_B);
		PORT_LPWM_B &= ~(1<<PIN_LPWM_B);
	}
	else if (OCR2A != 0)
	{
		PORT_LPWM_B &= ~(1<<PIN_LPWM_B);
		PORT_RPWM_B |= (1<<PIN_RPWM_B);
	}
	else if (OCR2B != 0)
	{
		PORT_RPWM_B &= ~(1<<PIN_RPWM_B);
		PORT_LPWM_B |= (1<<PIN_LPWM_B);
	}
}

ISR(TIMER2_COMPA_vect)
{
	if (OCR2A != 255)
	{
		PORT_RPWM_B &= ~(1<<PIN_RPWM_B);
	}
}

ISR(TIMER2_COMPB_vect)
{
	if (OCR2B != 255)
	{
		PORT_LPWM_B &= ~(1<<PIN_LPWM_B);
	}
}

void init_h_bridge_b(void)
{
	// Config pins as output
	DDR_RPWM_B |= (1<<PIN_RPWM_B);
	DDR_LPWM_B |= (1<<PIN_LPWM_B);

	// Output low
	PORT_RPWM_B &= ~(1<<PIN_RPWM_B);
	PORT_LPWM_B &= ~(1<<PIN_LPWM_B);

	// Use mode 0, clkdiv = 64
	TCCR2A = 0;
	TCCR2B = (0<<CS22) | (1<<CS21) | (1<<CS20);

	// Disable PWM output
	OCR2A = 0;
	OCR2B = 0;

	// Interrupts on OCA, OCB and OVF
	TIMSK2 = (1<<OCIE2B) | (1<<OCIE2A) | (1<<TOIE2);

	sei();
}

void h_bridge_set_percentage_b(signed char percentage_b)
{
	if (percentage_b >= -100 && percentage_b <= 100)
	{
		if (percentage_b >= 0)
		{
			// Disable LPWM, calculate RPWM
			OCR2B = 0;
			OCR2A = (255*percentage_b)/100;
		}
		else // percentage < 0
		{
			// Disable RPWM, calculate LPWM
			OCR2A = 0;
			OCR2B = (255*percentage_b)/-100;
		}
	}
}
