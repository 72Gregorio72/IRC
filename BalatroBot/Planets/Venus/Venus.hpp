#ifndef VENUS_HPP
#define VENUS_HPP

#include "../IPlanet.hpp"

class Venus : public IPlanet {
	public:
		Venus();
		Venus(const Venus& other);
		~Venus();

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