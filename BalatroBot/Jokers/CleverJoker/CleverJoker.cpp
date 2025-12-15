#include "CleverJoker.hpp"

CleverJoker::CleverJoker() : bot(), cost(4), effect("+80 chips if a Two Pair is present"), name("Clever Joker") {}

CleverJoker::~CleverJoker() {}

CleverJoker::CleverJoker(const CleverJoker &other) : bot(other.bot), cost(other.cost), effect(other.effect), name(other.name) {}

void CleverJoker::printJoker(){
    // Implementazione UI se necessaria
}


void CleverJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)mult;
	(void)bot;
	if (bot->getBestHandName() == "Two Pair")
    	chips += 80;
}

int CleverJoker::getCost(){
	return cost;
}

std::string CleverJoker::getName() {
	return name;
}

std::string CleverJoker::getEffect() {
	return effect;
}