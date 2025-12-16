#ifndef MISPRINT_HPP
#define MISPRINT_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Misprint : public IJoker {

	public:
		Misprint();
		Misprint(const Misprint& other);
		~Misprint();

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