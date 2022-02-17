#pragma once

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

namespace romi
{
	class SocketException : std::exception
	{
	private:
		std::unique_ptr<const char> message;

	public:
		SocketException() = delete;
		SocketException(const SocketException&) = delete;
		SocketException(SocketException&&) = delete;
		SocketException& operator=(const SocketException&) = delete;
		SocketException& operator=(SocketException&&) = delete;
		virtual ~SocketException() NOEXCEPT = default;

		SocketException(const char* format, ...);
		const char* what() const NOEXCEPT;
	};
}

#endif
