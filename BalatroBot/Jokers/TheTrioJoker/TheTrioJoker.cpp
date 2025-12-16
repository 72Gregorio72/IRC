#include "TheTrioJoker.hpp"

TheTrioJoker::TheTrioJoker() :bot(), cost(8), effect("X3 Mult if played hand contains a Three of a Kind"), name("The Trio"), rarity("Rare") {}

TheTrioJoker::~TheTrioJoker() {}

TheTrioJoker::TheTrioJoker(const TheTrioJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void TheTrioJoker::printJoker(){
    // Implementazione UI se necessaria
}


void TheTrioJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Three of a Kind")
    	mult *= 3;
}

int TheTrioJoker::getCost(){
	return cost;
}

std::string TheTrioJoker::getName() {
	return name;
}

std::string TheTrioJoker::getEffect() {
	return effect;
}

std::string TheTrioJoker::getRarity() {
	return rarity;
}