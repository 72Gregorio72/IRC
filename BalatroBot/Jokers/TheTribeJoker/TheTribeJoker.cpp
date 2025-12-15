#include "TheTribeJoker.hpp"

TheTribeJoker::TheTribeJoker() :bot(), cost(8), effect("X2 Mult if played hand contains a Flush"), name("The Tribe"), rarity("Rare") {}

TheTribeJoker::~TheTribeJoker() {}

TheTribeJoker::TheTribeJoker(const TheTribeJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void TheTribeJoker::printJoker(){
    // Implementazione UI se necessaria
}


void TheTribeJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Flush")
    	mult *= 2;
}

int TheTribeJoker::getCost(){
	return cost;
}

std::string TheTribeJoker::getName() {
	return name;
}

std::string TheTribeJoker::getEffect() {
	return effect;
}

std::string TheTribeJoker::getRarity() {
	return rarity;
}