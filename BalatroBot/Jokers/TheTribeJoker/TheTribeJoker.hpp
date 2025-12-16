#ifndef THE_TRIBE_JOKER_HPP
#define THE_TRIBE_JOKER_HPP

#include "../IJoker.hpp"
#include "Server.hpp"

class Server;

class TheTribeJoker : public IJoker {

	public:
		TheTribeJoker();
		TheTribeJoker(const TheTribeJoker& other);
		~TheTribeJoker();

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