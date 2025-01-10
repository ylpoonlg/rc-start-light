BUILD_DIR = ./build

ifndef PORT
	PORT = /dev/ttyUSB0
endif

ifndef FQBN
	FQBN = "arduino:avr:nano"
endif

ifndef BOARD_OPT
	BOARD_OPT = "cpu=atmega328old"
endif

.SILENT:

.PHONY: compile upload test clean

default: compile

compile:
	arduino-cli compile --fqbn $(FQBN) rc-start-light
	echo "Done."

upload:
	arduino-cli compile --fqbn $(FQBN) --board-options $(BOARD_OPT) \
			--upload --port $(PORT) rc-start-light
	echo "Uploaded."

serial:
	picocom -b 115200 $(PORT)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test: $(BUILD_DIR)
	$(CXX) -o $@ test/test.cpp

test: $(BUILD_DIR)/test
	./$(BUILD_DIR)/test

clean:
	rm -rf $(BUILD_DIR)
