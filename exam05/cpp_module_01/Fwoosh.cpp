#include "Fwoosh.hpp"

Fwoosh::Fwoosh() : ASpell("Fwoosh", "fwooshed")
{}

Fwoosh::Fwoosh( const Fwoosh & src ) : ASpell(src)
{}

Fwoosh::~Fwoosh()
{}

Fwoosh & Fwoosh::operator= (const Fwoosh & rhs)
{
	*this = rhs;
	return (*this);
}

ASpell* Fwoosh::clone() const {
	return (new Fwoosh());
}
