#ifndef SCARYFACE_HPP
#define SCARYFACE_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class ScaryFace : public IJoker {

	public:
		ScaryFace();
		ScaryFace(const ScaryFace& other);
		~ScaryFace();

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