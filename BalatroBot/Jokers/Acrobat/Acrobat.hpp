#ifndef ACROBAT_HPP
#define ACROBAT_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Acrobat : public IJoker {

	public:
		Acrobat();
		Acrobat(const Acrobat& other);
		~Acrobat();

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