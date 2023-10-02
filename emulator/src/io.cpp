#include "io.h"
#include "io/serial.h"
#include <iostream>
#include <map>

namespace omam8::IOEmu {

std::map<int, IODevice> pins_to_devices = {};
std::map<IODevice, int> devices_to_pins = {};
std::map<int, bool> known_pin_states = {};

std::map<std::string, IODevice> str_to_iodevice = {
    {"serial", SERIAL},
};

void handlePinSet(int pin, bool value) {
  known_pin_states[pin] = value;

  // handle device-specific stuff
  switch (pins_to_devices[pin]) {
  case SERIAL_CLK:
    Serial::handleClockPulse();
    break;
  default:
    break;
  }
}

void setIODevice(int pinStart, IODevice device) {
  switch (device) {
  case SERIAL:
    pins_to_devices[pinStart] = SERIAL_CLK;
    pins_to_devices[pinStart + 1] = SERIAL_IN;
    pins_to_devices[pinStart + 2] = SERIAL_OUT;
    break;
  default:
    pins_to_devices[pinStart] = device;
    break;
  }

#ifdef DEBUG
  std::cerr << "[IO] device " << device << " assigned to pin " << pinStart
            << "\n";
#endif

  // reset devices_to_pins
  devices_to_pins.clear();
  for (auto &[key, val] : pins_to_devices) {
    devices_to_pins[val] = key;
  }
}

} // namespace omam8::IOEmu
