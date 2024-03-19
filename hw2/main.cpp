#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"

#include <string>
#include <sstream>

class DataSocket {
private:
    static constexpr uint16_t remotePort = 8080;

    NetworkInterface* m_interfacePtr;
    TCPSocket m_socket;
    int16_t m_acceleroData[3];
    int m_sampleCount;

public:
    DataSocket(): m_interfacePtr{NetworkInterface::get_default_instance()} {
        BSP_ACCELERO_Init();
    }

    ~DataSocket() {
        if(m_interfacePtr != nullptr) {
            m_interfacePtr->disconnect();
        }
    }

    void run();

private:
    bool m_connectNetwork();
    bool m_openSocket();
    bool m_resolveHostname(SocketAddress& address);
    bool m_connectSocket(SocketAddress& address);
    bool m_sendSensorData();
};

bool DataSocket::m_connectNetwork() {
    printf("Connecting to network...\n");
    nsapi_error_t result = m_interfacePtr->connect();

    if(result != 0) {
        printf("Network connection error: %d\n", result);
    }
    return result == 0;
}

bool DataSocket::m_openSocket() {
    printf("Opening socket...\n");
    nsapi_error_t result = m_socket.open(m_interfacePtr);

    if(result != 0) {
        printf("Socket error: %d\n", result);
    }
    return result == 0;
}

bool DataSocket::m_resolveHostname(SocketAddress& address) {
    const char hostname[] = MBED_CONF_APP_HOSTNAME;

    printf("Resolving hostname %s\n", hostname);
    nsapi_error_t result = m_interfacePtr->gethostbyname(hostname, &address);
    if(result != 0) {
        printf("Host resolving error: %d\n", result);
        return false;
    }

    printf("Resolved address: %s\n", address.get_ip_address());
    return true;
}

bool DataSocket::m_connectSocket(SocketAddress& address) {
    printf("Opening connection to remote port %d\n", remotePort);
    address.set_port(remotePort);
    nsapi_error_t result = m_socket.connect(address);

    if(result != 0) {
        printf("Remote host connection error: %d", result);
    }
    return result == 0;
}

bool DataSocket::m_sendSensorData() {
    BSP_ACCELERO_AccGetXYZ(m_acceleroData);
    ++m_sampleCount;

    stringstream jsonStream;
    jsonStream << "{\"x\":" << m_acceleroData[0] << ",\"y\":" << m_acceleroData[1] << ",\"z\":" << m_acceleroData[2] << ",\"s\":" << m_sampleCount << "}";
    string json = jsonStream.str();

    nsapi_size_or_error_t length = m_socket.send(json.c_str(), json.length());

    if(length <= 0) {
        printf("Sending data error: %d", length);
    }
    return length > 0;
}

void DataSocket::run() {
    if(m_interfacePtr == nullptr) {
        printf("No network interface found.\n");
        return;
    }

    bool stageSuccess;

    stageSuccess = m_connectNetwork();
    if(!stageSuccess) return;

    stageSuccess = m_openSocket();
    if(!stageSuccess) return;

    SocketAddress address;
    stageSuccess = m_resolveHostname(address);
    if(!stageSuccess) return;

    stageSuccess = m_connectSocket(address);
    if(!stageSuccess) return;

    while(true) {
        stageSuccess = m_sendSensorData();
        if(!stageSuccess) break;
        
        ThisThread::sleep_for(100ms);
    }
}

int main() {
    DataSocket socket;
    socket.run();

    return 0;
}
