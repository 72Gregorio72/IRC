#ifndef URANUS_HPP
#define URANUS_HPP

#include "../IPlanet.hpp"

class Uranus : public IPlanet {
	public:
		Uranus();
		Uranus(const Uranus& other);
		~Uranus();

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