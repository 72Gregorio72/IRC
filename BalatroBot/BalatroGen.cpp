#include "includes/Balatro.hpp"
#include "Jokers/BaseJoker/BaseJoker.hpp"
#include "Jokers/GreedyJoker/GreedyJoker.hpp"
#include "Jokers/WrathfulJoker/WrathfulJoker.hpp"
#include "Jokers/LustyJoker/LustyJoker.hpp"
#include "Jokers/GluttonousJoker/GluttonousJoker.hpp"
#include "Jokers/JollyJoker/JollyJoker.hpp"
#include "Jokers/ZanyJoker/ZanyJoker.hpp"
#include "Jokers/MadJoker/MadJoker.hpp"
#include "Jokers/CrazyJoker/CrazyJoker.hpp"
#include "Jokers/DrollJoker/DrollJoker.hpp"
#include "Jokers/SlyJoker/SlyJoker.hpp"
#include "Jokers/WilyJoker/WilyJoker.hpp"
#include "Jokers/CleverJoker/CleverJoker.hpp"
#include "Jokers/DeviousJoker/DeviousJoker.hpp"
#include "Jokers/CraftyJoker/CraftyJoker.hpp"

void Balatro::dealInitialHand() {
    hand.clear();
    for (int i = 0; i < 8 && !deck.empty(); ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

bool Balatro::isGameOver() {
	return gameOver;
}

void	Balatro::setGameOver(bool value) {
	gameOver = value;
}

std::string Balatro::getBestHandName() const {
	return bestHandName;
}

int Balatro::calculateAnteScore() {
    int currentAnte = (ante < 1) ? 1 : ante;
    
    double baseChips = 300.0;
    double growthFactor = 1.5; 
    
    double target = baseChips * std::pow(growthFactor, currentAnte - 1);
    
    return static_cast<int>(target);
}

void Balatro::freeJokers() {
    jokers.clear();
}

int Balatro::getSd() {
	return sd;
}

void Balatro::shuffleDeck(){
	std::random_shuffle(deck.begin(), deck.end());
}

void Balatro::startNewGame() {
    jokers.clear();
    shopJokers.clear();
    for (size_t i = 0; i < allJokers.size(); ++i) delete allJokers[i];
    allJokers.clear();

    initAllJokers();

    startNewRound();
    initPokerHands();
    ante = 1;
    anteScore = calculateAnteScore();
    printUI();
}