#ifndef POLYMORPH_HPP
# define POLYMORPH_HPP

# include "ASpell.hpp"

class Polymorph : public ASpell {
	public:
		Polymorph();
		Polymorph( const Polymorph & src );
		~Polymorph();

		Polymorph & operator= (const Polymorph & rhs );

		ASpell* clone() const;
};
#endif
