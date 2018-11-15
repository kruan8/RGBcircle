/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "RGBlibrary.h"
#include "effects.h"
#include <string.h>


/*
 *  Detonate: náhodna barva
 *  ColorWhipeCenter: mazat obracene
 *  TheaterChaseTwoColor:  asi zrusit, stejne jako rotate
 *  TheaterChaseTwoColorRotate: pole 4/4, 5 sekund stat a pak rotovat (rotovani neni napsany)
 *
 *
 */


int main(void)
{
	// kontrola hodin
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks); // Get system clocks

	RGBlib_Init();

  // test svitivosti vsech barev
//	Eff_Test();

//  while (1)
//  {
//    Candle(COLOR_YELLOW);
//  }

	while (1)
	{
	  Eff_EffectsLoop();
	}
}

