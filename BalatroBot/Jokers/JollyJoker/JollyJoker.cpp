#include "JollyJoker.hpp"

JollyJoker::JollyJoker() : cost(3), effect("+8 mult if a Pair is present"), name("Jolly Joker") {}

JollyJoker::~JollyJoker() {}

JollyJoker::JollyJoker(const JollyJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void JollyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void JollyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Pair")
    	mult += 8;
}

int JollyJoker::getCost(){
	return cost;
}

std::string JollyJoker::getName() {
	return name;
}