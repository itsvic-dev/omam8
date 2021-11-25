#include <cstdlib>
#include <cstdint>
#include <core.h>
#include <vector>
#include <iostream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	EmulatorCore core;
	if (Size == 0) return 0;
	std::vector<uint8_t> rom_data(Size);
	
	for (size_t i = 0; i < Size; i++) {
		rom_data[i] = Data[i];
	}

	core.initialize(rom_data);

	size_t clocks = 0;
	
	while(true) {
		try {
			core.run_clock_cycle();
		} catch (int exception) {
			return 0;
		}
		clocks++;
		if (clocks > 0xFFFF) {
			return 0; // prevent it from getting stuck in a loop
		}
	}
}
