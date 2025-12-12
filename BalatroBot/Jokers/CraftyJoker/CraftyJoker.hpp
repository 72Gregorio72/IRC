#ifndef CRAFTY_JOKER_HPP
#define CRAFTY_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class CraftyJoker : public IJoker {

	public:
		CraftyJoker();
		CraftyJoker(const CraftyJoker& other);
		~CraftyJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
};

#endif