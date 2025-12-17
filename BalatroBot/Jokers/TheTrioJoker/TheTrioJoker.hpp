#ifndef THE_TRIO_JOKER_HPP
#define THE_TRIO_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class TheTrioJoker : public IJoker {

	public:
		TheTrioJoker();
		TheTrioJoker(const TheTrioJoker& other);
		~TheTrioJoker();

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