#ifndef GREEDYJOKER_HPP
#define GREEDYJOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class GreedyJoker : public IJoker {
	public:
		GreedyJoker();
		GreedyJoker(const GreedyJoker& other);
		~GreedyJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
};

#endif