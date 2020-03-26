// Wash Your Hands Timer - Craig Rettew - March 26, 2020
// COVID-19 has reminded us how to wash our hands again
// the standard rule seems to be around 30s
// The Wash Your Hands Timer gives a visual ~30 timer so you know when
// you're supposed to be done washing your hands.
// There's an IR sensor to start the timer and badazzling at the end
// Codes codes codes codez

// Best addressabe LED library this side of the Mississippi
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 28

// Data/Clock Pins
#define DATA_PIN 0
#define CLOCK_PIN 2

// IR Sensor Pin
#define SENSORPIN 4

// Set brightneess level of LEDS
#define BRIGHTNESS          100

// How long should the LEDs badazzle when timer is complete
#define BADAZZLETIME 3000 //ms
#define TIMERTIME 600000
// Define the array of leds
CRGB leds[NUM_LEDS];

// variables for keeping track of counters and such
int pixelFill = 0;
int lastPixel = 0;
int badazzleTimer = 0;

// rotating "base color" used by many of the patterns
uint8_t gHue = 0; 

// badazzle function
void badazzle() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

// actual timer function which is called with the IR sensor interrupt
void startTimer(){
    // do the timer stuff as long as the pixel strip isn't filled
    while(pixelFill <= NUM_LEDS) {
      // the second timer to count up by the second
      delayMicroseconds(TIMERTIME);
      // loop to "send" a pixel down the strip
      for (int i=0; i <= NUM_LEDS-1-pixelFill; i++){
        // fill it with a random color
        leds[i] = CHSV( gHue, 255, 192); 
        FastLED.show();
        delayMicroseconds(10000);
        // shut that shit off
        leds[i] = CRGB::Black;
        FastLED.show();
        // get the last pixel in the loop
        lastPixel = i;
      }

      // loop to keep the last pixel illuminated
      for (int p = lastPixel; p <= NUM_LEDS-1; p++)  {
        leds[p] = CHSV( gHue, 255, 192);
      }  

      // turn it on
      FastLED.show();
      // increment the "fill"
      pixelFill++;
    }
    // Once the whole strip is filled, reset the variable
    pixelFill = 0;

    // once the timer is complete, do the badazzle for while with differnt colors
    while (badazzleTimer <= BADAZZLETIME)  {
      badazzle();
      FastLED.show();
      delayMicroseconds(1000);
      gHue++;
      badazzleTimer++;
    }

    // reset everything for next time and shut it down
    badazzleTimer = 0;
    gHue = 0;
    fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    FastLED.show();
}

void setup() {
  // define the IR sensor input and make it a pullup
  pinMode(SENSORPIN, INPUT_PULLUP);
  // setting the LED strip parameters, I used a DOTSTAR
  FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);

  // clear the strip
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  FastLED.show();

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // create the IR Sensor pin as an interrupt on the Falling edge and call the startTimer ISR
  attachInterrupt(digitalPinToInterrupt(SENSORPIN), startTimer, FALLING);
}

void loop() {
  // this just changes the gHue value in this loop to get some rando colors when it's ready
  EVERY_N_MILLISECONDS( 10 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}
