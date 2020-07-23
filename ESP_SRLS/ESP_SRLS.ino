//#define FASTLED_INTERNAL

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 300

#define DATA_PIN D2

#define BRIGHTNESS  255

#define updateLEDS 1

CRGB leds[NUM_LEDS];

#define analogPin A0
#define strobePin D5
#define resetPin D7

//int analogPin = 0; // MSGEQ7 OUT
//int strobePin = 14; //D5 MSGEQ7 STROBE 
//int resetPin = 13; //D7 MSGEQ7 RESET
int spectrumValue[7];

// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 80;

int thres = 100;

int level = 0;

long previousMillis = 0;
int interval = 30;
int pos = 0;

void setup()
{
  delay(2000);
  Serial.begin(9600);

  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  // Set analogPin's reference voltage
  //analogReference(DEFAULT); // 5V

  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  for (int i = 0; i < NUM_LEDS ; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}


void loop()
{
  // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle

    spectrumValue[i] = analogRead(analogPin);

    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);

    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);

    // Remove serial stuff after debugging
    //    Serial.print(spectrumValue[i]);
    //    Serial.print("\t");
    digitalWrite(strobePin, HIGH);
  }
  //  Serial.println();

  //  fullStripHSV();
  //  beatDetection();

  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis > interval) {
  //    previousMillis = currentMillis;
  //    if (spectrumValue[0] > thres) {
  //      fill_solid(leds, NUM_LEDS, CHSV(hue, 255, spectrumValue[0]));
  //      FastLED.show();
  //    }
  //    else {
  //      fill_solid(leds, NUM_LEDS, CRGB::Black);
  //      FastLED.show();
  //    }
  //  }



  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis > interval) {
  //    previousMillis = currentMillis;
  //    leds[pos] = CRGB::Red;
  //    FastLED.show();
  //    leds[pos] = CRGB::Black;
  //    if (pos < NUM_LEDS) {
  //      pos++;
  //    }
  //    else {
  //      pos = 0;
  //    }
  //  }

  //  Update the leds - shift them to the right
  for (int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - updateLEDS];
  }


  //  Add the colors to the beginning of the array
  int currentAvg = averageAll();
  int currentHigh = highest();
  int blackThres = 120;
  int range = 2;
  int spectrum = 100;

  if (currentAvg > blackThres) {
    for (int i = 0; i < updateLEDS; i++) {
      leds[i] = CHSV((currentAvg*range) + spectrum, 255, 255);
    }
    FastLED.show();
  }
  else {
    for (int i = 0; i < updateLEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }


  //delay(1);

}
