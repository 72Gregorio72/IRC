#ifndef MARS_HPP
#define MARS_HPP

#include "../IPlanet.hpp"

class Mars : public IPlanet {
	public:
		Mars();
		Mars(const Mars& other);
		~Mars();

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