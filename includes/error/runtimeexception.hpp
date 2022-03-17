// The Romi Project
//
// @ File   :  runtimeexception.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 15:56:02
//

#pragma once

#include "exception.hpp"

class RRuntimeException : public RException
{
public:
	RRuntimeException() noexcept = default;
	RRuntimeException(const RRuntimeException&) noexcept = default;
	RRuntimeException(RRuntimeException&&) noexcept = default;
	virtual ~RRuntimeException() noexcept = default;
	RRuntimeException& operator= (const RRuntimeException&) = default;
	RRuntimeException& operator= (RRuntimeException&&) = default;

	RRuntimeException(std::string msg);
    RRuntimeException(std::string msg, int err);
	RRuntimeException(int err);
};
