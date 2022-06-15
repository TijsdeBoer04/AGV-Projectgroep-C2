#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "h_bridge_a.h"
#include "h_bridge_b.h"

#define IrPinLinksVoor PL5
#define IrPinRechtsVoor PC1
#define IrPinVoor PC5
#define IrPinLinksAchter PB1
#define IrPinRechtsAchter PB3

#define PirPin PG1
#define LdrPinLinks PF0
#define LdrPinRechts PF2


#define ModusKnop PA2
#define Noodstop PA6
#define Noodstop PA6
#define BuzzerPin



void init_ir(void){            //Hier word de LDR geinitialiseerd
    DDRC &= ~(1 << IrPinLinksVoor);
    DDRB &= ~(1 << IrPinLinksAchter);
    DDRC &= ~(1 << IrPinRechtsVoor);
    DDRB &= ~(1 << IrPinRechtsAchter);
    DDRL &= ~(1 << IrPinVoor);
}

void init_overige_sensor(void){ //Hier worden de PIR en LDR geinitialiseerd
    DDRG &= ~(1 << PirPin);
    DDRF &= ~(1 << LdrPinLinks);
    DDRF &= ~(1 << LdrPinRechts);
}

void init_niet_sensor(void){    //Hier wordt alles dat geen sensor is geinitialiseerd
    DDRA &= ~(1 << Noodstop);
    DDRA &= ~(1 << ModusKnop);
 //   DDRx |= (1 << BuzzerPin);

}

void init(void){

    init_ir();
    init_overige_sensor();
    init_niet_sensor();
    init_h_bridge_a();
    init_h_bridge_b();

}
void zoemer_beep (void) {

    /*
    PORTx |= (1<<BuzzerPin);
    _delay_ms (10);
    PORTx &= ~(1<<BuzzerPin);
    _delay_ms (10);
    PORTx |= (1<<BuzzerPin);
    _delay_ms (10);
    PORTx &= ~(1<<BuzzerPin);
    */
}

void bocht_maken_rechts {
    for (int i=1;1<1500;i++){
        h_bridge_set_percentage_a(MotorOn);
        h_bridge_set_percentage_b(MotorBocht);
        _delay_ms(1);
    }

}

void bocht_maken_links {
    for (int i=1;1<1500;i++){
        h_bridge_set_percentage_a(MotorBocht);
        h_bridge_set_percentage_b(MotorOn);
        _delay_ms(1);
    }

}
int main(void)
{
    init();
    enum AGV_Toestand {noodtoestand, autonoom_rijden, medewerker_volgen, ruststand, test_toestand};
    enum AGV_Toestand huidige_toestand = autonoom_rijden;
while(1){

    switch(huidige_toestand){

        case noodtoestand:
            h_bridge_set_percentage_a(0);
            h_bridge_set_percentage_b(0);
            zoemer_beep();
            break;

        case autonoom_rijden:
            if((PINL & (1<<IrPinLinksVoor))&&(PINC & (1<<IrPinRechtsVoor)))
            {
                h_bridge_set_percentage_a(100);
                h_bridge_set_percentage_b(100);
            }
            if ((!(PINL & (1<<IrPinLinksVoor)))|(!(PINC & (1<<IrPinRechtsVoor))))
            {
                h_bridge_set_percentage_a(0);
                h_bridge_set_percentage_b(0);
                zoemer_beep();
                for(int i=0;i<100;i++)
                {
                    _delay_ms(5);
                }
                h_bridge_set_percentage_a(100);
                h_bridge_set_percentage_b(100);
                for(int i=0;i<200;i++)
                {
                    _delay_ms(5);
                }
            }
            break;

        case medewerker_volgen:



            break;

        case ruststand:
            h_bridge_set_percentage_a(0);
            h_bridge_set_percentage_b(0);
            zoemer_beep();
            break;

        case test_toestand:

            for (int i=1;1<500;i++){
            h_bridge_set_percentage_a(100);
            h_bridge_set_percentage_b(25);
            _delay_ms(5);
            }


            break;


    }
}
 return 0;
}

