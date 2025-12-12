#ifndef WRATHFULJOKER_HPP
#define WRATHFULJOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class WrathfulJoker : public IJoker {
	public:
		WrathfulJoker();
		WrathfulJoker(const WrathfulJoker& other);
		
		~WrathfulJoker();

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