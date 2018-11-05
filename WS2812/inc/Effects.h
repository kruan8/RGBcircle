#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include "RGBlibrary.h"

void Eff_RandomEffects();
void Eff_Stars(RGB_colors_e color, uint32_t nDuration_ms);
void Eff_ColorWhipe();
void Eff_TheaterChase(RGB_colors_e color, uint8_t cycles, uint8_t space, uint16_t wait_ms);
void Eff_TheaterChaseTwoColor(RGB_colors_e color1, RGB_colors_e color2, uint8_t cycles, uint16_t wait_ms);
void Eff_Rainbow(uint8_t cycles, uint16_t wait_ms);
void Eff_RainbowCycle(uint8_t cycles, uint16_t wait_ms);
void Eff_TheaterChaseRainbow(uint16_t wait_ms);
void Eff_TheaterChaseTwoColorRotate(RGB_colors_e color1, RGB_colors_e color2, uint8_t cycles, uint16_t wait_ms);
void Eff_Detonate(RGB_colors_e color, uint16_t nStartDelay_ms);
void Eff_Fade(RGB_colors_e color);
void Eff_SpeedRotateLed();
void Eff_Candle(RGB_colors_e color);

void Eff_Test();

#endif /* EFFECTS_H_INCLUDED */
