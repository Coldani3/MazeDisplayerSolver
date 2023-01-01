#pragma once

#include <chrono>

class TimeUtil {
public:
	static constexpr long now() {
		using namespace std::chrono;
		return duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		).count();
	}
};