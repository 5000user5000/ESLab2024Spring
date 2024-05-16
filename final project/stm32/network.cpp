#include "network.h"

WifiSocket::WifiSocket(const string& hostname, const uint16_t remotePort):
    m_hostname{hostname},
    m_remotePort{remotePort},
    m_pInterface{NetworkInterface::get_default_instance()},
    m_availability{false}
{
    nsapi_error_t error;

    // connect wifi network
    error = m_pInterface->connect();
    if(error != NSAPI_ERROR_OK) return;

    // open socket
    error = m_socket.open(m_pInterface);
    if(error != NSAPI_ERROR_OK) return;

    // resolve hostname
    SocketAddress address;
    error = m_pInterface->gethostbyname(m_hostname.c_str(), &address);
    if(error != NSAPI_ERROR_OK) return;

    // connect to host
    address.set_port(m_remotePort);
    error = m_socket.connect(address);
    if(error != NSAPI_ERROR_OK) return;

    m_availability = true;
}

WifiSocket::~WifiSocket() {
    if(m_pInterface != nullptr) {
        m_pInterface->disconnect();
    }
}

void WifiSocket::sendData(const string& data) {
    nsapi_size_or_error_t size_or_error;
    
    size_or_error = m_socket.send(data.c_str(), data.length());
    if(size_or_error <= 0) m_availability = false;
}
