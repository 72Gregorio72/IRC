#include "CrazyJoker.hpp"

CrazyJoker::CrazyJoker() : bot(), cost(4), effect("+12 mult if a Straight is present"), name("Crazy Joker"), rarity("Uncommon") {}

CrazyJoker::~CrazyJoker() {}

CrazyJoker::CrazyJoker(const CrazyJoker &other) : bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void CrazyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void CrazyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Straight")
    	mult += 12;
}

int CrazyJoker::getCost(){
	return cost;
}

std::string CrazyJoker::getName() {
	return name;
}

std::string CrazyJoker::getEffect() {
	return effect;
}

std::string CrazyJoker::getRarity() {
	return rarity;
}