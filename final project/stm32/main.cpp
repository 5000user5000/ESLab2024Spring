#include "mbed.h"
#include "accelero.h"
#include "network.h"
#include <sstream>
#include <chrono>
#include <iostream>
using events::EventQueue;
using std::chrono::milliseconds;
using std::chrono::microseconds;

constexpr char HOSTNAME[] = "192.168.0.1";
constexpr milliseconds EVENT_INTERVAL = 100ms;

static EventQueue eventQueue(32*EVENTS_EVENT_SIZE);
static AcceleroSensor* pSensor;
static WifiSocket* pSocket;
static Ticker trigger;
//static DigitalOut led1(LED1);
static InterruptIn button(BUTTON1);

static Timer timer;
static Timer t1;
static microseconds elapse;

static void stopSendingData() {
    trigger.detach();
    delete pSocket;
    //led1 = 0;
}

static void sendSensorData() {
    while(1){
        if(pSocket == nullptr) return;
        if(!pSocket->isAvailable()) {
            stopSendingData();
            return;
        }
        //timer.start();
        stringstream stringBuilder;

        pSensor->update();
        stringBuilder << "{\"x\":" << pSensor->getX() << ",\"y\":" << pSensor->getY() << ",\"z\":" << pSensor->getZ() << "}";//<< ",\"t\":" << elapse.count() << "}";
        t1.start();
        pSocket->sendData(stringBuilder.str());
        pSocket->recvData();
        t1.stop();
        std::cout << "time slice: " << std::chrono::duration_cast<std::chrono::microseconds>(t1.elapsed_time()).count() << "ms" << std::endl;
        t1.reset();
        //timer.stop();
        //elapse = timer.elapsed_time();        
    }

}
static BufferedSerial serial_port(USBTX, USBRX, 115200);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}
int main() {

    pSensor = new AcceleroSensor();

    pSocket = new WifiSocket(HOSTNAME, 8081);
    /*
    trigger.attach([] {
        eventQueue.call(sendSensorData);
    }, EVENT_INTERVAL);
    */
    button.rise([] {
        eventQueue.call(stopSendingData);
    });

    //led1 = 1;
    //eventQueue.dispatch_forever();
    printf("seond!\n");
    sendSensorData();
    printf("not here bro\n");
    return 0;
}