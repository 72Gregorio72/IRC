#ifndef SATURN_HPP
#define SATURN_HPP

#include "../IPlanet.hpp"

class Saturn : public IPlanet {
	public:
		Saturn();
		Saturn(const Saturn& other);
		~Saturn();

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