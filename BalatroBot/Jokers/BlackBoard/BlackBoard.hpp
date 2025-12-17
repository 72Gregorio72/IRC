#ifndef BLACKBOARD_HPP
#define BLACKBOARD_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class BlackBoard : public IJoker {

	public:
		BlackBoard();
		BlackBoard(const BlackBoard& other);
		~BlackBoard();

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