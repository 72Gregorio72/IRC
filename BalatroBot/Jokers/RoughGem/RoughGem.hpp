#ifndef ROUGHGEM_HPP
#define ROUGHGEM_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class RoughGem : public IJoker {

	public:
		RoughGem();
		RoughGem(const RoughGem& other);
		~RoughGem();

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