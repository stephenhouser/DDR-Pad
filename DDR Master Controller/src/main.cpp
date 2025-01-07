#include <Arduino.h>
#include <Keyboard.h>
#include <Bounce2.h>
#include <FastLED.h>

// number of pads/squares
#define NUM_PADS 		9
// number of LEDs on each pad
#define LEDS_PER_PANEL 	16
#define NUM_LEDS		(NUM_PADS * LEDS_PER_PANEL)

// Overall brightness
#define BRIGHTNESS 		128
// Pin to use to control LEDs
#define LED_DATA_PIN 	0

// Pins to use for button inputs
#define PAD_1 			2
#define PAD_2 			3
#define PAD_3 			4
#define PAD_4 			5
#define PAD_5 			6
#define PAD_6 			7
#define PAD_7 			8
#define PAD_8 			9
#define PAD_9 			10

/* Enable or disable pads; 0 disabled, 1 enabled */
int pad_enabled[NUM_PADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
/* Pins for button inputs for each pad */
int button_pin[NUM_PADS] = {PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9};
Bounce2::Button button[NUM_PADS];

#if defined(USB_KEYBOARDONLY)
int button_keys[NUM_PADS] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};
#endif

/* array of LED colors for FastLED */
CRGB leds[NUM_LEDS];

void setup_buttons() {
	Serial.print("Initialize buttons...");
    for (int b = 0; b < NUM_PADS; b++) {
        if (pad_enabled[b]) {
			button[b].attach(button_pin[b], INPUT_PULLUP);
			button[b].interval(5);
			button[b].setPressedState(LOW);
		}
	}

	Serial.println(" done.");
}

/* Reads all button inputs, sets button[] based on read values. */
void read_buttons() {
    for (int b = 0; b < NUM_PADS; b++) {
        if (pad_enabled[b]) {
			button[b].update();
        }
    }
}
/* Change state of LEDs for specific pad, value=0 for off, 1 for on (white) */
void light_pad(int pad, int value) {
	/* compute which LEDs to turn on. */
	int led_start = pad * LEDS_PER_PANEL;
	int led_end = (pad + 1) * LEDS_PER_PANEL;

	for (int i = led_start; i < led_end; i++) {
		leds[i] = value ? CRGB::White : CRGB::Black;
	}
}

/* Set up LED pads, really just lights up each one for testing. */
void setup_pads() {
	Serial.print("Initialize pads...");

	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

	for (int b = 0; b < NUM_PADS; b++) {
		Serial.print(b);
		Serial.print(" ");

		if (pad_enabled[b]) {
			light_pad(b, 1);
			FastLED.show();

			delay(111);
			light_pad(b, 0);
			FastLED.show();
		}
	}
	Serial.println(" done.");
}

/* Light up pads based on button states; on=lit, off=unlit. */
void light_pads() {
	for (int b = 0; b < NUM_PADS; b++) {
		if (pad_enabled[b] && button[b].changed()) {
			light_pad(b, button[b].isPressed());
		}
	}

	FastLED.show();
}

void send_keys() {
#if defined(USB_KEYBOARDONLY)
	for (int b = 0; b < NUM_PADS; b++) {
		if (pad_enabled[b]) {
			if (button[b].fallingEdge()) {
				Keyboard.press(button_keys[b]);
			} else {
				Keyboard.release(button_keys[b]);
			}
		}
	}
#endif
}

/* Arduino setup, initialize serial, FastLED, and button pins.
 * Play short sequence to light up all pads briefly.
 */
void setup() {
	Serial.begin(9600);
	Serial.println("Starting...");

	setup_pads();
	setup_buttons();

	Serial.println("Ready!");
}

/* Arduino loop, read buttons and light up pads. */
void loop() {
	read_buttons();
	light_pads();
	send_keys();
}