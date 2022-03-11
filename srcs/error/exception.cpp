// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-11 14:51:43
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-11 14:51:43
//
#include "exception.hpp"

RException::RException(std::string mes)
    : _mes(mes)
{
}

const std::string& RException::GetMessage() const noexcept
{
    return _mes;
}