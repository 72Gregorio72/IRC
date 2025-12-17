#ifndef NEPTUNE_HPP
#define NEPTUNE_HPP

#include "../IPlanet.hpp"

class Neptune : public IPlanet {
	public:
		Neptune();
		Neptune(const Neptune& other);
		~Neptune();

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