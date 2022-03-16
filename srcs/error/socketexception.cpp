// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 14:26:11
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 14:26:11
//

#include "error/socketexception.hpp"

RSocketException::RSocketException(int err)
    : RException(MakeMessage(err))
{
}

RSocketException::RSocketException(std::string msg, int err)
    : RException(MakeMessage(msg, err))
{
}