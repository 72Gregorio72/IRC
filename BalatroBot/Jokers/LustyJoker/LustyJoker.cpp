#include "LustyJoker.hpp"

LustyJoker::LustyJoker() :bot(), cost(5), effect("Played cards with Heart suit give +3 Mult when scored"), name("Lusty Joker"){}

LustyJoker::~LustyJoker() {}

LustyJoker::LustyJoker(const LustyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name) {}

void LustyJoker::printJoker(){
    // Implementazione UI se necessaria
}

std::string LustyJoker::getName(){
    return name;
}

std::string LustyJoker::getEffect(){
    return effect;
}

void LustyJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips; // Questo joker non modifica le chips

    // 1. Salviamo le carte in una variabile locale per evitare il crash
    std::vector<Card> playedCards = bot->getSelectedCards(); 

    // 2. Iteriamo sulla variabile locale 'playedCards'
    for (std::vector<Card>::iterator it = playedCards.begin(); it != playedCards.end(); ++it) {
        if (it->getSuit() == "Hearts") {
            mult += 3; // +3 Mult per ogni carta di Fiori giocata
        }
    }
}

int LustyJoker::getCost(){
	return cost;
}