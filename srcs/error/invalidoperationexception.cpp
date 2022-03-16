// The Romi Project
//
// @ File   :  invalidoperationexception.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 17:31:19
//

#include "error/invalidoperationexception.hpp"

RInvalidOperationException::RInvalidOperationException(std::string msg)
	: RException(msg)
{
}