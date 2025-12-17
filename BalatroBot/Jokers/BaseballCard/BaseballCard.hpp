#ifndef BASEBALLCARD_HPP
#define BASEBALLCARD_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class BaseballCard : public IJoker {

	public:
		BaseballCard();
		BaseballCard(const BaseballCard& other);
		~BaseballCard();

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