#include "TheOrderJoker.hpp"

TheOrderJoker::TheOrderJoker() :bot(), cost(8), effect("X3 Mult if played hand contains a Straight"), name("The Order"), rarity("Rare") {}

TheOrderJoker::~TheOrderJoker() {}

TheOrderJoker::TheOrderJoker(const TheOrderJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void TheOrderJoker::printJoker(){
    // Implementazione UI se necessaria
}


void TheOrderJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Straight")
    	mult *= 3;
}

int TheOrderJoker::getCost(){
	return cost;
}

std::string TheOrderJoker::getName() {
	return name;
}

std::string TheOrderJoker::getEffect() {
	return effect;
}

std::string TheOrderJoker::getRarity() {
	return rarity;
}