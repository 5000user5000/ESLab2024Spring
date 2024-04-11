#include "mbed.h"
#include "stm32l4xx_hal.h"

using events::EventQueue;

constexpr uint32_t tim1Prescaler = 8000 - 1;
constexpr uint32_t tim1Period = 10000 - 1;

TIM_HandleTypeDef hTim1;
ADC_HandleTypeDef hAdc1;
static EventQueue eventQueue(32*EVENTS_EVENT_SIZE);
volatile int counter = 0;

void onError() {
    __disable_irq();
    while(true);
}

void onTimerReload() {
    const uint16_t value = HAL_ADC_GetValue(&hAdc1);
    printf("%d: %d\n", counter++, value);
}

// @Override
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hAdc) {
    eventQueue.call(onTimerReload);
}

void onAdc1Irq() {
    HAL_ADC_IRQHandler(&hAdc1);
}

// @Override
void HAL_ADC_MspInit(ADC_HandleTypeDef* hAdc) {
    if(hAdc->Instance == ADC1) {
        __HAL_RCC_ADC_CLK_ENABLE();

        NVIC_SetVector(ADC1_2_IRQn, reinterpret_cast<uint32_t>(&onAdc1Irq));
        HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    }
}

// @Override
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hAdc) {
    if(hAdc->Instance == ADC1) {
        __HAL_RCC_ADC_CLK_DISABLE();

        HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
    }
}

void initAdc1() {
    hAdc1.Instance = ADC1;
    hAdc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hAdc1.Init.Resolution = ADC_RESOLUTION_12B;
    hAdc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hAdc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hAdc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hAdc1.Init.LowPowerAutoWait = DISABLE;
    hAdc1.Init.ContinuousConvMode = DISABLE;
    hAdc1.Init.NbrOfConversion = 1;
    hAdc1.Init.DiscontinuousConvMode = DISABLE;
    hAdc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T1_TRGO;
    hAdc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hAdc1.Init.DMAContinuousRequests = DISABLE;
    hAdc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    hAdc1.Init.OversamplingMode = DISABLE;

    HAL_StatusTypeDef halStatus;
    halStatus = HAL_ADC_Init(&hAdc1);
    if(halStatus != HAL_OK) onError();
    halStatus = HAL_ADCEx_Calibration_Start(&hAdc1, ADC_SINGLE_ENDED);
    if(halStatus != HAL_OK) onError();

    ADC_MultiModeTypeDef multiMode = {0};
    multiMode.Mode = ADC_MODE_INDEPENDENT;
    halStatus = HAL_ADCEx_MultiModeConfigChannel(&hAdc1, &multiMode);
    if(halStatus != HAL_OK) onError();

    ADC_ChannelConfTypeDef channelConfig = {0};
    channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    channelConfig.Rank = ADC_REGULAR_RANK_1;
    channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    channelConfig.SingleDiff = ADC_SINGLE_ENDED;
    channelConfig.OffsetNumber = ADC_OFFSET_NONE;
    channelConfig.Offset = 0;
    halStatus = HAL_ADC_ConfigChannel(&hAdc1, &channelConfig);
    if(halStatus != HAL_OK) onError();
}

void onTim1Irq() {
    HAL_TIM_IRQHandler(&hTim1);
}

// @Override
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* hTim) {
    if(hTim->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
}

// @Override
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* hTim) {
    if(hTim->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_DISABLE();

        HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
    }
}

void initTim1() {
    hTim1.Instance = TIM1;
    hTim1.Init.Prescaler = tim1Prescaler;
    hTim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTim1.Init.Period = tim1Period;
    hTim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hTim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_StatusTypeDef halStatus;
    halStatus = HAL_TIM_Base_Init(&hTim1);
    if(halStatus != HAL_OK) onError();

    TIM_ClockConfigTypeDef clockSourceConfig = {0};
    clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    halStatus = HAL_TIM_ConfigClockSource(&hTim1, &clockSourceConfig);
    if(halStatus != HAL_OK) onError();

    TIM_MasterConfigTypeDef masterConfig = {0};
    masterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    halStatus = HAL_TIMEx_MasterConfigSynchronization(&hTim1, &masterConfig);
    if(halStatus != HAL_OK) onError();
}

int main() {
    initTim1();
    initAdc1();

    HAL_ADC_Start_IT(&hAdc1);
    HAL_TIM_Base_Start_IT(&hTim1);

    eventQueue.dispatch_forever();

    return 0;
}
