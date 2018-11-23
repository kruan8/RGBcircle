#include "effects.h"
#include "timer.h"
#include <string.h>

typedef struct
{
  uint8_t nPwmValue;    // 4 bit-Register
  uint8_t nFrameCtrl;   // 5 bit-Counter
  uint8_t nNextBright;  // 4 bit-Register
}candle_t;

uint32_t g_nLeds;

// jeste prohlednout:
// https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/RGBWstrandtest/RGBWstrandtest.ino


void Eff_EffectsLoop()
{
  g_nLeds = RGBlib_GetLedsCount();

  while (1)
  {
    RGBlib_IsDark();
  }

//  {
//    // pri svetle uspat, jednou za sekundu kontrolovat
//    return;
//  }


  while (1)
  {
      Eff_Candle(c_blue, 10000);
//    Eff_Tears2(c_red);
//    Eff_Tears();

  }

  uint32_t rnd = RGBlib_Rand(1, 16);
  switch (rnd)
  {
    case  1: Eff_ColorWhipe(); break;
    case  2: Eff_ColorWipeCenter(c_blue, 150); break;
    case  3: Eff_ColorWipeCenter(c_white_dark, 120); break;
    case  4: Eff_Rainbow(10, 10); break;
    case  5: Eff_SpeedRotateLed(); break;
    case  6: Eff_TheaterChase(RGBlib_GetRandomColor(), 30, 3, 150); break;
    case  7: Eff_Stars(RGBlib_GetRandomColor(), 20000); break;
    case  8: Eff_Fade(RGBlib_GetRandomColor()); break;
    case  9: Eff_TheaterChaseTwoColors(c_red, c_blue, 15, 1000); break;
    case 10: Eff_TheaterChaseTwoColors(c_yellow, c_blue, 15, 1000); break;
    case 11: Eff_TheaterChaseTwoColorRotate(c_yellow, c_blue, 7000); break;
    case 12: Eff_TheaterChaseTwoColorRotate(c_red, c_blue, 7000); break;
    case 13: Eff_RainbowCycle(5, 10); break;
    case 14: Eff_Detonate(RGBlib_GetRandomColor(), 600); break;
    case 15: Eff_Candle(RGBlib_GetRandomColor(), 10000);
    case 16: break;
    default: break;
  }

  RGBlib_Clear();
}

// postupne vyplneni barvou
void Eff_ColorWhipe()
{
  RGBlib_ColorWipe(c_red, 60, true);
	RGBlib_ColorWipe(c_blue, 60, false);
	RGBlib_ColorWipe(c_green, 60, false);
	RGBlib_ColorWipe(c_black, 60, false);
	RGBlib_Delay_ms(1000);
}

// postupne vyplneni barvou ze dvou stran
void Eff_ColorWipeCenter(RGB_colors_e eColor, uint16_t nWait_ms)
{
  uint8_t mid = g_nLeds / 2;
  RGBlib_SetLED(mid, eColor);
  for (uint16_t i = 0; i <= g_nLeds / 2; i++)
  {
    RGBlib_SetLED(mid + i, eColor);
    RGBlib_SetLED(mid - i, eColor);
    RGBlib_Show();
    RGBlib_Delay_ms(nWait_ms);
  }

  RGBlib_SetLED(mid, c_black);
  for (uint16_t i = 0; i <= g_nLeds / 2; i++)
  {
    RGBlib_SetLED(mid + i, c_black);
    RGBlib_SetLED(mid - i, c_black);
    RGBlib_Show();
    RGBlib_Delay_ms(nWait_ms);
  }

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
void Eff_TheaterChaseTwoColors(RGB_colors_e eColor1, RGB_colors_e eColor2, uint8_t cycles, uint16_t nWait_ms)
{
  const uint8_t nColorWidth = 5;

  while (cycles--)
  {
    uint32_t i = 0;
    while (i < g_nLeds)
    {
      for (uint8_t space = 0; space < nColorWidth; space++)
      {
        RGBlib_SetLED(i + space, eColor1);
        RGBlib_SetLED(i + nColorWidth + space, eColor2);
      }

      i += nColorWidth * 2;
    }

    RGB_colors_e eColor = eColor1;
    eColor1 = eColor2;
    eColor2 = eColor;
    RGBlib_Show();
    RGBlib_Delay_ms(nWait_ms);
  }

}

// skupiny 5led/2barvy, nejdrive stoji, pak rotuji v obou smerech
void Eff_TheaterChaseTwoColorRotate(RGB_colors_e color1, RGB_colors_e color2, uint16_t nDuration_ms)
{
  const uint8_t nColorWidth = 5;

  RGBlib_Clear();

  uint32_t i = 0;
  while (i < g_nLeds)
  {
    for (uint8_t space = 0; space < nColorWidth; space++)
    {
      RGBlib_SetLED(i + space, color1);
      RGBlib_SetLED(i + nColorWidth + space, color2);
    }

    i += nColorWidth * 2;
  }

  RGBlib_Show();
  RGBlib_Delay_ms(3000);

  uint32_t nTime = RGBlib_GetTicks();
  while (nTime + nDuration_ms > RGBlib_GetTicks())
  {
    RGBlib_RotateRight(50);
  }

  nTime = RGBlib_GetTicks();
  while (nTime + nDuration_ms > RGBlib_GetTicks())
  {
    RGBlib_RotateLeft(50);
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

// plynule rozsveceni a zhasnuti
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
/*
void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip.show();
      }
}*/


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

// nahodne rozsveceni a zhasinani (blikajici hvezdicky)
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

// rychle rotujici jedna LED
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

void Eff_Candle(RGB_colors_e eColor, uint32_t nDuration_ms)
{
  candle_t arrCandle[g_nLeds];

  uint8_t nLedBright;
  uint8_t nRand = 0;        // 5 bit Signal
  uint8_t nRandFlag = 0;    // 1 bit Signal

  uint16_t nSize = sizeof(arrCandle);
  memset (arrCandle, 0, nSize);

  uint32_t nEndTime = Timer_GetTicks_ms() + nDuration_ms;
//  while (Timer_GetTicks_ms() < nEndTime)

//  for (uint32_t i = 0; i < g_nLeds; i++)
  while(1)
  {
    // pri pouziti PWM je jeden cyklus PWM 150 us
    // sirka PWM je 16 kroku
    // bez PWM je cyklus 150*16=2400 us

 		// PWM
//    arrCandle[0].nPwmCtrl++;
//    arrCandle[0].nPwmCtrl &= 0xf;		// only 4 bit
//
// 		if (arrCandle[0].nPwmCtrl <= arrCandle[0].nPwmValue)
// 		{
// 		  RGBlib_SetLED(0, eColor);
//    }
//    else
//    {
//      RGBlib_SetLED(0, c_black);
//    }

 		// FRAME
// 		if (arrCandle[0].nPwmCtrl == 0)
// 		{

 		  arrCandle[0].nFrameCtrl++;
 		  arrCandle[0].nFrameCtrl &= 0x1f;

 			if ((arrCandle[0].nFrameCtrl & 0x07) == 0)  // generate a new random number every 8 cycles. In reality this is most likely bit serial
 			{
 				nRand = RGBlib_Rand(0, 31);
 				if ((nRand & 0x0c) != 0)
        {
          nRandFlag = 1;
        }
        else
        {
          nRandFlag = 0;// only update if valid
        }
 			}

			// NEW FRAME
 			if (arrCandle[0].nFrameCtrl == 0)
 			{
 			  nLedBright = arrCandle[0].nNextBright; // reload PWM
 				nRandFlag = 1;		    // force update at beginning of frame
 			}

 			if (nRandFlag)
 			{
 			  arrCandle[0].nNextBright = nRand > 15 ? 15 : nRand;
 			}

 			RGBlib_SetLEDWithBrightness(0, eColor, nLedBright << 1);

 			RGBlib_Show();
 			TimerUs_Delay(2400);   // zkusit prodlouzit na 50 Hz = 20 ms

// 		}
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



// ---------------------------------------------------------------------------------------------------------
// ------------- RAINBOW --------------------------------------
// vypada to, ze se vysouvaji barvy duhy (rainbowcolors) z nulove pozice a pritom se upravuje jejich jas
//#define MAXPIX 253
//#define COLORLENGTH 100
//#define FADE 5
//
//
//struct cRGB colors[8];
//struct cRGB led[MAXPIX];
//
//
//int main(void)
//{
//
//  uint8_t j = 1;
//  uint8_t k = 1;
//
//
//  DDRB|=_BV(ws2812_pin);
//
//    uint8_t i;
//    for(i=MAXPIX; i>0; i--)
//    {
//        led[i-1].r=0;led[i-1].g=0;led[i-1].b=0;
//    }
//
//    //Rainbowcolors
//    colors[0].r=150; colors[0].g=150; colors[0].b=150;
//    colors[1].r=255; colors[1].g=000; colors[1].b=000;//red
//    colors[2].r=255; colors[2].g=100; colors[2].b=000;//orange
//    colors[3].r=100; colors[3].g=255; colors[3].b=000;//yellow
//    colors[4].r=000; colors[4].g=255; colors[4].b=000;//green
//    colors[5].r=000; colors[5].g=100; colors[5].b=255;//light blue (türkis)
//    colors[6].r=000; colors[6].g=000; colors[6].b=255;//blue
//    colors[7].r=100; colors[7].g=000; colors[7].b=255;//violet
//
//  while(1)
//    {
//        //shift all vallues by one led
//        uint8_t i=0;
//        for(i=MAXPIX; i>1; i--)
//            led[i-1]=led[i-2];
//        //change colour when colourlength is reached
//        if(k>COLORLENGTH)
//        {
//            j++;
//           k=0;
//       }
//       k++;
//       //loop colouers
//       if(j>8)
//           j=0;
//
//       //fade red
//       if(led[0].r<colors[j].r)
//           led[0].r+=FADE;
//       if(led[0].r>255.r)
//           led[0].r=255;
//
//       if(led[0].r>colors[j].r)
//           led[0].r-=FADE;
//   //    if(led[0].r<0)
//   //        led[0].r=0;
//       //fade green
//       if(led[0].g<colors[j].g)
//           led[0].g+=FADE;
//       if(led[0].g>255)
//           led[0].g=255;
//
//       if(led[0].g>colors[j].g)
//           led[0].g-=FADE;
//   //    if(led[0].g<0)
//   //        led[0].g=0;
//       //fade blue
//       if(led[0].b<colors[j].b)
//           led[0].b+=FADE;
//       if(led[0].b>255)
//           led[0].b=255;
//
//       if(led[0].b>colors[j].b)
//           led[0].b-=FADE;
//   //    if(led[0].b<0)
//   //        led[0].b=0;
//
//
//     _delay_ms(10);
//     ws2812_sendarray((uint8_t *)led,MAXPIX*3);
//    }
//}


// ------------------------------------------------------------------------------------
// Theatre-style crawling lights.
// Changes spacing to be dynmaic based on string size


//#define THEATER_SPACING (PIXELS/20)
//
//
//void theaterChase( unsigned char r , unsigned char g, unsigned char b, unsigned char wait ) {
//
//  for (int j=0; j< 3 ; j++) {
//
//    for (int q=0; q < THEATER_SPACING ; q++) {
//
//      unsigned int step=0;
//
//      cli();
//
//      for (int i=0; i < PIXELS ; i++) {
//
//        if (step==q) {
//
//          sendPixel( r , g , b );
//
//        } else {
//
//          sendPixel( 0 , 0 , 0 );
//
//        }
//
//        step++;
//
//        if (step==THEATER_SPACING) step =0;
//
//      }
//
//      sei();
//
//      show();
//      delay(wait);
//
//    }
//
//  }
//
//}

//---------------------------------------------------------------------------------------------
// -------------------------- RAINBOW_CYCLE ----------------------------
// I rewrite this one from scrtach to use high resolution for the color wheel to look nicer on a *much* bigger string

//void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance ) {
//
//  // Hue is a number between 0 and 3*256 than defines a mix of r->g->b where
//  // hue of 0 = Full red
//  // hue of 128 = 1/2 red and 1/2 green
//  // hue of 256 = Full Green
//  // hue of 384 = 1/2 green and 1/2 blue
//  // ...
//
//  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string
//
//  for(unsigned int j=0; j<frames; j++) {
//
//    unsigned int currentPixelHue = firstPixelHue;
//
//    cli();
//
//    for(unsigned int i=0; i< PIXELS; i++) {
//
//      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
//        currentPixelHue -= (3*256);
//      }
//
//      unsigned char phase = currentPixelHue >> 8;
//      unsigned char step = currentPixelHue & 0xff;
//
//      switch (phase) {
//
//        case 0:
//          sendPixel( ~step , step ,  0 );
//          break;
//
//        case 1:
//          sendPixel( 0 , ~step , step );
//          break;
//
//
//        case 2:
//          sendPixel(  step ,0 , ~step );
//          break;
//
//      }
//
//      currentPixelHue+=pixelAdvance;
//
//
//    }
//
//    sei();
//
//    show();
//
//    firstPixelHue += frameAdvance;
//
//  }
//}

// --------------------------------------------------------------------------------------
// ----  dalsi kody

//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  if(WheelPos < 85) {
//   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//  } else if(WheelPos < 170) {
//   WheelPos -= 85;
//   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  } else {
//   WheelPos -= 170;
//   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//}


// -----------------------------------------------------------------------------------
// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
//uint32_t rainbowOrder(byte position)
//{
//  // 6 total zones of color change:
//  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
//  {
//    return leds.Color(0xFF, position * 8, 0);
//  }
//  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
//  {
//    position -= 31;
//    return leds.Color(0xFF - position * 8, 0xFF, 0);
//  }
//  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
//  {
//    position -= 63;
//    return leds.Color(0, 0xFF, position * 8);
//  }
//  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
//  {
//    position -= 95;
//    return leds.Color(0, 0xFF - position * 8, 0xFF);
//  }
//  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
//  {
//    position -= 127;
//    return leds.Color(position * 8, 0, 0xFF);
//  }
//  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
//  {
//    position -= 159;
//    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
//  }
//}
