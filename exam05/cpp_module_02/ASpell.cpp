#include "ASpell.hpp"
#include "ATarget.hpp"
#include <iostream>

ASpell::ASpell() {
	this->name = "";
	this->effects = "";
}

ASpell::ASpell(const std::string & name, const std::string & effects)
{
	this->name = name;
	this->effects = effects;
}

ASpell::ASpell(const ASpell &src)
{
	this->name = src.name;
	this->effects = src.effects;
}

ASpell::~ASpell()
{

}

ASpell & ASpell::operator= (const ASpell & rh)
{
	this->name = rh.name;
	this->effects = rh.effects;
	return (*this);
}

std::string ASpell::getName(void) const {
	return (this->name);
}

std::string ASpell::getEffects(void) const {
	return (this->effects);
}

void	ASpell::launch(const ATarget & target ) const {
	target.getHitBySpell(*this);
}
