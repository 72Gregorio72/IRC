#ifndef CRAZY_JOKER_HPP
#define CRAZY_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class CrazyJoker : public IJoker {

	public:
		CrazyJoker();
		CrazyJoker(const CrazyJoker& other);
		~CrazyJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
};

#endif