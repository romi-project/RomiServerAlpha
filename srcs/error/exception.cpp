// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-11 14:51:43
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-11 14:51:43
//

#include "error/exception.hpp"
#include "error/errorutils.hpp"

RException::RException(const std::string& mes)
    : _mes(mes)
{
}

std::string RException::MakeMessage(const int err)
{
    return ErrorUtils::GetString(err);
}

std::string RException::MakeMessage(const std::string& msg, const int err)
{
    std::string ret;

    ret += msg;

    ret += " (";
    ret += std::to_string(err);
    ret += ")";

    return ret;
}
