#include "WrathfulJoker.hpp"

WrathfulJoker::WrathfulJoker() :bot(), cost(5), effect("Played cards with Spade suit give +3 Mult when scored") ,name("Wrathful Joker"), rarity("Uncommon"){}

WrathfulJoker::~WrathfulJoker() {}

WrathfulJoker::WrathfulJoker(const WrathfulJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

std::string WrathfulJoker::getName(){
    return name;
}

std::string WrathfulJoker::getEffect(){
    return effect;
}

std::string WrathfulJoker::getRarity(){
	return rarity;
}

void WrathfulJoker::printJoker(){
    // Implementazione UI se necessaria
}

void WrathfulJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips; // Questo joker non modifica le chips

    // 1. Salviamo le carte in una variabile locale per evitare il crash
    std::vector<Card> playedCards = bot->getSelectedCards(); 

    // 2. Iteriamo sulla variabile locale 'playedCards'
    for (std::vector<Card>::iterator it = playedCards.begin(); it != playedCards.end(); ++it) {
        if (it->getSuit() == "Spades") {
            mult += 3; // +3 Mult per ogni carta di Fiori giocata
        }
    }
}

int WrathfulJoker::getCost(){
	return cost;
}