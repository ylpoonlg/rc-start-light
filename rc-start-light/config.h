#define STRIP0_PIN 5
#define STRIP1_PIN 6
#define RST_BTN_PIN 2
#define IR_RCV_PIN 4

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

// IR receiver protocol
#define IR_PROTO NEC
// Command to use for reset
#define IR_CMD_RST 0x40
