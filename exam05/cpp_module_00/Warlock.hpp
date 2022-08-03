#include <string>

class Warlock {
	private:
		std::string	name;
		std::string	title;

	public:
		Warlock (std::string name, std::string title);
		~Warlock();

		const std::string & getName ( void ) const;
		const std::string & getTitle ( void ) const;

		void	setTitle ( const std::string & title );

		void introduce( void ) const;
};
