#ifndef MAD_JOKER_HPP
#define MAD_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class MadJoker : public IJoker {

	public:
		MadJoker();
		MadJoker(const MadJoker& other);
		~MadJoker();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();
		std::string getRarity();


	private:
		// Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
		std::string rarity;
};

#endif