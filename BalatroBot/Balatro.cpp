#include "Balatro.hpp"

Balatro::Balatro() : ante(0), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(0), player(), pokerHands(), isSuitSorting(false) {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
}

Balatro::Balatro(int sd, User player) : ante(0), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(sd), player(player), pokerHands(), isRankSorting(false) {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
}
Balatro::~Balatro() {}

Balatro::Balatro(const Balatro &other)
	: ante(other.ante),
	  anteScore(other.anteScore),
	  discards(other.discards),
	  hand(other.hand),
	  deck(other.deck),
	  selectedCards(other.selectedCards),
	  hands(other.hands), 
	  coins(other.coins),
	  currentBet(other.currentBet),
	  totalBet(other.totalBet),
	  sd(other.sd),
	  player(other.player),
	  pokerHands(other.pokerHands),
	  isSuitSorting(other.isSuitSorting),
	  isRankSorting(other.isRankSorting) {}

Balatro &Balatro::operator=(const Balatro &other) {
	if (this != &other) {
		ante = other.ante;
		anteScore = other.anteScore;
		discards = other.discards;
		hand = other.hand;
		deck = other.deck;
		selectedCards = other.selectedCards;
		hands = other.hands;
		coins = other.coins;
		currentBet = other.currentBet;
		totalBet = other.totalBet;
		sd = other.sd;
		player = other.player;
		pokerHands = other.pokerHands;
		isSuitSorting = other.isSuitSorting;
		isRankSorting = other.isRankSorting;
	}
	return *this;
}

void Balatro::initPokerHands() {
	pokerHands.HighCard.setPokerHand("High Card", 1, 5, 1);
    pokerHands.OnePair.setPokerHand("Pair", 2, 10, 2);
    pokerHands.TwoPair.setPokerHand("Two Pair", 3, 20, 2);
    pokerHands.ThreeOfAKind.setPokerHand("Three of a Kind", 4, 30, 3);
    pokerHands.Straight.setPokerHand("Straight", 5, 30, 4);
    pokerHands.Flush.setPokerHand("Flush", 6, 35, 4);
    pokerHands.FullHouse.setPokerHand("Full House", 7, 40, 4);
    pokerHands.FourOfAKind.setPokerHand("Four of a Kind", 8, 60, 7);
    pokerHands.StraightFlush.setPokerHand("Straight Flush", 9, 100, 8);
    pokerHands.RoyalFlush.setPokerHand("Royal Flush", 10, 100, 8);
    pokerHands.FiveOfAKind.setPokerHand("Five of a Kind", 11, 120, 12);
    pokerHands.FlushHouse.setPokerHand("Flush House", 12, 140, 14);
    pokerHands.FlushFive.setPokerHand("Flush Five", 13, 160, 16);
}

void Balatro::startNewRound() {
	hand.clear();
	deck.clear();
	selectedCards.clear();
	currentBet = 0;
	totalBet = 0;
	discards = 4;
	hands = 4;
	std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
	std::string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 13; ++j) {
			deck.push_back(Card(suits[i], ranks[j]));
		}
	}
	shuffleDeck();
	dealInitialHand();
}

void Balatro::dealInitialHand() {
    hand.clear();
    for (int i = 0; i < 8 && !deck.empty(); ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    printSelectedCardsUI();
}

int getRankValue(std::string rank) {
    if (rank == "J") return 11;
    if (rank == "Q") return 12;
    if (rank == "K") return 13;
    if (rank == "A") return 14;
    return std::atol(rank.c_str());
}

bool compareCardsByRank(const Card& a, const Card& b) {
    int valA = getRankValue(a.getRank());
    int valB = getRankValue(b.getRank());
    
    if (valA != valB) {
        return valA > valB;
    }
    return a.getSuit() < b.getSuit();
}

bool compareCardsBySuit(const Card& a, const Card& b) {
    if (a.getSuit() != b.getSuit()) {
        return a.getSuit() < b.getSuit();
    }
    return getRankValue(a.getRank()) > getRankValue(b.getRank());
}

int getRankChips(std::string rank) {
    if (rank == "J" || rank == "Q" || rank == "K") return 10;
    if (rank == "A") return 11;
    return std::atol(rank.c_str());
}

int Balatro::calculateHand() {
    if (selectedCards.empty()) {
        std::cout << "DEBUG: Nessuna carta selezionata." << std::endl;
        return 0;
    }

    std::cout << "\n--- DEBUG: INIZIO CALCOLO MANO ---" << std::endl;

    bool isFlush = true;
    std::string firstSuit = selectedCards[0].getSuit();
    int handChipsFromCards = 0;
    
    std::map<std::string, int> rankCounts;
    std::vector<int> values;

    std::cout << "DEBUG: Carte selezionate (" << selectedCards.size() << "):" << std::endl;

    for (size_t i = 0; i < selectedCards.size(); ++i) {
        Card currentCard = selectedCards[i];
        
        int rVal = getRankValue(currentCard.getRank());
        int rChips = getRankChips(currentCard.getRank());
        
        std::cout << "  [" << i << "] " << currentCard.getRank() << " " << currentCard.getSuit() 
                  << " -> Valore: " << rVal << ", Chips: " << rChips << std::endl;

        if (currentCard.getSuit() != firstSuit) {
            isFlush = false;
        }
        
        rankCounts[currentCard.getRank()]++;
        handChipsFromCards += rChips;
        values.push_back(rVal);
    }

    std::cout << "DEBUG: Controllo Flush -> " << (isFlush ? "SI" : "NO") << std::endl;

    bool isStraight = false;
    if (selectedCards.size() == 5) {
        std::sort(values.begin(), values.end());
        
        std::cout << "DEBUG: Valori ordinati per Scala: ";
        for(size_t i=0; i<values.size(); ++i) std::cout << values[i] << " ";
        std::cout << std::endl;

        bool sequential = true;
        for (size_t i = 0; i < values.size() - 1; ++i) {
            if (values[i] + 1 != values[i+1]) {
                sequential = false;
                break;
            }
        }
        
        if (!sequential && values[0]==2 && values[1]==3 && values[2]==4 && values[3]==5 && values[4]==14) {
            std::cout << "DEBUG: Rilevata scala bassa A-5" << std::endl;
            sequential = true;
        }
        
        isStraight = sequential;
    }
    std::cout << "DEBUG: Controllo Straight -> " << (isStraight ? "SI" : "NO") << std::endl;

    bool hasFiveOfAKind = false;
    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    bool hasPair = false;
    int pairCount = 0;

    std::cout << "DEBUG: Conteggio Rank:" << std::endl;
    for (std::map<std::string, int>::iterator it = rankCounts.begin(); it != rankCounts.end(); ++it) {
        std::cout << "  Rank " << it->first << ": " << it->second << " volte" << std::endl;
        
        int count = it->second;

        if (count == 5) hasFiveOfAKind = true;
        if (count == 4) hasFourOfAKind = true;
        if (count == 3) hasThreeOfAKind = true;
        if (count == 2) {
            hasPair = true;
            pairCount++;
        }
    }

    std::cout << "DEBUG: Flags -> 5oaK: " << hasFiveOfAKind 
              << ", 4oaK: " << hasFourOfAKind 
              << ", 3oaK: " << hasThreeOfAKind 
              << ", Pairs: " << pairCount << std::endl;

    if (hasFiveOfAKind && isFlush) {
        std::cout << "DEBUG: RISULTATO -> Flush Five" << std::endl;
        return (pokerHands.FlushFive.getChips() + handChipsFromCards) * pokerHands.FlushFive.getMult();
    }

    if (isFlush && hasThreeOfAKind && hasPair) {
        std::cout << "DEBUG: RISULTATO -> Flush House" << std::endl;
        return (pokerHands.FlushHouse.getChips() + handChipsFromCards) * pokerHands.FlushHouse.getMult();
    }

    if (hasFiveOfAKind) {
        std::cout << "DEBUG: RISULTATO -> Five of a Kind" << std::endl;
        return (pokerHands.FiveOfAKind.getChips() + handChipsFromCards) * pokerHands.FiveOfAKind.getMult();
    }

    if (isFlush && isStraight && values.back() == 14 && values[0] == 10) { 
         std::cout << "DEBUG: RISULTATO -> Royal Flush" << std::endl;
         return (pokerHands.RoyalFlush.getChips() + handChipsFromCards) * pokerHands.RoyalFlush.getMult();
    }

    if (isFlush && isStraight) {
        std::cout << "DEBUG: RISULTATO -> Straight Flush" << std::endl;
        return (pokerHands.StraightFlush.getChips() + handChipsFromCards) * pokerHands.StraightFlush.getMult();
    }

    if (hasFourOfAKind) {
        std::cout << "DEBUG: RISULTATO -> Four of a Kind" << std::endl;
        return (pokerHands.FourOfAKind.getChips() + handChipsFromCards) * pokerHands.FourOfAKind.getMult();
    }

    if (hasThreeOfAKind && hasPair) {
        std::cout << "DEBUG: RISULTATO -> Full House" << std::endl;
        return (pokerHands.FullHouse.getChips() + handChipsFromCards) * pokerHands.FullHouse.getMult();
    }

    if (isFlush && selectedCards.size() == 5) {
        std::cout << "DEBUG: RISULTATO -> Flush" << std::endl;
        return (pokerHands.Flush.getChips() + handChipsFromCards) * pokerHands.Flush.getMult();
    }

    if (isStraight) {
        std::cout << "DEBUG: RISULTATO -> Straight" << std::endl;
        return (pokerHands.Straight.getChips() + handChipsFromCards) * pokerHands.Straight.getMult();
    }

    if (hasThreeOfAKind) {
        std::cout << "DEBUG: RISULTATO -> Three of a Kind" << std::endl;
        return (pokerHands.ThreeOfAKind.getChips() + handChipsFromCards) * pokerHands.ThreeOfAKind.getMult();
    }

    if (pairCount >= 2) {
        std::cout << "DEBUG: RISULTATO -> Two Pair" << std::endl;
        return (pokerHands.TwoPair.getChips() + handChipsFromCards) * pokerHands.TwoPair.getMult();
    }

    if (hasPair) {
        std::cout << "DEBUG: RISULTATO -> Pair" << std::endl;
        return (pokerHands.OnePair.getChips() + handChipsFromCards) * pokerHands.OnePair.getMult();
    }

    std::cout << "DEBUG: RISULTATO -> High Card" << std::endl;
    return (pokerHands.HighCard.getChips() + handChipsFromCards) * pokerHands.HighCard.getMult();
}

void Balatro::getMessagePrompt(std::string msg) {
	std::cout << "BalatroBot message: " << msg << std::endl;
	if (msg.find("!") == 0) {
		msg.erase(0, 1);
		std::cout << "Received command: " << msg << std::endl;
		if (msg.find("select ") == 0) {
			msg.erase(0, 7);
			selectedCards.clear();
			std::vector<std::string> tokens;
			size_t pos = 0;
			while ((pos = msg.find(" ")) != std::string::npos) {
				tokens.push_back(msg.substr(0, pos));
				msg.erase(0, pos + 1);
			}
			tokens.push_back(msg);
			if (tokens.size() < 1) {
				std::cout << "No card index provided." << std::endl;
				return;
			}
			std::vector<int> cardIndices;
			for (size_t i = 0; i < tokens.size(); ++i) {
				try {
					int index = std::atol(tokens[i].c_str());
					index--;
					std::cout << "Parsed index: " << index << std::endl;
					if (index >= 8){
						std::string msg = ":BalatroBot PRIVMSG " + player.getNickName() + " :Invalid card index: " + tokens[i] + "\r\n";
						send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
						return;
					}
					cardIndices.push_back(index);
				} catch (const std::invalid_argument&) {
					std::cout << "Invalid card index: " << tokens[i] << std::endl;
					return;
				}
			}
			if (cardIndices.size() > 5) {
				std::cout << "Too many cards selected." << std::endl;
				return;
			}
			for(size_t i = 0; i < cardIndices.size(); ++i) {
				std::cout << "Selected card: " << hand[cardIndices[i]].getRank() << " of " << hand[cardIndices[i]].getSuit() << std::endl;
			}
			for(size_t i = 0; i < cardIndices.size(); ++i) {
				selectedCards.push_back(hand[cardIndices[i]]);
			}
			printSelectedCardsUI();
		} else if (msg.find("discard") == 0) {
			msg.erase(0, 7);
			if (selectedCards.empty()) {
				std::string msg = ":BalatroBot PRIVMSG " + player.getNickName() + " :No cards selected to discard.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			if (discards == 0){
				std::string msg = ":BalatroBot PRIVMSG " + player.getNickName() + " :You have finished your discards.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			for(size_t i = 0; i < selectedCards.size(); ++i) {
				for (size_t j = 0; j < hand.size(); ++j) {
					if (hand[j].getRank() == selectedCards[i].getRank() &&
						hand[j].getSuit() == selectedCards[i].getSuit()) {
						hand.erase(hand.begin() + j);
						break;
					}
				}
			}
			while (hand.size() < 8 && !deck.empty()) {
				hand.push_back(deck.back());
				deck.pop_back();
			}
			selectedCards.clear();
			discards--;
			if (isSuitSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsBySuit);
			if (isRankSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsByRank);
			printSelectedCardsUI();
		} else if (msg.find("play") == 0) {
			msg.erase(0, 4);
			playHand();
			selectedCards.clear();
			printSelectedCardsUI();
		} else if (msg.find("sort suit") == 0) {
            std::sort(hand.begin(), hand.end(), compareCardsBySuit);
            printSelectedCardsUI();
            isSuitSorting = true;
			isRankSorting = false;
        } else if (msg.find("sort rank") == 0) {
            std::sort(hand.begin(), hand.end(), compareCardsByRank);
            printSelectedCardsUI();
			isSuitSorting = false;
			isRankSorting = true;
        }
	}
}

int Balatro::calculateAnteScore() {
    int currentAnte = (ante < 1) ? 1 : ante;
    
    double baseChips = 300.0;
    double growthFactor = 1.5; 
    
    double target = baseChips * std::pow(growthFactor, currentAnte - 1);
    
    return static_cast<int>(target);
}

void Balatro::playHand() {
	totalBet += calculateHand();
	for(size_t i = 0; i < selectedCards.size(); ++i) {
		for (size_t j = 0; j < hand.size(); ++j) {
			if (hand[j].getRank() == selectedCards[i].getRank() &&
				hand[j].getSuit() == selectedCards[i].getSuit()) {
				hand.erase(hand.begin() + j);
				break;
			}
		}
	}
	if (totalBet >= anteScore){
		ante++;
		startNewRound();
	}
	std::cout << "current score: " << totalBet << std::endl;
}

int Balatro::getSd() {
	return sd;
}

void Balatro::shuffleDeck(){
	std::random_shuffle(deck.begin(), deck.end());
}

void Balatro::startNewGame() {
	startNewRound();
	initPokerHands();
	ante = 0;
	anteScore = calculateAnteScore();
}