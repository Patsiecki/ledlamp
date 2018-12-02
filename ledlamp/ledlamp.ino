
// Neopixel library and pins
#include <Adafruit_NeoPixel.h>
#define PIN      6
#define N_LEDS 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Define Arduino pins
const int numModes = 5; //number of possible light modes
const int potPin = A0; //pin A0 to read analog input
const int buttonPin = 2;

// other constants
const int buttonPause = 500; //length of pause before allowing next button press

// RGB variables for static color modes
int red;
int green;
int blue;

// RGB variables for randomized colors
int randr = 0;
int randg = 0;
int randb = 0;

// Special variables
float potval; // variable for potentiometer value
float pwr = .01; // pot value converted into a decimal
float flickerpwr = .01; // dimming multiplier used in candle function
int lightPhase = 0; // variable for changing light modes or turning off
int buttonState = 0;
int flickerDelay = 0; // randomized delay that will be timed against millis() in the candleflicker function


// Timing variables
unsigned long curTime = 0; //current timing in millisenconds
unsigned long prevButtonTime = 0; //last time the button was pressed
unsigned long lastFlicker = 0; //last time candle mode flickered


// ########## SETUP ##########
void setup() {
  strip.begin();
  pinMode(potPin, INPUT);
  pinMode(buttonPin, INPUT);
}

// ########## FUNCTIONS ##########

// buttonRead() will READ BUTTON STATE
void buttonRead() {
  if (millis() - prevButtonTime >= buttonPause) {
    // buttonState = digitalRead(buttonPin);
    if (digitalRead(buttonPin) == HIGH) {
      prevButtonTime = millis();
      if (lightPhase < numModes) {
        lightPhase ++; //move to the next mode
      } else {
        // turn lamp to off mode
        lightPhase = 0;
      }
    }
  }
}

// turnoff() turns off the strip
void turnoff() {
  for (uint16_t n = 0; n < strip.numPixels() + 2; n++) {
    strip.setPixelColor(n, 0, 0, 0);
    strip.show();
  }
}

// solidlight(r, g, b) sets the strip to a single, steady color
void solidlight(int red, int green, int blue) {
  for (uint16_t n = 0; n < strip.numPixels() + 2; n++) {
    strip.setPixelColor(n, red * pwr, green * pwr, blue * pwr);
    strip.show();
  }
}

// candleflicker() flickers leds to simluate candle light. pair with solidlight() function to define color of LED strip
void candleflicker() {
  if (millis() - lastFlicker >= flickerDelay) {
    strip.setBrightness(flickerpwr);
    strip.show();
    // delay(random(100, 150));
    lastFlicker = millis();
    flickerDelay = random(100, 150);
    flickerpwr = random(150, 250);
  }
}

// popomode simulates a police siren light
void popomode() {
  for (uint16_t n = 0; n < strip.numPixels() + 2; n++) {
    if (n >= (strip.numPixels() / 2)) {
      strip.setPixelColor(n, 250 * pwr, 0, 0); // set red pixel
    }
    else {
      strip.setPixelColor(n, 0, 0, 250 * pwr); // set blue pixel
    }
    delay(65);
    strip.setPixelColor(n - 2, 0); // shut off 2 pixels back
    strip.show();
    buttonRead();
  }

}




// ########## START PROGRAM ##########
void loop() {

  // Generating random values
  randr = random(250);
  randg = random(250);
  randb = random(250);


  // defining potentiometer variable pwr
  potval = analogRead(potPin);
  potval = map(potval, 0, 1023, 100, 0); // it may be desirable to reverse this mapping depending potentiometer wiring
  pwr = (potval / 100); // converts the integer pot value into a decimal

  curTime = millis(); //start recording time

  buttonRead(); // run the button reading function

  // Off mode, also default
  if (lightPhase == 0) {
    turnoff();
  }

  // Neutral / Daylight
  if (lightPhase == 1) {
    solidlight(250, 250, 250);
  }

  // Warm / Soft White
  if (lightPhase == 2) {
    solidlight(250, 140, 30);
  }

  // Candle mode
  if (lightPhase == 3) {
    solidlight(250, 100, 0);
    candleflicker();
  }

  //popo mode
  if (lightPhase == 4) {
    popomode();
  }

  //Random Colors
  if (lightPhase == 5) {
    for (uint16_t n = 0; n < strip.numPixels() + 1; n++) {
      strip.setPixelColor(n, random(250) * pwr, random(250) * pwr, random(250) * pwr);
      strip.show();
      buttonRead();
      delay(50);
    }
  }




  //END
}
