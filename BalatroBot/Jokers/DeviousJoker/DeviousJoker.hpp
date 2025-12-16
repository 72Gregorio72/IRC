#ifndef DEVIOUS_JOKER_HPP
#define DEVIOUS_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class DeviousJoker : public IJoker {

	public:
		DeviousJoker();
		DeviousJoker(const DeviousJoker& other);
		~DeviousJoker();

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