// The Romi Project
//
// @ File   :  interlocked.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 17:55:32
//

#pragma once

#include "std.hpp"

class RInterlocked
{
public:
    RInterlocked() = delete;
    virtual ~RInterlocked() = delete;
    RInterlocked(const RInterlocked&) = delete;
    RInterlocked(RInterlocked&&) = delete;
    RInterlocked& operator= (const RInterlocked&) = delete;
    RInterlocked& operator= (RInterlocked&&) = delete;

    static size_t CompareExchange(volatile size_t* dest, size_t exchange, size_t comperand);
};
