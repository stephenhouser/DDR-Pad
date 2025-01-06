#include <Arduino.h>
#include <FastLED.h>

// number of pads/squares
#define NUM_PADS 9
// number of LEDs on each pad
#define LEDS_PER_PANEL 16
#define NUM_LEDS (NUM_PADS * LEDS_PER_PANEL)

// Overall brightness
#define BRIGHTNESS 128
// Pin to use to control LEDs
#define LED_DATA_PIN A0

// Pins to use for button inputs
#define PAD_1 2
#define PAD_2 3
#define PAD_3 4
#define PAD_4 5
#define PAD_5 6
#define PAD_6 7
#define PAD_7 8
#define PAD_8 9
#define PAD_9 10

#define ON 	1
#define OFF 0

/* Enable or disable pads; 0 disabled, 1 enabled */
int pad_enabled[NUM_PADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
/* Pins for button inputs for each pad */
int button_pin[NUM_PADS] = {PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9};
/* current button state; 0=off, 1=on */
int button[NUM_PADS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/* pervious button state; 0=off, 1=on */
int button_last[NUM_PADS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/* array of LED colors for FastLED */
CRGB leds[NUM_LEDS];

/* Reads all button inputs, sets button[] based on read values. */
void read_buttons() {
    for (int b = 0; b < NUM_PADS; b++) {
        if (pad_enabled[b]) {
            button_last[b] = button[b];

			/* buttons are active low, with a pullup, invert so 1=on, 0=off*/
            button[b] = !digitalRead(button_pin[b]);
        }
    }
}

/* Change state of LEDs for specific pad, value=0 for off, 1 for on (white) */
void light_pad(int pad, int value) {
	/* compute which LEDs to turn on. */
	int led_start = pad * LEDS_PER_PANEL;
	int led_end = (pad + 1) * LEDS_PER_PANEL;

	CRGB color = (value == OFF) ? CRGB::Black : CRGB::White;
	for (int i = led_start; i < led_end; i++) {
		leds[i] = color;
	}
}

/* Light up pads based on button states; on=lit, off=unlit. */
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

/* Arduino setup, initialize serial, FastLED, and button pins.
 * Play short sequence to light up all pads briefly.
 */
void setup() {
	Serial.begin(115200);
	Serial.println("Starting...");

	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

	for (int b = 0; b < NUM_PADS; b++) {
		if (pad_enabled[b]) {
			Serial.print("Enable pad ");
			Serial.println(b + 1);

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

/* Arduino loop, read buttons and light up pads. */
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
