#ifndef BULL_HPP
#define BULL_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Bull : public IJoker {

	public:
		Bull();
		Bull(const Bull& other);
		~Bull();

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