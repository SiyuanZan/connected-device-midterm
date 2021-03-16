#include <FastLED.h>
#define NUM_LEDS 99
#define DATA_PIN 12
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

#define generateInterval  100
#define generateChance 80

uint8_t max_bright = 128;
uint8_t a;
CRGB leds[NUM_LEDS];

CHSVPalette16 myColorPalette =  CHSVPalette16 (
                                  CHSV(140, 28, 56),  CHSV(139, 21, 70),   CHSV(6, 80, 30),    CHSV(170, 52, 81),
                                  CHSV(239, 23, 44),    CHSV(215, 57, 60),     CHSV(212, 56, 63),    CHSV(227, 74, 75),
                                  CHSV(259, 64, 67),  CHSV(248, 62, 71),  CHSV(250, 44, 76),  CHSV(249, 49, 78),
                                  CHSV(10, 74, 53),    CHSV(11, 63, 63),    CHSV(357, 43, 59),   CHSV(6, 80, 30));
CHSVPalette16 myColorPalette2 =  CHSVPalette16 (
                                   CHSV(230, 92, 26),  CHSV(239, 66, 35),   CHSV(230, 92, 26),  CHSV(239, 66, 35),
                                   CHSV(12, 75, 53),    CHSV(12, 100, 47),  CHSV(12, 75, 53),    CHSV(12, 100, 47),
                                   CHSV(104, 43, 27),  CHSV(140, 94, 45),  CHSV(104, 43, 27),  CHSV(140, 94, 45),
                                   CHSV(286, 41, 37),    CHSV(300, 79, 45), CHSV(286, 41, 37),    CHSV(300, 79, 45));

CRGBPalette16 myRGBColorPalette = CRGBPalette16(
                                    CRGB(116, 56, 143), CRGB(206, 24, 206), CRGB(116, 56, 143),  CRGB(206, 24, 206),
                                    CRGB(5, 25, 127), CRGB(30, 31, 149), CRGB(5, 25, 127),  CRGB(30, 31, 149),
                                    CRGB(225, 81, 44), CRGB(242, 50, 0), CRGB(225, 81, 44),  CRGB(242, 50, 0),
                                    CRGB(55, 98, 39), CRGB(7, 255, 81),  CRGB(55, 98, 39),  CRGB(7, 255, 81)
                                  );

void setup() {
  Serial.begin(9600);
  delay(1000);

  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
}

void loop() {
  int pos = random8(NUM_LEDS);



  EVERY_N_MILLISECONDS( generateInterval ) {
    if ( random8() < generateChance) {

      leds[pos] = ColorFromPalette(myColorPalette, random8(255), 120, LINEARBLEND);

    }

  }

  int pos2 = random8(NUM_LEDS);
  EVERY_N_SECONDS( 2 ) {

    if ( random8() < 100) {

      leds[pos2] = ColorFromPalette(myRGBColorPalette, random8(255), 80, LINEARBLEND);



    }

    a = getPos(pos2);
  }

  EVERY_N_MILLISECONDS( 50 ) {

    if (a) {

      fadeToBlackBy( leds + a, 1, 10);

    }

  }



  FastLED.show();
  delay(50);
}

int getPos(int pos) {
  return pos;
}
