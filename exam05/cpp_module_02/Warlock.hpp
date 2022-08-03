#include <string>
#include "ATarget.hpp"
#include "ASpell.hpp"
#include <map>

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

class Warlock {
	private:
		std::string						name;
		std::string						title;
		std::map<std::string, ASpell*>	know_spell;

	public:
		Warlock (std::string name, std::string title);
		~Warlock();

		const std::string & getName ( void ) const;
		const std::string & getTitle ( void ) const;

		void	setTitle ( const std::string & title );

		void introduce( void ) const;

		void learnSpell ( ASpell* spell );
		void forgetSpell ( const std::string & spell );
		void launchSpell ( const std::string & spell, ATarget & target) const;
};

#endif
