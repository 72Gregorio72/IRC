#ifndef SCHOLAR_HPP
#define SCHOLAR_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Scholar : public IJoker {

	public:
		Scholar();
		Scholar(const Scholar& other);
		~Scholar();

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