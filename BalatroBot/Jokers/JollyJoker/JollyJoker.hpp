#ifndef JOLLY_JOKER_HPP
#define JOLLY_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class JollyJoker : public IJoker {

	public:
		JollyJoker();
		JollyJoker(const JollyJoker& other);
		~JollyJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();


	private:
		// Balatro *bot;bot
		int cost;
		std::string effect;
		std::string name;
};

#endif