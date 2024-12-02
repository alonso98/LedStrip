#define NUM_LEDS 81
#define LED_PIN 13
#define INTERRUPT_PIN 3

#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 3 

#include <microLED.h>

LEDdata leds[NUM_LEDS];  // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, NUM_LEDS, LED_PIN);  // объект лента



volatile boolean switchMode = false;
volatile uint32_t debounce;
int mode = 0; // available values: 0, 1




void setup() {
  Serial.begin(9600);

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(INTERRUPT_PIN == 3 ? 1 : 0, greenBtnClick_switchMode, CHANGE);
//  switchOnFunc_buildingOnSides();
//  blinkStripe(3, 200);
}

void greenBtnClick_switchMode() {
  if (millis() - debounce >= 100 && digitalRead(INTERRUPT_PIN)) {
    Serial.println("Interrupt");
    debounce = millis();
    switchMode = true;
  }
}

void loop() {
//  strip.clear();
//  strip.show();

  int intensity = analogRead(0);
  int brightness = map(intensity, 0, 1023, 32, 1);
  strip.setBrightness(brightness * 8 - 1);

  if (switchMode) {
    switchMode = false;
    mode = mode == 0 ? 1 : 0;
  }

  switch(mode){
    case 0: lightWhite(); break;
    case 1: rainbowEffect(); break;
  }
//breathing();
//  lightWhite();
//  delay(20);
//  switchOnFunc_buildingOnSides();
//  blinkStripe(3, 200);
}

void switchOnFunc_buildingOnSides() {
  boolean built = false;
  while(!built) {
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      if (i != NUM_LEDS / 2 && strip.getColorHEX(NUM_LEDS / 2 - i) != 0) {
        if(i == 0) {
          built = true;
        }

        break;
      }

      if (i != 0) {
        leds[NUM_LEDS / 2 - i + 1] = mRGB(0, 0, 0);
        leds[NUM_LEDS / 2 + i - 1] = mRGB(0, 0, 0);
      }

      leds[NUM_LEDS / 2 - i] = mCOLOR(WHITE);
      leds[NUM_LEDS / 2 + i] = mCOLOR(WHITE);
      strip.show();
      delay(10);
    }

    strip.show();
    delay(100);
  }
}



// effects
void lightWhite() {
//  strip.setBrightness(255);
  strip.clear();
  strip.fill(mCOLOR(WHITE));
  strip.show();
}


void rainbowEffect() {
  static byte counter = 0;
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = mHSV(counter + i * (255 / NUM_LEDS), 255, 255);
  }
  counter += 1;
  strip.show();
  delay(20);
}

void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 5;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip.setBrightness(bright);
}


// others
void blinkStripe(int count, int delaySecs) {
  for(int i = 0; i < count; i++) {
    strip.fill(mCOLOR(WHITE));
    strip.show();
    delay(delaySecs);
    strip.clear();
    strip.show();
    delay(delaySecs);
  }
}
