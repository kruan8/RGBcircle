/*
 * RGBlibrary.c
 *
 *  Created on: 11. 11. 2015
 *      Author: priesolv
 */

#include "RGBlibrary.h"
#include "WS2812driver.h"
#include "ad_driver.h"
#include "timer.h"

#define LEDS 60
#define BUFF_SIZE (LEDS * 3)

RGB_colors_e colors[] = { c_red, c_green, c_blue, c_white_dark, c_violet, c_yellow };  // pouzita paleta barev

uint8_t g_arrRGBbuff[BUFF_SIZE];

void RGBlib_Init()
{
  Timer_Init();
	WS2812_Init();
	AD_Init();
	AD_InitOpto();
	RGBlib_Clear();
}

// postupne rozsveceni vsech LED (stirani)
void RGBlib_ColorWipe(RGB_colors_e color, uint16_t wait_ms, bool bClear)
{
  if (bClear)
  {
      RGBlib_Clear();
  }

  for (uint16_t i = 0; i < LEDS; i++)
  {
	  RGBlib_SetLED(i, color);
	  RGBlib_Show();
	  Timer_Delay_ms(wait_ms);
  }
}


//
void RGBlib_Scanner(RGB_colors_e color, uint16_t wait_ms, bool bReturn)
{
  RGBlib_Clear();
  for(uint16_t i = 0; i < LEDS; i++)
  {
	  RGBlib_SetColorAll(c_black, 0);
	  RGBlib_SetLED(i, color);
	  RGBlib_Show();
	  Timer_Delay_ms(wait_ms);
  }

  if (bReturn)
  {
    for(int16_t i = LEDS; i > 0; i--)
    {
      RGBlib_SetColorAll(c_black, 0);
      RGBlib_SetLED(i - 1, color);
      RGBlib_Show();
      Timer_Delay_ms(wait_ms);
    }
  }

  RGBlib_SetColorAll(c_black, 0);
}

// rozsviti kazdy n-ty bod
void RGBlib_FillWithSpace(RGB_colors_e color, uint16_t nSpace, uint16_t nWait_ms)
{
  for (uint16_t i = 0; i < LEDS; i++)
  {
    if (i % nSpace == 0)
    {
      RGBlib_SetLED(i, color);
      RGBlib_Show();
    }

    RGBlib_Delay_ms(nWait_ms);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t RGBlib_Wheel(uint8_t nWheelPos)
{
	if (nWheelPos < 85)
	{
		return RGBlib_GetColorFromRGB(nWheelPos * 3, 255 - nWheelPos * 3, 0);
	}
	else if (nWheelPos < 170)
	{
		nWheelPos -= 85;
		return RGBlib_GetColorFromRGB(255 - nWheelPos * 3, 0, nWheelPos * 3);
	}
	else
	{
		nWheelPos -= 170;
		return RGBlib_GetColorFromRGB(0, nWheelPos * 3, 255 - nWheelPos * 3);
	}
}

// ------------------------------------------------------------
void RGBlib_RotateRight(uint32_t nDelay_ms)
{
  uint32_t nColorTmp =  RGBlib_GetColor(LEDS - 1);
  for (uint32_t i = LEDS - 1; i > 0; i--)
  {
    uint32_t nColor = RGBlib_GetColor(i - 1);
    RGBlib_SetLED(i, nColor);
  }

  RGBlib_SetLED(0, nColorTmp);

  RGBlib_Show();
  RGBlib_Delay_ms(nDelay_ms);
}

void RGBlib_RotateLeft(uint32_t nDelay_ms)
{
  uint32_t nColorTmp =  RGBlib_GetColor(0);
  for (uint32_t i = 0; i < LEDS - 1; i++)
  {
    uint32_t nColor = RGBlib_GetColor(i + 1);
    RGBlib_SetLED(i, nColor);
  }

  RGBlib_SetLED(LEDS - 1, nColorTmp);

  RGBlib_Show();
  RGBlib_Delay_ms(nDelay_ms);
}

// nastavi barvu jedne LED
void RGBlib_SetLED(uint8_t position, RGB_colors_e color)
{
  if (position < LEDS)
  {
    position *= 3;
	  g_arrRGBbuff[position] = (uint8_t)(color >> 16);
	  g_arrRGBbuff[position + 1] = (uint8_t)(color >> 8);
	  g_arrRGBbuff[position + 2] = (uint8_t)color;
  }
}

// nastavi barvu a jas jedne LED
void RGBlib_SetLEDWithBrightness(uint8_t position, RGB_colors_e eColor, uint8_t nBrightness)
{
  if (position < LEDS)
  {
    uint8_t *pColor = (uint8_t*) &eColor;

    position *= 3;
    nBrightness >>= 3;
    g_arrRGBbuff[position + 2] = ((uint16_t)pColor[0] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
    g_arrRGBbuff[position + 1] = ((uint16_t)pColor[1] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
    g_arrRGBbuff[position + 0] = ((uint16_t)pColor[2] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
  }
}

uint32_t RGBlib_GetColor(uint8_t position)
{
  if (position < LEDS)
  {
    position *= 3;
	  return (g_arrRGBbuff[position] << 16) + (g_arrRGBbuff[position + 1] << 8) + g_arrRGBbuff[position + 2];
  }

  return 0;
}

void RGBlib_SetColorAll(RGB_colors_e color, uint16_t wait_ms)
{
	for (uint16_t i = 0; i < LEDS; i++)
	{
		RGBlib_SetLED(i, color);
	}

	RGBlib_Show();
	Timer_Delay_ms(wait_ms);
}

void RGBlib_Clear(void)
{
  RGBlib_SetColorAll(c_black, 0);
}

void RGBlib_WaitAndClear(uint16_t wait_ms)
{
	RGBlib_Delay_ms(wait_ms);
	RGBlib_SetColorAll(c_black, 0);
}

uint32_t RGBlib_GetColorFromRGB(uint8_t r, uint8_t g, uint8_t b)
{
	return (r << 16) + (g << 8) + b;
}

void RGBlib_Show()
{
	WS2812_Send(g_arrRGBbuff, sizeof(g_arrRGBbuff));
}

void RGBlib_SetBrightness(uint8_t nBrightness)
{
	WS2812_SetBrightness(nBrightness);
	RGBlib_Show();
}

uint8_t RGBlib_GetBrightness()
{
	return WS2812_GetBrightness();
}

uint8_t RGBlib_GetBrightnessMax()
{
	return WS2812_GetBrightnessMax();
}

uint8_t RGBlib_GetBrightnessValue(uint8_t nBrightness)
{
  return WS2812_GetBrightnessValue(nBrightness);
}

uint16_t RGBlib_GetLedsCount()
{
	return LEDS;
}

uint32_t RGBlib_Rand(uint32_t nMin, uint32_t nMax)
{
  if (nMin == nMax)
  {
    return nMin;
  }

  uint32_t rnd = AD_GetRandomNumber();
  rnd = (rnd % (nMax + 1 - nMin)) + nMin;
  return rnd;
}

RGB_colors_e RGBlib_GetRandomColor()
{
	uint8_t c = RGBlib_Rand(1, sizeof(colors) / sizeof (RGB_colors_e));
	return colors[--c];
}

void RGBlib_Delay_ms(uint32_t delay_ms)
{
  Timer_Delay_ms(delay_ms);
}

uint32_t RGBlib_GetTicks(void)
{
  return Timer_GetTicks_ms();
}

bool RGBlib_IsDark(void)
{
  if (AD_GetCfgPin())
  {
    return true;
  }

  uint32_t nOpto = AD_GetOpto_mV();

  // rozhodovaci uroven bude 0,5V
  //
  if (nOpto < 80)
  {
    return true;
  }

  return false;
}

