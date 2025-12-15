#include "GluttonousJoker.hpp"

GluttonousJoker::GluttonousJoker() :bot(), cost(5), effect("Played cards with club suit give +3 Mult when scored"), name("Gluttonous Joker") {}

GluttonousJoker::~GluttonousJoker() {}

GluttonousJoker::GluttonousJoker(const GluttonousJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name) {}

void GluttonousJoker::printJoker(){
    // Implementazione UI se necessaria
}

void GluttonousJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips; // Questo joker non modifica le chips

    // 1. Salviamo le carte in una variabile locale per evitare il crash
    std::vector<Card> playedCards = bot->getSelectedCards(); 

    // 2. Iteriamo sulla variabile locale 'playedCards'
    for (std::vector<Card>::iterator it = playedCards.begin(); it != playedCards.end(); ++it) {
        if (it->getSuit() == "Clubs") {
            mult += 3; // +3 Mult per ogni carta di Fiori giocata
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