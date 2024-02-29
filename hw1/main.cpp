// header files
#include "mbed.h"
#include <chrono>

using chrono::milliseconds;

// Semaphore and thread declaration
Semaphore ledSemaphore(0);
Thread led1Thread, led2Thread;

// I/O declaration
DigitalOut led1(LED1);
DigitalOut led2(LED2);
InterruptIn button(BUTTON1);

// constant definition
constexpr milliseconds ledDelayTime = 500ms;

// global variables
volatile int ledCounter = 0;

// thread program generator
Callback<void()> ledThreadCallback(DigitalOut &led, int activeCondition)
{
    return [&led, activeCondition]()
    {
        while (true)
        {
            ledSemaphore.acquire();
            while (ledCounter % 2 == activeCondition)
            {
                led = !led;
                ThisThread::sleep_for(ledDelayTime);
            }
            led = 0;
            ledSemaphore.release();
        }
    };
}

// entry point
int main()
{
    button.rise([]()
                { ++ledCounter; });
    led1Thread.start(ledThreadCallback(led1, 0));
    led2Thread.start(ledThreadCallback(led2, 1));
    ledSemaphore.release();
    while (true)
        ;
}
