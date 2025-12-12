#ifndef CLEVER_JOKER_HPP
#define CLEVER_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class CleverJoker : public IJoker {

	public:
		CleverJoker();
		CleverJoker(const CleverJoker& other);
		~CleverJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
};

#endif