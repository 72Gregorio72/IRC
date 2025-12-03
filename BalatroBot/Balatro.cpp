#include "Balatro.hpp"

Balatro::Balatro() : ante(0), discards(0), hands(0), coins(0), currentBet(0), totalBet(0), sd(0), player() {}

Balatro::Balatro(int sd, User player) : ante(0), discards(0), hands(0), coins(0), currentBet(0), totalBet(0), sd(sd), player(player) {}

Balatro::~Balatro() {}

Balatro::Balatro(const Balatro &other)
	: ante(other.ante),
	  discards(other.discards),
	  hand(other.hand),
	  deck(other.deck),
	  hands(other.hands),
	  coins(other.coins),
	  currentBet(other.currentBet),
	  totalBet(other.totalBet) {}

Balatro &Balatro::operator=(const Balatro &other) {
	if (this != &other) {
		ante = other.ante;
		discards = other.discards;
		hand = other.hand;
		deck = other.deck;
		hands = other.hands;
		coins = other.coins;
		currentBet = other.currentBet;
		totalBet = other.totalBet;
	}
	return *this;
}

void Balatro::startNewRound() {
	hand.clear();
	deck.clear();
	currentBet = 0;
	totalBet = 0;
	discards = 0;
	hands = 0;
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

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Helper per creare stringhe di spazi (padding)
std::string getSpaces(int count) {
    if (count < 0) return "";
    return std::string(count, ' ');
}

void Balatro::printUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player.getNickName() + " :";
    
    std::string RED = "\x03" "04";
    std::string BLACK = "\x03" "01";
	std::string BLUE = "\x03" "12";
	std::string ORANGE = "\x03" "07";
    std::string RESET = "\x0f";
    std::string BOLD = "\x02";

    std::string cL1 = "", cL2 = "", cL3 = "", cL4 = "", cL5 = "", cNums = "";
    
    for (size_t i = 0; i < hand.size(); ++i) {
        std::string rank = hand[i].getRank();
        std::string suit = hand[i].getSuit();
        std::string symbol = "";
        std::string color = "";

        if (suit == "Hearts")        { symbol = "♥"; color = RED; }
        else if (suit == "Diamonds") { symbol = "♦"; color = ORANGE; }
        else if (suit == "Spades")   { symbol = "♠"; color = BLACK; }
        else if (suit == "Clubs")    { symbol = "♣"; color = BLUE; }
        
        std::string spaceR = (rank == "10") ? "" : " "; 

        cL1 += color + "┌───────┐" + RESET + " ";
        cL2 += color + "│ " + rank + spaceR + "    │" + RESET + " ";
        cL3 += color + "│   " + symbol + "   │" + RESET + " ";
        cL4 += color + "│    " + spaceR + rank + " │" + RESET + " ";
        cL5 += color + "└───────┘" + RESET + " ";
        cNums += "   (" + toString(i + 1) + ")    ";
    }

    int leftColWidth = 12;
    std::string vLine = " │ ";
    std::string statsLabels[] = {"HANDS", "DISC", "COINS", "ANTE"};
    std::string statsValues[] = {toString(hands), toString(discards), toString(coins), toString(ante)};
    int statsCount = 4;
    int totalRows = 12; 
    
    std::string msg = "";
    for(int i=0; i<15; i++) msg += prefix + " \r\n";

    msg += prefix + "=============" + "┬" + "====================================================================\r\n";

    for (int row = 0; row < totalRows; ++row) {
        std::string leftContent = "";
        std::string rightContent = "";

        if (row < statsCount) {
            std::string label = statsLabels[row];
            std::string val = statsValues[row];
            leftContent = label + ": " + val;
        } else {
            leftContent = "";
        }
        leftContent += getSpaces(leftColWidth - leftContent.length());

        int cardStartRow = totalRows - 6;

        if (row == 0) {
            rightContent = "      " + BOLD + "BALATRO CLI" + RESET;
        }
        else if (row >= cardStartRow) {
            int cardRowIndex = row - cardStartRow;
            if (cardRowIndex == 0) rightContent = cL1;
            else if (cardRowIndex == 1) rightContent = cL2;
            else if (cardRowIndex == 2) rightContent = cL3;
            else if (cardRowIndex == 3) rightContent = cL4;
            else if (cardRowIndex == 4) rightContent = cL5;
            else if (cardRowIndex == 5) rightContent = cNums;
        } 
        else {
            rightContent = "";
        }

        msg += prefix + " " + leftContent + vLine + rightContent + "\r\n";
    }

    msg += prefix + "=============" + "┴" + "====================================================================\r\n";

    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

void Balatro::dealInitialHand() {
    hand.clear();
    for (int i = 0; i < 8 && !deck.empty(); ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    printUI();
}

void Balatro::shuffleDeck(){
	std::random_shuffle(deck.begin(), deck.end());
}

void Balatro::startNewGame() {
	startNewRound();
	coins = 0;
	ante = 0;
	discards = 0;
	hands = 0;
}