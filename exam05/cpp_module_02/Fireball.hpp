#ifndef FIREBALL_HPP
# define FIREBALL_HPP

# include "ASpell.hpp"

class Fireball : public ASpell {
	public:
		Fireball();
		Fireball( const Fireball & src );
		~Fireball();

		Fireball & operator= (const Fireball & rhs );

		ASpell* clone() const;
};
#endif
