#ifndef GLUTTONOUSJOKER_HPP
#define GLUTTONOUSJOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class GluttonousJoker : public IJoker {
	public:
		GluttonousJoker();
		GluttonousJoker(const GluttonousJoker& other);
		~GluttonousJoker();

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