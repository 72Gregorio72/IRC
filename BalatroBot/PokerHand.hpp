#ifndef POKER_HANDS_HPP
#define POKER_HANDS_HPP

#include <vector>
#include <algorithm>
#include "Card.hpp"

class Card;

class PokerHand {
	private:
		std::string name;
		int rank;
		int baseMult;
		int baseChips;

	public:
		PokerHand(const std::string& name, int rank);
		~PokerHand();
		PokerHand();
		PokerHand &operator=(const PokerHand &other);

		std::string getName();
		int getRank();
		int getChips();
		int getMult();

		void setPokerHand(std::string name, int rank, int baseChips, int baseMult);
};

#endif