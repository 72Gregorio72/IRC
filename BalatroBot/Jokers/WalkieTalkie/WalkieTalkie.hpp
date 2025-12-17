#ifndef WALKIETALKIE_HPP
#define WALKIETALKIE_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class WalkieTalkie : public IJoker {

	public:
		WalkieTalkie();
		WalkieTalkie(const WalkieTalkie& other);
		~WalkieTalkie();

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