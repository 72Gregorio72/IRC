#include "BootStraps.hpp"

BootStraps::BootStraps() :bot(), cost(7), effect("+2 Mult for every $5 you have"), name("BootStraps"), rarity("Uncommon") {}

BootStraps::~BootStraps() {}

BootStraps::BootStraps(const BootStraps &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BootStraps::printJoker(){
    // Implementazione UI se necessaria
}


void BootStraps::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	int bonusMult = (bot->getCoins() / 5) * 2;
	mult += bonusMult;
}

int BootStraps::getCost(){
	return cost;
}

std::string BootStraps::getName() {
	return name;
}

std::string BootStraps::getEffect() {
	return effect;
}

std::string BootStraps::getRarity() {
	return rarity;
}