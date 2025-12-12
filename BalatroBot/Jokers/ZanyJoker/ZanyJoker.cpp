#include "ZanyJoker.hpp"

ZanyJoker::ZanyJoker() : cost(4), effect("+12 mult if a Three of a Kind is present"), name("Zany Joker") {}

ZanyJoker::~ZanyJoker() {}

ZanyJoker::ZanyJoker(const ZanyJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

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