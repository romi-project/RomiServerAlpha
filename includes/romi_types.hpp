#pragma once

#ifndef ROMI_TYPES_HPP
#define ROMI_TYPES_HPP

#if defined(_WIN32) || defined(_WIN64)

typedef SOCKET rmSocket;
typedef HANDLE rmHandle;

#else

typedef int rmSocket;
typedef int rmHandle;

# ifdef __APPLE__
#  define NOEXCEPT _NOEXCEPT
# else
#  define NOEXCEPT noexcept
# endif

#endif

#endif
