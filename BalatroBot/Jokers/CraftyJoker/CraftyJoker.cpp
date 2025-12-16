#include "CraftyJoker.hpp"

CraftyJoker::CraftyJoker() : bot(), cost(4), effect("+80 chips if a Flush is present"), name("Crafty Joker"), rarity("Common") {}

CraftyJoker::~CraftyJoker() {}

CraftyJoker::CraftyJoker(const CraftyJoker &other) : bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void CraftyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void CraftyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)mult;
	(void)bot;
	if (bot->getBestHandName() == "Flush")
    	chips += 80;
}

int CraftyJoker::getCost(){
	return cost;
}

std::string CraftyJoker::getName() {
	return name;
}

std::string CraftyJoker::getEffect() {
	return effect;
}

std::string CraftyJoker::getRarity() {
	return rarity;
}