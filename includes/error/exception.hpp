// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 14:05:39
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 14:05:39
//

#pragma once

#include "std.hpp"

#if _ROMI_WINDOWS
#pragma push_macro("GetMessage")
#undef GetMessage
#endif

class RException
{
private:
	std::string _mes;

public:
	RException() noexcept = default;
	RException(const RException&) noexcept = default;
	RException(RException&&) noexcept = default;
	virtual ~RException() noexcept = default;
	RException& operator= (const RException&) = default;
	RException& operator= (RException&&) = default;

	RException(const std::string& mes);

	inline const std::string& GetMessage() const noexcept { return _mes; }

protected:
	std::string MakeMessage(const std::string& msg, const int err);
	std::string MakeMessage(const int err);
};

#if _ROMI_WINDOWS
#pragma pop_macro("GetMessage")
#endif