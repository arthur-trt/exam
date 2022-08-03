#include <string>

#ifndef ATARGET_HPP
# define ATARGET_HPP

//#include "ASpell.hpp"

class ASpell;

class ATarget {
	protected:
		std::string type;

	public:
		ATarget();
		ATarget(std::string type);
		ATarget(const ATarget & src);
		~ATarget();

		ATarget & operator= (const ATarget & rh);

		const std::string & getType(void) const;

		void getHitBySpell(const ASpell & spell) const;

		virtual ATarget * clone() const = 0;
};

#endif
