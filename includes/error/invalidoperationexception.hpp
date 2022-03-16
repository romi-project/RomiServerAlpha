// The Romi Project
//
// @ File   :  invalidoperationexception.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 17:31:19
//

#pragma once

#include "exception.hpp"

class RInvalidOperationException : RException
{
private:

public:
    RInvalidOperationException() = default;
    virtual ~RInvalidOperationException() = default;
    RInvalidOperationException(const RInvalidOperationException&) = default;
    RInvalidOperationException(RInvalidOperationException&&) = default;
    RInvalidOperationException& operator= (const RInvalidOperationException&) = default;
    RInvalidOperationException& operator= (RInvalidOperationException&&) = default;

    RInvalidOperationException(std::string msg);
protected:

private:

};
