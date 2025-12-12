#include "MadJoker.hpp"

MadJoker::MadJoker() : cost(4), effect("+10 mult if a Two Pair is present"), name("Mad Joker") {}

MadJoker::~MadJoker() {}

MadJoker::MadJoker(const MadJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void MadJoker::printJoker(){
    // Implementazione UI se necessaria
}


void MadJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Two Pair")
    	mult += 10;
}

int MadJoker::getCost(){
	return cost;
}

std::string MadJoker::getName() {
	return name;
}