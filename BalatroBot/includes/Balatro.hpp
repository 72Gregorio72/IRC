#ifndef BALATRO_HPP
#define BALATRO_HPP

#include "ft_irc.h"
#include "User.hpp"
#include "Card.hpp"
#include "PokerHand.hpp"
#include <algorithm>
#include "../Jokers/IJoker.hpp"
#include "../Planets/IPlanet.hpp"

class IJoker;
class IPlanet;
class PokerHand;
class Card;
class User;

struct PokerHands {
    PokerHand HighCard;
    PokerHand OnePair;
    PokerHand TwoPair;
    PokerHand ThreeOfAKind;
    PokerHand Straight;
    PokerHand Flush;
    PokerHand FullHouse;
    PokerHand FourOfAKind;
    PokerHand StraightFlush;
    PokerHand RoyalFlush;
    PokerHand FiveOfAKind;
    PokerHand FlushHouse;
    PokerHand FlushFive;
} typedef PokerHands;

struct RowData {
    std::string label;
    int val;
    std::string color;
    int hiddenColorLen;
};

class Balatro {
    public:
        Balatro();
        ~Balatro();
        Balatro(const Balatro &other);
        Balatro &operator=(const Balatro &other);
        Balatro(int sd, User *player);

        int getAnte() const;
        int getDiscards();
        std::vector<Card> getHand() const;
        std::vector<Card> getDeck();
        int getHands();
        int getCoins();
		int getChips();
		void setCoins(int newCoins);
        int getCurrentBet() const;
        int getTotalBet() const;
        std::string getBestHandName() const;

        void setAnte(int newAnte);
        void setDiscards(int newDiscards);
        void setHand(const std::vector<Card> &newHand);
        void setDeck(const std::vector<Card> &newDeck);
        void setHands(int newHands);
        void setCurrentBet(int newCurrentBet);
        void setTotalBet(int newTotalBet);

        void startNewRound();
        void startNewGame();
        void shuffleDeck();
        void dealInitialHand();
        void playHand();

        void printUI();
        void printSelectedCardsUI();
        void printEndRoundUI();

        int calculateAnteScore();

        // std::vector<std::string> createMsgBox(std::string text, std::string colorCode);
        // std::vector<std::string> createSimpleItem(int id, int cost);
        // void pasteObject(std::vector<std::string>& canvas, const std::vector<std::string>& object, int startRow, int startCol);
        // void initPokerHands();
        // void printShopUI();
        // std::vector<std::string> createButton(std::string text, std::string subtext, std::string bgColorCode);
        
        // String helpers
        std::string repeat_char(int count, char c);
		std::vector<std::string> createMsgBox(std::string text, std::string colorCode);
		std::vector<std::string> createSimpleItem(int id, int cost);
		void pasteObject(std::vector<std::string>& canvas, const std::vector<std::string>& object, int startRow, int startCol);
		void initPokerHands();
		void printShopUI();
		void printWinUI();
		std::vector<std::string> createButton(std::string text, std::string subtext, std::string bgColorCode);
		// std::string repeat_char(int count, char c);

        template <typename T>
        std::string to_string_98(T value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }

        std::vector<std::string> getCardRows(const Card& c);
        std::vector<std::string> getCardRowsSelected(const Card& c, bool isSelected);
        std::vector<std::string> printDeck();
        std::string centerText(std::string text, int width);
        void getLeftPanelContent(int row, std::string& raw, std::string& colored);
        std::string getRightPanelContent(int row, int handStart, int handH, int deckStart, int deckH, 
                                 const std::vector<std::vector<std::string> >& cardMatrix, 
                                 const std::vector<std::string>& deckVisual);

        std::vector<std::string> getCombinedJokersVisual(const std::vector<IJoker*>& targetJokers);
        std::vector<std::string> getCombinedPlanetsVisual(const std::vector<IPlanet*>& targetPlanets);

        std::string getSpaces(int count);
        
        
        int getVisualLength(const std::string& s);

        int getSd();
        void getMessagePrompt(std::string msg);
        bool    isGameOver();
        void    setGameOver(bool value);
        int calculateHand();

        void freeJokers();
        void initAllJokers();
        void initPlanets();
        
        // Shop and Pack generation
        void generateShopJokers();
        void generatePackJokers(); // Added to match definition in JokerPackUI.cpp
        void generatePackPlanets();

        std::vector<std::string> createJokerItem(IJoker* joker);
        std::vector<std::string> createPlanetItem(IPlanet* planet);
        const std::vector<Card>& getSelectedCards() const;

        void jokerPackUI(); // Renamed from jokerPackUI to match definition in JokerPackUI.cpp
		std::string repeat_string(int count, const std::string& pattern);
		void pickJokerFromPack(int index);
        void pickPlanetFromPack(int index);
        void planetPackUI();
		std::vector<IJoker*> getAllJokers() const;

		std::vector<Card> getHandCards();
        PokerHand& getPokerHands(std::string handName);

        std::vector<std::string> createPackItem(std::string type, int cost, std::string color);
    
        std::vector<int>    findCommandIndices(std::string input);
        void                orderVectorIndices(std::vector<int> &input);
    private:

        bool    gameOver;
        bool    gameWon;

        int ante;
        int anteScore;
        int discards;
        std::vector<Card> hand;
        std::vector<Card> deck;
        std::vector<Card> selectedCards;
        int hands;
        int coins;
        int currentBet;
        int totalBet;
        int sd;
        User *player;
        PokerHands pokerHands;
        bool isSuitSorting;
        bool isRankSorting;
        bool isCashingOut;
        bool isShopUI;
        std::vector<IJoker*> jokers;
        std::vector<IJoker*> allJokers; 
        std::vector<IJoker*> shopJokers;
        std::vector<IPlanet*> allPlanets;
        std::string bestHandName;
        int pendingShopIndex;
        int rollPrice;
        
        int blind; // 0 small 1 big 2 boss
        std::vector<IJoker*> packJokers;
        std::vector<IPlanet*> packPlanets;
		int isInJokerPackUI;
        int isInPlanetPackUI;
};

#endif