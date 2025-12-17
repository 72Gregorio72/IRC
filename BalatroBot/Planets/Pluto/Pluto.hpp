#ifndef PLUTO_HPP
#define PLUTO_HPP

#include "../IPlanet.hpp"

class Pluto : public IPlanet {
	public:
		Pluto();
		Pluto(const Pluto& other);
		~Pluto();

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