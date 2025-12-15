#ifndef THE_ORDER_JOKER_HPP
#define THE_ORDER_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class TheOrderJoker : public IJoker {

	public:
		TheOrderJoker();
		TheOrderJoker(const TheOrderJoker& other);
		~TheOrderJoker();

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