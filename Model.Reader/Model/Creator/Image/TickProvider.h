#pragma once
#include <cstdint>
#include <mutex>

class TickProvider
{
public:
	uint32_t GetCurrentFrame() const {
		return m_ticksSinceMidnight;
	}

	void UpateTickBasedOnCurrentTime() {
		auto now = std::chrono::system_clock::now();
		auto now_time_t = std::chrono::system_clock::to_time_t(now);
		auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		std::tm now_tm;
		localtime_s(&now_tm, &now_time_t);

		m_ticksSinceMidnight = (static_cast<uint64_t>(now_tm.tm_hour) * 3600000 + static_cast<uint64_t>(now_tm.tm_min) * 60000 + static_cast<uint64_t>(now_tm.tm_sec) * 1000 + now_ms.count()) / 50;
	}

	void UpateTickBasedOnSetValue(uint32_t ticksSinceMidnight) {
		m_ticksSinceMidnight = ticksSinceMidnight;
	}
private:
	uint32_t m_ticksSinceMidnight = 0;
};

