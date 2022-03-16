
#include "std.hpp"
#include "error/errorutils.hpp"

std::string ErrorUtils::GetString(int err)
{
	LPSTR message;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		err,
		0,
		reinterpret_cast<LPSTR>(&message),
		0,
		NULL);
	
	std::string ret(message);
	LocalFree(message);
	return ret;
}