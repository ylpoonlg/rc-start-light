#include "config.h"

#define DECODE_NEC

#include <Adafruit_NeoPixel.h>
#include <IRremote.hpp>

// Enable virtual LED if verbosity is 0
#if VERBOSE == 0
#define VIRT_LED
#endif

#ifndef VIRT_LED
#define LOGPRINT(v, s) if (VERBOSE >= v) Serial.print(s);
#else
#define LOGPRINT(v, s)
#endif

// States between STATE_STOP and STATE_GO represent the countdown states
#define STATE_OFF  -1
#define STATE_STOP 0
#define STATE_GO   (NUM_STEPS + 1)

// Define LED strips
Adafruit_NeoPixel strip_0(NUM_PIXELS, STRIP0_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_1(NUM_PIXELS, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

// Global states
int32_t cur_state = STATE_STOP;
uint32_t state_start_time = 0;
char log_buf[256];

void update_state(int32_t state) {
  cur_state = state;
  state_start_time = millis();
}

/**
 * Poll for IR signal and handle command.
 */
void ir_scan() {
  if (IrReceiver.decode()) {
    IRData ir_data = IrReceiver.decodedIRData;
    if (ir_data.protocol == IR_PROTO) {
      sprintf(log_buf, "[IR] CMD: 0x%x\r\n", ir_data.command);
      LOGPRINT(4, log_buf)

      switch (ir_data.command) {
      case IR_CMD_RST:
        update_state(STATE_STOP);
        break;
      case IR_CMD_STR:
        update_state(STATE_STOP + 1);
        break;
      case IR_CMD_OFF:
        update_state(STATE_OFF);
        break;
      }
    }
    IrReceiver.resume();
  }
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

#ifdef VIRT_LED
  for (int i = 0; i < NUM_PIXELS; i++) {
    const uint32_t color = strip_0.getPixelColor(i);
    const uint8_t r = color >> 16 & 0xff;
    const uint8_t g = color >> 8 & 0xff;
    const uint8_t b = color & 0xff;
    char strip_s[NUM_PIXELS * 10];
    sprintf(strip_s, "\033[48;2;%u;%u;%um  \033[0m ", r, g, b);
    Serial.print(strip_s);
  }
  Serial.print("\033[0m\r");
#endif
}

/**
 * Wrapper for setPixelColor.
 */
void led_set_color(uint16_t pixel, uint32_t color) {
  strip_0.setPixelColor(pixel, color);
  strip_1.setPixelColor(pixel, color);
}

/**
 * Set led strips state for a given state.
 */
void set_led_state(int32_t state) {
  sprintf(log_buf, "[STATE %d] Set LED State\r\n", state);
  LOGPRINT(3, log_buf)

  led_clear();


#if PATTERN_STYLE == 0
  if (state == STATE_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_GREEN);
  } else if (state == STATE_STOP) {
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_RED);
  } else if (state != STATE_OFF) {
    const float grp_size = (float)((int)(NUM_PIXELS / 2)) / NUM_STEPS;
    for (int i = floor((state - 1) * grp_size); i <= NUM_PIXELS / 2; i++) {
      led_set_color(i, COLOR_AMBER);
      led_set_color(NUM_PIXELS - i - 1, COLOR_AMBER);
    }
  }
#else
  if (state == STATE_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_GREEN);
  } else if (state == STATE_STOP) {
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_RED);
  } else if (state != STATE_OFF) {
    const float grp_size = (float)NUM_PIXELS / NUM_STEPS;
    const int px_len = ceil((NUM_STEPS - state + 1) * grp_size);
    for (int i = 0; i < px_len; i++) {
      led_set_color(i, COLOR_AMBER);
    }
  }
#endif

  led_show();
}

void setup() {
  pinMode(RST_BTN_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  IrReceiver.begin(IR_RCV_PIN, ENABLE_LED_FEEDBACK);
  strip_0.begin();

  cur_state = STATE_STOP;
  set_led_state(cur_state);
}

void loop() {
  const int32_t last_state = cur_state;

  ir_scan();

  // State transition
  const uint32_t cur_time = millis();
  if (cur_state >= STATE_GO) {
    if (cur_time >= state_start_time + COUNT_INTERVAL * 2) {
      update_state(STATE_STOP);
    }
  } else if (cur_state > STATE_STOP) {
    if (cur_time >= state_start_time + COUNT_INTERVAL) {
      update_state(cur_state + 1);
    }
  }

  // Check if state has changed
  if (cur_state != last_state) {
    set_led_state(cur_state);
  }

  delay(100);
}
