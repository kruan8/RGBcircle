#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include "RGBlibrary.h"

void Eff_EffectsLoop();

void Eff_Stars(RGB_colors_e color, uint32_t nDuration_ms);
void Eff_ColorWhipe();
void Eff_ColorWipeCenter(RGB_colors_e eColor);
void Eff_RotateLed(RGB_colors_e eColor);
void Eff_TheaterChase(RGB_colors_e color, uint8_t cycles, uint8_t space, uint16_t wait_ms);
void Eff_Rainbow(uint8_t cycles, uint16_t wait_ms);
void Eff_RainbowCycle(uint8_t cycles, uint16_t wait_ms);
void Eff_TheaterChaseRainbow(uint16_t wait_ms);
void Eff_TheaterChaseTwoColorRotate(RGB_colors_e eColor1, RGB_colors_e eColor2);
void Eff_Detonate(RGB_colors_e color, uint16_t nStartDelay_ms);
void Eff_Fade(RGB_colors_e color);
void Eff_Tears();
void Eff_Tears2(RGB_colors_e color);
void Eff_SpeedRotateLed();

void Eff_Candle(RGB_colors_e color, uint32_t nDuration_ms);

void Eff_Test();

#endif /* EFFECTS_H_INCLUDED */
