#include "io/serial.h"
#include "io.h"
#include "platform.h"

namespace omam8::IOEmu::Serial {

uint8_t outBuf = 0;
int outBufBits = 0;

void handleClockPulse() {
  int clkPin = IOEmu::devices_to_pins[SERIAL_CLK];
  int outPin = IOEmu::devices_to_pins[SERIAL_OUT];

  if (known_pin_states[clkPin]) {
#ifdef DEBUG
    // std::cerr << "[SERIAL] clock pulsed\n";
    Platform::debugConsolePrint("[SERIAL] clock pulsed");
#endif

    // push the outPin's content into outBuf
    outBuf <<= 1;
    outBuf |= known_pin_states[outPin];
    outBufBits++;

    // if outBufBits==8, putchar
    if (outBufBits == 8) {
      Platform::serialPut(outBuf);
      outBuf = 0;
      outBufBits = 0;
    }
  }
}

} // namespace omam8::IOEmu::Serial
