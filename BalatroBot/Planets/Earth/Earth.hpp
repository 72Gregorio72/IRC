#ifndef EARTH_HPP
#define EARTH_HPP

#include "../IPlanet.hpp"

class Earth : public IPlanet {
	public:
		Earth();
		Earth(const Earth& other);
		~Earth();

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