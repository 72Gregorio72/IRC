#include "DeviousJoker.hpp"

DeviousJoker::DeviousJoker() : cost(4), effect("+100 chips if a Straight is present"), name("Devious Joker") {}

DeviousJoker::~DeviousJoker() {}

DeviousJoker::DeviousJoker(const DeviousJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

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