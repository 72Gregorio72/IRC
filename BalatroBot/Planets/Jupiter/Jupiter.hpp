#ifndef JUPITER_HPP
#define JUPITER_HPP

#include "../IPlanet.hpp"

class Jupiter : public IPlanet {
	public:
		Jupiter();
		Jupiter(const Jupiter& other);
		~Jupiter();

		void printPlanet();
		void playPlanet(Balatro *bot);

		std::string getEffect();
		std::string getName();

	private:
		Balatro *bot;
		std::string effect;
		std::string name;
};


#endif
