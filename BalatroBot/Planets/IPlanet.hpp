#ifndef IPLANET_HPP
#define IPLANET_HPP

#include <iostream>
#include "../includes/Balatro.hpp"

class IPlanet
{
	public:
		virtual ~IPlanet() {}
		virtual void printPlanet() = 0;
		virtual void playPlanet(Balatro *bot) = 0;

		virtual std::string getEffect() = 0;
		virtual std::string getName() = 0;
};

#endif