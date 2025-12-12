#ifndef DROLL_JOKER_HPP
#define DROLL_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class DrollJoker : public IJoker {

	public:
		DrollJoker();
		DrollJoker(const DrollJoker& other);
		~DrollJoker();

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