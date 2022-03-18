
#include "std.hpp"
#include "utils/errorutils.hpp"

#if _ROMI_WINDOWS

static std::string GetString_Windows(int err)
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

#else

static std::string GetString_POSIX(int err)
{
	static std::mutex errorLock;

	std::scoped_lock<std::mutex> _(errorLock);
	std::string ret(std::strerror(err));
	return ret;
}

#endif

std::string ErrorUtils::GetString(int err)
{
#if _ROMI_WINDOWS
	return GetString_Windows(err);
#else
	return GetString_POSIX(err);
#endif
}
