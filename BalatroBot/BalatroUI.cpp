#include "includes/Balatro.hpp"

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::vector<std::string> Balatro::getCombinedJokersVisual(const std::vector<IJoker*>& targetJokers) {
    if (targetJokers.empty()) {
        return std::vector<std::string>();
    }

    // 1. Genera i singoli box
    std::vector< std::vector<std::string> > jokerImages;
    for (size_t i = 0; i < targetJokers.size(); ++i) {
        jokerImages.push_back(createJokerItem(targetJokers[i]));
    }

    if (jokerImages.empty()) return std::vector<std::string>();

    // 2. Unisci orizzontalmente
    int itemsHeight = (int)jokerImages[0].size();
    int gapSize = 1; 
    std::string spacer = std::string(gapSize, ' ');

    std::vector<std::string> mergedRow(itemsHeight, "");

    for (int r = 0; r < itemsHeight; ++r) {
        for (size_t i = 0; i < jokerImages.size(); ++i) {
            mergedRow[r] += jokerImages[i][r];
            if (i < jokerImages.size() - 1) {
                mergedRow[r] += spacer;
            }
        }
    }
    return mergedRow;
}

std::vector<std::string> Balatro::getCombinedPlanetsVisual(const std::vector<IPlanet*>& targetPlanets) {
    if (targetPlanets.empty()) {
        return std::vector<std::string>();
    }

    // 1. Genera i singoli box
    std::vector< std::vector<std::string> > planetImages;
    for (size_t i = 0; i < targetPlanets.size(); ++i) {
        planetImages.push_back(createPlanetItem(targetPlanets[i]));
    }

    if (planetImages.empty()) return std::vector<std::string>();

    // 2. Unisci orizzontalmente
    int itemsHeight = (int)planetImages[0].size();
    int gapSize = 1; 
    std::string spacer = std::string(gapSize, ' ');

    std::vector<std::string> mergedRow(itemsHeight, "");

    for (int r = 0; r < itemsHeight; ++r) {
        for (size_t i = 0; i < planetImages.size(); ++i) {
            mergedRow[r] += planetImages[i][r];
            if (i < planetImages.size() - 1) {
                mergedRow[r] += spacer;
            }
        }
    }
    return mergedRow;
}

std::vector<std::string> Balatro::createPokerHandsRankBox() {
    std::vector<std::string> box;

    std::string RESET = "\x0f";
    std::string GREY  = "\x03" "14";
    std::string WHITE = "\x03" "00";
    std::string YEL   = "\x03" "08";

    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";

    // Title (kept separate for centering)
    std::string title = YEL + "\x02" + std::string("POKER HANDS") + RESET;

    // Pairs of display name and lookup key for getPokerHands
    std::vector< std::pair<std::string,std::string> > items;
    items.push_back(std::make_pair("High Card", "High Card"));
    items.push_back(std::make_pair("One Pair", "Pair"));
    items.push_back(std::make_pair("Two Pair", "Two Pair"));
    items.push_back(std::make_pair("Three of a Kind", "Three of a Kind"));
    items.push_back(std::make_pair("Straight", "Straight"));
    items.push_back(std::make_pair("Flush", "Flush"));
    items.push_back(std::make_pair("Full House", "Full House"));
    items.push_back(std::make_pair("Four of a Kind", "Four of a Kind"));
    items.push_back(std::make_pair("Straight Flush", "Straight Flush"));
    items.push_back(std::make_pair("Royal Flush", "Royal Flush"));
    items.push_back(std::make_pair("Five of a Kind", "Five of a Kind"));
    items.push_back(std::make_pair("Flush House", "Flush House"));
    items.push_back(std::make_pair("Flush Five", "Flush Five"));

    // Build colored content lines with levels
    std::vector<std::string> outLines;
    outLines.push_back(title);
    for (size_t i = 0; i < items.size(); ++i) {
        const std::string& disp = items[i].first;
        const std::string& key  = items[i].second;
        int lv = getPokerHands(key).getLevel();
        std::string line = WHITE + disp + RESET + "  " + YEL + "Lv " + to_string_98(lv) + RESET;
        outLines.push_back(line);
    }

    // Compute inner width (max visual length)
    int inner = 0;
    for (size_t i = 0; i < outLines.size(); ++i) {
        int v = getVisualLength(outLines[i]);
        if (v > inner) inner = v;
    }
    inner += 2; // one space left/right

    // Top
    box.push_back(GREY + TL + repeat_string(inner, H) + TR + RESET);

    for (size_t i = 0; i < outLines.size(); ++i) {
        std::string text = outLines[i];
        int v = getVisualLength(text);
        int padLeft = (inner - v) / 2;
        int padRight = inner - v - padLeft;
        if (padLeft < 0) padLeft = 0;
        if (padRight < 0) padRight = 0;
        box.push_back(GREY + V + RESET + repeat_char(padLeft, ' ') + text + repeat_char(padRight, ' ') + GREY + V + RESET);
    }

    // Bottom
    box.push_back(GREY + BL + repeat_string(inner, H) + BR + RESET);

    return box;
}

std::vector<std::string> Balatro::getCardRows(const Card& c) {
    std::vector<std::string> rows;
    std::string rank = c.getRank();
    std::string suit = c.getSuit();
    
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

    // Pattern base
    std::string mid = "     " + sym + "     ";
    std::string col = "  " + sym + "     " + sym + "  ";
	std::string midCol = "  " + sym + "  " + sym + "  " + sym + "  ";

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
            line[0] = col;
            line[1] = col;
            line[2] = mid;
            line[3] = col;
            line[4] = col;
        }
        else if (r == 10) {
            line[0] = col;
            line[1] = midCol;
            line[2] = mid;
            line[3] = col;
            line[4] = col;
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

    rows.push_back(color + "┌───────────┐" + RESET);      
    rows.push_back(color + "│ " + rL + "        │" + RESET); 
    rows.push_back(color + "│" + line[0] + "│" + RESET);    
    rows.push_back(color + "│" + line[1] + "│" + RESET);    
    rows.push_back(color + "│" + line[2] + "│" + RESET);    
    rows.push_back(color + "│" + line[3] + "│" + RESET);    
    rows.push_back(color + "│" + line[4] + "│" + RESET);    
    rows.push_back(color + "│        " + rR + " │" + RESET); 
    rows.push_back(color + "└───────────┘" + RESET);

    return rows;
}

std::vector<std::string> Balatro::printDeck() {
    std::vector<std::string> rows;
    
    std::string DARK_BLUE = "\x03" "02";
	std::string RED = "\x03" "04";
    std::string LIGHT_BLUE = "\x03" "12";
    std::string BLACK = "\x03" "01";
	std::string LINE_BG = "\x03" "01,04";
	std::string WHITE_ON_RED = "\x03" "00,04";
    std::string TEXTURE = "╬";
    std::string RESET = "\x0f";
    std::string BLOCK = "█";

    int width = 18; 
    int height = 12;

    // double cx = (width - 1) / 2.0;
    // double cy = (height - 1) / 2.0;
    
    // double rx = 5.0; 
    // double ry = 3.5; 

    rows.push_back(BLACK + "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" + RESET);

    // for (int y = 0; y < height; ++y) {
    //     std::string line = "";
    //     for (int x = 0; x < width; ++x) {
    //         double dx = x - cx;
    //         double dy = y - cy;
    //         double dist = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);

    //         if (x == 2 && y == 1) {
    //             line += LINE_BG + "╠";
    //         }
    //         else if (x == width - 3 && y == 1) {
    //             line += LINE_BG + "╦";
    //         }
    //         else if (x == 2 && y == height - 2) {
    //             line += LINE_BG + "╩";
    //         }
    //         else if (x == width - 3 && y == height - 2) {
    //             line += LINE_BG + "╣";
    //         }
    //         else if (x == 2 && y <= height - 2) {
    //             line += LINE_BG + "║";
    //         }
    //         else if (x == width - 3 && y >= 1) {
    //             line += LINE_BG + "║";
    //         }
    //         else if (y == 1 && x >= 2) {
    //             line += LINE_BG + "═";
    //         }
    //         else if (y == height - 2 && x <= width - 3) {
    //             line += LINE_BG + "═";
    //         }
    //         else {
    //             if (dist <= 1.0) {
    //                 line += RESET + BLACK + BLOCK;
    //             } else {
    //                 line += RESET + RED + BLOCK;
    //             }
    //         }
    //     }
    //     rows.push_back(BLACK + "█" + line + BLACK + "█" + RESET);
    // }

	for (int y = 0; y < height; ++y) {
        std::string line = "";
        for (int x = 0; x < width; ++x) {
            // Applica il carattere ╬ con linee bianche e sfondo rosso su ogni cella
            line += WHITE_ON_RED + TEXTURE;
        }
        // Aggiunge i bordi laterali neri attorno alla linea generata
        rows.push_back(BLACK + "█" + line + BLACK + "█" + RESET);
    }

    rows.push_back(BLACK + "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" + RESET);

    return rows;
}

std::string Balatro::centerText(std::string text, int width) {
    int len = text.length();
    if (len >= width) return text;
    int left = (width - len) / 2;
    int right = width - len - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}

void Balatro::getLeftPanelContent(int row, std::string& rawOut, std::string& colOut) {
    // Definizioni colori
    std::string C_RED     = "\x03" "04";
    std::string C_BLUE    = "\x03" "12";
    std::string C_ORANGE  = "\x03" "07";
    std::string C_YELLOW  = "\x03" "08";
    std::string C_WHITE   = "\x03" "00";
    std::string C_GREY    = "\x03" "14";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // Valori di default (vuoti)
    rawOut = ""; 
    colOut = "";

    // --- LOGICA PANNELLO SINISTRO ---
    
    // GOAL CHIPS (In alto)
    if (row == 2) {
        rawOut = "         GOAL CHIPS          ";
        colOut = "         " + C_RED + BOLD + "GOAL CHIPS" + RESET + "          ";
    }
    else if (row == 3) {
		std::cout << "ante score: " << anteScore << std::endl;
		std::stringstream ss;
		ss << anteScore;
        std::string tVal = ss.str();
        std::string cVal = centerText(tVal, 29);
        rawOut = cVal;
        colOut = C_RED + BOLD + cVal + RESET;
    }
    
    // ROUND SCORE
    else if (row == 5) {
        rawOut = "        ROUND SCORE          ";
        colOut = "        " + C_WHITE + "ROUND SCORE" + RESET + "          ";
    }
    else if (row == 6) {
        std::string sVal = toString(totalBet);
        std::string cVal = centerText(sVal, 29);
        rawOut = cVal; 
        colOut = C_WHITE + BOLD + cVal + RESET;
    }

    // MONEY / COINS ($)
    else if (row == 9) {
        rawOut = "              $              ";
        colOut = "              " + C_YELLOW + BOLD + "$" + RESET + "              ";
    }
    else if (row == 10) {
        rawOut = "          +------+           ";
        colOut = "          " + C_YELLOW + "┌──────┐" + RESET + "           ";
    }
    else if (row == 11) {
        std::string cVal = centerText("$" + toString(coins), 6);
        rawOut = "          |      |           ";
        colOut = "          " + C_YELLOW + "│" + RESET + BOLD + cVal + RESET + C_YELLOW + "│" + RESET + "           ";
    }
    else if (row == 12) {
        rawOut = "          +------+           ";
        colOut = "          " + C_YELLOW + "└──────┘" + RESET + "           ";
    }

    // HANDS & DISCARDS (In basso)
    else if (row == 16) {
        rawOut = "    HANDS       DISC    ";
        colOut = "    " + C_BLUE + BOLD + "HANDS" + RESET + "       " + C_RED + BOLD + "DISC" + RESET + "    ";
    }
    else if (row == 17) {
        rawOut = "   +-----+     +-----+  ";
        colOut = "   " + C_BLUE + "┌─────┐" + RESET + "     " + C_RED + "┌─────┐" + RESET + "  ";
    }
    else if (row == 18) {
        std::string hVal = centerText(toString(hands), 5);
        std::string dVal = centerText(toString(discards), 5);
        rawOut = "   |     |     |     |  ";
        colOut = "   " + C_BLUE + "│" + RESET + BOLD + hVal + RESET + C_BLUE + "│" + RESET + "     " + C_RED + "│" + RESET + BOLD + dVal + RESET + C_RED + "│" + RESET + "  ";
    }
    else if (row == 19) {
        rawOut = "   +-----+     +-----+  ";
        colOut = "   " + C_BLUE + "└─────┘" + RESET + "     " + C_RED + "└─────┘" + RESET + "  ";
    }

    // ANTE (Ultimo in basso)
    else if (row == 22) {
        rawOut = "            ANTE             ";
        colOut = "            " + C_ORANGE + BOLD + "ANTE" + RESET + "             ";
    }
    else if (row == 23) {
        rawOut = "          +------+           ";
        colOut = "          " + C_ORANGE + "┌──────┐" + RESET + "           ";
    }
    else if (row == 24) {
        std::string aVal = centerText(toString(ante) + "/8", 6);
        rawOut = "          |      |           ";
        colOut = "          " + C_ORANGE + "│" + RESET + BOLD + aVal + RESET + C_ORANGE + "│" + RESET + "           ";
    }
    else if (row == 25) {
        rawOut = "          +------+           ";
        colOut = "          " + C_ORANGE + "└──────┘" + RESET + "           ";
    }

    // POKER HAND LEVELS BOX (sotto ANTE, qualche riga più in basso)
    else if (row >= 31 && row < 31 + 16) {
        // Build box content
        std::vector< std::pair<std::string,std::string> > items;
        items.push_back(std::make_pair("High Card", "High Card"));
        items.push_back(std::make_pair("One Pair", "Pair"));
        items.push_back(std::make_pair("Two Pair", "Two Pair"));
        items.push_back(std::make_pair("Three of a Kind", "Three of a Kind"));
        items.push_back(std::make_pair("Straight", "Straight"));
        items.push_back(std::make_pair("Flush", "Flush"));
        items.push_back(std::make_pair("Full House", "Full House"));
        items.push_back(std::make_pair("Four of a Kind", "Four of a Kind"));
        items.push_back(std::make_pair("Straight Flush", "Straight Flush"));
        items.push_back(std::make_pair("Royal Flush", "Royal Flush"));
        items.push_back(std::make_pair("Five of a Kind", "Five of a Kind"));
        items.push_back(std::make_pair("Flush House", "Flush House"));
        items.push_back(std::make_pair("Flush Five", "Flush Five"));

        // Fixed inner width 27 (fits 29 total with borders)
        const int inner = 27;
        std::vector<std::string> rawBox;
        std::vector<std::string> colBox;

        // Top border
        rawBox.push_back("+---------------------------+");
        colBox.push_back(C_GREY + "┌" + repeat_string(inner, "─") + "┐" + RESET);

        // Title
        std::string title = "POKER HANDS";
        int tPad = (inner - (int)title.length()) / 2;
        if (tPad < 0) tPad = 0;
        std::string rawTitle = "|" + std::string(tPad, ' ') + title + std::string(inner - tPad - (int)title.length(), ' ') + "|";
        std::string colTitle = C_GREY + "│" + RESET + std::string(tPad, ' ') + C_YELLOW + "\x02" + title + RESET + std::string(inner - tPad - (int)title.length(), ' ') + C_GREY + "│" + RESET;
        rawBox.push_back(rawTitle);
        colBox.push_back(colTitle);

        // Lines with: Name (dark yellow), Lv (grey), Mult (red), Chips (blue)
        // Allineamento: nome fisso 11 char, poi Lv M C allineati
        for (size_t i = 0; i < items.size(); ++i) {
            const std::string& disp = items[i].first;
            const std::string& key  = items[i].second;
            int lv   = getPokerHands(key).getLevel();
            int mult = getPokerHands(key).getMult();
            int chip = getPokerHands(key).getChips();

            // Nome: max 11 caratteri, padding a destra
            int nameWidth = 11;
            std::string name = disp;
            if ((int)name.length() > nameWidth) name = name.substr(0, nameWidth);
            std::string namePadded = name + std::string(nameWidth - (int)name.length(), ' ');

            // Valori: allineati a larghezza fissa (Lv:4, M:5, C:5)
            std::stringstream ssLv, ssMult, ssChip;
            ssLv << lv;
            ssMult << mult;
            ssChip << chip;
            
            std::string lvStr = "Lv" + ssLv.str();
            std::string multStr = ssMult.str();
            std::string chipStr = ssChip.str();
            
            // Padding per allineamento (larghezza fissa)
            lvStr += std::string(4 - (int)lvStr.length() > 0 ? 4 - (int)lvStr.length() : 0, ' ');
            chipStr += std::string(5 - (int)chipStr.length() > 0 ? 5 - (int)chipStr.length() : 0, ' ');
            int multPadding = 6 - ((int)multStr.length() + 1); // mult + 'X' occupano 6 caratteri totali
            if (multPadding < 0) multPadding = 0;

            // Raw line (27 char interni: 11 nome + 1 spazio + 4 lv + 6 mult/X + 5 chip)
            std::string rawLine = "|" + namePadded + " " + lvStr + multStr + "X" + chipStr + std::string(multPadding, ' ') + "|";

            // Colored line
            std::string colLine = C_GREY + "│" + RESET
                                 + C_ORANGE + namePadded + RESET
                                 + " "
                                 + C_GREY + lvStr + RESET
                                 + C_RED + multStr + RESET
                                 + "X"
                                 + C_BLUE + chipStr + RESET
                                 + std::string(multPadding, ' ')
                                 + C_GREY + "│" + RESET;

            rawBox.push_back(rawLine);
            colBox.push_back(colLine);
        }

        // Bottom border
        rawBox.push_back("+---------------------------+");
        colBox.push_back(C_GREY + "└" + repeat_string(inner, "─") + "┘" + RESET);

        int base = 31;
        int idx = row - base;
        if (idx >= 0 && idx < (int)rawBox.size()) {
            rawOut = rawBox[idx];
            colOut = colBox[idx];
        }
    }
}

std::string Balatro::getRightPanelContent(int row, int handStart, int handH, int deckStart, int deckH, 
                                          const std::vector<std::vector<std::string> >& cardMatrix, 
                                          const std::vector<std::string>& deckVisual) {
    
    // --- CONFIGURAZIONE DIMENSIONI ---
    int boxWidth = 44;          // Larghezza Box Comandi
    int statsWidth = 44;        // Larghezza Box Statistiche
    int gapSize = 2;            // Spazio tra i due box
    int cardWidth = 14;         

    // Spostamento a sinistra (fisso)
    int targetRightCol = 4;    
    
    int currentHandWidth = hand.size() * cardWidth;
    int boxStartCol = targetRightCol;

    // --- COLORI ---
    std::string C_ORANGE  = "\x03" "07";
    std::string C_SCALE_1 = "\x03" "02"; 
    std::string C_SCALE_2 = "\x03" "12"; 
    std::string C_SCALE_3 = "\x03" "11"; 
    std::string C_SCALE_4 = "\x03" "10"; 
    std::string C_SCALE_5 = "\x03" "09"; 
    std::string C_SCALE_6 = "\x03" "08"; 
    std::string C_SCALE_7 = "\x03" "07"; 
    std::string C_SCALE_8 = "\x03" "04"; 
    std::string C_SCALE_9 = "\x03" "05"; 
    std::string C_SCALE_10 = "\x03" "06"; 
    std::string C_SCALE_11 = "\x03" "13"; 
    std::string C_GREY    = "\x03" "14";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";
    std::string vBorder = C_ORANGE + "│" + RESET; 

    // ==========================================================
    // 1. CONTENUTO "SINISTRO" (CARTE DELLA MANO)
    // ==========================================================
    std::string leftPart = "";
    int visibleLeftLen = 0; 

    if (row >= handStart && row < handStart + handH) {
        int slice = row - handStart;
        for(size_t c = 0; c < cardMatrix.size(); c++) {
            leftPart += cardMatrix[c][slice] + " ";
        }
        visibleLeftLen = currentHandWidth;
    }
    else if (row == handStart + handH) {
        for(size_t c = 0; c < hand.size(); c++) {
            std::string cardNum = "(" + to_string_98(c + 1) + ")";
            int paddingLen = (13 - cardNum.length()) / 2;
            std::string item = std::string(paddingLen, ' ') + cardNum + std::string(13 - paddingLen - cardNum.length(), ' ') + " ";
            leftPart += item;
        }
        visibleLeftLen = currentHandWidth;
    }

    // ==========================================================
    // 2. SPAZIATURA CENTRALE
    // ==========================================================
    int paddingNeeded = boxStartCol - visibleLeftLen;
    if (paddingNeeded < 0) paddingNeeded = 0;
    std::string spacer = std::string(paddingNeeded, ' ');

    // ==========================================================
    // 3. CONTENUTO DESTRO (COMANDI + STATS + DECK)
    // ==========================================================
    std::string rightPart = "";

    int boxesStartRow = 2;
    int cmdBoxHeight = 24; 
    int boxesEndRow = boxesStartRow + cmdBoxHeight;

    std::vector<std::string> pokerStats = getPokerHandVisuals();
    
    // --- A & B. BOX COMANDI E STATS ---
    if (row >= boxesStartRow && row < boxesEndRow) {
        
        // 1. Box Comandi (Sinistra)
        std::string cmdLine = "";
        int r = row - boxesStartRow + 1; 
        
        if (r == 1) {
            cmdLine = C_ORANGE + "┌──────────────────────────────────────────┐" + RESET;
        } 
        else if (r == cmdBoxHeight) {
            cmdLine = C_ORANGE + "└──────────────────────────────────────────┘" + RESET;
        }
        else {
            std::string text = "";
            if (r == 2)      text = BOLD + centerText("COMMANDS", boxWidth - 2) + RESET;
            else if (r == 3) text = C_ORANGE + "├──────────────────────────────────────────┤" + RESET;
            else if (r == 4) text = " " + C_SCALE_1 + "!select" + RESET + " <id> " + C_GREY + "(es: !select 1 3)" + RESET;
            else if (r == 6) text = " " + C_SCALE_2 + "!discard" + C_GREY + " (Discard selected)" + RESET;
            else if (r == 8) text = " " + C_SCALE_3 + "!play" + C_GREY + "    (Play selected)" + RESET;
            else if (r == 10) text = " " + C_SCALE_4 + "!sort suit/rank" + C_GREY + " (Sort Hand)" + RESET;
            else if (r == 12) text = " " + C_SCALE_5 + "!cash out" + C_GREY + " (Cash Out)" + RESET;
            else if (r == 14) text = " " + C_SCALE_6 + "!shop"  + RESET + " <id> " + C_GREY + "(es: !shop 1 3)" + RESET;
            else if (r == 16) text = " " + C_SCALE_7 + "!next" + C_GREY + " (Start New Round)" + RESET;
            else if (r == 18) text = " " + C_SCALE_8 + "!replace"  + RESET + " <id> " + C_GREY + "(es: !replace 1 3)" + RESET;
            else if (r == 20) text = " " + C_SCALE_9 + "!reroll" + C_GREY + " (Reroll The Shop)" + RESET;
            else if (r == 22) text = " " + C_SCALE_10 + "!sell"  + RESET + " <id> " + C_GREY + "(es: !sell 1 3)" + RESET;
            else if (r == 24) text = " " + C_SCALE_11 + "!swap"  + RESET + " <id1> <id2> " + C_GREY + "(es: !swap 1 3)" + RESET;
            else {
                text = std::string(boxWidth - 2, ' '); 
            }

            if (r == 3) {
                cmdLine = text;
            } else {
                int txtLen = getVisualLength(text); 
                if (txtLen > boxWidth - 2) text = std::string(boxWidth - 2, ' '); 
                int padRight = (boxWidth - 2) - txtLen;
                if(padRight < 0) padRight = 0;
                cmdLine = vBorder + text + std::string(padRight, ' ') + vBorder;
            }
        }

        // 2. Box Stats (Destra)
        std::string statLine = "";
        int statIdx = row - boxesStartRow; 

        if (statIdx >= 0 && statIdx < (int)pokerStats.size()) {
            statLine = pokerStats[statIdx];
        } else {
            statLine = std::string(statsWidth, ' ');
        }

        std::string gap = std::string(gapSize, ' ');
        rightPart = cmdLine + gap + statLine;
    }

    // --- D. DECK (SPOSTATO TUTTO A DESTRA) ---
    else if (row >= deckStart && row < deckStart + deckH) {
        int dSlice = row - deckStart;
        std::string deckRow = deckVisual[dSlice];
        
        // Calcolo per allineare a DESTRA
        // Larghezza totale dei box sopra = boxWidth (44) + gapSize (2) + statsWidth (44) = 90
        // Larghezza Deck = 20
        // Padding = 90 - 20 = 70
        
        int deckWidth = 20; 
        int totalBlockWidth = boxWidth + gapSize + statsWidth; 
        int deckPadLen = totalBlockWidth - deckWidth - 20;
        
        if (deckPadLen < 0) deckPadLen = 0;
        rightPart = std::string(deckPadLen, ' ') + deckRow;
    }
    else if (row == deckStart + deckH) {
        std::string deckLabel = "(" + to_string_98(deck.size()) + ")";
        
        // Centrare l'etichetta rispetto alla nuova posizione del deck (a destra)
        int deckWidth = 20;
        int totalBlockWidth = boxWidth + gapSize + statsWidth; 
        
        // Il deck inizia graficamente a: totalBlockWidth - deckWidth
        // Il centro del deck è a: (totalBlockWidth - deckWidth) + (deckWidth / 2)
        // La label deve iniziare a: CentroDeck - (LabelLen / 2)
        
        int centerDeck = (totalBlockWidth - deckWidth) + (deckWidth / 2);
        int labelStart = centerDeck - (deckLabel.length() / 2);

        if (labelStart < 0) labelStart = 0;
        rightPart = std::string(labelStart, ' ') + deckLabel; 
    }
    else {
        rightPart = ""; 
    }

    return leftPart + spacer + rightPart;
}

std::vector<std::string> Balatro::getPokerHandVisuals() {
    std::vector<std::string> lines;
    
    // --- COLORI ---
    std::string C_WHITE   = "\x03" "00";
    std::string C_BLUE    = "\x03" "12";
    std::string C_RED     = "\x03" "04";
    std::string C_ORANGE  = "\x03" "07";
    std::string C_GREY    = "\x03" "14";
    std::string C_YELLOW  = "\x03" "08";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    int boxWidth = 44;
    
    // CORREZIONE 1: Usa repeat_string e doppi apici per "─"
    std::string hBorder = repeat_string(boxWidth - 2, "─"); 
    std::string vBorder = C_GREY + "│" + RESET;

    // Header del Box
    lines.push_back(C_GREY + "┌" + hBorder + "┐" + RESET);
    
    std::string title = "HAND CONTENTS";
    int padTitle = (boxWidth - 2 - title.length()) / 2;
    std::string titleLine = std::string(padTitle, ' ') + C_YELLOW + BOLD + title + RESET + std::string(boxWidth - 2 - padTitle - title.length(), ' ');
    
    lines.push_back(vBorder + titleLine + vBorder);
    lines.push_back(C_GREY + "├" + hBorder + "┤" + RESET);

    // Lista Mani
    std::vector<PokerHand*> handsList;
    handsList.push_back(&pokerHands.HighCard);
    handsList.push_back(&pokerHands.OnePair);
    handsList.push_back(&pokerHands.TwoPair);
    handsList.push_back(&pokerHands.ThreeOfAKind);
    handsList.push_back(&pokerHands.Straight);
    handsList.push_back(&pokerHands.Flush);
    handsList.push_back(&pokerHands.FullHouse);
    handsList.push_back(&pokerHands.FourOfAKind);
    handsList.push_back(&pokerHands.StraightFlush);
    handsList.push_back(&pokerHands.RoyalFlush);
    handsList.push_back(&pokerHands.FiveOfAKind);
    handsList.push_back(&pokerHands.FlushHouse);
    handsList.push_back(&pokerHands.FlushFive);

    for (size_t i = 0; i < handsList.size(); ++i) {
        PokerHand* h = handsList[i];

        std::string name = h->getName();
        if (name.length() > 14) name = name.substr(0, 14);
        std::string namePad = std::string(14 - name.length(), ' ');

        // CORREZIONE 2: Assumiamo che tu abbia aggiunto getLevel() in PokerHand.hpp
        std::string lvlStr = "lvl." + to_string_98(h->getRank()); 
        if (lvlStr.length() < 6) lvlStr += std::string(6 - lvlStr.length(), ' ');

        std::string chipsStr = to_string_98(h->getChips());
        std::string multStr = to_string_98(h->getMult());
        
        int contentLen = 1 + 14 + 1 + 6 + 2 + chipsStr.length() + 3 + multStr.length() + 1;
        int remainingSpace = (boxWidth - 2) - contentLen;
        if (remainingSpace < 0) remainingSpace = 0;

        std::stringstream ss;
        ss << vBorder 
           << " " << C_WHITE << name << RESET << namePad << " "
           << C_GREY << lvlStr << RESET << "  "
           << C_BLUE << chipsStr << RESET << C_RED << " X " << multStr << RESET
           << std::string(remainingSpace, ' ')
           << vBorder;

        lines.push_back(ss.str());
    }
    
    lines.push_back(C_GREY + "└" + hBorder + "┘" + RESET);

    return lines;
}

void Balatro::printWinUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29;
    int rightColWidth = 85; 

    // --- COLORI ---
    std::string C_L_GREEN = "\x03" "09"; // Light Green
    std::string C_ORANGE  = "\x03" "07";
    std::string C_GREY    = "\x03" "14";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // --- CANVAS ---
    std::vector<std::string> rightCanvas(totalRows, std::string(rightColWidth, ' '));

    // =======================================================================
    // 1. ASCII ART "YOU WON"
    // =======================================================================
    std::vector<std::string> bigWin;
    std::string c = C_L_GREEN + BOLD;
    
    bigWin.push_back(c + "Y   Y  OOO  U   U     W   W  OOO  N   N  !!!" + RESET);
    bigWin.push_back(c + " Y Y  O   O U   U     W   W O   O NN  N  !!!" + RESET);
    bigWin.push_back(c + "  Y   O   O U   U     W W W O   O N N N  !!!" + RESET);
    bigWin.push_back(c + "  Y   O   O U   U     WW WW O   O N  NN     " + RESET);
    bigWin.push_back(c + "  Y    OOO   UUU      W   W  OOO  N   N  !!!" + RESET);

    // =======================================================================
    // 2. BOX ISTRUZIONI
    // =======================================================================
    std::vector<std::string> infoBox;
    int boxWidth = 50; 
    
    // *** CORREZIONE QUI SOTTO ***
    // Usiamo repeat() invece del costruttore, e "─" tra doppi apici
    std::string hBorder = repeat_string(boxWidth - 2, "─"); 
    std::string vBorder = C_GREY + "│" + RESET;

    infoBox.push_back(C_GREY + "┌" + hBorder + "┐" + RESET);
    
    // Riga vuota
    infoBox.push_back(vBorder + std::string(boxWidth - 2, ' ') + vBorder);
    
    // Testo Centrale
    std::string txtPart1 = "Type ";
    std::string txtCmd   = "/balatro";
    std::string txtPart2 = " to play again";
    
    int txtLen = txtPart1.length() + txtCmd.length() + txtPart2.length();
    int padLeft = (boxWidth - 2 - txtLen) / 2;
    int padRight = (boxWidth - 2) - txtLen - padLeft;
    if (padLeft < 0) padLeft = 0; // Sicurezza
    if (padRight < 0) padRight = 0;
    
    std::string lineContent = std::string(padLeft, ' ') 
                            + txtPart1 + C_ORANGE + BOLD + txtCmd + RESET 
                            + txtPart2 + std::string(padRight, ' ');

    infoBox.push_back(vBorder + lineContent + vBorder);
    
    // Riga vuota
    infoBox.push_back(vBorder + std::string(boxWidth - 2, ' ') + vBorder);
    
    infoBox.push_back(C_GREY + "└" + hBorder + "┘" + RESET);

    // =======================================================================
    // 3. POSIZIONAMENTO
    // =======================================================================
    
    int centerY = totalRows / 2;
    
    int winH = (int)bigWin.size();
    int winRow = centerY - winH - 2;
    int winCol = (rightColWidth - 37) / 2; 
    if (winCol < 0) winCol = 0;
    
    pasteObject(rightCanvas, bigWin, winRow, winCol);

    // int infoH = (int)infoBox.size();
    int infoRow = centerY + 2; 
    int infoCol = (rightColWidth - boxWidth) / 2;

    pasteObject(rightCanvas, infoBox, infoRow, infoCol);

    // =======================================================================
    // 4. INVIO AL CLIENT
    // =======================================================================
    std::string msg = "";
    
    msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        
        int padLeft = leftColWidth - (int)leftRaw.length();
        if(padLeft < 0) padLeft = 0;

        std::string leftPanel = leftColor + std::string(padLeft, ' ');
        std::string rightPanel = rightCanvas[row];

        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

void Balatro::printUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    std::string C_RED     = "\x03" "04";
    std::string C_WHITE   = "\x03" "00";
    std::string C_YELLOW  = "\x03" "08";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // --- GAME OVER CHECK ---
    if (gameOver) {
        std::string msg = "";
        // Pulizia schermo
        for(int i=0; i<50; i++) msg += prefix + " \r\n";

        std::vector<std::string> art;
        art.push_back("  /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$        /$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$   ");
        art.push_back(" /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/       /$$__  $$| $$   | $$| $$_____/| $$__  $$  ");
        art.push_back("| $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$            | $$  \\ $$| $$   | $$| $$      | $$  \\ $$  ");
        art.push_back("| $$ /$$$$| $$$$$$$$| $$ $$/$$ $$| $$$$$         | $$  | $$|  $$ / $$/| $$$$$   | $$$$$$$/  ");
        art.push_back("| $$|_  $$| $$__  $$| $$  $$$| $$| $$__/         | $$  | $$ \\  $$ $$/ | $$__/   | $$__  $$  ");
        art.push_back("| $$  \\ $$| $$  | $$| $$ \\ $ | $$| $$            | $$  | $$  \\  $$$/  | $$      | $$  \\ $$  ");
        art.push_back("|  $$$$$$/| $$  | $$| $$ \\/  | $$| $$$$$$$$      |  $$$$$$/   \\  $/   | $$$$$$$$| $$  | $$  ");
        art.push_back(" \\______/ |__/  |__/|__/     |__/|________/       \\______/     \\_/    |________/|__/  |__/  ");

        for (size_t i = 0; i < art.size(); ++i) {
            std::string centeredLine = centerText(art[i], 100); 
            msg += prefix + C_RED + BOLD + centeredLine + RESET + "\r\n";
        }

        msg += prefix + " \r\n";
        msg += prefix + " \r\n";

        std::string scoreText = "FINAL SCORE: " + toString(totalBet); 
        std::string centeredScore = centerText(scoreText, 100);
        msg += prefix + C_WHITE + BOLD + centeredScore + RESET + "\r\n";

        std::string restartText = "Type /balatro to play again";
        std::string centeredRestart = centerText(restartText, 100);
        msg += prefix + C_YELLOW + centeredRestart + RESET + "\r\n";

        // Footer pulizia
        for(int i=0; i<20; i++) msg += prefix + " \r\n";

        send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
        return; 
    }

    // --- LOGICA DI GIOCO STANDARD ---

    int totalRows = 58;
    int leftColWidth = 29; 

    // 1. Preparazione Grafica Elementi
    // Carte
    std::vector< std::vector<std::string> > cardMatrix;
    for(size_t i = 0; i < hand.size(); i++) {
        cardMatrix.push_back(getCardRows(hand[i]));
    }
    // Mazzo
    std::vector<std::string> deckVisual = printDeck();
    // Joker (Grafica combinata)
    std::vector<std::string> jokersVisual = getCombinedJokersVisual(this->jokers); 
    
    // 2. Calcolo altezze e posizioni verticali
    int handHeight = (!cardMatrix.empty() && !cardMatrix[0].empty()) ? (int)cardMatrix[0].size() : 0;
    int deckHeight = (int)deckVisual.size();
    int jokerHeight = (int)jokersVisual.size();

    int bottomBase = totalRows - 3; 
    int handStartRow = bottomBase - handHeight;
    int deckStartRow = bottomBase - deckHeight;
    
    // Posizione verticale dei Joker: Riga 2 (subito sotto la linea di header)
    int jokerStartRow = 2; 

    std::string msg = "";

    // 3. Header Tavolo
    for(int i=0; i<10; i++) msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    // 4. Ciclo di Rendering (Riga per riga)
    for (int row = 0; row < totalRows; ++row) {
        
        // --- Pannello Sinistro (Stats) ---
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        int spacesNeeded = leftColWidth - (int)leftRaw.length();
        if (spacesNeeded < 0) spacesNeeded = 0;
        std::string leftPanel = leftColor + std::string(spacesNeeded, ' ');

        // --- Pannello Destro Base (Senza Joker) ---
        // Contiene la mano (in basso) e il box comandi/deck (a destra)
        std::string rightPanel = getRightPanelContent(row, handStartRow, handHeight, deckStartRow, deckHeight, cardMatrix, deckVisual);
        
        // --- INIEZIONE JOKER NEL CENTRO ---
        // Se siamo nelle righe dedicate ai joker...
        if (row >= jokerStartRow && row < jokerStartRow + jokerHeight) {
            // Ottieni la riga grafica del joker corrente
            std::string jLine = jokersVisual[row - jokerStartRow];
            
            // Calcolo posizione orizzontale
            // Il Box Comandi inizia circa a colonna 85.
            // Vogliamo centrare i joker nello spazio disponibile (0 -> 85). Centro = ~42.
            int centerPos = 42;
            int visualLen = getVisualLength(jLine);
            int startPos = centerPos - (visualLen / 2);
            
            if (startPos < 2) startPos = 2; // Margine minimo sinistro

            // Controllo sovrapposizione: Assicuriamoci che rightPanel sia abbastanza lunga
            // e che non stiamo scrivendo sopra il box comandi
            if (startPos + visualLen < (int)rightPanel.length()) {
                // Sovrascriviamo gli spazi vuoti con la grafica del joker.
                // Nota: replace accetta la lunghezza in bytes, ma noi stiamo rimpiazzando spazi (1 byte = 1 char).
                // Quindi rimuoviamo 'visualLen' spazi e inseriamo 'jLine'.
                rightPanel.replace(startPos, visualLen, jLine);
            }
        }

        // Ranks box moved to left panel under ANTE

        // Assemblaggio messaggio IRC
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    // 5. Footer Tavolo
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";

    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}