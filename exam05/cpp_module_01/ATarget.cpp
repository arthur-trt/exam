#include "ATarget.hpp"
#include "ASpell.hpp"
#include <string>
#include <iostream>

ATarget::ATarget() {
	this->type = "";
}

ATarget::ATarget(std::string type) {
	this->type = type;
}

ATarget::ATarget(const ATarget & src) {
	this->type = src.type;
	//return (*this);
}

ATarget::~ATarget() {}

ATarget & ATarget::operator= (const ATarget & rh) {
	this->type = rh.type;
	return (*this);
}

const std::string & ATarget::getType( void ) const {
	return (this->type);
}

void	ATarget::getHitBySpell(const ASpell & spell) const
{
	std::cout << this->type << " has been " << spell.getEffects() << "!" << std::endl;
}
