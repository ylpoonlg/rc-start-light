// Pin assignments
//	 * Primary LED strip
#define STRIP0_PIN 5
//	 * Secondary LED strip (optional)
#define STRIP1_PIN 6
//	 * IR receiver
#define IR_RCV_PIN 4
//	 * Reset button (optional)
#define RST_BTN_PIN 2

// Number of pixels per led strips
#define NUM_PIXELS 20
// Number of steps in the countdown
#define NUM_STEPS 3
// Which LED pattern to use
//   0: Sequential
//   1: Symmetrical
//   2: Symmetrical, w/Blinking red
#define PATTERN_STYLE 2
// Reverse countdown pattern
#define PATTERN_REVERSE true

// IR receiver protocol
#define IR_PROTO NEC
// IR commands - Set VERBOSE to 4 (or above) and check serial monitor to find
// the command for each button on the remote.
//   * Reset
#define IR_CMD_RST 0x40
//   * Start
#define IR_CMD_STR 0x41
//   * Turn off
#define IR_CMD_OFF 0x5D

// Interval between each step (in milliseconds)
#define COUNT_INTERVAL 1000
// Interval between each refresh loop (in milliseconds)
#define REFRESH_INTERVAL 50

// Customize colors
#define COLOR_RED   0xff0000
#define COLOR_AMBER 0xffaa00
#define COLOR_GREEN 0x00ff00

// Verbosity of the debug log
//  -1: Disable logs.
//   0: Enables virtual LED (Prints simulated strip to serial).
//   >0: Enable debug logs.
#define VERBOSE 0
