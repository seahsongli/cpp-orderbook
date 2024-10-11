#include "Order.h"

std::time_t Order::getRawTimestamp() const
{
	return std::chrono::system_clock::to_time_t(m_timestamp);
}
void Order::displayTimestamp() const
{
	// converts m_timestamp from type std::chrono::time_point to std::time_t type
	std::time_t timestamp_c = std::chrono::system_clock::to_time_t(m_timestamp);

	// converts std::time_t value into C style string representing glocal time.
	std::cout << std::ctime(&timestamp_c) << std::endl;
}