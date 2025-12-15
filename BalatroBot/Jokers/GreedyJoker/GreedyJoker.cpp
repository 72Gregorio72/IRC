#include "GreedyJoker.hpp"

GreedyJoker::GreedyJoker() :bot(), cost(5), effect("Played cards with Diamond suit give +3 Mult when scored"), name("Greedy Joker") {}

GreedyJoker::~GreedyJoker() {}

GreedyJoker::GreedyJoker(const GreedyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect) {}

void GreedyJoker::printJoker(){
    // Implementazione UI se necessaria
}

void GreedyJoker::playJoker(int& chips, int& mult, Balatro *bot) {
    (void)chips; // Questo joker non modifica le chips

    // 1. Salviamo le carte in una variabile locale per evitare il crash
    std::vector<Card> playedCards = bot->getSelectedCards(); 

    // 2. Iteriamo sulla variabile locale 'playedCards'
    for (std::vector<Card>::iterator it = playedCards.begin(); it != playedCards.end(); ++it) {
        if (it->getSuit() == "Diamonds") {
            mult += 3; // +3 Mult per ogni carta di Fiori giocata
        }
    }
}

int GreedyJoker::getCost(){
	return cost;
}

std::string GreedyJoker::getName(){
	return name;
}

std::string GreedyJoker::getEffect(){
	return effect;
}