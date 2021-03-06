/*
 * ad_driver.h
 *
 *  Created on: 20. 11. 2018
 *  Author:     Priesol Vladimir
 */

#ifndef AD_DRIVER_H_
#define AD_DRIVER_H_

#include "stm32f0xx.h"
#include <stdbool.h>

void AD_Init(void);
void AD_InitOpto(void);
uint32_t AD_GetRandomNumber(void);
uint32_t AD_GetOpto_mV(void);
bool AD_GetCfgPin(void);
void AD_SleepMode(void);

#endif /* AD_DRIVER_H_ */
