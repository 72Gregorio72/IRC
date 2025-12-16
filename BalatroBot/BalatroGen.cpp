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
	std::cout << "Calculating ante score for ante level: " << ante << " and blind level: " << blind << std::endl;
    long long baseScores[] = {
        300, 800, 2800, 6000, 11000, 20000, 35000, 50000
    };

	if (blind == 0) {
		return baseScores[ante - 1];
	} else if (blind == 1) {
		return baseScores[ante - 1] * 1.5;
	} else {
		return baseScores[ante - 1] * 2;
	}

	return 0;
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
    coins = 0;
	blind = -1;
    jokers.clear();
    shopJokers.clear();
    for (size_t i = 0; i < allJokers.size(); ++i) delete allJokers[i];
    allJokers.clear();

    initAllJokers();

    ante = 1;
    startNewRound();
    initPokerHands();
    printUI();
}