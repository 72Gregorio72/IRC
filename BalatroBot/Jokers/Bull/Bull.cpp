#include "Bull.hpp"

Bull::Bull() :bot(), cost(6), effect("+2 Chips for each $1 you have"), name("Bull"), rarity("Uncommon") {}

Bull::~Bull() {}

Bull::Bull(const Bull &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Bull::printJoker(){
    // Implementazione UI se necessaria
}


void Bull::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	int currentCoins = bot->getCoins();
	chips += currentCoins * 2;
}

int Bull::getCost(){
	return cost;
}

std::string Bull::getName() {
	return name;
}

std::string Bull::getEffect() {
	return effect;
}

std::string Bull::getRarity() {
	return rarity;
}