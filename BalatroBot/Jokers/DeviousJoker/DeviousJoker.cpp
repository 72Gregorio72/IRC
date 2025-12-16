#include "DeviousJoker.hpp"

DeviousJoker::DeviousJoker() : bot(), cost(4), effect("+100 chips if a Straight is present"), name("Devious Joker"), rarity("Uncommon") {}

DeviousJoker::~DeviousJoker() {}

DeviousJoker::DeviousJoker(const DeviousJoker &other) : bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void DeviousJoker::printJoker(){
    // Implementazione UI se necessaria
}


void DeviousJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)mult;
	(void)bot;
	if (bot->getBestHandName() == "Straight")
    	chips += 100;
}

int DeviousJoker::getCost(){
	return cost;
}

std::string DeviousJoker::getName() {
	return name;
}

std::string DeviousJoker::getEffect() {
	return effect;
}

std::string DeviousJoker::getRarity() {
	return rarity;
}