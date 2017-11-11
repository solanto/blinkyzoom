#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(147, PIN, NEO_GRB + NEO_KHZ800);

int arrayIndex = 0; // max 255
int reds [255];
int greens [255];
int blues [255];
int locations[255];

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across 
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  addColorStop(129, 255, 64, 0);
  addColorStop(255, 244, 43, 89);
  addColorStop(255, 177, 64, 153);
  addColorStop(255, 7, 99, 255);

  Serial.begin(9600);

  Serial.println("Color stops:");
  for (int i = 0; i < arrayIndex; i++) {
    Serial.println(String(i) + " at " + String(locations[i]) + ": " + String(reds[i]) + ", " + String(greens[i]) + ", " + String(blues[i]));
  }
  Serial.println("~");
  Serial.println("");

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  sendToStrip(0, 10, false);
}

void loop() {

}



void sendToStrip(int begin, int end, boolean reversed) {
  int length = end - begin;
  int r[length];
  int g[length];
  int b[length];

  int ibegin;
  int iend;

  if (!reversed) {
    ibegin = 0;
    iend = arrayIndex;
  } else {
    ibegin = arrayIndex;
    iend = 0;
  }

  for (int i = ibegin; i < iend - 1; i++) {
    int loc = linterp(locations[i], 0, 255, 0, length);
    int nextLoc = linterp(locations[i + 1], 0, 255, 0, length);
    Serial.println(String(loc) + " to " + String(nextLoc));
    r[loc] = reds[i];
    g[loc] = greens[i];
    b[loc] = blues[i];

    Serial.println(String(i) + ": " + String(r[loc]) + ", " + String(g[loc]) + ", " + String(b[loc]));

    for (int j = loc; j < nextLoc; ++j) {
      r[j] = linterp(j, loc, nextLoc, r[loc], reds[i + 1]);
      g[j] = linterp(j, loc, nextLoc, g[loc], greens[i + 1]);
      b[j] = linterp(j, loc, nextLoc, b[loc], blues[i + 1]);

      Serial.println(String(i) + ": " + String(r[j]) + ", " + String(g[j]) + ", " + String(b[j]));
    }
  }

  for (int i = begin; i <= end; i++) {
    int loc = i - begin;
    strip.setPixelColor(i, r[loc], g[loc], b[loc]);
  }
}

void addColorStop(int r_, int g_, int b_, int location_) {
  reds[arrayIndex] = r_;
  greens[arrayIndex] = g_;
  blues[arrayIndex] = b_;
  locations[arrayIndex++] = location_;
}


int linterp(int val, int smin, int smax, int min, int max) {
  return(min + (max - min) * ((val - smin) / (smax - smin)));
}
