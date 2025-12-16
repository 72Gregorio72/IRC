#ifndef SMILEYFACE_HPP
#define SMILEYFACE_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class SmileyFace : public IJoker {

	public:
		SmileyFace();
		SmileyFace(const SmileyFace& other);
		~SmileyFace();

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