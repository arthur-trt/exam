#include "Dummy.hpp"

Dummy::Dummy() : ATarget("Target Practice Dummy")
{}

Dummy::Dummy(const Dummy & src) : ATarget(src)
{}

Dummy::~Dummy()
{}

Dummy & Dummy::operator=(const Dummy & rhs)
{
	*this = rhs;
	return (*this);
}

ATarget * Dummy::clone() const {
	return (new Dummy());
}
