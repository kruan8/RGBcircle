#include "effects.h"

uint32_t g_nLeds;

void Eff_EffectsLoop()
{
  g_nLeds = RGBlib_GetLedsCount();

//  while (1)
//  {
//    Eff_TheaterChaseTwoColorRotate(c_red, c_blue, 10, 100);
//
////    Eff_Tears2(c_red);
////    Eff_Tears();
//
//  }



  uint32_t rnd = RGBlib_Rand(1, 16);
  switch (rnd)
  {
    case  1: Eff_ColorWhipe(); break;
    case  2: RGBlib_ColorWipeCenter(c_blue, 150); break;
    case  3: RGBlib_ColorWipeCenter(c_white_dark, 120); break;
    case  4: Eff_Rainbow(10, 10); break;
    case  5: Eff_SpeedRotateLed(); break;
    case  6: Eff_TheaterChase(RGBlib_GetRandomColor(), 30, 3, 150); break;
    case  7: Eff_Stars(RGBlib_GetRandomColor(), 20000); break;
    case  8: Eff_Fade(RGBlib_GetRandomColor()); break;
    case  9: Eff_TheaterChaseTwoColor(c_red, c_blue, 10, 500); break;
    case 10: Eff_TheaterChaseTwoColor(c_yellow, c_blue, 10, 500); break;
    case 11: Eff_TheaterChaseTwoColorRotate(c_yellow, c_blue, 10, 100); break;
    case 12: Eff_TheaterChaseTwoColorRotate(c_red, c_blue, 10, 100); break;
    case 13: Eff_RainbowCycle(5, 10); break;
    case 14: Eff_Detonate(c_green, 600); break;
    case 15: break;
    case 16: break;
    case 17: break;
    default: break;
  }
}

void Eff_ColorWhipe()
{
  RGBlib_ColorWipe(c_red, 80, true);
	RGBlib_ColorWipe(c_blue, 80, false);
	RGBlib_ColorWipe(c_green, 80, false);
	RGBlib_ColorWipe(c_black, 80, false);
	RGBlib_Delay_ms(1000);
}

void Eff_Tears2(RGB_colors_e eColor)
{
  const uint8_t nTearLength = 6;
  const uint8_t arrBrightness[] = { 31, 25, 20, 15, 10, 5};
  for (uint8_t cycles = 0; cycles < 5; cycles++)
  {
    for (uint8_t i = 0; i < g_nLeds / 2; i++)
    {
      RGBlib_Clear();
      for (uint8_t nLength = 0; nLength < nTearLength; nLength++)
      {
        RGBlib_SetLEDWithBrightness(i - nLength, eColor, arrBrightness[nLength]);
        if (nLength == i)
        {
          break;
        }
      }

      RGBlib_Show();
      RGBlib_Delay_ms(100);
    }

    // vysunout ven
    for (uint8_t nLength = 0; nLength < nTearLength; nLength++)
    {
      for (uint8_t nPos = 0; nPos < nTearLength; nPos++)
      {
        uint32_t nColor = RGBlib_GetColor(g_nLeds / 2 - 2 - nPos);
        RGBlib_SetLED(g_nLeds / 2 - 1 - nPos, nColor);
      }

      RGBlib_Show();
      RGBlib_Delay_ms(100);
    }

    RGBlib_Delay_ms(RGBlib_Rand(300, 1000));
  }
}

void Eff_Tears()
{
	for (uint8_t cycles = 0; cycles < 5; cycles++)
	{
		for (uint8_t i = 0; i < g_nLeds / 2; i++)
		{
      RGBlib_SetLED(i, c_red);
      RGBlib_SetLED(g_nLeds - 1 - i, c_red);
      RGBlib_Show();
      RGBlib_Delay_ms(50);
      RGBlib_SetLED(i, c_black);
      RGBlib_SetLED(g_nLeds - 1 - i, c_black);
		}

		RGBlib_Delay_ms(RGBlib_Rand(300, 1000));
	}

}

//Theatre-style crawling lights.
void Eff_TheaterChase(RGB_colors_e color, uint8_t cycles, uint8_t space, uint16_t wait_ms)
{
  RGBlib_Clear();
  while (cycles--)
  {
    for (uint8_t q = 0; q < space; q++)
    {
      for (uint8_t i = 0; i < g_nLeds; i += space)
      {
        RGBlib_SetLED(i + q, color);    //turn every third pixel on
      }

      RGBlib_Show();
      RGBlib_Delay_ms(wait_ms);

      for (uint8_t i = 0; i < g_nLeds; i += space)
      {
        RGBlib_SetLED(i + q, c_black);        //turn every third pixel off
      }
    }
  }
}

// stridani dvou barev
void Eff_TheaterChaseTwoColor(RGB_colors_e color1, RGB_colors_e color2, uint8_t cycles, uint16_t wait_ms)
{
  RGBlib_Clear();
  while (cycles--)
  {
    for (uint8_t i = 0; i < g_nLeds; i += 2)
    {
      RGBlib_SetLED(i, color1);
      RGBlib_SetLED(i + 1, color2);
    }

    RGBlib_Show();
    RGBlib_Delay_ms(wait_ms);

    for (uint8_t i = 0; i < g_nLeds; i += 2)
    {
      RGBlib_SetLED(i, color2);
      RGBlib_SetLED(i + 1, color1);
    }

    RGBlib_Show();
    RGBlib_Delay_ms(wait_ms);
  }
}

void Eff_TheaterChaseTwoColorRotate(RGB_colors_e color1, RGB_colors_e color2, uint8_t cycles, uint16_t wait_ms)
{
  RGBlib_Clear();
  while (cycles--)
  {
    for (uint8_t space = 0; space < RGBlib_GetLedsCount() / 2; space++)
    {
      for (uint8_t i = 0; i < g_nLeds; i += (g_nLeds / 2))
      {
          RGBlib_SetLED(i + space, color1);
          RGBlib_SetLED(i + 3 - space, color2);
      }

      RGBlib_Show();
      RGBlib_Delay_ms(wait_ms);
      RGBlib_Clear();
    }
  }
}

// //Theatre-style crawling lights with rainbow effect
void Eff_TheaterChaseRainbow(uint16_t wait_ms)
{

  for (uint8_t j = 0; j < 256; j++) // cycle all 256 colors in the wheel
  {
    for (uint8_t q = 0; q < 3; q++)
    {
      for (uint8_t i = 0; i < RGBlib_GetLedsCount(); i = i + 3)
      {
        RGBlib_SetLED(i + q, RGBlib_Wheel((i + j) % 255));    //turn every third pixel on
      }

      RGBlib_Show();
      RGBlib_Delay_ms(wait_ms);

      for (uint8_t i = 0; i < g_nLeds; i = i + 3)
      {
        RGBlib_SetLED(i + q, 0);        //turn every third pixel off
      }
    }
  }
}

void Eff_Rainbow(uint8_t cycles, uint16_t wait_ms)
{
  RGBlib_Clear();
  for (uint16_t j = 0; j < 256 * cycles; j++)
  {
    for (uint16_t i = 0; i < g_nLeds; i++)
    {
      RGBlib_SetLED(i, RGBlib_Wheel((i + j) & 255));
    }

    RGBlib_Show();
    RGBlib_Delay_ms(wait_ms);
  }
}

void Eff_RainbowCycle(uint8_t cycles, uint16_t wait_ms)
{
  RGBlib_Clear();
  for (uint16_t j = 0; j < 256 * cycles; j++)
  {
    for (uint16_t i = 0; i < g_nLeds; i++)
    {
      RGBlib_SetLED(i, RGBlib_Wheel(((i * 256 / g_nLeds) + j) & 255));
    }

    RGBlib_Show();
    RGBlib_Delay_ms(wait_ms);
  }

  RGBlib_Clear();
}

void Eff_Fade(RGB_colors_e color)
{
  RGBlib_SetBrightness(0);
  RGBlib_SetColorAll(color, 0);
  for (uint8_t nBrightness = 0; nBrightness <= RGBlib_GetBrightnessMax(); nBrightness++)
  {
    RGBlib_SetBrightness(nBrightness);
    RGBlib_Delay_ms(100);
  }

  RGBlib_Delay_ms(500);

  for (uint8_t nBrightness = RGBlib_GetBrightnessMax() - 1; nBrightness > 0; nBrightness--)
  {
    RGBlib_SetBrightness(nBrightness + 1);
    RGBlib_Delay_ms(100);
  }

  RGBlib_SetColorAll(c_black, 0);
  RGBlib_SetBrightness(RGBlib_GetBrightnessMax());
  RGBlib_Delay_ms(500);
}

// --------------------- DETONATE --------------------------------------------------
void Eff_Detonate(RGB_colors_e color, uint16_t nStartDelay_ms)
{
  while (nStartDelay_ms)
  {
    RGBlib_SetColorAll(color, 0);   // Flash the color
    RGBlib_Delay_ms(6);
    RGBlib_Clear();
    RGBlib_Delay_ms(nStartDelay_ms);
    nStartDelay_ms = (nStartDelay_ms * 10) / 11;    // delay between flashes is halved each time until zero
  }

  RGBlib_SetColorAll(color, 0);
  RGBlib_Delay_ms(1000);

  // Then we fade to black....
  for (uint16_t nBrightness = RGBlib_GetBrightnessMax(); nBrightness > 0; nBrightness--)
  {
    RGBlib_SetBrightness(nBrightness);
    RGBlib_Delay_ms(100);
  }

  RGBlib_SetColorAll(c_black, 500);
  RGBlib_SetBrightness(RGBlib_GetBrightnessMax());
}

void Eff_Stars(RGB_colors_e color, uint32_t nDuration_ms)
{
  uint8_t arrBrightness[g_nLeds];
  uint8_t nMaxBrigntness = RGBlib_GetBrightnessMax();
  const uint8_t nCutDown = 0x80;
  uint32_t nStartTime = RGBlib_GetTicks();

  for (uint8_t i = 0; i < g_nLeds; i++)
  {
    arrBrightness[i] = 0;
  }

  while (RGBlib_GetTicks() < nStartTime + nDuration_ms)
  {
    for (uint8_t i = 0; i < g_nLeds; i++)
    {
      uint8_t nBrightness = arrBrightness[i];
      if (nBrightness == 0)
      {
        uint8_t nRnd = RGBlib_Rand(1, 50);
        if (nRnd == 1)
        {
          nBrightness = 1;
        }
      }
      else
      {
        // zmena jasu
        if (nBrightness & nCutDown)
        {
          nBrightness--;
          if (nBrightness == nCutDown)
          {
            nBrightness = 0;
          }
        }
        else
        {
          nBrightness++;
          if (nBrightness == nMaxBrigntness)
          {
            nBrightness |= nCutDown;
          }
        }
      }

      arrBrightness[i] = nBrightness;
      RGBlib_SetLEDWithBrightness(i, color, nBrightness & ~nCutDown);
    }

    RGBlib_Show();
    RGBlib_Delay_ms(5);
  }
}

void Eff_SpeedRotateLed()
{
    for (uint8_t i = 110; i > 0; i -= 10)  // zrychlujici scanner
    {
      RGBlib_Scanner(c_green, i, false);
    }

    for (uint8_t w = 0; w < 12; w++)     // drzet
    {
      RGBlib_Scanner(c_green, 10, false);
    }

    for (uint8_t w = 0; w < 12; w++)     // drzet
    {
      RGBlib_Scanner(c_red, 10, false);
    }

    for (uint8_t w = 0; w < 12; w++)     // drzet
    {
      RGBlib_Scanner(c_blue, 10, false);
    }

    for (uint8_t i = 10; i < 90; i += 10)  // zpomalujici scanner
    {
        RGBlib_Scanner(c_blue, i, false);
    }

    RGBlib_Delay_ms(2000);
}

void Eff_Candle(RGB_colors_e color)
{
  uint8_t nPwmCtrl = 0;		// 4 bit-Counter
  uint8_t nFrameCtrl = 0;	// 5 bit-Counter

  uint8_t PWM_VAL = 0;		// 4 bit-Register
  uint8_t NEXTBRIGHT = 0;	// 4 bit-Register
  uint8_t RAND = 0;			  // 5 bit Signal
  uint8_t randflag = 0;		// 1 bit Signal

  while(1)
  {
    //_delay_us(1e6/440/16);   // Main clock=440*16 Hz
//    Delay_us(142);

 		// PWM
 		nPwmCtrl++;
 		nPwmCtrl &= 0xf;		// only 4 bit

 		if (nPwmCtrl <= PWM_VAL)
 		{
 		  RGBlib_SetColorAll(color, 0);
    }
    else
    {
      RGBlib_SetColorAll(c_black, 0);
    }

 		// FRAME
 		if (nPwmCtrl == 0)
 		{
 			nFrameCtrl++;
 			nFrameCtrl &= 0x1f;

 			if ((nFrameCtrl & 0x07) == 0)  // generate a new random number every 8 cycles. In reality this is most likely bit serial
 			{
 				RAND = RGBlib_Rand(0, 31);
 				if ((RAND & 0x0c)!=0)
        {
          randflag = 1;
        }
        else
        {
          randflag = 0;// only update if valid
        }
 			}

			// NEW FRAME
 			if (nFrameCtrl == 0)
 			{
 				PWM_VAL = NEXTBRIGHT; // reload PWM
 				randflag = 1;		    // force update at beginning of frame
 			}

 			if (randflag)
 			{
 				NEXTBRIGHT = RAND > 15 ? 15 : RAND;
 			}
 		}
  }
}

void Eff_Test()
{
	RGBlib_Delay_ms(1000);
	RGBlib_SetColorAll(c_red, 1000);
	RGBlib_SetColorAll(c_green, 1000);
	RGBlib_SetColorAll(c_blue, 1000);
	RGBlib_Clear();
	RGBlib_Delay_ms(1000);
}
