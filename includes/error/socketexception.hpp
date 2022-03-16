// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 14:19:13
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 14:19:13
//

#pragma once

#include "exception.hpp"

class RSocketException : public RException
{
public:
	RSocketException() noexcept = default;
	RSocketException(const RSocketException&) noexcept = default;
	RSocketException(RSocketException&&) noexcept = default;
	virtual ~RSocketException() noexcept = default;
	RSocketException& operator= (const RSocketException&) = default;
	RSocketException& operator= (RSocketException&&) = default;

	RSocketException(int err);
    RSocketException(std::string msg, int err);
};
