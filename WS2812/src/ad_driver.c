/*
 * ad_driver.c
 *
 *  Created on: 20. 11. 2018
 *  Author:     Priesol Vladimir
 */


#include "ad_driver.h"

#define AD_OPTO_OUT_PORT      GPIOA
#define AD_OPTO_OUT_PIN       GPIO_Pin_6

#define AD_OPTO_INPUT         ADC_CHSELR_CHSEL9

#define AD_TEMP_CYCLES        8
#define AD_OPTO_CYCLES        8

void AD_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  // Initialize ADC 14MHz RC
  RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  RCC_HSI14Cmd(ENABLE);
  while (!RCC_GetFlagStatus(RCC_FLAG_HSI14RDY));

  //enable ADC1 clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  // Enable ADCperipheral
  ADC1->CR |= (uint32_t)ADC_CR_ADEN;
  while (!(ADC1->ISR & ADC_FLAG_ADRDY));

  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; //default
  ADC_Init(ADC1, &ADC_InitStructure);

  //Convert the ADC1 temperature sensor, user shortest sample time to generate most noise

  ADC1->SMPR = 0;     // ADC_SampleTime_1_5Cycles

  //enable internal temperature sensor
  ADC->CCR |= (uint32_t)ADC_CCR_TSEN;

}

void AD_InitOpto(void)
{
  GPIO_InitTypeDef InitStruct;
  GPIO_StructInit(&InitStruct);

  // pin pro TIM3_CH1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* GPIOA Configuration: TIM3 Channel 1 as alternate function push-pull */
  InitStruct.GPIO_Pin = AD_OPTO_OUT_PIN;
  InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  InitStruct.GPIO_OType = GPIO_OType_PP;
  InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(AD_OPTO_OUT_PORT, &InitStruct);

  AD_OPTO_OUT_PORT->BSRR = AD_OPTO_OUT_PIN;
}


uint32_t AD_GetRandomNumber(void)
{
  // Enable CRC clock
  RCC->AHBENR |= RCC_AHBENR_CRCEN;

  // select temp sensor input
  ADC1->CHSELR = (uint32_t)ADC_Channel_TempSensor;

  for (uint8_t i = 0; i < AD_TEMP_CYCLES; i++)
  {
    //Start ADC1 Software Conversion
    ADC1->CR |= (uint32_t)ADC_CR_ADSTART;

    //wait for conversion complete
    while (!(ADC1->ISR = (uint32_t)ADC_FLAG_EOC));

    CRC->DR = ADC1->DR;

    //clear EOC flag
//    ADC1->ISR = (uint32_t)ADC_FLAG_EOC;
  }

  CRC->DR = 0xBADA55E5;
  uint32_t nValue = CRC->DR;

  RCC->AHBENR &= ~RCC_AHBENR_CRCEN;

  return nValue;
}

uint32_t AD_GetOpto(void)
{
  ADC1->CHSELR = ADC_CHSELR_CHSEL9;

  uint32_t nValue = 0;
  for (uint8_t i = 0; i < AD_OPTO_CYCLES; i++)
  {
    //Start ADC1 Software Conversion
    ADC1->CR |= (uint32_t)ADC_CR_ADSTART;

    //wait for conversion complete
    while (!(ADC1->ISR = (uint32_t)ADC_FLAG_EOC));

    nValue += ADC1->DR;

    //clear EOC flag
//    ADC1->ISR = (uint32_t)ADC_FLAG_EOC;
  }

  nValue /= AD_OPTO_CYCLES;

  return nValue;
}
