#include "mbed.h"
#include "accelero.h"
#include "network.h"
#include <sstream>

constexpr char HOSTNAME[] = "192.168.50.200";

int main() {
    DigitalOut led1(LED1);
    DigitalOut led2(LED2);
    AcceleroSensor sensor;
    WifiSocket socket(HOSTNAME, 8080);

    stringstream stringBuilder;
    while(socket.isAvailable()) {
        stringBuilder.clear();

        sensor.update();
        stringBuilder << sensor.getX() << "|" << sensor.getY() << "|" << sensor.getZ();
        socket.sendData(stringBuilder.str());

        ThisThread::sleep_for(100ms);
    }

    return 0;
}
