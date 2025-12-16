#ifndef WILY_JOKER_HPP
#define WILY_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class WilyJoker : public IJoker {

	public:
		WilyJoker();
		WilyJoker(const WilyJoker& other);
		~WilyJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();


	private:
		// Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
};

#endif