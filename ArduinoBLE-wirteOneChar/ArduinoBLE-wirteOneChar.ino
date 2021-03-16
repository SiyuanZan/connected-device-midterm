/*
  p5.js control panel linkhttps://editor.p5js.org/sz2971/sketches/jXqumcokH
  reference:
  This example code is in the public domain.
  https://editor.p5js.org/sz2971/sketches/jXqumcokH
*/


#include <FastLED.h>
#include <ArduinoBLE.h>

BLEService ledService("19b10000-e8f2-537e-4f6c-d104768a1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic switchCharacteristic("19b10000-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);
BLEIntCharacteristic switchCharacteristic2("6fe109b3-636e-4781-ae3b-dfc8a88b082c", BLERead | BLEWrite);
BLEBooleanCharacteristic switchCharacteristic3("fc8ce101-a19e-4088-8ebc-7465880d131c", BLERead | BLEWrite);




#define NUM_LEDS 99
#define DATA_PIN 12
#define COLOR_ORDER GRB
#define LED_TYPE WS2812

#define generateInterval  100
#define generateChance 80

uint8_t max_bright = 128;
uint8_t a;
CRGB leds[NUM_LEDS];
int  colorIndex;
int  LEDIndex;
bool  ifAutoPlay;
bool  ifPlay;

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
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  while (!Serial);
  delay(1000);

  FastLED.setBrightness(max_bright);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(switchCharacteristic2);
  ledService.addCharacteristic(switchCharacteristic3);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  switchCharacteristic2.writeValue(0);
  switchCharacteristic3.writeValue(false);


  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {






  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      int pos = random8(NUM_LEDS);
      static unsigned long lastRefreshTime = 0;

      if (millis() - lastRefreshTime >= generateInterval) {

        lastRefreshTime = millis();
        if ( random8() < generateChance) {


          leds[pos] = ColorFromPalette(myColorPalette, random8(255), 128, LINEARBLEND);

        }

      }

      if (ifAutoPlay) {

        int pos2 = random8(NUM_LEDS);
        static unsigned long lastRefreshTime2 = 0;

        if (millis() - lastRefreshTime2 >= 2000) {

          lastRefreshTime2 = millis();
          if ( random8() < 150) {

            leds[pos2] = ColorFromPalette(myRGBColorPalette, random8(255), 60, LINEARBLEND);

          }

          a = getPos(pos2);


        }
      }

      static unsigned long lastRefreshTime3 = 0;
      if (millis() - lastRefreshTime3 >= 50) {

        lastRefreshTime3 = millis();


        fadeToBlackBy( leds + a , 1, 10);


      }
      FastLED.show();
      delay(50);

      if (switchCharacteristic3.written()) {

        ifAutoPlay = switchCharacteristic3.value();
        Serial.println(ifAutoPlay);
        ifPlay = getifAutoPlay(ifAutoPlay);
      }

      if (switchCharacteristic.written() || switchCharacteristic2.written()) {



        colorIndex = switchCharacteristic.value();
        Serial.print(colorIndex);
        LEDIndex = switchCharacteristic2.value();
        Serial.print(LEDIndex);
        int pos = map(colorIndex, 0, 7, 0, 255);
        leds[0] = CRGB::Black;

        if (ifAutoPlay != true) {
          leds[LEDIndex] = ColorFromPalette(myRGBColorPalette, pos, 128, LINEARBLEND);

          a = getPos(LEDIndex);
          FastLED.show();
        }



      }

    }


    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }






}

int getPos(int pos) {
  return pos;
}

bool getifAutoPlay(bool a) {
  return a;
}
