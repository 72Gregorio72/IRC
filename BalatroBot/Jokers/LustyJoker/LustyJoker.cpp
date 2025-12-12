#include "LustyJoker.hpp"

LustyJoker::LustyJoker() : cost(5), effect("Played cards with Heart suit give +3 Mult when scored"), name("Lusty Joker"){}

LustyJoker::~LustyJoker() {}

LustyJoker::LustyJoker(const LustyJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void LustyJoker::printJoker(){
    // Implementazione UI se necessaria
}

std::string LustyJoker::getName(){
    return name;
}

void LustyJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips;
    mult += 4;
    for (std::vector<Card>::iterator it = bot->getSelectedCards().begin(); it != bot->getSelectedCards().end(); ++it) {
        if (it->getSuit() == "Hearts") {
            mult += 3;
        }
    }
}

int LustyJoker::getCost(){
	return cost;
}