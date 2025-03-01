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

#define MODE_MANUAL 0
#define MODE_AUTO   1

// Define LED strips
Adafruit_NeoPixel strip_0(NUM_PIXELS, STRIP0_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_1(NUM_PIXELS, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

// Global states
int32_t cur_state = STATE_STOP;
int32_t last_state = STATE_OFF;
uint32_t cur_state_start_time = 0;
char log_buf[256];
int32_t cur_mode = MODE_AUTO;
bool is_buzzer_on = false;

void update_state(int32_t state) {
  sprintf(log_buf, "[STATE] Update state: %d -> %d\r\n", cur_state, state);
  LOGPRINT(3, log_buf)

  cur_state = state;
  cur_state_start_time = millis();
}

/**
 * Poll for IR signal and handle command.
 */
void ir_scan() {
  if (IrReceiver.decode()) {
    IRData ir_data = IrReceiver.decodedIRData;
    bool is_repeat = ir_data.flags & IRDATA_FLAGS_IS_REPEAT;
    if (ir_data.protocol == IR_PROTO) {
      sprintf(log_buf, "[IR] CMD: 0x%x\r\n", ir_data.command);
      LOGPRINT(4, log_buf)

      switch (ir_data.command) {
      case IR_CMD_MODE:
        update_state(STATE_STOP);
        if (!is_repeat)
          cur_mode = (cur_mode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
        is_buzzer_on = (cur_mode == MODE_MANUAL);
        break;
      case IR_CMD_START:
        update_state(STATE_STOP + 1);
        break;
      case IR_CMD_OFF:
        update_state(STATE_OFF);
        break;
      case IR_CMD_BUZZER:
        if (!is_repeat) is_buzzer_on = !is_buzzer_on;
        break;
      }
    }
    IrReceiver.resume();
  }
}

/**
 * Poll for IR signal and handle command.
 */
void btn_scan() {
  if (!digitalRead(RST_BTN_PIN)) {
    update_state(STATE_STOP);
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
  Serial.print("\r");
  for (int i = 0; i < NUM_PIXELS; i++) {
    const uint32_t color = strip_0.getPixelColor(i);
    const uint8_t r = color >> 16 & 0xff;
    const uint8_t g = color >> 8 & 0xff;
    const uint8_t b = color & 0xff;
    char strip_s[NUM_PIXELS * 10];
    sprintf(strip_s, "\033[48;2;%u;%u;%um  \033[0m ", r, g, b);
    Serial.print(strip_s);
  }
  Serial.print("\033[0m | \033[40m");
  if (digitalRead(BUZZER_PIN)) {
    Serial.print("\033[33mBUZZ");
  } else {
    Serial.print("    ");
  }
  Serial.print("\033[0m ");
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
 * Apply state to led strips.
 */
void apply_state(int32_t mode, int32_t state, uint32_t state_time) {
  led_clear();

#if PATTERN_REVERSE
  if (state > STATE_STOP && state < STATE_GO)
    state = NUM_STEPS - state + 1;
#endif

#if PATTERN_STYLE == 1 || PATTERN_STYLE == 2
  if (state == STATE_GO) {
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_GREEN);
  } else if (state == STATE_STOP) {
#if PATTERN_STYLE == 2
    if (mode == MODE_MANUAL) {
      for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_RED);
    } else {
      const int side_width = NUM_PIXELS / 3;
      for (int i = 0; i < side_width; i++) {
        led_set_color(i, COLOR_RED);
        led_set_color(NUM_PIXELS - i - 1, COLOR_RED);
      }
      // Blink
      const uint32_t blink_time = state_time / REFRESH_INTERVAL;
      if (blink_time % 20 < 2) {
        const int side_offset = side_width;
        for (int i = side_offset; i < NUM_PIXELS - side_offset; i++) {
          led_set_color(i, COLOR_RED);
        }
      }
    }
#else
    for (int i = 0; i < NUM_PIXELS; i++) led_set_color(i, COLOR_RED);
#endif
  } else if (state != STATE_OFF) {
    const float grp_size = ((float)NUM_PIXELS / 2) / (NUM_STEPS + 1 - 0.5);
    const int start_px = floor((state) * grp_size);
    for (int i = start_px; i < NUM_PIXELS - start_px; i++) {
      led_set_color(i, COLOR_AMBER);
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

  // Buzzer
  if (is_buzzer_on) {
    if (state > STATE_STOP && state < STATE_GO) {
      if (state_time < BUZZER_DURATION) digitalWrite(BUZZER_PIN, HIGH);
      else digitalWrite(BUZZER_PIN, LOW);
    } else if (state == STATE_GO) {
      if (state_time < GO_STATE_DURATION - 1) digitalWrite(BUZZER_PIN, HIGH);
      else digitalWrite(BUZZER_PIN, LOW);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

/**
 * Mapping of the state transition.
 */
int32_t get_next_state(int32_t mode, int32_t state, uint32_t state_time) {
  if (state >= STATE_GO) {
    if (state_time >= GO_STATE_DURATION) {
      return STATE_STOP;
    }
  } else if (state > STATE_STOP) {
    if (state_time >= COUNT_INTERVAL) {
      return state + 1;
    }
  } else if (state == STATE_STOP && mode == MODE_AUTO) {
    if (state_time >= RESTART_INTERVAL) return STATE_STOP + 1;
  }
  return state;
}

void setup() {
  pinMode(RST_BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);
  IrReceiver.begin(IR_RCV_PIN, ENABLE_LED_FEEDBACK);
  strip_0.begin();
  strip_1.begin();

  update_state(STATE_STOP);
}

void loop() {
  // Polling
  ir_scan();
  btn_scan();

  // Update state
  const uint32_t cur_time = millis();
  int32_t next_state =
    get_next_state(cur_mode, cur_state, cur_time - cur_state_start_time);
  if (next_state != cur_state) update_state(next_state);

  // Apply state
  apply_state(cur_mode, cur_state, cur_time - cur_state_start_time);
  last_state = cur_state;

  delay(REFRESH_INTERVAL);
}
