#ifndef FWOOSH_HPP
# define FWOOSH_HPP

# include "ASpell.hpp"

class Fwoosh : public ASpell {
	public:
		Fwoosh();
		Fwoosh( const Fwoosh & src );
		~Fwoosh();

		Fwoosh & operator= (const Fwoosh & rhs );

		ASpell* clone() const;
};

#endif
