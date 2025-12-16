#ifndef BUSINESSCARD_HPP
#define BUSINESSCARD_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class BusinessCard : public IJoker {

	public:
		BusinessCard();
		BusinessCard(const BusinessCard& other);
		~BusinessCard();

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