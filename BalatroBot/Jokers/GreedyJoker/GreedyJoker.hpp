#ifndef GreedyJOKER_HPP
#define GreedyJOKER_HPP

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
	private:
		Balatro *bot;
		int cost;
		std::string effect;
};

#endif