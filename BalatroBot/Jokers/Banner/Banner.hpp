#ifndef BANNER_HPP
#define BANNER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class Banner : public IJoker {

	public:
		Banner();
		Banner(const Banner& other);
		~Banner();

        void printJoker();
        void playJoker(int& chips, int& mult, Balatro *bot);

		int getCost();
		std::string getName();
		std::string getEffect();
		std::string getRarity();

	private:
		Balatro *bot;
		int cost;
		std::string effect;
		std::string name;
		std::string rarity;
};

#endif