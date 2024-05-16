#include "accelero.h"

AcceleroSensor::AcceleroSensor() {
    BSP_ACCELERO_Init();
}

void AcceleroSensor::update() {
    BSP_ACCELERO_AccGetXYZ(m_acceleroData);
}

