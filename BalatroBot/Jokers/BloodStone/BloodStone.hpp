#ifndef BLOODSTONE_HPP
#define BLOODSTONE_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class BloodStone : public IJoker {

	public:
		BloodStone();
		BloodStone(const BloodStone& other);
		~BloodStone();

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