#ifndef ZANY_JOKER_HPP
#define ZANY_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class ZanyJoker : public IJoker {

	public:
		ZanyJoker();
		ZanyJoker(const ZanyJoker& other);
		~ZanyJoker();

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