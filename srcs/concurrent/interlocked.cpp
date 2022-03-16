// The Romi Project
//
// @ File   :  interlocked.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 17:55:32
//

#include "std.hpp"
#include "concurrent/interlocked.hpp"

size_t RInterlocked::CompareExchange(volatile size_t* dest, size_t exchange, size_t comperand)
{
	return InterlockedCompareExchange(dest, exchange, comperand);
}