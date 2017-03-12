#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9
#define NUM_PIXELS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  // Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// Fill the dots one after the other with a color
void colorTravel(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    int prev_i = i - 1;
    if (prev_i < 0) {
      prev_i = NUM_PIXELS;
    }
    strip.setPixelColor(prev_i, strip.Color(1, 1, 1));
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(strip.numPixels() - 1, strip.Color(1, 1, 1));
  strip.show();
}

void fillPercent(uint32_t color, uint8_t percent) {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
    if (i < percent) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void loop() {
  // rainbowCycle(30);
  colorTravel(strip.Color(0, 0, 50), 30);
  // colorTravel(strip.Color(0, 255, 0), 30);
  // colorTravel(strip.Color(0, 0, 255), 30);
  delay(30);
}
