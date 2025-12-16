#ifndef MERCURY_HPP
#define MERCURY_HPP

#include "../IPlanet.hpp"

class Mercury : public IPlanet {
	public:
		Mercury();
		Mercury(const Mercury& other);
		~Mercury();

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