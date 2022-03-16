

#pragma once

#include <string>

class ErrorUtils
{
public:
	ErrorUtils() = delete;
	~ErrorUtils() = delete;
	ErrorUtils(const ErrorUtils&) = delete;
	ErrorUtils(ErrorUtils&&) = delete;
	ErrorUtils& operator=(const ErrorUtils&) = delete;
	ErrorUtils& operator=(ErrorUtils&&) = delete;

	static std::string GetString(int err);
};
