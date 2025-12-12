#include "CraftyJoker.hpp"

CraftyJoker::CraftyJoker() : cost(4), effect("+80 chips if a Flush is present"), name("Crafty Joker") {}

CraftyJoker::~CraftyJoker() {}

CraftyJoker::CraftyJoker(const CraftyJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void CraftyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void CraftyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
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