#ifndef _IO_H
#define _IO_H

#include "core.h"
#include <map>

namespace omam8::IOEmu {

enum IODevice {
  // human-friendly devices
  SERIAL,

  // internal devices
  SERIAL_CLK,
  SERIAL_IN,
  SERIAL_OUT,
};

extern std::map<std::string, IODevice> str_to_iodevice;

extern std::map<int, IODevice> pins_to_devices;
extern std::map<IODevice, int> devices_to_pins;
extern std::map<int, bool> known_pin_states;

void setIODevice(int pinStart, IODevice device);
void handlePinSet(int pin, bool value);

} // namespace omam8::IOEmu

#endif // _IO_H
