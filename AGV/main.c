#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "h_bridge_a.h"
#include "h_bridge_b.h"

#define IrPinLinksVoor PL5
#define IrPinRechtsVoor PC1
#define IrPinVoor PC5
#define IrPinLinksAchter PB1
#define IrPinRechtsAchter PL3

#define PirPin PG1
#define LdrPinLinks PF0
#define LdrPinRechts PF2


#define ModusKnop PA2
#define Noodstop PD0
#define BuzzerPin

#define MotorOn 100
#define MotorOff 0
#define MotorBocht 30
enum AGV_Toestand {noodtoestand, autonoom_rijden, medewerker_volgen, ruststand, test_toestand};
enum AGV_Toestand huidige_toestand = autonoom_rijden;

ISR(INT0_vect) {
    static int aantal_interrupts = 2;
    aantal_interrupts++;
    switch(aantal_interrupts%2) {

        case(0):
            huidige_toestand = autonoom_rijden;
        break;

        case(1):
            huidige_toestand = noodtoestand;
    break;
    }


}

void init_ir(void){            //Hier word de LDR geinitialiseerd
    DDRC &= ~(1 << IrPinLinksVoor);
    DDRB &= ~(1 << IrPinLinksAchter);
    DDRC &= ~(1 << IrPinRechtsVoor);
    DDRL &= ~(1 << IrPinRechtsAchter);
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

void init_interrupt(){
    sei();
    EICRA = (0 << ISC01) | (1 << ISC00);



}
void init(void){

    init_ir();
    init_overige_sensor();
    init_niet_sensor();
    init_h_bridge_a();
    init_h_bridge_b();
    init_interrupt();

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

void bocht_maken_links (void){
    for(int i=0; i<1500; i++)
    {
        h_bridge_set_percentage_a(MotorOn);
        h_bridge_set_percentage_b(MotorBocht);
        _delay_ms(1);
    }
}

void bocht_maken_rechts (void){
    for(int i=0; i<1500; i++)
    {
        h_bridge_set_percentage_a(MotorBocht);
        h_bridge_set_percentage_b(MotorOn);
        _delay_ms(1);
    }
}


void boom_detectie (void)
{
    if ((!(PINL & (1<<IrPinLinksVoor)))|(!(PINC & (1<<IrPinRechtsVoor))))
        {
            h_bridge_set_percentage_a(MotorOff);
            h_bridge_set_percentage_b(MotorOff);
            zoemer_beep();
            for(int i=0;i<100;i++)
            {
                _delay_ms(5);
            }
            h_bridge_set_percentage_a(MotorOn);
            h_bridge_set_percentage_b(MotorOn);
            for(int i=0;i<300;i++)
            {
                _delay_ms(5);
            }
        }
}
void bocht_detecie (void)
{
    static int loop_break=0;
    if(!(PINC&(1<<IrPinVoor)))
    {
        static int turn = 2;
        switch(turn%2)
        {
        case(0):
            bocht_maken_rechts();
            break;
        case(1):
            bocht_maken_links();
            break;
        }
        if(loop_break==0)
        {
            loop_break=1;
            turn++;
        }
    }
    if(PINC&(1<<IrPinVoor))
    {
        loop_break=0;
    }
}
/*int NoodSituatie (int toestand)
{
    if (PINA & (1<<Noodstop))
    {
        toestand = 0;
    }
    return toestand;
} */

int main(void)
{
    DDRE &= ~(1 << PH3);
    DDRE |= (1 << PH4);
    PORTH &= ~(1 << PH4);



    return 0;



  /*  init();

while(1){

        static int mode_loop_break = 0;
        if (DDRA & (1 << ModusKnop)&&(mode_loop_break == 0)){   // Ik weet niet zeker hoe de knop is aangesloten, dus kan zijn dat hier nog een ! tussen moet
           mode_loop_break = 1;
           if (huidige_toestand == 4){
            huidige_toestand = 1;
            }
           else {
            huidige_toestand++;
            }
        }
        else
        {
            mode_loop_break = 0;
        }
    switch(huidige_toestand){

        case noodtoestand:
            h_bridge_set_percentage_a(MotorOff);
            h_bridge_set_percentage_b(MotorOff);
            zoemer_beep();
            break;

        case autonoom_rijden:
            if((PINL & (1<<IrPinLinksVoor))&&(PINC & (1<<IrPinRechtsVoor))&&(PINC & (1<<IrPinVoor)))
            {
                h_bridge_set_percentage_a(MotorOn);
                h_bridge_set_percentage_b(MotorOn);
            }

            boom_detectie();
            bocht_detecie();
            break;

        case medewerker_volgen:



            break;

        case ruststand:
            h_bridge_set_percentage_a(MotorOff);
            h_bridge_set_percentage_b(MotorOff);
            zoemer_beep();
            break;

        case test_toestand:


            for (int i=1;1<500;i++){
            h_bridge_set_percentage_a(MotorOn);
            h_bridge_set_percentage_b(25);
            _delay_ms(5);
            }


            break;


    }
}
 return 0; */
}

/* Dit is een Dummy van Patrick Schuurman voor versiebeheer.*/

