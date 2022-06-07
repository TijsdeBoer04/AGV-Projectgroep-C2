#include <avr/io.h>
#include "h_bridge_a.h"
#include "h_bridge_b.h"
#include <util/delay.h>

int main(void)
{
    init_h_bridge_a();
    init_h_bridge_b();
while(1){
        h_bridge_set_percentage_a(80);
        h_bridge_set_percentage_b(80);
        _delay_ms(1000);
        h_bridge_set_percentage_a(0);
        h_bridge_set_percentage_b(0);
        _delay_ms(1000);
        h_bridge_set_percentage_a(0);
        h_bridge_set_percentage_b(100);
        _delay_ms(500);
        h_bridge_set_percentage_a(0);
        h_bridge_set_percentage_b(0);
        _delay_ms(1000);}

    return 0;
}
