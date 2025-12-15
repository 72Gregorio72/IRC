#include "TheDuoJoker.hpp"

TheDuoJoker::TheDuoJoker() :bot(), cost(8), effect("X2 Mult if played hand contains a Pair"), name("The Duo"), rarity("Rare") {}

TheDuoJoker::~TheDuoJoker() {}

TheDuoJoker::TheDuoJoker(const TheDuoJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void TheDuoJoker::printJoker(){
    // Implementazione UI se necessaria
}


void TheDuoJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Pair")
    	mult *= 2;
}

int TheDuoJoker::getCost(){
	return cost;
}

std::string TheDuoJoker::getName() {
	return name;
}

std::string TheDuoJoker::getEffect() {
	return effect;
}

std::string TheDuoJoker::getRarity() {
	return rarity;
}