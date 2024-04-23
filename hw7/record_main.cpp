#include "mbed.h"
#include <cstdint>
#include <cmath>
#include "math_helper.h"
#include "stm32l475e_iot01_audio.h"
#include "stm32l4xx_hal.h"

static constexpr uint32_t sampleTimeSec = 1;
static constexpr size_t audioBufferSize = AUDIO_SAMPLING_FREQUENCY * sampleTimeSec;
static constexpr size_t blockSize = 32;
static constexpr uint16_t tapCount = 29;
static constexpr size_t blockCount = audioBufferSize / blockSize;

static EventQueue eventQueue;

DigitalOut led(LED1);

static arm_fir_instance_f32 filter;
static const float firLpf6kCoeff[tapCount] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};
static float firState[blockSize + tapCount - 1];

static uint8_t pcmBuffer[PCM_BUFFER_LEN];

static int16_t* audioBuffer;
static int16_t* filteredAudio;
static size_t audioBufferIndex = 0;

static InterruptIn userButton(BUTTON1);

void dumpWav(int16_t* buffer) {
    constexpr size_t wavHeaderSize = 44;

    const size_t frequency = AUDIO_SAMPLING_FREQUENCY;
    const size_t dataSize = audioBufferSize * sizeof(int16_t);
    const size_t fileSize = wavHeaderSize + dataSize;

    const uint8_t wavHeader[wavHeaderSize] = {
        'R', 'I', 'F', 'F',
        fileSize & 0xff, (fileSize >> 8) & 0xff, (fileSize >> 16) & 0xff, (fileSize >> 24) & 0xff,
        'W', 'A', 'V', 'E',
        'f', 'm', 't', ' ',
        0x10, 0x00, 0x00, 0x00,  // 16 bytes of format data
        0x01, 0x00,  // 1 = PCM
        0x01, 0x00,  // 1 channel
        frequency & 0xff, (frequency >> 8) & 0xff, (frequency >> 16) & 0xff, (frequency >> 24) & 0xff,
        0x00, 0x7d, 0x00, 0x00,
        0x02, 0x00, 0x10, 0x00,
        'd', 'a', 't', 'a',
        dataSize & 0xff, (dataSize >> 8) & 0xff, (dataSize >> 16) & 0xff, (dataSize >> 24) & 0xff
    };

    for(int i=0; i<wavHeaderSize; ++i) {
        printf("%02x", wavHeader[i]);
    }

    uint8_t* pData = (uint8_t*)buffer;
    for(int i=0; i<dataSize; ++i) {
        printf("%02x", pData[i]);
    }
    printf("\n");
}

void onBufferFull() {
    int32_t error = BSP_AUDIO_IN_Pause(AUDIO_INSTANCE);
    if(error != BSP_ERROR_NONE) {
        printf("audio pause error %d\n", error);
    }
    printf("finished recording\n");
    led = 0;

    float firInput[blockSize];
    float firOutput[blockSize];
    for(int i=0; i<blockCount; ++i) {
        const size_t offset = i * blockSize;
        for(int j=0; j<blockSize; ++j) {
            firInput[j] = (float)audioBuffer[j + offset] / 32768.0;
        }
        arm_fir_f32(&filter, firInput, firOutput, blockSize);
        for(int j=0; j<blockSize; ++j) {
            filteredAudio[j + offset] = (int16_t)(firOutput[j] * 32768.0);
        }
    }

    printf("Raw audio:\n");
    dumpWav(audioBuffer);
    printf("Filtered audio:\n");
    dumpWav(filteredAudio);
}

// Override callbac
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t) {
    const size_t newSampleCount = PCM_BUFFER_LEN / sizeof(int16_t) / 2;
    if(audioBufferIndex + newSampleCount > audioBufferSize) {
        return;
    }

    memcpy((uint8_t*)(audioBuffer + audioBufferIndex), pcmBuffer, PCM_BUFFER_LEN / 2);
    audioBufferIndex += newSampleCount;

    if(audioBufferIndex >= audioBufferSize) {
        eventQueue.call(&onBufferFull);
    }
}
// Override callback
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t) {
    const size_t newSampleCount = PCM_BUFFER_LEN / sizeof(int16_t) / 2;
    if(audioBufferIndex + newSampleCount > audioBufferSize) {
        return;
    }

    memcpy((uint8_t*)(audioBuffer + audioBufferIndex), pcmBuffer + PCM_BUFFER_LEN/2, PCM_BUFFER_LEN / 2);
    audioBufferIndex += newSampleCount;

    if(audioBufferIndex >= audioBufferSize) {
        eventQueue.call(&onBufferFull);
    }
}

void startRecording() {
    int error;
    uint32_t micState;

    error = BSP_AUDIO_IN_GetState(AUDIO_INSTANCE, &micState);
    if(error != BSP_ERROR_NONE) {
        printf("get audio state error %d\n", error);
        return;
    }
    if(micState == AUDIO_IN_STATE_RECORDING) {
        printf("already recording\n");
        return;
    }

    audioBufferIndex = 0;

    error = BSP_AUDIO_IN_Record(AUDIO_INSTANCE, pcmBuffer, PCM_BUFFER_LEN);
    if(error != BSP_ERROR_NONE) {
        printf("audio record error %d\n", error);
        return;
    }
    printf("started recording\n");
    led = 1;
}

int32_t initMicrophone() {
    BSP_AUDIO_Init_t micParams;
    micParams.BitsPerSample = 16;
    micParams.ChannelsNbr = AUDIO_CHANNELS;
    micParams.Device = AUDIO_IN_DIGITAL_MIC1;
    micParams.SampleRate = AUDIO_SAMPLING_FREQUENCY;
    micParams.Volume = 32;

    return BSP_AUDIO_IN_Init(AUDIO_INSTANCE, &micParams);
}

int main() {
    arm_fir_init_f32(&filter, tapCount, const_cast<float32_t*>(firLpf6kCoeff), firState, blockSize);

    audioBuffer = new int16_t[audioBufferSize];
    filteredAudio = new int16_t[audioBufferSize];

    int32_t error = initMicrophone();
    if(error != BSP_ERROR_NONE) {
        printf("audio init error %d\n", error);
        return 1;
    }

    printf("Press user button to start recording\n");

    userButton.rise(eventQueue.event(&startRecording));
    eventQueue.dispatch_forever();

    return 0;
}
