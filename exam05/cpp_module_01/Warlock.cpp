#include "Warlock.hpp"
#include <iostream>

Warlock::Warlock( std::string name, std::string title )
{
	this->name = name;
	this->title = title;
	std::cout << this->name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock ( void )
{
	std::cout << this->name << ": My job here is done!" << std::endl;
}

const std::string & Warlock::getName ( void ) const
{
	return (this->name);
}

const std::string & Warlock::getTitle ( void ) const
{
	return (this->title);
}

void Warlock::setTitle ( const std::string & title )
{
	this->title = title;
}

void	Warlock::introduce( void ) const
{
	std::cout << this->name << ": I am " << this->name << ", " << this->title << std::endl;
}

void	Warlock::learnSpell( ASpell * spell )
{
	if (this->know_spell.count(spell->getName()) == 0)
		this->know_spell[spell->getName()] = spell;
}

void	Warlock::forgetSpell( const std::string & spell )
{
	std::map<std::string, ASpell*>::iterator	it;

	it = this->know_spell.find(spell);
	if (it != this->know_spell.end())
		this->know_spell.erase(it);
}

void	Warlock::launchSpell ( const std::string & spell, ATarget & target ) const
{
	std::map<std::string, ASpell *>::const_iterator	it;

	it = this->know_spell.find(spell);
	if (it != this->know_spell.end())
		it->second->launch(target);
}
