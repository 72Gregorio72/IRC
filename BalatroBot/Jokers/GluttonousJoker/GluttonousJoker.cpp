#include "GluttonousJoker.hpp"

GluttonousJoker::GluttonousJoker() : cost(5), effect("Played cards with club suit give +3 Mult when scored"), name("Glottonous Joker") {}

GluttonousJoker::~GluttonousJoker() {}

GluttonousJoker::GluttonousJoker(const GluttonousJoker &other) : cost(other.cost), effect(other.effect), name(other.name) {}

void GluttonousJoker::printJoker(){
    // Implementazione UI se necessaria
}

void GluttonousJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips;
    mult += 4;
    for (std::vector<Card>::iterator it = bot->getSelectedCards().begin(); it != bot->getSelectedCards().end(); ++it) {
        if (it->getSuit() == "Clubs") {
            mult += 3;
        }
    }
}

int GluttonousJoker::getCost(){
	return cost;
}

std::string GluttonousJoker::getName(){
	return name;
}

std::string GluttonousJoker::getEffect(){
	return effect;
}