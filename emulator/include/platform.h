#ifndef _PLATFORM_H
#define _PLATFORM_H

namespace omam8::Platform {

extern void serialPut(const char c);
extern void debugConsolePrint(const char *fmt, ...);
extern void dumpCoreState();

} // namespace omam8::Platform

#endif // _PLATFORM_H
