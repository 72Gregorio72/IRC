#include "WrathfulJoker.hpp"

WrathfulJoker::WrathfulJoker() : cost(5), effect("Played cards with Spade suit give +3 Mult when scored") ,name("Wrathful Joker"){}

WrathfulJoker::~WrathfulJoker() {}

WrathfulJoker::WrathfulJoker(const WrathfulJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

std::string WrathfulJoker::getName(){
    return name;
}

std::string WrathfulJoker::getEffect(){
    return effect;
}

void WrathfulJoker::printJoker(){
    // Implementazione UI se necessaria
}

void WrathfulJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips;
    mult += 4;
    for (std::vector<Card>::iterator it = bot->getSelectedCards().begin(); it != bot->getSelectedCards().end(); ++it) {
        if (it->getSuit() == "Spades") {
            mult += 3;
        }
    }
}

int WrathfulJoker::getCost(){
	return cost;
}