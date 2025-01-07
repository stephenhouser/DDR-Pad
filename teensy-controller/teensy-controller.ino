#include <Keyboard.h>
#include <Bounce2.h>
#include <FastLED.h>

/* 
 * *** *** 
 * - Make sure to select Tools -> USB Type -> Serial + Keyboard + ...
 *
 * - use Github master for teensy cores to patch FastLED hanging on setup,
 *   in /Users/houser/Library/Arduino15/packages/teensy/hardware/avr/1.59.0:
 *   `git clone git@github.com:PaulStoffregen/cores.git`
 *   Refer to: https://forum.pjrc.com/index.php?threads/is-fastled-ws2812serial-broken-in-teensyduino-1-59.74548/page-2
 * *** **
 */

// number of pads/squares
#define NUM_PADS 	    	9
// number of LEDs on each pad
#define LEDS_PER_PANEL 	16
#define NUM_LEDS		    (NUM_PADS * LEDS_PER_PANEL)

// Overall brightness
#define BRIGHTNESS 		96
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
int pad_state[NUM_PADS]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Pins for button inputs for each pad */
int button_pin[NUM_PADS] = {PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9};
Bounce2::Button button[NUM_PADS];

int button_keys[NUM_PADS] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};

/* array of LED colors for FastLED */
CRGB leds[NUM_LEDS];

void set_pad(int pad, CRGB color) {
  if (0 <= pad && pad < NUM_PADS) {
    /* compute which LEDs to turn on. */
    int led_start = pad * LEDS_PER_PANEL;
    if (led_start < NUM_LEDS) {
      int led_end = min(NUM_LEDS, (pad + 1) * LEDS_PER_PANEL);

      pad_state[pad] = (color != CRGB::Black);

      for (int i = led_start; i < led_end; i++) {
        leds[i] = color;
        // Serial.printf("# LED %d=%d\n", i, leds[i]);
      }

      FastLED.show();
    }
  }
}

void toggle_pad(int pad) {
  set_pad(pad, pad_state[pad] ? CRGB::Black : CRGB::White);
}

void report_pad(int pad) {
  if (0 <= pad && pad < NUM_PADS) {
    int led_start = pad * LEDS_PER_PANEL;
    Serial.printf("%d=%d (%d)", pad+1, pad_state[pad], leds[led_start]);
    Serial.println();
  }
}

/* Arduino setup, initialize serial, FastLED, and button pins.
 * Play short sequence to light up all pads briefly.
 */
void setup() {
	Serial.begin(115200);

  delay(250);
	Serial.println("# Starting...");

	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

  // light up pads and setup buttons
	for (int b = 0; b < NUM_PADS; b++) {
		if (pad_enabled[b]) {
    	Serial.print("# Pad ");
    	Serial.println(b+1);

			delay(100);
			set_pad(b, CRGB::White);

			button[b].attach(button_pin[b], INPUT_PULLUP);
			button[b].interval(5);
			button[b].setPressedState(LOW);

			delay(250);
			set_pad(b, CRGB::Black);
    }
	}

	Serial.println("# Ready!");
}

/* Arduino loop, read buttons and light up pads. */
void loop() {
  for (int b = 0; b < NUM_PADS; b++) {
    if (pad_enabled[b]) {
      button[b].update();

      if (button[b].changed()) {
        if (button[b].fell()) {
          set_pad(b, CRGB::White);
          Keyboard.press(button_keys[b]);
        } else {
          set_pad(b, CRGB::Black);
          Keyboard.release(button_keys[b]);
        }

        report_pad(b);
      }
    }
  }

  if (Serial.available()) {
    int i = Serial.parseInt();
    if (0 < i && i <= NUM_PADS && pad_enabled[i]) {
      int pad = i - 1;
      Serial.printf("# Toggle %d", i);
      Serial.println();

      toggle_pad(pad);
      report_pad(pad);
    }
  }
}