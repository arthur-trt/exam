#include <string>

#ifndef ASPELL_HPP
# define ASPELL_HPP


//#include "ATarget.hpp"

class ATarget;

class ASpell {
	protected:
		std::string	name;
		std::string	effects;

	public:
		ASpell();
		ASpell(const std::string & name, const std::string & effects);
		ASpell(const ASpell &src);
		~ASpell();

		ASpell & operator= (const ASpell &rh);

		std::string getName(void) const;
		std::string getEffects(void) const;

		void	launch(const ATarget & target ) const;

		virtual ASpell* clone() const = 0;
};

#endif
