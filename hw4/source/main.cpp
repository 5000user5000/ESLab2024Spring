#include "mbed.h"
#include <events/mbed_events.h>
#include "ble/BLE.h"
#include "ble/gap/Gap.h"
#include "ble/services/HeartRateService.h"
#include "pretty_printer.h"

using events::EventQueue;

const static char deviceName[] = "Suspecious Device";

static EventQueue eventQueue(16*EVENTS_EVENT_SIZE);

InterruptIn button(BUTTON1);
DigitalOut led(LED1);

class SuspiciousSerivice {
private:
    BLE& m_ble;

    bool m_buttonState;
    ReadOnlyGattCharacteristic<bool> m_buttonCharacteristic;
public:
    constexpr static uint16_t serviceUuid = 0xa000;
    constexpr static uint16_t characteristicUuid = 0xa001;

    SuspiciousSerivice(BLE& ble, bool initialValue):
        m_ble{ble},
        m_buttonState{false},
        m_buttonCharacteristic{serviceUuid, &m_buttonState, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY}
    {
        setupService();
    }
    
    void notifyButtonClick();
private:
    void setupService();
};

class BleDevice: ble::Gap::EventHandler {
private:
    BLE& m_ble;
    EventQueue& m_eventQueue;

    const UUID m_heartrateUuid;
    uint16_t m_heartrateValue;
    HeartRateService m_heartrateService;

    SuspiciousSerivice m_suspiciousService;

    uint8_t m_advBuffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder m_advDataBuilder;
public:
    BleDevice(BLE& ble, EventQueue& eventQueue):
        m_ble{ble},
        m_eventQueue{eventQueue},
        m_heartrateUuid{GattService::UUID_HEART_RATE_SERVICE},
        m_heartrateValue{100},
        m_heartrateService{ble, m_heartrateValue, HeartRateService::LOCATION_FINGER},
        m_suspiciousService{ble, false},
        m_advDataBuilder{m_advBuffer}
        {}
    
    void start();
private:
    void m_onInitComplete(BLE::InitializationCompleteCallbackContext* params);
    void m_startAdvertising();
    void m_updateSensorValue();
    // Override section
    virtual void onConnectionComplete(const ble::ConnectionCompleteEvent& event);
    virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event);
};

void BleDevice::m_onInitComplete(BLE::InitializationCompleteCallbackContext* params) {
    if(params->error != BLE_ERROR_NONE) {
        printf("BLE initialization failed\n");
        return;
    }

    print_mac_address();

    eventQueue.call_every(1000ms, [this] {
        m_updateSensorValue();
    });
    button.rise([this] {
        eventQueue.call([this] {
            m_suspiciousService.notifyButtonClick();
        });
    });

    m_startAdvertising();
}

void BleDevice::m_startAdvertising() {
    ble::AdvertisingParameters advParameters(ble::advertising_type_t::CONNECTABLE_UNDIRECTED, ble::adv_interval_t(ble::millisecond_t(100)));

    m_advDataBuilder.setFlags();
    m_advDataBuilder.setAppearance(ble::adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);
    m_advDataBuilder.setLocalServiceList({&m_heartrateUuid, 1});
    m_advDataBuilder.setName(deviceName);

    ble_error_t error;

    error = m_ble.gap().setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, advParameters);
    if(error) {
        printf("Advertising failed\n");
        return;
    }

    error = m_ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE, m_advDataBuilder.getAdvertisingData());
    if(error) {
        printf("Advertising failed\n");
        return;
    }

    error = m_ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
    if(error) {
        printf("Advertising failed\n");
        return;
    }

    printf("Device advertising\n");
}

void BleDevice::onConnectionComplete(const ble::ConnectionCompleteEvent& event) {
    if(event.getStatus() == ble_error_t::BLE_ERROR_NONE) {
        printf("Client connected\n");
    }
}

void BleDevice::onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event) {
    printf("Client disconnected\n");

    ble_error_t error = m_ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if(error) {
        printf("Advertising failed\n");
        return;
    }
}

void BleDevice::m_updateSensorValue() {
    ++m_heartrateValue;
    if(m_heartrateValue >= 110) {
        m_heartrateValue = 60;
    }

    m_heartrateService.updateHeartRate(m_heartrateValue);
}

void SuspiciousSerivice::setupService() {
    GattCharacteristic* charTable[] = {&m_buttonCharacteristic};
    GattService buttonService(characteristicUuid, charTable, 1);
    m_ble.gattServer().addService(buttonService);
}

void SuspiciousSerivice::notifyButtonClick() {
    m_buttonState = !m_buttonState;
    led = m_buttonState;
    m_ble.gattServer().write(m_buttonCharacteristic.getValueHandle(), (uint8_t*) &m_buttonState, sizeof(m_buttonState));
}

void BleDevice::start() {
    m_ble.gap().setEventHandler(this);
    m_ble.init(this, &BleDevice::m_onInitComplete);

    m_eventQueue.dispatch_forever();
}

void schedulBleEvents(BLE::OnEventsToProcessCallbackContext* context) {
    eventQueue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

int main() {
    BLE& ble = BLE::Instance();
    ble.onEventsToProcess(schedulBleEvents);

    BleDevice bleDevice(ble, eventQueue);
    bleDevice.start();

    return 0;
}
