#include "BaseJoker.hpp"

BaseJoker::BaseJoker() : cost(2), effect("+4 Mult") {}

BaseJoker::~BaseJoker() {}

BaseJoker::BaseJoker(const BaseJoker &other) : cost(other.cost), effect(other.effect) {}

void BaseJoker::printJoker(){
    // Implementazione UI se necessaria
}

void BaseJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
    mult += 4;
}

int BaseJoker::getCost(){
	return cost;
}