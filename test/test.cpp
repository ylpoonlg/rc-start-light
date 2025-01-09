#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>

using namespace std;

// Number of pixels per led strips
#define NUM_PIXELS 20
// Number of steps in the countdown (preferably divisible from NUM_PIXELS)
#define NUM_STEPS 4
// Which LED pattern to use
#define PATTERN_STYLE 1

// Interval between each step (in milliseconds)
#define COUNT_INTERVAL 1000

#define COLOR_RED   0xff0000
#define COLOR_AMBER 0xffaa00
#define COLOR_GREEN 0x00ff00

//==============================================================================

// Steps between STEP_STOP and STEP_GO represent the countdown steps
#define STEP_STOP 0
#define STEP_GO   (NUM_STEPS + 1)

uint32_t strip_0[NUM_PIXELS];

bool is_reset_btn() {
  printf("[RESET]\n");
  return 1;
}

void led_clear() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip_0[i] = 0;
  }
}

void led_show() {
  printf("| ");
  for (int i = 0; i < NUM_PIXELS; i++) {
    const uint32_t color = strip_0[i];
    const uint8_t r = color >> 16 & 0xff;
    const uint8_t g = color >> 8 & 0xff;
    const uint8_t b = color & 0xff;
    printf("\033[48;2;%u;%u;%um  \033[0m ", r, g, b);
  }
  printf("\033[0m|\n");
}

void led_set_color(uint16_t pixel, uint32_t color) {
  assert(pixel < NUM_PIXELS);
  strip_0[pixel] = color;
}

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

  printf("[STEP %u] ", step);
  led_show();
}

void delay(uint32_t ms) {
  printf("[DELAY] %u ms\n", ms);
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

int main() {
  set_led_state(STEP_STOP);
  if (is_reset_btn()) {
    do_reset();
  }
  return 0;
}
