#include "SlyJoker.hpp"

SlyJoker::SlyJoker() :bot(), cost(3), effect("+50 chips if a Pair is present"), name("Sly Joker"), rarity("Common") {}

SlyJoker::~SlyJoker() {}

SlyJoker::SlyJoker(const SlyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void SlyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void SlyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)mult;
	(void)bot;
	if (bot->getBestHandName() == "Pair")
    	chips += 50;
}

int SlyJoker::getCost(){
	return cost;
}

std::string SlyJoker::getName() {
	return name;
}


std::string SlyJoker::getEffect() {
	return effect;
}

std::string SlyJoker::getRarity() {
	return rarity;
}