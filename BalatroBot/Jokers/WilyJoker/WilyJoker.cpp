#include "WilyJoker.hpp"

WilyJoker::WilyJoker() :bot(), cost(4), effect("+100 chips if a Three of a Kind is present"), name("Wily Joker") {}

WilyJoker::~WilyJoker() {}

WilyJoker::WilyJoker(const WilyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name) {}

void WilyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void WilyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)mult;
	(void)bot;
	if (bot->getBestHandName() == "Three of a Kind")
    	chips += 100;
}

int WilyJoker::getCost(){
	return cost;
}

std::string WilyJoker::getName() {
	return name;
}


std::string WilyJoker::getEffect() {
	return effect;
}