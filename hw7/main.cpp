#include "mbed.h"
#include <math.h>
#include "math_helper.h"

constexpr size_t testDataSize = 320;
constexpr uint16_t tapCount = 29;
constexpr uint32_t blockSize = 32;
constexpr int blockCount = testDataSize / blockSize;

extern float32_t testInput_f32_1kHz_15kHz[testDataSize];
extern float32_t refOutput[testDataSize];

const float32_t firCoeffs32[tapCount] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f//, 0.0f,0.0f,0.0f
};
static float32_t firStateF32[blockSize + tapCount - 1];
static float32_t testOutput[testDataSize];

DigitalOut led(LED1);

inline bool isNaN(float sus) {
    constexpr uint32_t nanPattern = 0x7fc00000;
    union {
        float f;
        uint32_t u;
    } test = {.f = sus};
    return test.u == nanPattern;
}

template<size_t size>
float computeSNR(float(&ref)[size], float(&test)[size]) {
    float signalEnergy = 0.0;
    float errorEnergy = 0.0;

    for(int i=0; i<size; ++i) {
        if(isNaN(ref[i]) || isNaN(test[i])) return 0;
        signalEnergy += ref[i] * ref[i];
        errorEnergy += (ref[i]-test[i]) * (ref[i]-test[i]);
    }

    if(isNaN(errorEnergy)) return 0;

    float snr = 10 * (float)log10((double)signalEnergy / (double)errorEnergy);
    return snr;
}

int main() {
    led = 0;

    arm_fir_instance_f32 filter;
    arm_fir_init_f32(&filter, tapCount, const_cast<float32_t*>(firCoeffs32), firStateF32, blockSize);

    for(int i=0; i<blockCount; ++i) {
        float* pInputF32 = testInput_f32_1kHz_15kHz + i*blockSize;
        float* pOutputF32 = testOutput + i*blockSize;
        arm_fir_f32(&filter, pInputF32, pOutputF32, blockSize);
    }

    float filteredSNR = computeSNR(refOutput, testOutput);
    printf("SNR: %f\n", filteredSNR);

    led = 1;

    while(true);
    return 0;
}
