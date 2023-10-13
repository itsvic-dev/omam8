#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "core.h"

namespace omam8::Platform {

extern void serialPut(const char c);
extern void debugConsolePrint(const char *fmt, ...);
extern void dumpCoreState(bool io_pins[32], Core::IOMode io_pin_modes[32]);

} // namespace omam8::Platform

#endif // _PLATFORM_H
