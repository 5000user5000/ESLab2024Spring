#include "mbed.h"
#include "accelero.h"
#include "network.h"
#include <sstream>
#include <chrono>

using events::EventQueue;
using std::chrono::milliseconds;

constexpr char HOSTNAME[] = "192.168.50.200";
constexpr milliseconds EVENT_INTERVAL = 100ms;

static EventQueue eventQueue(32*EVENTS_EVENT_SIZE);
static AcceleroSensor* pSensor;
static WifiSocket* pSocket;
static Ticker timer;
static DigitalOut led1(LED1);
static InterruptIn button(BUTTON1);

void stopSendingData() {
    timer.detach();
    delete pSocket;
    led1 = 0;
}

void sendSensorData() {
    if(pSocket == nullptr) return;
    if(!pSocket->isAvailable()) {
        stopSendingData();
        return;
    }

    stringstream stringBuilder;

    pSensor->update();
    stringBuilder << "{\"x\":" << pSensor->getX() << ",\"y\":" << pSensor->getY() << ",\"z\":" << pSensor->getZ() << "}";
    pSocket->sendData(stringBuilder.str());
}

int main() {
    pSensor = new AcceleroSensor();
    pSocket = new WifiSocket(HOSTNAME, 8080);
    timer.attach([] {
        eventQueue.call(sendSensorData);
    }, EVENT_INTERVAL);
    button.rise([] {
        eventQueue.call(stopSendingData);
    });

    led1 = 1;
    eventQueue.dispatch_forever();

    return 0;
}
