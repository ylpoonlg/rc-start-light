#include "config.h"

#include <Adafruit_NeoPixel.h>

// Steps between STEP_STOP and STEP_GO represent the countdown steps
#define STEP_STOP 0
#define STEP_GO   (NUM_STEPS + 1)

// Define LED strips
Adafruit_NeoPixel strip_0(NUM_PIXELS, STRIP0_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_1(NUM_PIXELS, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

/**
 * Returns true if reset button is triggered.
 */
bool is_reset_btn() {
  return !digitalRead(RST_BTN_PIN);
}

/**
 * Wrapper for clear.
 */
void led_clear() {
  strip_0.clear();
  strip_1.clear();
}

/**
 * Wrapper for show.
 */
void led_show() {
  strip_0.show();
  strip_1.show();
}

/**
 * Wrapper for setPixelColor.
 */
void led_set_color(uint16_t pixel, uint32_t color) {
  strip_0.setPixelColor(pixel, color);
  strip_1.setPixelColor(pixel, color);
}

/**
 * Set led strips state for a given step.
 *
 * The value of step is within [STEP_STOP, STEP_GO].
 */
void set_led_state(uint32_t step) {
  led_clear();

#if PATTERN_STYLE == 0
  if (step == STEP_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_GREEN);
    }
  } else if (step == STEP_STOP) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_RED);
    }
  } else {
    const float grp_size = (float)((int)(NUM_PIXELS / 2)) / NUM_STEPS;
    for (int i = floor((step - 1) * grp_size); i <= NUM_PIXELS / 2; i++) {
      led_set_color(i, COLOR_AMBER);
      led_set_color(NUM_PIXELS - i - 1, COLOR_AMBER);
    }
  }
#else
  if (step == STEP_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_GREEN);
    }
  } else if (step == STEP_STOP) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_RED);
    }
  } else {
    const float grp_size = (float)NUM_PIXELS / NUM_STEPS;
    const int px_len = ceil((NUM_STEPS - step + 1) * grp_size);
    for (int i = 0; i < px_len; i++) {
      led_set_color(i, COLOR_AMBER);
    }
  }
#endif

  led_show();
}

void do_reset() {
  for (int i = 1; i <= NUM_STEPS; i++) {
    set_led_state(i);
    delay(COUNT_INTERVAL);
  }

  set_led_state(STEP_GO);
  delay(COUNT_INTERVAL * 2);
  set_led_state(STEP_STOP);
}

void setup() {
  pinMode(RST_BTN_PIN, INPUT_PULLUP);

  strip_0.begin();
}

void loop() {
  set_led_state(STEP_STOP);
  if (is_reset_btn()) {
    do_reset();
  }

  delay(50);
}
