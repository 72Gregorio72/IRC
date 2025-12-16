#include "ZanyJoker.hpp"

ZanyJoker::ZanyJoker() :bot(), cost(4), effect("+12 mult if a Three of a Kind is present"), name("Zany Joker"), rarity("Uncommon") {}

ZanyJoker::~ZanyJoker() {}

ZanyJoker::ZanyJoker(const ZanyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void ZanyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void ZanyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Three of a Kind")
    	mult += 12;
}

int ZanyJoker::getCost(){
	return cost;
}

std::string ZanyJoker::getName() {
	return name;
}

std::string ZanyJoker::getEffect() {
	return effect;
}

std::string ZanyJoker::getRarity() {
	return rarity;
}