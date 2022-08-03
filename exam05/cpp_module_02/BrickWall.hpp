#ifndef BRICKWALL_HPP
# define BRICKWALL_HPP

#include "ATarget.hpp"

class BrickWall : public ATarget {
	public:
		BrickWall();
		BrickWall(const BrickWall & src);
		~BrickWall();

		BrickWall & operator= (const BrickWall & rhs);

		ATarget* clone() const;
};

#endif
