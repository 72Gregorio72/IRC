#ifndef PHOTOGRAPH_HPP
#define PHOTOGRAPH_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Photograph : public IJoker {

	public:
		Photograph();
		Photograph(const Photograph& other);
		~Photograph();

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