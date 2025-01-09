#include <Adafruit_NeoPixel.h>

#define LED_PIN 3
#define RST_BTN_PIN 2

#define NUM_PIXELS 20
#define GRP_SIZE 4

#define COUNT_INTERVAL 1000

#define COLOR_RED   255,   0, 0
#define COLOR_AMBER 255, 200, 0
#define COLOR_GREEN   0, 255, 0

// NUM_STEPS = ceil(floor(NUM_PIXELS / 2) / GRP_SIZE)
#define NUM_STEPS ((NUM_PIXELS / 2 + (GRP_SIZE - 1)) / GRP_SIZE)
#define STEP_STOP 0
#define STEP_GO   (NUM_STEPS + 1)

Adafruit_NeoPixel strip_0(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

bool is_reset_btn() {
  return !digitalRead(RST_BTN_PIN);
}

/* Wrapper for clear */
void led_clear() {
  strip_0.clear();
}

/* Wrapper for show */
void led_show() {
  strip_0.show();
}

/* Wrapper for setPixelColor */
void led_set_color(const int pixel, const int color_r, const int color_g,
    const int color_b) {
  strip_0.setPixelColor(pixel, strip_0.Color(color_r, color_g, color_b));
}

void set_led_state(const int step) {
  led_clear();

  if (step == STEP_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_GREEN);
    }
  } else if (step > STEP_STOP) {
    if (step == NUM_STEPS) {
      // Last step
      for (int i = 0; i < NUM_PIXELS; i++) {
        led_set_color(i, COLOR_AMBER);
      }
    } else {
      for (int i = 0; i < step * GRP_SIZE; i++) {
        led_set_color(i, COLOR_AMBER);
        led_set_color(NUM_PIXELS - i - 1, COLOR_AMBER);
      }
    }
  } else {
    for (int i = 0; i < NUM_PIXELS; i++) {
      led_set_color(i, COLOR_RED);
    }
  }

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
