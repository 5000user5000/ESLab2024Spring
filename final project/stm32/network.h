#include "mbed.h"
#include <cstdint>
#include <string>

class WifiSocket {
private:
    const string m_hostname;
    const uint16_t m_remotePort;
    NetworkInterface* m_pInterface;
    TCPSocket m_socket;

    bool m_availability;

public:
    WifiSocket(const string& hostname, const uint16_t remotePort);
    ~WifiSocket();

    inline bool isAvailable() {
        return m_availability;
    }

    void sendData(const string& data);
    void recvData();
};
