#include "mbed.h"

PwmOut logic(D9);
PwmOut led(LED1);

int main()
{
    logic.period_ms(50);
    logic = 0.5;
    led.period(1);
    led = 0.5;
    while(true);
}
