#include "includes/Balatro.hpp"
#include "Jokers/BaseJoker/BaseJoker.hpp"

Balatro::Balatro() : gameOver(false), ante(1), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(0), player(), pokerHands(), isSuitSorting(false), isCashingOut(false), jokers(), allJokers() {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
}

Balatro::Balatro(int sd, User *player) : gameOver(false), ante(1), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(sd), player(player), pokerHands(), isRankSorting(false), isCashingOut(false), jokers(), allJokers() {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
}
Balatro::~Balatro() {}

Balatro::Balatro(const Balatro &other)
	: gameOver(other.gameOver),
	  ante(other.ante),
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
	  isRankSorting(other.isRankSorting), 
	  isCashingOut(other.isCashingOut), jokers() {}

Balatro &Balatro::operator=(const Balatro &other) {
	if (this != &other) {
		gameOver = other.gameOver;
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
		isCashingOut = other.isCashingOut;
		jokers = other.jokers;
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

void Balatro::initAllJokers() {
	allJokers.push_back(BaseJoker());
	allJokers.push_back(GreedyJoker());
}

void Balatro::startNewRound() {
	anteScore = calculateAnteScore();
	hand.clear();
	deck.clear();
	selectedCards.clear();
	gameOver = false;
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
}

int getRankValue(std::string rank) {
    if (rank == "J") return 11;
    if (rank == "Q") return 12;
    if (rank == "K") return 13;
    if (rank == "A") return 14;
    return std::atol(rank.c_str());
}

bool Balatro::isGameOver() {
	return gameOver;
}

void	Balatro::setGameOver(bool value) {
	gameOver = value;
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

// ... (codice precedente invariato fino a calculateHand)

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
        int rChips = getRankChips(currentCard.getRank()); // Assumiamo che getRankChips sia accessibile o definito prima
        
        // ... (Logica di print invariata)

        if (currentCard.getSuit() != firstSuit) {
            isFlush = false;
        }
        
        rankCounts[currentCard.getRank()]++;
        handChipsFromCards += rChips;
        values.push_back(rVal);
    }

    // ... (Logica di controllo Straight invariata) ...
    bool isStraight = false;
    if (selectedCards.size() == 5) {
        std::sort(values.begin(), values.end());
        bool sequential = true;
        for (size_t i = 0; i < values.size() - 1; ++i) {
            if (values[i] + 1 != values[i+1]) {
                sequential = false;
                break;
            }
        }
        if (!sequential && values[0]==2 && values[1]==3 && values[2]==4 && values[3]==5 && values[4]==14) {
            sequential = true;
        }
        isStraight = sequential;
    }
    // ...

    bool hasFiveOfAKind = false;
    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    bool hasPair = false;
    int pairCount = 0;

    // ... (Logica conteggio rank invariata) ...
    for (std::map<std::string, int>::iterator it = rankCounts.begin(); it != rankCounts.end(); ++it) {
        int count = it->second;
        if (count == 5) hasFiveOfAKind = true;
        if (count == 4) hasFourOfAKind = true;
        if (count == 3) hasThreeOfAKind = true;
        if (count == 2) {
            hasPair = true;
            pairCount++;
        }
    }

    // --- IDENTIFICAZIONE MANO MIGLIORE ---
    PokerHand* bestHand = &pokerHands.HighCard; // Default

    if (hasFiveOfAKind && isFlush) {
        bestHand = &pokerHands.FlushFive;
    } else if (isFlush && hasThreeOfAKind && hasPair) {
        bestHand = &pokerHands.FlushHouse;
    } else if (hasFiveOfAKind) {
        bestHand = &pokerHands.FiveOfAKind;
    } else if (isFlush && isStraight && values.back() == 14 && values[0] == 10) { 
        bestHand = &pokerHands.RoyalFlush;
    } else if (isFlush && isStraight) {
        bestHand = &pokerHands.StraightFlush;
    } else if (hasFourOfAKind) {
        bestHand = &pokerHands.FourOfAKind;
    } else if (hasThreeOfAKind && hasPair) {
        bestHand = &pokerHands.FullHouse;
    } else if (isFlush && selectedCards.size() == 5) {
        bestHand = &pokerHands.Flush;
    } else if (isStraight) {
        bestHand = &pokerHands.Straight;
    } else if (hasThreeOfAKind) {
        bestHand = &pokerHands.ThreeOfAKind;
    } else if (pairCount >= 2) {
        bestHand = &pokerHands.TwoPair;
    } else if (hasPair) {
        bestHand = &pokerHands.OnePair;
    }
    int currentChips = bestHand->getChips() + handChipsFromCards;
    int currentMult = bestHand->getMult();

    std::cout << "DEBUG: Base Score -> Chips: " << currentChips << " Mult: " << currentMult << std::endl;

    for (size_t i = 0; i < jokers.size(); ++i) {
        if (jokers[i]) {
            jokers[i]->playJoker(currentChips, currentMult, this);
        }
    }

    std::cout << "DEBUG: Final Score -> Chips: " << currentChips << " Mult: " << currentMult << " = " << (currentChips * currentMult) << std::endl;

    return currentChips * currentMult;
}

void Balatro::getMessagePrompt(std::string msg) {

	if (gameOver) {
		std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Send /balatro to start a new round\r\n";
		send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
		return ;
	}

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
						std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Invalid card index: " + tokens[i] + "\r\n";
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
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :No cards selected to discard.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			if (discards == 0){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You have finished your discards.\r\n";
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
			if (selectedCards.empty()) {
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :No cards selected to play.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			msg.erase(0, 4);
			playHand();
			selectedCards.clear();
			if (isSuitSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsBySuit);
			if (isRankSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsByRank);
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
        } else if (msg.find("cash out") == 0) {
			if (!isCashingOut){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the cashing out screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			printShopUI();
			isCashingOut = false;
        } else if (msg.find("next") == 0) {
			startNewRound();
			printSelectedCardsUI();
        } else if (msg.find("shop") == 0) {
			printShopUI();
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

void Balatro::freeJokers(){
	for(size_t i = 0; i < jokers.size(); i++){
		delete(jokers[i]);
	}
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
	while (hand.size() < 8 && !deck.empty()) {
		hand.push_back(deck.back());
		deck.pop_back();
	}
	selectedCards.clear();
	if (totalBet >= anteScore){
		printEndRoundUI();
		return ;
	}
	hands--;
	if (isSuitSorting)
		std::sort(hand.begin(), hand.end(), compareCardsBySuit);
	if (isRankSorting)
		std::sort(hand.begin(), hand.end(), compareCardsByRank);
	printSelectedCardsUI();
	if (hands == 0)
	{
		std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Game Over MF\r\n";
		send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
		setGameOver(true);
		printUI();
		freeJokers();
		return ;
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
	for (size_t i = 0; i < jokers.size(); ++i) {
        delete jokers[i];
    }
    jokers.clear();

    jokers.push_back(new BaseJoker());
	startNewRound();
	initPokerHands();
	ante = 1;
	anteScore = calculateAnteScore();
	printUI();
}