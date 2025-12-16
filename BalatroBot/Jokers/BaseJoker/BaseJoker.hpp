#ifndef BASEJOKER_HPP
#define BASEJOKER_HPP

#include "../IJoker.hpp"

class Balatro;

class BaseJoker : public IJoker {
	public:
		BaseJoker();
		BaseJoker(const BaseJoker& other);
		~BaseJoker();

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