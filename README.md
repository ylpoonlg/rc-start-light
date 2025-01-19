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
Clone (download) the repository and open the sketch `rc-start-light.ino` in
Arduino IDE. Make sure everything in the sketch directory is included.

Alternatively, the code can be built with `make`. It includes a simple wrapper
for [`arduino-cli`](https://docs.arduino.cc/arduino-cli/) to compile and upload
to an Arduino board. See the `Makefile` for more details.

## Configuration
The configurations can be customized by modifying `config.h`, see the
[file](https://github.com/ylpoonlg/rc-start-light/blob/master/rc-start-light/config.h)
for more details.
