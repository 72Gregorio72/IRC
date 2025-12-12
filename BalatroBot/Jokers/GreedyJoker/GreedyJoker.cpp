#include "GreedyJoker.hpp"

GreedyJoker::GreedyJoker() : cost(2), effect("Played cards with Diamond suit give +3 Mult when scored") {}

GreedyJoker::~GreedyJoker() {}

GreedyJoker::GreedyJoker(const GreedyJoker &other) : cost(other.cost), effect(other.effect) {}

void GreedyJoker::printJoker(){
    // Implementazione UI se necessaria
}

void GreedyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
    mult += 4;
    for (std::vector<Cards>::iterator it = bot->getPlayedCards().begin(); it != bot->getPlayedCards().end(); ++it) {
        if (it->getSuit() == Diamond) {
            mult += 3;
        }
    }
}

int GreedyJoker::getCost(){
	return cost;
}