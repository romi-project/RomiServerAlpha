#include "error/runtimeexception.hpp"

RRuntimeException::RRuntimeException(std::string msg)
    : RException(msg)
{
}

RRuntimeException::RRuntimeException(std::string msg, int err)
    : RException(MakeMessage(msg, err))
{
}

RRuntimeException::RRuntimeException(int err)
    : RException(MakeMessage(err))
{
}