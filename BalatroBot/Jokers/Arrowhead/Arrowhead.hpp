#ifndef ARROWHEAD_HPP
#define ARROWHEAD_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Arrowhead : public IJoker {

	public:
		Arrowhead();
		Arrowhead(const Arrowhead& other);
		~Arrowhead();

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