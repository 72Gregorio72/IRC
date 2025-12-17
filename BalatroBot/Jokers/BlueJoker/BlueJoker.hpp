#ifndef BLUEJOKER_HPP
#define BLUEJOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class BlueJoker : public IJoker {

	public:
		BlueJoker();
		BlueJoker(const BlueJoker& other);
		~BlueJoker();

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