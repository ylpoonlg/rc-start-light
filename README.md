# RC Start Light
This is the Arduino code for a start light system designed for RC cars. It
controls LED strips to display countdown sequences and the lights can be reset
with an IR remote.

## Usage
### Prerequisite
The code depend on the following libraries. They can be installed from the
Arduino library manager directly:
- [Adafruit_NeoPixel.h](https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html)
- [IRremote.hpp](https://github.com/Arduino-IRremote/Arduino-IRremote)

### Compile
The code can be built with `make`. It includes a simple wrapper for
[`arduino-cli`](https://docs.arduino.cc/arduino-cli/) to compile and upload to
an Arduino board. A few options are available:

- `FQBN`: The Fully Qualified Board Name. Default: `arduino:avr:nano`
- `PORT`: Port connected to the Arduino board. Default: `/dev/ttyUSB0`
- `BOARD_OPT`: Additional board options. Default: `cpu=atmega328old`

Example:

```
# Compile
make compile FQBN="arduino:avr:uno"

# Compile and upload
make upload FQBN="arduino:avr:uno" PORT="/dev/ttyUSB1"
```

Alternatively, the sketch `rc-start-light.ino` can be opened/compiled/uploaded
in the Arduino IDE.

### Test Demo
There is a small program that can be run in a terminal to test the logic and
functionalities. It can be run with
```
make test
```

## Configuration
The configurations can be customized by modifying `config.h`, see the
[file](https://github.com/ylpoonlg/rc-start-light/blob/master/rc-start-light/config.h)
for more details.
