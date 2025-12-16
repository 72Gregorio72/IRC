#include "includes/Balatro.hpp"
#include <sstream>

// Helper per gli spazi
std::string Balatro::getSpaces(int count) {
    if (count < 0) return "";
    return std::string(count, ' ');
}

// Funzione Unificata: Gestisce sia carte normali che selezionate
// Se isSelected = true, la carta viene stampata "in alto" (senza padding sopra)
// Se isSelected = false, la carta viene stampata "in basso" (con padding sopra)
std::vector<std::string> Balatro::getCardRowsSelected(const Card& c, bool isSelected) {
    std::vector<std::string> rows;
    std::string rank = c.getRank();
    std::string suit = c.getSuit();
    
    // Definizioni colori
    std::string RED = "\x03" "04";
    std::string ORANGE = "\x03" "07";
    std::string BLACK = "\x03" "01";
    std::string BLUE = "\x03" "12";
    std::string RESET = "\x0f";

    std::string color = BLACK; 
    std::string symbol = "";
    
    if (suit == "Hearts")        { symbol = "♥"; color = RED; }
    else if (suit == "Diamonds") { symbol = "♦"; color = ORANGE; }
    else if (suit == "Spades")   { symbol = "♠"; color = BLACK; }
    else if (suit == "Clubs")    { symbol = "♣"; color = BLUE; }
    
    std::string sym = color + symbol; 

    std::string rL = rank + (rank == "10" ? "" : " ");
    std::string rR = (rank == "10" ? "" : " ") + rank;

    // Pattern grafici
    std::string mid = "     " + sym + "     ";
    std::string col = "  " + sym + "     " + sym + "  ";
    std::string midCol = "  " + sym + "  " + sym + "  " + sym + "  ";

    // Costruzione delle 5 righe interne
    std::string line[5]; 
    for(int i=0; i<5; i++) line[i] = "           "; 

    if (rank == "J" || rank == "Q" || rank == "K") {
        line[1] = "     " + color + rank + "     "; 
        line[2] = "     " + sym + "     ";
        line[3] = "     " + color + rank + "     ";
    }
    else if (rank == "A") {
        line[2] = mid;
    }
    else {
        int r = 0;
        if (rank == "2") r=2; else if (rank == "3") r=3; else if (rank == "4") r=4;
        else if (rank == "5") r=5; else if (rank == "6") r=6; else if (rank == "7") r=7;
        else if (rank == "8") r=8; else if (rank == "9") r=9; else if (rank == "10") r=10;

        if (r == 9) {
            line[0] = col; line[1] = col; line[2] = mid; line[3] = col; line[4] = col;
        }
        else if (r == 10) {
            line[0] = col; line[1] = midCol; line[2] = mid; line[3] = col; line[4] = col;
        }
        else {
            if (r >= 4) { line[0] = col; line[4] = col; }
            if (r == 6 || r == 7 || r == 8) { line[2] = col; }
            if (r == 2 || r == 3) { line[0] = mid; line[4] = mid; }
            if (r == 3 || r == 5) { line[2] = mid; }
            if (r == 7) { line[1] = mid; }
            if (r == 8) { line[1] = mid; line[3] = mid; }
        }
    }

    // Costruiamo le 9 righe della grafica della carta
    std::vector<std::string> cardGraphic;
    cardGraphic.push_back(color + "┌───────────┐" + RESET);      
    cardGraphic.push_back(color + "│ " + rL + "        │" + RESET); 
    cardGraphic.push_back(color + "│" + line[0] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[1] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[2] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[3] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[4] + "│" + RESET);    
    cardGraphic.push_back(color + "│        " + rR + " │" + RESET); 
    cardGraphic.push_back(color + "└───────────┘" + RESET);

    // --- LOGICA DI SPOSTAMENTO VERTICALE (PADDING) ---
    // La larghezza di una carta è 13 caratteri
    std::string emptyRow = "             "; 

    // Selezionata: Carta in alto (0-8), Spazio vuoto sotto (9-10)
    if (isSelected) {
        for(size_t i = 0; i < cardGraphic.size(); i++) rows.push_back(cardGraphic[i]);
        rows.push_back(emptyRow);
        rows.push_back(emptyRow);
    } 
    // Non Selezionata: Spazio vuoto sopra (0-1), Carta in basso (2-10)
    else {
        rows.push_back(emptyRow);
        rows.push_back(emptyRow);
        for(size_t i = 0; i < cardGraphic.size(); i++) rows.push_back(cardGraphic[i]);
    }

    return rows; // Ritorna sempre 11 righe
}
void Balatro::printSelectedCardsUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    if (selectedCards.empty()) {
        printUI();
        return;
    }

    int totalRows = 58;
    int leftColWidth = 29; 

    // 1. Grafica Carte
    std::vector< std::vector<std::string> > cardMatrix;
    for(size_t i = 0; i < hand.size(); i++) {
        bool isSelected = false;
        for (size_t j = 0; j < selectedCards.size(); j++) {
            if (hand[i].getRank() == selectedCards[j].getRank() &&
                hand[i].getSuit() == selectedCards[j].getSuit()) {
                isSelected = true;
                break;
            }
        }
        cardMatrix.push_back(getCardRowsSelected(hand[i], isSelected));
    }

    // 2. Grafica Deck e Joker
    std::vector<std::string> deckVisual = printDeck();
    std::vector<std::string> jokersVisual = getCombinedJokersVisual(this->jokers); 
    
    // 3. Altezze
    int handHeight = (!cardMatrix.empty() && !cardMatrix[0].empty()) ? (int)cardMatrix[0].size() : 0;
    int deckHeight = (int)deckVisual.size();
    int jokerHeight = (int)jokersVisual.size();

    int bottomBase = totalRows - 3; 
    int handStartRow = bottomBase - handHeight;
    int deckStartRow = bottomBase - deckHeight;
    int jokerStartRow = 2; // Posizione Joker

    std::string msg = "";

    // Header
    for(int i=0; i<10; i++) msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        int spacesNeeded = leftColWidth - (int)leftRaw.length();
        if (spacesNeeded < 0) spacesNeeded = 0;
        std::string leftPanel = leftColor + std::string(spacesNeeded, ' ');

        std::string rightPanel = getRightPanelContent(row, handStartRow, handHeight, deckStartRow, deckHeight, cardMatrix, deckVisual);
        
        // --- INIEZIONE JOKER ---
        if (row >= jokerStartRow && row < jokerStartRow + jokerHeight) {
            std::string jLine = jokersVisual[row - jokerStartRow];
            int centerPos = 42; 
            int startPos = centerPos - (getVisualLength(jLine) / 2);
            if (startPos < 2) startPos = 2;

            int visualLen = getVisualLength(jLine);
            if (startPos + visualLen < (int)rightPanel.length()) {
                rightPanel.replace(startPos, visualLen, jLine);
            }
        }
        
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";

    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

std::string Balatro::repeat_char(int count, char c) {
    if (count <= 0) return "";
    return std::string(count, c);
}

void Balatro::printEndRoundUI() {
	isCashingOut = true;
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29;
    int rightColWidth = 65; // Larghezza approssimativa pannello destro
    
    std::string RED = "\x03" "04";
    std::string ORANGE = "\x03" "07";
    std::string GREEN = "\x03" "09"; 
    std::string BLUE = "\x03" "12";
    std::string GREY = "\x03" "14";
    std::string WHITE = "\x03" "00";
    std::string RESET = "\x0f";

    long blindScoreReq = anteScore;
    int blindReward = 0;
    
    int handsLeft = hands;
    int moneyPerHand = 1;
    int handsReward = handsLeft * moneyPerHand;
    
    int interest = 0; 
    int bossBonus = 0;
    
    int totalCashOut = blindReward + handsReward + interest + bossBonus;

    std::vector<RowData> dataRows;

    // Riempiamo i dati
    RowData r1; r1.label = "Score at least " + RED + to_string_98(blindScoreReq) + RESET; 
    r1.val = blindReward; r1.color = GREEN; r1.hiddenColorLen = RED.length() + RESET.length();
    dataRows.push_back(r1);

    RowData r2; r2.label = BLUE + to_string_98(handsLeft) + RESET + " Remaining Hands ($1)"; 
    r2.val = handsReward; r2.color = GREEN; r2.hiddenColorLen = BLUE.length() + RESET.length();
    dataRows.push_back(r2);

    RowData r3; r3.label = "Interest (max 5)"; 
    r3.val = interest; r3.color = GREEN; r3.hiddenColorLen = 0;
    dataRows.push_back(r3);
    
    if (bossBonus > 0) {
        RowData r4; r4.label = "Boss Blind Defeated"; 
        r4.val = bossBonus; r4.color = GREEN; r4.hiddenColorLen = 0;
        dataRows.push_back(r4);
    }

    std::vector<std::string> boxLines;
    int boxInternalWidth = 38;

    std::string sTotal = to_string_98(totalCashOut);
    std::string title = " Cash Out: $" + sTotal + " ";
    int padHead = (boxInternalWidth - (int)title.length()) / 2;
    if (padHead < 0) padHead = 0;
    std::string btn = "\x03" "00,07" + repeat_char(padHead, ' ') + title + repeat_char(padHead, ' ') + ((title.length() % 2 != 0) ? " " : "") + RESET;

    boxLines.push_back(GREY + "╭────────────────────────────────────────╮" + RESET);
    boxLines.push_back(GREY + "│" + RESET + " " + btn + " " + GREY + "│" + RESET);
    boxLines.push_back(GREY + "├────────────────────────────────────────┤" + RESET);

    for(size_t i=0; i<dataRows.size(); ++i) {
        std::string sVal = "$" + to_string_98(dataRows[i].val);
        int visibleLen = (int)dataRows[i].label.length() - dataRows[i].hiddenColorLen;
        int dots = boxInternalWidth - (visibleLen + (int)sVal.length() + 2);
        if (dots < 0) dots = 0;

        std::string line = dataRows[i].label + " " + GREY + repeat_char(dots, '.') + RESET + " " + dataRows[i].color + sVal + RESET;
        boxLines.push_back(GREY + "│" + RESET + " " + line + " " + GREY + "│" + RESET);
    }

    std::vector<std::string> deckVisual = printDeck(); // Recupera la grafica del mazzo
    int deckHeight = (int)deckVisual.size();

    int boxStartRow = 20; 
    int boxEndRow = totalRows - 3; // Lascia spazio per il footer inferiore
    int contentHeight = (int)boxLines.size();
    
    std::string msg = "";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        std::string leftPanel = leftColor + repeat_char(leftColWidth - (int)leftRaw.length(), ' ');

        std::string rightPanelContent = repeat_char(rightColWidth, ' '); // Sfondo vuoto

        int deckStartRow = (totalRows - 3) - deckHeight;
        if (row >= deckStartRow && row < (totalRows - 3)) {
            int deckRowIdx = row - deckStartRow;
            if (deckRowIdx < deckHeight) {
                std::string deckLine = deckVisual[deckRowIdx];
            }
        }
        std::string rowStr = "";
        
        std::string boxPart = "                                          ";
        
        if (row >= boxStartRow && row < boxEndRow) {
            int boxIdx = row - boxStartRow;
            
            if (boxIdx < contentHeight) {
                // Parte col contenuto (Header, soldi, ecc)
                boxPart = boxLines[boxIdx];
            } else if (row == boxEndRow - 1) {
                // Ultima riga del box: chiusura
                boxPart = GREY + "╰────────────────────────────────────────╯" + RESET;
            } else {
                // Parte centrale vuota "stirata" fino in fondo
                boxPart = GREY + "│" + RESET + repeat_char(boxInternalWidth + 2, ' ') + GREY + "│" + RESET;
            }
        } else {
            boxPart = "                                          "; // Invisibile fuori dal range
        }

        std::string deckPart = "";
        if (row >= deckStartRow && row < (totalRows - 3)) {
            int dIdx = row - deckStartRow;
            if (dIdx >= 0 && dIdx < (int)deckVisual.size()) {
                deckPart = deckVisual[dIdx];
            }
        }
        
        std::string spacerLeft = "                  ";
        
        
        if (!deckPart.empty()) {
             rowStr = "                                                            " + boxPart + "                                                     " + deckPart; 
        } else {
             rowStr = "                                                            " + boxPart;
        }

        msg += prefix + " " + leftPanel + " ║" + rowStr + "\r\n";
    }

    // Footer UI
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
	coins += totalCashOut;
}

