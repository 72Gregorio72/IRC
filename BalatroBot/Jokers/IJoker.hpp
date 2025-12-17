#ifndef IJOKER_HPP
#define IJOKER_HPP

#include "ft_irc.h"
#include "../includes/Card.hpp"
#include "../includes/Balatro.hpp"
#include "../includes/PokerHand.hpp"

class Card;
class Balatro;
class PokerHand;

class IJoker
{
    public:
        virtual ~IJoker() {}
        virtual void printJoker() = 0;
        virtual void playJoker(int &chips, int &mult, Balatro *bot) = 0;

		virtual int getCost() = 0;
		virtual std::string getName() = 0;
		virtual std::string getEffect() = 0;
        virtual std::string getRarity() = 0;
};

#endif