#include "Polymorph.hpp"

Polymorph::Polymorph() : ASpell("Polymorph", "turned into critter")
{}

Polymorph::Polymorph( const Polymorph & src ) : ASpell(src)
{}

Polymorph::~Polymorph()
{}

Polymorph & Polymorph::operator= (const Polymorph & rhs)
{
	*this = rhs;
	return (*this);
}

ASpell* Polymorph::clone() const {
	return (new Polymorph());
}
