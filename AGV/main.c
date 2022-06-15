#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "h_bridge_a.h"
#include "h_bridge_b.h"

#define TrigPinLinks PL1
#define TrigPinRechts PL3
#define EchoPinLinks PB1
#define EchoPinRechts PB3

#define IrPinLinks PC1
#define IrPinRechts PC5
#define IrPinVoor PL5

#define PirPin PG1
#define LdrPinLinks PF0
#define LdrPinRechts PF2

#define ModusKnop PA2
#define Noodstop PA6
#define Led1
#define Led2
#define Led3
#define Led4

void init_ultrasoon(void){      //De trig en echo pin van de ultrasoon worden hier geinitialiseerd
    DDRL &= ~(1 << TrigPinLinks);
    DDRB &= ~(1 << EchoPinLinks);
    DDRL &= ~(1 << TrigPinRechts);
    DDRB &= ~(1 << EchoPinRechts);
}

void init_ir(void){            //Hier word de LDR geinitialiseerd
    DDRC &= ~(1 << IrPinLinks);
    DDRC &= ~(1 << IrPinRechts);
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
  /*DDRx |= (1 << Led1);
    DDRx |= (1 << Led2);
    DDRx |= (1 << Led3);
    DDRx |= (1 << Led4); */
}

void init(void){
    init_ultrasoon();
    init_ir();
    init_overige_sensor();
    init_niet_sensor();
    init_h_bridge_a();
    init_h_bridge_b();
}

int main(void)
{
    init();
    enum AGV_Toestand {noodtoestand, autonoom_rijden, medewerker_volgen, ruststand, test_toestand};
    enum AGV_Toestand huidige_toestand = ruststand;
while(1){

    switch(huidige_toestand){

        case noodtoestand:
            h_bridge_set_percentage_a(0);
            h_bridge_set_percentage_b(0);
            // Zoemer beept
            break;

        case autonoom_rijden:
            h_bridge_set_percentage_a(100);
            h_bridge_set_percentage_b(100);
            if(IrLinksVoor)
            break;

        case medewerker_volgen:

            break;

        case ruststand:

            break;

        case test_toestand:


            h_bridge_set_percentage_a(100);
            h_bridge_set_percentage_b(100);
            _delay_ms(3000);
            for (int i=1;1<500;i++){
            h_bridge_set_percentage_a(100);
            h_bridge_set_percentage_b(25);
            _delay_ms(5);
            }


            break;


    }
 return 0;
}
