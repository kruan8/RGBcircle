/*
 * RGBlibrary.c
 *
 *  Created on: 11. 11. 2015
 *      Author: priesolv
 */

#include "RGBlibrary.h"
#include "WS2812driver.h"

#define LEDS 60
#define BUFF_SIZE (LEDS * 3)

RGB_colors_e colors[] = { c_red, c_green, c_blue, c_white_dark, c_violet };  // pouzita paleta barev

uint8_t g_arrRGBbuff[BUFF_SIZE];

void RGBlib_Init()
{
	WS2812_Init();
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
	  WS2812_Delay_ms(wait_ms);
  }
}

// color wipe from center
void RGBlib_ColorWipeCenter(RGB_colors_e color, uint16_t wait_ms)
{
  RGBlib_Clear();

	uint8_t mid = LEDS / 2;
	RGBlib_SetLED(mid, color);
	for (uint16_t i = 0; i <= LEDS / 2; i++)
	{
		RGBlib_SetLED(mid + i, color);
		RGBlib_SetLED(mid - i, color);
		RGBlib_Show();
		WS2812_Delay_ms(wait_ms);
	}

	RGBlib_SetLED(mid, c_black);
	for (uint16_t i = 0; i <= LEDS / 2; i++)
	{
		RGBlib_SetLED(mid + i, c_black);
		RGBlib_SetLED(mid - i, c_black);
		RGBlib_Show();
		WS2812_Delay_ms(wait_ms);
	}

	WS2812_Delay_ms(1000);
}

void RGBlib_Scanner(RGB_colors_e color, uint16_t wait_ms, bool bReturn)
{
  RGBlib_Clear();
  for(uint16_t i = 0; i < LEDS; i++)
  {
	  RGBlib_SetColorAll(c_black, 0);
	  RGBlib_SetLED(i, color);
	  RGBlib_Show();
	  WS2812_Delay_ms(wait_ms);
  }

  if (bReturn)
  {
    for(int16_t i = LEDS; i > 0; i--)
    {
      RGBlib_SetColorAll(c_black, 0);
      RGBlib_SetLED(i - 1, color);
      RGBlib_Show();
      WS2812_Delay_ms(wait_ms);
    }
  }

  RGBlib_SetColorAll(c_black, 0);
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

    g_arrRGBbuff[position * 3 + 2] = ((uint16_t)pColor[0] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
    g_arrRGBbuff[position * 3 + 1] = ((uint16_t)pColor[1] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
    g_arrRGBbuff[position * 3 + 0] = ((uint16_t)pColor[2] * WS2812_GetBrightnessValue(nBrightness)) >> 8;
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
	WS2812_Delay_ms(wait_ms);
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

  uint32_t rnd = WS2812_GetRandomNumber();
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
  WS2812_Delay_ms(delay_ms);
}

uint32_t RGBlib_GetTicks(void)
{
  return WS2812_GetTicks();
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
//	uint8_t j = 1;
//	uint8_t k = 1;
//
//
//	DDRB|=_BV(ws2812_pin);
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
//	while(1)
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
//		 _delay_ms(10);
//		 ws2812_sendarray((uint8_t *)led,MAXPIX*3);
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
