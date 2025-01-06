// /Users/houser/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude -v -V -patmega328p -carduino "-P/dev/cu.usbserial-840" -b115200

#include <FastLED.h>
 
// How many leds in your strip?
#define NUM_LEDS        144
#define LED_DATA_PIN    A0
#define BRIGHTNESS      32

#define NUM_PADS         9
#define LEDS_PER_PANEL  16

#define PAD_1           2 //
#define PAD_2           3 //
#define PAD_3           4 //
#define PAD_4           5 //
#define PAD_5           6 //
#define PAD_6           7
#define PAD_7           8
#define PAD_8           9
#define PAD_9           10  // 
                    
int pad_enabled[NUM_PADS]  = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                                                          // input pin for buttons
int button_pin[NUM_PADS]  = {PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9};
int button[NUM_PADS]      = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // current button state
int button_last[NUM_PADS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // previous button state
CRGB leds[NUM_LEDS];                                      // Array of LEDs for FastLED
 
void read_buttons() {
  // for (int b = 0; b < 1; b++) {
  for (int b = 0; b < NUM_PADS; b++) {
    if (pad_enabled[b]) {
      button_last[b] = button[b];
      button[b] = digitalRead(button_pin[b]);
    }
  }
}

void light_pad(int pad, int value) {
  int led_start = pad * LEDS_PER_PANEL;
  int led_end   = (pad + 1) * LEDS_PER_PANEL;

  CRGB color = CRGB::White;
  if (value == HIGH) {
    color = CRGB::Black;
  }

  for(int i = led_start; i < led_end; i++) {
    leds[i] = color;
  }
}

void light_pads() {
  for (int b = 0; b < NUM_PADS; b++) {
    if (pad_enabled[b] && (button[b] != button_last[b])) {
      char buffer[50];
      sprintf(buffer, "%d:%d", b, button[b]);
      Serial.println(buffer);

      light_pad(b, button[b]);
    }
  }

  FastLED.show();
}

void setup() { 
  Serial.begin(115200);
  Serial.println("Starting...");

  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  for (int b = 0; b < NUM_PADS; b++) {
    if (pad_enabled[b]) {
      Serial.print("Enable pad ");
      Serial.println(b+1);

      light_pad(b, LOW);
      FastLED.show();

      pinMode(button_pin[b], INPUT_PULLUP);
      button[b] = digitalRead(button_pin[b]);
      button_last[b] = button[b];

      delay(111);
      light_pad(b, HIGH);
      FastLED.show();
    }
  }

  Serial.println("Ready!");
}

void loop() { 
  read_buttons();
  light_pads();
}





/*

void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i].nscale8(250);
  } 
}


    if (lastButtonState == HIGH) {
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show(); 
      delay(25);
    } else {
      // First slide the led in one direction
      for(int i = 0; i < NUM_LEDS; i++) {
          // Set the i'th led to red 
          leds[i] = CHSV(hue++, 255, 255);
          // Show the leds
          FastLED.show(); 
          // now that we've shown the leds, reset the i'th led to black
          // leds[i] = CRGB::Black;
          fadeall();
          // Wait a little bit before we loop around and do it again
          delay(25);
      }
  
      // Now go in the other direction.  
      for(int i = (NUM_LEDS)-1; i >= 0; i--) {
          // Set the i'th led to red 
          leds[i] = CHSV(hue++, 255, 255);
          // Show the leds
          FastLED.show();
          // now that we've shown the leds, reset the i'th led to black
          // leds[i] = CRGB::Black;
          fadeall();
          // Wait a little bit before we loop around and do it again
          delay(25);
      }
    }
    */
