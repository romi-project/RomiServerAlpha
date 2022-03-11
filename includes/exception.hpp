// The Romi Project
//
//  - Tenisya(T2SU)
//  romi@romi-project.net

#pragma once

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

	RException(std::string mes);

	const std::string& virtual GetMessage() const noexcept;
};
