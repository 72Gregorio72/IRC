#include "DrollJoker.hpp"

DrollJoker::DrollJoker() : cost(4), effect("+10 mult if a Flush is present"), name("Droll Joker") {}

DrollJoker::~DrollJoker() {}

DrollJoker::DrollJoker(const DrollJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void DrollJoker::printJoker(){
    // Implementazione UI se necessaria
}


void DrollJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Flush")
    	mult += 10;
}

int DrollJoker::getCost(){
	return cost;
}

std::string DrollJoker::getName() {
	return name;
}

std::string DrollJoker::getEffect() {
	return effect;
}