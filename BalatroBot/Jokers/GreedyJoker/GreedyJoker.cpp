#include "GreedyJoker.hpp"

GreedyJoker::GreedyJoker() : cost(5), effect("Played cards with Diamond suit give +3 Mult when scored"), name("Greedy Joker") {}

GreedyJoker::~GreedyJoker() {}

GreedyJoker::GreedyJoker(const GreedyJoker &other) : cost(other.cost), effect(other.effect) {}

void GreedyJoker::printJoker(){
    // Implementazione UI se necessaria
}

void GreedyJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips;
    mult += 4;
    for (std::vector<Card>::iterator it = bot->getSelectedCards().begin(); it != bot->getSelectedCards().end(); ++it) {
        if (it->getSuit() == "Diamonds") {
            mult += 3;
        }
    }
}

int GreedyJoker::getCost(){
	return cost;
}

std::string GreedyJoker::getName(){
	return name;
}