#include "Banner.hpp"

Banner::Banner() :bot(), cost(5), effect("+30 Chips for each remaining discard"), name("Banner"), rarity("Common") {}

Banner::~Banner() {}

Banner::Banner(const Banner &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Banner::printJoker(){
    // Implementazione UI se necessaria
}


void Banner::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	int remainingDiscards = bot->getDiscards();
	chips += remainingDiscards * 30;
}

int Banner::getCost(){
	return cost;
}

std::string Banner::getName() {
	return name;
}

std::string Banner::getEffect() {
	return effect;
}

std::string Banner::getRarity() {
	return rarity;
}