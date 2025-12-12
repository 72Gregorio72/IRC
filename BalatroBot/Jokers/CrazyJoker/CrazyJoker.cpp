#include "CrazyJoker.hpp"

CrazyJoker::CrazyJoker() : cost(4), effect("+12 mult if a Straight is present"), name("Crazy Joker") {}

CrazyJoker::~CrazyJoker() {}

CrazyJoker::CrazyJoker(const CrazyJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

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