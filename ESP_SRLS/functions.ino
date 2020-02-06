void vuMeterMiddle() {
  level = map(spectrumValue[0], 0, 255, 0, NUM_LEDS / 2);
  for (int i = NUM_LEDS / 2; i < (NUM_LEDS / 2) + level; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  for (int i = level; i < NUM_LEDS; i ++) {
    leds[i] = CRGB::Black;
  }

  for (int i = NUM_LEDS / 2; i > (NUM_LEDS / 2) - level; i--) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  for (int i = (NUM_LEDS / 2) - level; i > 0; i --) {
    leds[i] = CRGB::Black;
  }
}

void vuMeterSeven() {
  int lenght = NUM_LEDS / 7;
  for (int i = 0; i < map(spectrumValue[0], 0, 200, 0, lenght); i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[0], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[1], 0, 200, 0, lenght); i++) {
    leds[i + lenght] = CRGB::Blue;
  }
  FastLED.show();

  for (int i = map(spectrumValue[1], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[2], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 2] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[2], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 2] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[3], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 3] = CRGB::Blue;
  }
  FastLED.show();

  for (int i = map(spectrumValue[3], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 3] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[4], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 4] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[4], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 4] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[5], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 5] = CRGB::Blue;
  }
  FastLED.show();

  for (int i = map(spectrumValue[5], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 5] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[6], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 6] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[6], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 6] = CRGB::Black;
  }

}

void vuMeterFour() {
  int lenght = 7;
  int d = 100;
  for (int i = 0; i < map(spectrumValue[0], 0, 200, 0, lenght); i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[0], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[2], 0, 200, 0, lenght); i++) {
    leds[i + lenght] = CRGB::Blue;
  }
  FastLED.show();

  for (int i = map(spectrumValue[2], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[3], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 2] = CRGB::White;
  }
  FastLED.show();

  for (int i = map(spectrumValue[3], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 2] = CRGB::Black;
  }


  for (int i = 0; i < map(spectrumValue[5], 0, 200, 0, lenght); i++) {
    leds[i + lenght * 3] = CRGB::Blue;
  }
  FastLED.show();

  for (int i = map(spectrumValue[5], 0, 200, 0, lenght); i < lenght; i ++) {
    leds[i + lenght * 3] = CRGB::Black;
  }
}



void fullStripRGB() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(spectrumValue[1], spectrumValue[3], spectrumValue[5]);
  }
  FastLED.show();
}



int spectrum = 50;
void fullStripHSV() {
  int range = 5;

  int nullLevel = 50;
  for (int i = 0; i < NUM_LEDS; i++) {
    int color = ((spectrumValue[1] / range) + (spectrumValue[3] / range) + (spectrumValue[5] / range) + spectrum);
    leds[i] = CHSV(color, 255, spectrumValue[1] + nullLevel);
  }
  FastLED.show();
  int hueInterval = 100;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > hueInterval) {
    previousMillis = currentMillis;
    spectrum++;
  }
}

int averageAll() {
  int avg = (spectrumValue[0] + spectrumValue[1] + spectrumValue[2] + spectrumValue[3] + spectrumValue[4] + spectrumValue[5] + spectrumValue[6]) / 7;
  Serial.print("AvgAll: ");
  Serial.print("\t");
  Serial.println(avg);
  return avg;
}

int averageThree() {
  int avg = (spectrumValue[1]+ spectrumValue[3] + spectrumValue[5]) / 3;
  Serial.print("AvgThree: ");
  Serial.print("\t");
  Serial.println(avg);
  return avg;
}


int highest() {
  int highest = -1;
  int index = -1;
  for (int i = 0; i < 6; i++ ) {
    if (spectrumValue[i] > highest) {
      index = i;
    }
  }
  
  return index;
}

int colorSelector(int num) {
  if (num == 0) {
    return 0;
  }
  if (num == 1) {
    return 72 ;
  }
  if (num == 2) {
    return 108;
  }
  if (num == 3) {
    return 144;
  }
  if (num == 4) {
    return 180;
  }
  if (num == 5) {
    return 216;
  }
  if (num == 6) {
    return 254;
  }
}
