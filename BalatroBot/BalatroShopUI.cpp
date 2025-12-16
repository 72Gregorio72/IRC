#include "includes/Balatro.hpp"

std::string Balatro::repeat_string(int count, const std::string& pattern) {
    std::string result = "";
    for (int i = 0; i < count; ++i) {
        result += pattern;
    }
    return result;
}


int Balatro::getVisualLength(const std::string& s) {
    int visualLen = 0;
    //bool inColor = false; // Flag per tracking stato colore (opzionale, qui gestiamo per char)

    for (size_t i = 0; i < s.length(); ++i) {
        unsigned char c = (unsigned char)s[i];

        // 1. Gestione Colori IRC (\x03)
        if (c == '\x03') {
            // Salta le cifre del primo colore (foreground)
            while (i + 1 < s.length() && isdigit(s[i+1])) { i++; }
            // Se c'è una virgola, controlla se seguono cifre (background)
            if (i + 1 < s.length() && s[i+1] == ',') {
                size_t j = i + 1; // Peek ahead
                if (j + 1 < s.length() && isdigit(s[j+1])) {
                    i++; // Consuma la virgola
                    while (i + 1 < s.length() && isdigit(s[i+1])) { i++; } // Consuma cifre bg
                }
            }
            continue; // Non contare questo carattere
        }

        // 2. Gestione altri controlli (Reset \x0f, Bold \x02, ecc.)
        // I char di controllo ASCII sono < 32. 
        if (c < 32) continue; 

        // 3. Gestione UTF-8
        // Byte di continuazione (10xxxxxx) non contano come colonna visiva
        if ((c & 0xC0) == 0x80) {
            continue;
        }

        // Se siamo qui, è un carattere visibile (ASCII o Inizio UTF-8)
        visualLen++;
    }
    return visualLen;
}

void Balatro::generateShopJokers() {
    std::srand(static_cast<unsigned int>(std::time(0)) + std::clock());
    shopJokers.clear();
    initAllJokers();

    if (allJokers.empty()) return;

    // 1. Creiamo una lista di candidati escludendo quelli già posseduti
    std::vector<IJoker*> candidates;
    for (size_t i = 0; i < allJokers.size(); ++i) {
        bool isOwned = false;
        
        // Controllo se è già nella mano del giocatore
        for (size_t j = 0; j < jokers.size(); ++j) {
            // Confrontiamo i puntatori o gli ID/Nomi se i puntatori vengono ricreati
            if (allJokers[i] == jokers[j]) { 
                isOwned = true;
                break;
            }
        }
        
        if (!isOwned) {
            candidates.push_back(allJokers[i]);
        }
    }

    // 2. Mescola i candidati disponibili
    // Nota: std::random_shuffle è deprecato in C++14 ma ok per C++98
    std::random_shuffle(candidates.begin(), candidates.end());

    // 3. Seleziona fino a 4 joker
    int maxShopItems = 4;
    int count = (candidates.size() < (size_t)maxShopItems) ? candidates.size() : maxShopItems;
    
    for (int i = 0; i < count; ++i) {
        shopJokers.push_back(candidates[i]);
    }
}

// Helper per spezzare il testo su più righe (Word Wrap)
std::vector<std::string> wrapText(std::string text, int maxWidth) {
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word;
    std::string currentLine;

    while (words >> word) {
        // Controlliamo la lunghezza visiva (senza codici colore, se il testo ne avesse)
        // Qui assumiamo che la descrizione sia testo pulito per semplicità
        if (currentLine.length() + word.length() + 1 > (size_t)maxWidth) {
            lines.push_back(currentLine);
            currentLine = word;
        } else {
            if (!currentLine.empty()) currentLine += " ";
            currentLine += word;
        }
    }
    if (!currentLine.empty()) lines.push_back(currentLine);
    return lines;
}

std::vector<std::string> Balatro::createJokerItem(IJoker* joker) {
    std::vector<std::string> box;
    
    // --- DEFINIZIONE COLORI E BORDI ---
    std::string RESET = "\x0f";
    std::string GREY  = "\x03" "14";
    std::string WHITE = "\x03" "00";
    std::string GREEN = "\x03" "09"; 
    std::string RED   = "\x03" "04"; 
    std::string BLUE  = "\x03" "12";

    // --- PUNTO DI CAMBIO COLORE ---
    // Determina il colore del bordo in base alla rarità del Joker
    std::string BORDER_COLOR = BLUE; // Default: Common (Blu)
    
    // Assumiamo che esista un metodo getRarity() che restituisce "Uncommon", "Rare", etc.
    std::string rarity = joker->getRarity(); 
    
    if (rarity == "Uncommon") {
        BORDER_COLOR = GREEN; // Non-comune (Verde chiaro)
    } else if (rarity == "Rare") {
        BORDER_COLOR = RED;   // Raro (Rosso)
    } else if (rarity == "Legendary") {
        BORDER_COLOR = "\x03" "06"; // Leggendario (Opzionale: Viola/Magenta)
    }
    
    // Bordi UTF-8
    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE"; // ╭ ╮
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF"; // ╰ ╯
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82"; // ─ │
    
    // Connettori per la linea divisoria (├ ┤)
    std::string T_L = "\xE2\x94\x9C"; 
    std::string T_R = "\xE2\x94\xA4"; 

    // --- DATI JOKER ---
    std::string name = joker->getName();
    std::string effect = joker->getEffect(); 
    std::string cost = "$" + to_string_98(joker->getCost());

    // --- DIMENSIONI ---
    int width = 16;         // Larghezza totale interna
    int fixedHeight = 8;    // Altezza FISSA del contenuto

    // 1. BORDO SUPERIORE (Uso BORDER_COLOR)
    box.push_back(BORDER_COLOR + TL + repeat_string(width, H) + TR + RESET);

    // 2. NOME (Centrato)
    if (name.length() > (size_t)width) name = name.substr(0, width);
    int padN = (width - getVisualLength(name)) / 2;
    if (padN < 0) padN = 0;
    
    // Nota: I bordi laterali (V) sono colorati con BORDER_COLOR
    // Il nome resta RED, ma puoi cambiarlo in WHITE se vuoi contrasto col bordo rosso
    box.push_back(BORDER_COLOR + V + RESET + 
                  repeat_char(padN, ' ') + RED + name + RESET + 
                  repeat_char(width - padN - getVisualLength(name), ' ') + 
                  BORDER_COLOR + V + RESET);

    // 3. DIVISORE (├──────────────┤) (Uso BORDER_COLOR)
    box.push_back(BORDER_COLOR + T_L + repeat_string(width, H) + T_R + RESET);

    // 4. EFFETTO (Word Wrapped) e COSTO
    std::vector<std::string> effectLines = wrapText(effect, width);
    
    int linesUsed = 0;
    int linesAvailable = fixedHeight - 2; 

    // Stampiamo le righe dell'effetto
    for (size_t i = 0; i < effectLines.size(); ++i) {
        if (linesUsed >= linesAvailable - 1) break; 
        
        std::string line = effectLines[i];
        int pad = width - getVisualLength(line);
        // Bordi laterali colorati con BORDER_COLOR
        box.push_back(BORDER_COLOR + V + RESET + BLUE + line + RESET + repeat_char(pad, ' ') + BORDER_COLOR + V + RESET);
        linesUsed++;
    }

    // Riempiamo con righe vuote se l'effetto è corto
    while (linesUsed < linesAvailable - 1) {
        box.push_back(BORDER_COLOR + V + repeat_char(width, ' ') + BORDER_COLOR + V + RESET);
        linesUsed++;
    }

    // 5. COSTO 
    int padC = width - getVisualLength(cost) - 1; 
    if (padC < 0) padC = 0;
    box.push_back(BORDER_COLOR + V + RESET + " " + GREEN + cost + RESET + repeat_char(padC, ' ') + BORDER_COLOR + V + RESET);

    // 6. BORDO INFERIORE (Uso BORDER_COLOR)
    box.push_back(BORDER_COLOR + BL + repeat_string(width, H) + BR + RESET);

    return box;
}

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}


void Balatro::pasteObject(std::vector<std::string>& canvas, const std::vector<std::string>& object, int startRow, int startCol) {
    if (object.empty()) return;
    int objHeight = (int)object.size();

    for (int r = 0; r < objHeight; ++r) {
        int targetRow = startRow + r;
        if (targetRow >= (int)canvas.size()) break;

        std::string& line = canvas[targetRow];
        
        // Se la posizione di partenza è valida
        if (startCol < (int)line.length()) {
            std::string objLine = object[r];
            
            // 1. Prendi la parte PRIMA dell'oggetto
            std::string before = line.substr(0, startCol);
            
            // 2. Calcola quanto spazio visivo occupa l'oggetto che stiamo incollando
            int visualWidth = getVisualLength(objLine);
            if(visualWidth == 0) visualWidth = (int)objLine.length(); // Fallback

            // 3. Prendi la parte DOPO l'oggetto
            // Dobbiamo saltare 'visualWidth' caratteri dallo sfondo originale (che sono spazi)
            std::string after = "";
            
            // Nota: Questo assume che lo sfondo 'line' sia composto da spazi semplici nel punto in cui incolliamo.
            // Se incolli sopra testo colorato esistente, l'indice sarà sballato. 
            // Ma per il Canvas vuoto funziona perfettamente.
            if (startCol + visualWidth < (int)line.length()) {
                after = line.substr(startCol + visualWidth);
            }
            
            // Ricostruisci la linea
            line = before + objLine + after;
        }
    }
}

// Helper per ripetere una stringa (necessario perché '─' sono 3 byte, non un char)


// Crea un quadrato semplice per l'item
std::vector<std::string> Balatro::createSimpleItem(int id, int cost) {
    std::vector<std::string> box;
    std::string RESET = "\x0f";
    std::string GREEN = "\x03" "09"; 
    std::string GREY = "\x03" "14";
    std::string WHITE = "\x03" "00";
    
    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";

    std::string sId = to_string_98(id);
    std::string sCost = "$" + to_string_98(cost);
    std::string hLine = repeat_string(10, H);

    // Top
    box.push_back(GREY + TL + hLine + TR + RESET);
    
    // Nome: ITEM X
    std::string name = "ITEM " + sId;
    int vLenN = getVisualLength(name); 
    int padN = (10 - vLenN) / 2;
    if (padN < 0) padN = 0;
    
    box.push_back(GREY + V + RESET + repeat_char(padN, ' ') + WHITE + name + RESET + repeat_char(10 - padN - vLenN, ' ') + GREY + V + RESET);
    
    // Costo: $10
    int vLenC = getVisualLength(sCost);
    int padC = (10 - vLenC) / 2;
    if (padC < 0) padC = 0;

    box.push_back(GREY + V + RESET + repeat_char(padC, ' ') + GREEN + sCost + RESET + repeat_char(10 - padC - vLenC, ' ') + GREY + V + RESET);
    
    // Bottom
    box.push_back(GREY + BL + hLine + BR + RESET);
    
    return box;
}


// Crea un box per i messaggi (Next / Buy)
std::vector<std::string> Balatro::createMsgBox(std::string text, std::string colorCode){
    std::vector<std::string> box;
    std::string RESET = "\x0f";
    std::string GREY = "\x03" "14";

    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";
    
    // Usa la lunghezza visiva per decidere la larghezza del box
    int vLen = getVisualLength(text);
    int innerWidth = vLen + 2; 
    
    std::string hLine = repeat_string(innerWidth, H);
    
    box.push_back(GREY + TL + hLine + TR + RESET);
    box.push_back(GREY + V + " " + RESET + colorCode + text + RESET + " " + GREY + V + RESET);
    box.push_back(GREY + BL + hLine + BR + RESET);
    
    return box;
}

// Crea un box per i Booster Pack (Pianeti/Joker)
// Crea un box per i Booster Pack con ASCII ART
// Crea un box per i Booster Pack con ASCII ART migliorata
// Crea un box per i Booster Pack con ASCII ART più grande e bella
// Crea un box per i Booster Pack con ASCII ART più grande e bella
std::vector<std::string> Balatro::createPackItem(std::string type, int cost, std::string color) {
    std::vector<std::string> box;
    
    // --- COLORI ---
    std::string RESET  = "\x0f";
    std::string WHITE  = "\x03" "00";
    std::string GREY   = "\x03" "14";
    std::string GREEN  = "\x03" "09"; 
    std::string YELLOW = "\x03" "08"; 

    // Bordi
    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";
    
    // Aumentiamo la larghezza per un look più "premium" (20 caratteri)
    int width = 20; 

    // --- ASCII ART AGGIORNATA ---
    std::vector<std::string> art;
    
    if (type == "BUFFOON") {
        // Cappello da Giullare grande con sonagli
        art.push_back("  " + color + "/\\   /\\" + RESET + "  ");
        art.push_back(" " + color + "/  \\ /  \\" + RESET + " ");
        art.push_back(YELLOW + "o" + RESET + color + "    v    " + RESET + YELLOW + "o" + RESET);
    } else {
        // Pianeta grande con anello e luna
        //      . '    
        //   _/( )\_   
        //  (   o   )  
        //   ~\( )/~   
        //    ' .      
        art.push_back("   " + GREY + "_/" + color + "( )" + GREY + "\\_" + RESET + "   ");
        art.push_back("  " + GREY + "(   " + WHITE + "o" + GREY + "   )" + RESET + "  ");
        art.push_back("   " + GREY + "~\\" + color + "( )" + GREY + "/~" + RESET + "   ");
    }

    // --- COSTRUZIONE BOX ---

    // 1. Top Border
    box.push_back(color + TL + repeat_string(width, H) + TR + RESET);

    // 2. Nome Tipo
    std::string label = type;
    int padL = (width - getVisualLength(label)) / 2;
    if (padL < 0) padL = 0;
    box.push_back(color + V + RESET + repeat_char(padL, ' ') + color + label + RESET + 
                  repeat_char(width - padL - getVisualLength(label), ' ') + color + V + RESET);

    // 3. Spazio extra
    box.push_back(color + V + repeat_char(width, ' ') + color + V + RESET);

    // 4. ASCII ART Centrata
    for (size_t i = 0; i < art.size(); ++i) {
        std::string line = art[i];
        int vLen = getVisualLength(line);
        int pad = (width - vLen) / 2;
        if (pad < 0) pad = 0;
        box.push_back(color + V + RESET + repeat_char(pad, ' ') + line + repeat_char(width - pad - vLen, ' ') + color + V + RESET);
    }

    // 5. Spazio extra
    box.push_back(color + V + repeat_char(width, ' ') + color + V + RESET);
    
    // 6. Label "PACK"
    std::string sub = "PACK";
    int padS = (width - 4) / 2;
    box.push_back(color + V + RESET + repeat_char(padS, ' ') + GREY + sub + RESET + 
                  repeat_char(width - padS - 4, ' ') + color + V + RESET);

    // 7. Costo
    std::string sCost = "$" + toString(cost);
    int padC = (width - getVisualLength(sCost)) / 2;
    if (padC < 0) padC = 0;
    box.push_back(color + V + RESET + repeat_char(padC, ' ') + GREEN + sCost + RESET + 
                  repeat_char(width - padC - getVisualLength(sCost), ' ') + color + V + RESET);

    // 8. Bottom Border
    box.push_back(color + BL + repeat_string(width, H) + BR + RESET);

    return box;
}

void Balatro::printShopUI() {
    isShopUI = true;
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29;
    int rightColWidth = 85; 
    
    // --- DEFINIZIONE COLORI ---
    std::string C_ORANGE  = "\x03" "07";
    std::string C_GREY    = "\x03" "14";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";
    std::string C_BLUE    = "\x03" "12"; 
    std::string C_RED     = "\x03" "04"; 
    
    // Scala colori comandi
    std::string C_SCALE_1 = "\x03" "11"; 
    std::string C_SCALE_3 = "\x03" "12"; 
    std::string C_SCALE_7 = "\x03" "04"; 
    
    std::vector<std::string> rightCanvas(totalRows, std::string(rightColWidth, ' '));

    // =================================================================================
    // 1. JOKER POSSEDUTI (Riga 2)
    // =================================================================================
    if (!this->jokers.empty()) {
        std::vector<std::string> ownedVisual = getCombinedJokersVisual(this->jokers);
        if (!ownedVisual.empty()) {
            int visualW = getVisualLength(ownedVisual[0]);
            int startCol = (rightColWidth - visualW) / 2;
            if (startCol < 0) startCol = 0;
            pasteObject(rightCanvas, ownedVisual, 2, startCol);
        }
    } else {
        std::vector<std::string> emptyMsg = createMsgBox("NO JOKERS OWNED", C_GREY);
        int startCol = (rightColWidth - getVisualLength(emptyMsg[0])) / 2;
        pasteObject(rightCanvas, emptyMsg, 4, startCol);
    }

    // =================================================================================
    // 2. JOKER DEL NEGOZIO
    // =================================================================================
    if (shopJokers.empty()) {
        generateShopJokers();
    }

    int shopContentStartRow = 13; // Leggermente più in alto per fare spazio ai pacchetti
    int shopContentHeight = 0;

    if (!shopJokers.empty()) {
        std::vector<std::string> shopVisual = getCombinedJokersVisual(shopJokers);
        shopContentHeight = (int)shopVisual.size();
        
        int visualW = getVisualLength(shopVisual[0]);
        int startCol = (rightColWidth - visualW) / 2;
        if (startCol < 0) startCol = 0;

        pasteObject(rightCanvas, shopVisual, shopContentStartRow, startCol);
    } else {
        std::vector<std::string> msg = createMsgBox("SOLD OUT", C_ORANGE);
        shopContentHeight = (int)msg.size();
        int startCol = (rightColWidth - getVisualLength(msg[0])) / 2;
        pasteObject(rightCanvas, msg, shopContentStartRow + 2, startCol);
    }

    // =================================================================================
    // 3. BOOSTER PACKS (CON ASCII ART)
    // =================================================================================
    
    std::vector<std::string> packBuffoon   = createPackItem("BUFFOON", 6, C_RED);
    std::vector<std::string> packCelestial = createPackItem("CELESTIAL", 6, C_BLUE);
    
    std::vector<std::string> packsRowVisual;
    // Assumiamo che abbiano la stessa altezza, usiamo quella di uno dei due
    int packHeight = (int)packBuffoon.size(); 
    
    // Uniamo i due pacchetti con spaziatura: [Buffoon] ...spazio... [Celestial]
    for(int i = 0; i < packHeight; ++i) {
        // 4 spazi tra i pacchetti per distanziarli bene
        std::string line = packBuffoon[i] + "    " + packCelestial[i];
        packsRowVisual.push_back(line);
    }
    
    // Posizione: Sotto i Joker shop
    // Dato che i pacchetti sono più alti (nuova art), abbassiamo leggermente lo startRow
    int packStartRow = shopContentStartRow + shopContentHeight + 2; 
    
    int packVisualW = getVisualLength(packsRowVisual[0]);
    int packStartCol = (rightColWidth - packVisualW) / 2;
    if (packStartCol < 0) packStartCol = 0;

    pasteObject(rightCanvas, packsRowVisual, packStartRow, packStartCol);

    // =================================================================================
    // 4. BOX COMANDI
    // =================================================================================
    
    std::vector<std::string> cmdBox;
    int boxWidth = 44;
    std::string vBorder = C_ORANGE + "│" + RESET;
    std::string spacerLine = vBorder + std::string(boxWidth - 2, ' ') + vBorder;

    cmdBox.push_back(C_ORANGE + "┌──────────────────────────────────────────┐" + RESET);
    std::string titleTxt = BOLD + centerText("SHOP COMMANDS", boxWidth - 2) + RESET;
    cmdBox.push_back(vBorder + titleTxt + vBorder); 
    cmdBox.push_back(C_ORANGE + "├──────────────────────────────────────────┤" + RESET);

    const int CMD_COUNT = 3;
    std::string cmdColors[CMD_COUNT]; 
    cmdColors[0] = C_SCALE_1; cmdColors[1] = C_SCALE_3; cmdColors[2] = C_SCALE_7;
    std::string cmdNames[CMD_COUNT];
    cmdNames[0] = "!shop"; cmdNames[1] = "!reroll"; cmdNames[2] = "!next";
    std::string cmdArgs[CMD_COUNT];
    cmdArgs[0] = "<id>"; cmdArgs[1] = ""; cmdArgs[2] = "";
    std::string cmdDescs[CMD_COUNT];
    cmdDescs[0] = "(es: !shop 1 3)"; cmdDescs[1] = "(Reroll Shop - $5)"; cmdDescs[2] = "(Start Next Round)";

    for(int i = 0; i < CMD_COUNT; ++i) {
        std::string rawLine = " " + cmdColors[i] + cmdNames[i] + RESET;
        if (cmdArgs[i] != "") rawLine += " " + cmdArgs[i];
        rawLine += C_GREY + " " + cmdDescs[i] + RESET;

        int visLen = getVisualLength(rawLine);
        int pad = (boxWidth - 2) - visLen;
        if (pad < 0) pad = 0;

        cmdBox.push_back(vBorder + rawLine + std::string(pad, ' ') + vBorder);
        if (i < CMD_COUNT - 1) cmdBox.push_back(spacerLine);
    }
    cmdBox.push_back(C_ORANGE + "└──────────────────────────────────────────┘" + RESET);

    // Posizionamento: In basso
    int cmdBoxHeight = (int)cmdBox.size();
    int cmdBoxRow = totalRows - cmdBoxHeight - 2; 
    int cmdBoxCol = (rightColWidth - boxWidth) / 2; 

    pasteObject(rightCanvas, cmdBox, cmdBoxRow, cmdBoxCol);

    // =================================================================================
    // 5. SEND
    // =================================================================================
    std::string outMsg = "";
    
    // Header
    for(int i=0; i<5; i++) outMsg += prefix + " \r\n";
    outMsg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    outMsg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        // Pannello Sinistro
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor); 
        
        int vLen = (int)leftRaw.length(); 
        int paddingNeeded = leftColWidth - vLen;
        if (paddingNeeded < 0) paddingNeeded = 0;

        std::string leftPanel = leftColor + std::string(paddingNeeded, ' ');
        std::string rightPanel = rightCanvas[row];
        
        outMsg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }
    // Footer
    outMsg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    outMsg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, outMsg.c_str(), outMsg.length(), MSG_NOSIGNAL);
}