#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"
#include <cstdint>

class AcceleroSensor {
private:
    int16_t m_acceleroData[3];

public:
    AcceleroSensor();

    void update();

    inline int16_t getX() {
        return m_acceleroData[0];
    }
    inline int16_t getY(){
        return m_acceleroData[1];
    }
    inline int16_t getZ(){
        return m_acceleroData[2];
    }
};
