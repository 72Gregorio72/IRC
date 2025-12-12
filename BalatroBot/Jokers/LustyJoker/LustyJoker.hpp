#ifndef LUSTYJOKER_HPP
#define LUSTYJOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class LustyJoker : public IJoker {
	public:
		LustyJoker();
		LustyJoker(const LustyJoker& other);
		~LustyJoker();

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