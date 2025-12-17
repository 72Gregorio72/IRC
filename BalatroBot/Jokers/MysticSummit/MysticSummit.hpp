#ifndef MYSTIC_SUMMIT_HPP
#define MYSTIC_SUMMIT_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class MysticSummit : public IJoker {

	public:
		MysticSummit();
		MysticSummit(const MysticSummit& other);
		~MysticSummit();

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