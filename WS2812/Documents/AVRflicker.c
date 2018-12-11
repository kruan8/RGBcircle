
/*
Here you can find my solution: http://bienonline.magix.net/public/avr-advent.html (it’s in German, Google translator can help).
I use an array of random brightness values. There are more values for higher brightness than for the lower ones.
That makes filckering more realistic (You detected this fact already.). The avr-library has a random number generator.
With this random number I select an array index for the next brightness value.
So you can get an realistic candle flickering depending on the distribution of the brightness value in the array and a long range of random values without repeating them.
Addionally I don’t switch directly from one brightness value to next. Instead I use a ramp with a random duration. The calculation of the duration is similar to that of the brightness.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include "GeneralDefinitions.h"
#include "AppVersion.h"

// LEDs auf PB 1, 2, 3, 4
// Taster PB0
#define LED1   _BV(PB1)
#define LED2   _BV(PB2)
#define LED3   _BV(PB3)
#define LED4   _BV(PB4)
#define Button _BV(PB0)
#define PCINT  PCINT0  // muss zu Button passen!
#define Reset  _BV(PB5)
#define Delay  5       // Zeit, mit der die Rampe abgearbeitet wird


void delay(void)
{ _delay_ms(Delay);
}

volatile int16_t Brightness[4]; // Brightness, Helligkeit
int16_t TargetBrightness[4];    // Ziel-Helligkeit; B bewegt sich in Richting D mit Schrittweite S
int16_t BrightnessValues[] = {7, 15, 31, 63, 127, 255, 25, 48, 77, 99, 190, 229, 150, 55, 255, 255}; // mögliche Ziel-Helligkeiten
  
int16_t Slope[4]; // Slope, aktuell eingestellte Rampe
int16_t SlopeValues[4] = {1, 2, 4, 8}; // mögliche Rampen

uint8_t LED[4] = {LED1, LED2, LED3, LED4}; // Pins der LEDs
  
volatile uint8_t Lights = 1; // Anzahl Lichter, brennen
uint8_t LightsMask[] = {0, LED1, LED1 | LED3, LED1 | LED2 | LED3, LED1 | LED2 | LED3 | LED4}; // Maske zum codieren der Lichter
  
void NewValues(uint8_t i) // Neue Ziel-Helligkeit und Rampe festlegen
{ uint8_t k = rand() & 0xFF;
  TargetBrightness[i] = BrightnessValues[k & 0x0F] >> 2; // Helligkeit regulieren
  if (Brightness[i] < TargetBrightness[i]) // steigende oder fallende Rampe notwendig?
    Slope[i] = +SlopeValues[(k & 0x30)>>4];
  else
    Slope[i] = -SlopeValues[(k & 0x30)>>4];
} 

int main(void)
{ DDRB  = LED1 | LED2 | LED3 | LED4;  // LED-Pins auf output
  PORTB = Button | Reset; //Pull-Up für Taste und Reset
  
  TCCR0B |= _BV(CS00);  // no Prescaling
  TIMSK  |= _BV(TOIE0); // Interrupt freigeben
  
  GIMSK = _BV(PCIE);    // PIN-Change-Interrupt für Button freigeben
  PCMSK = _BV(PCINT); 
  GIFR  = _BV(PCIF);    // Clear Interrupt
  
  for (uint8_t i = 0; i < 4; i++)
  { NewValues(i);
  }
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  sleep_enable();
  
  sei();
  
  while(1)
  { for (uint8_t i = 0; i < 4; i++) // Rampe für jede LED durchlaufen
    { Brightness[i] += Slope[i];
      if (((Slope[i] > 0) && (Brightness[i] > TargetBrightness[i])) // Ende der Rampe?
       || ((Slope[i] < 0) && (Brightness[i] < TargetBrightness[i])))
      { NewValues(i); 
      }
    } // for
  
    delay();
    if (!Lights) // Alle Lichter aus: Schlafen gehen
       sleep_cpu();
  }// while
}

ISR(TIMER0_OVF_vect) // PWM
{ static uint8_t cnt = 0; // Läuft immer wieder von 0..255
  uint8_t ctrl = 0;       // Sammelt, welche LEDs aktuell leuchten sollen
  
  cnt++;
  for (uint8_t i = 0; i < 4; i++)
  {  if(cnt < Brightness[i]) // cnt < Ziel-Helligkeit: LED an. Große Ziel-Helligkeit = lange Anschaltphase
      ctrl |= LED[i];
  }
  PORTB = (PORTB & ~(LED1 | LED2 | LED3 | LED4)) // LEDs maskieren
             | (ctrl & LightsMask[Lights]);  // LEDs an
}

ISR(PCINT0_vect, ISR_NOBLOCK) // damit der Timer weiterläuft
{ _delay_ms(10);
  if (!(PINB & Button)) // Taste ist gedrückt
    if (++Lights > 4)
       Lights = 0;
}
