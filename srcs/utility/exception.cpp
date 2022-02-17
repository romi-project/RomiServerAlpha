#include "romi.hpp"
#include "romi_types.hpp"
#include "utility/exception.hpp"

romi::SocketException::SocketException(const char* format, ...)
	: std::exception()
{
	va_list va;
	va_list clone;
	int len;
	char* buffer;

	va_start(va, format);
	va_copy(clone, va);
	len = std::vsnprintf(nullptr, 0, format, clone);
	va_end(va);
}

const char* romi::SocketException::what() const NOEXCEPT
{
	return message.c_str();
}
