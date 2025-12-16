#ifndef ONIXAGATE_HPP
#define ONIXAGATE_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class OnixAgate : public IJoker {

	public:
		OnixAgate();
		OnixAgate(const OnixAgate& other);
		~OnixAgate();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();
		std::string getRarity();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
		std::string rarity;
};

#endif