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
#define BRIGHTNESS 		192
// Pin to use to control LEDs
#define LED_DATA_PIN 	14

// Pins to use for button inputs
#define PAD_1 			2
#define PAD_2 			3
#define PAD_3 			4
#define PAD_4 			5
#define PAD_5 			6
#define PAD_6 			7
#define PAD_7 			8
#define PAD_8 			11
#define PAD_9 			12

// Pin to use for 4 or 9 pad toggle switch
#define PAD_TOGGLE_SW	17

/* Enable or disable pads; 0 disabled, 1 enabled */
int pad_enabled[NUM_PADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
/* Pins for button inputs for each pad */
int button_pin[NUM_PADS] = {PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9};
Bounce2::Button button[NUM_PADS];
Bounce2::Button pad_toggle;

#if defined(USB_KEYBOARDONLY)
int button_keys[NUM_PADS] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};
#endif

/* array of LED colors for FastLED */
CRGB leds[NUM_LEDS];

/* Change state of LEDs for specific pad, value=0 for off, 1 for on (white) */
void light_pad(int pad, int value) {
	/* compute which LEDs to turn on. */
	int led_start = pad * LEDS_PER_PANEL;
	int led_end = led_start + LEDS_PER_PANEL;

	for (int i = led_start; i < led_end; i++) {
		leds[i] = value ? CRGB::White : CRGB::Black;
	}

	FastLED.show();
}

void light_active_pads() {
	for (int pad = 0; pad < NUM_PADS; pad++) {		
		Serial.print(pad);

		if (pad_enabled[pad]) {
			Serial.print("=enabled ");
			light_pad(pad, 1);

			delay(250);
			light_pad(pad, 0);
		} else {
			Serial.print("=disabled ");
		}

		delay(1);
	}

	Serial.println();
}

void setup_4pad() {
	int pad4[NUM_PADS] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
	for (int p = 0; p < NUM_PADS; p++) {
		pad_enabled[p] = pad4[p];
	}

	light_active_pads();
}

void setup_8pad() {
	int pad8[NUM_PADS] = {1, 1, 1, 1, 0, 1, 1, 1, 1};
	for (int p = 0; p < NUM_PADS; p++) {
		pad_enabled[p] = pad8[p];
	}

	light_active_pads();
}

void setup_9pad() {
	int pad9[NUM_PADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	for (int p = 0; p < NUM_PADS; p++) {
		pad_enabled[p] = pad9[p];
	}
	
	light_active_pads();
}


/* Set up LED pads, really just lights up each one for testing. */
void setup_pads() {
	Serial.print("Initialize pads...");

	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

	setup_9pad();
	Serial.println(" done.");
}

/* Light up pads based on button states; on=lit, off=unlit. */
void light_pads_loop() {
	for (int b = 0; b < NUM_PADS; b++) {
		if (pad_enabled[b] && button[b].changed()) {
			light_pad(b, button[b].isPressed());
		}
	}

	FastLED.show();
}

void setup_buttons() {
	Serial.print("Initialize buttons...");
    for (int pad = 0; pad < NUM_PADS; pad++) {
        if (pad_enabled[pad]) {
			button[pad].attach(button_pin[pad], INPUT_PULLUP);
			button[pad].interval(5);
			button[pad].setPressedState(LOW);
		}
	}

	Serial.println(" done.");
}

/* Reads all button inputs, sets button[] based on read values. */
void read_buttons_loop() {
    for (int pad = 0; pad < NUM_PADS; pad++) {
        if (pad_enabled[pad]) {
			button[pad].update();
        }
    }
}

void send_keys_loop() {
#if defined(USB_KEYBOARDONLY)
	for (int pad = 0; pad < NUM_PADS; pad++) {
		if (pad_enabled[pad]) {
			if (button[pad].fell()) {
				Keyboard.press(button_keys[pad]);
			} else if (button[pad].rose()) {
				Keyboard.release(button_keys[pad]);
			}
		}
	}
#else
	for (int pad = 0; pad < NUM_PADS; pad++) {
		if (pad_enabled[pad]) {
			if (button[pad].fell()) {
				Serial.printf("%d down\n", pad);
			} else if (button[pad].rose()) {
				Serial.printf("%d up\n", pad);
			}
		}
	}
#endif
}

void setup_pad_toggle() {
	pad_toggle.attach(PAD_TOGGLE_SW, INPUT_PULLUP);
	pad_toggle.interval(5);
	pad_toggle.setPressedState(LOW);
}

void pad_toggle_loop() {
	pad_toggle.update();
	if (pad_toggle.changed()) {
		if (pad_toggle.isPressed()) {
			setup_4pad();
		} else {
			setup_9pad();
		}
	}
}

/* Arduino setup, initialize serial, FastLED, and button pins.
 * Play short sequence to light up all pads briefly.
 */
void setup() {
	Serial.begin(9600);
	Serial.println("Starting...");

	setup_pads();
	setup_buttons();
	setup_pad_toggle();

	Serial.println("Ready!");
}

/* Arduino loop, read buttons and light up pads. */
int counter = 0;
int butt = 0;

void loop() {
	read_buttons_loop();
	send_keys_loop();
	light_pads_loop();

	pad_toggle_loop();

	delay(1);
	// if (counter++ == 100) {
	// 	Serial.printf("light %d\n", butt);
	// 	counter = 0;
	// 	light_pad(butt, 1);
	// 	FastLED.show();

	// 	// delay(111);
	// 	delay(250);
	// 	light_pad(butt, 0);
	// 	FastLED.show();

	// 	butt = (butt + 1) % NUM_PADS;
	// }
}