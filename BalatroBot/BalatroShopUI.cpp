#include "includes/Balatro.hpp"

std::string repeat_string(int count, const std::string& pattern) {
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

    // Bordi UTF-8
    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE"; // ╭ ╮
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF"; // ╰ ╯
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82"; // ─ │
    
    // Connettori per la linea divisoria (├ ┤)
    std::string T_L = "\xE2\x94\x9C"; 
    std::string T_R = "\xE2\x94\xA4"; 

    // --- DATI JOKER ---
    // Assumo tu abbia un metodo getDescription() o getEffect(). 
    // Se non lo hai, sostituisci con una stringa fissa o costruiscila qui.
    std::string name = joker->getName();
    std::string effect = joker->getEffect(); // <--- Assicurati che questo metodo esista in IJoker
    std::string cost = "$" + to_string_98(joker->getCost());

    // --- DIMENSIONI ---
    int width = 16;         // Larghezza totale interna
    int fixedHeight = 8;    // Altezza FISSA del contenuto (esclusi bordi sopra/sotto)
                            // Totale righe = fixedHeight + 2 (bordi)

    // 1. BORDO SUPERIORE
    box.push_back(GREY + TL + repeat_string(width, H) + TR + RESET);

    // 2. NOME (Centrato)
    if (name.length() > (size_t)width) name = name.substr(0, width);
    int padN = (width - getVisualLength(name)) / 2;
    if (padN < 0) padN = 0;
    
    box.push_back(GREY + V + RESET + 
                  repeat_char(padN, ' ') + RED + name + RESET + 
                  repeat_char(width - padN - getVisualLength(name), ' ') + 
                  GREY + V + RESET);

    // 3. DIVISORE (├──────────────┤)
    box.push_back(GREY + T_L + repeat_string(width, H) + T_R + RESET);

    // 4. EFFETTO (Word Wrapped) e COSTO
    // Calcoliamo quanto spazio abbiamo
    // Abbiamo 'fixedHeight' righe totali. 
    // Abbiamo già usato 1 riga per il Nome e 1 per il Divisore.
    // Rimangono (fixedHeight - 2) righe per Effetto + Costo.
    
    std::vector<std::string> effectLines = wrapText(effect, width);
    
    int linesUsed = 0;
    int linesAvailable = fixedHeight - 2; // -2 perché ho usato nome e divisore

    // Stampiamo le righe dell'effetto
    for (size_t i = 0; i < effectLines.size(); ++i) {
        if (linesUsed >= linesAvailable - 1) break; // Lascia sempre 1 riga per il costo
        
        std::string line = effectLines[i];
        int pad = width - getVisualLength(line);
        box.push_back(GREY + V + RESET + BLUE + line + RESET + repeat_char(pad, ' ') + GREY + V + RESET);
        linesUsed++;
    }

    // Riempiamo con righe vuote se l'effetto è corto
    while (linesUsed < linesAvailable - 1) {
        box.push_back(GREY + V + repeat_char(width, ' ') + GREY + V + RESET);
        linesUsed++;
    }

    // 5. COSTO (Ultima riga interna, allineata a destra o sinistra)
    int padC = width - getVisualLength(cost) - 1; // -1 per un po' di margine
    if (padC < 0) padC = 0;
    box.push_back(GREY + V + RESET + " " + GREEN + cost + RESET + repeat_char(padC, ' ') + GREY + V + RESET);

    // 6. BORDO INFERIORE
    box.push_back(GREY + BL + repeat_string(width, H) + BR + RESET);

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

void Balatro::printShopUI() {
    isShopUI = true;
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29;
    
    // Aumentiamo la larghezza del canvas destro per gestire comodamente il layout
    int rightColWidth = 85; 
    
    std::string ORANGE = "\x03" "07";
    std::string BLUE = "\x03" "12";
    std::string GREY = "\x03" "14";
    std::string RESET = "\x0f";
    
    // 1. Inizializza Canvas destro vuoto
    std::vector<std::string> rightCanvas(totalRows, std::string(rightColWidth, ' '));

    // --- SEZIONE 1: JOKER POSSEDUTI (In Alto) ---
    // Simile a printUI: visualizziamo i joker che il giocatore ha già
    if (!this->jokers.empty()) {
        std::vector<std::string> ownedVisual = getCombinedJokersVisual(this->jokers);
        if (!ownedVisual.empty()) {
            // Centriamo orizzontalmente
            int visualW = getVisualLength(ownedVisual[0]);
            int startCol = (rightColWidth - visualW) / 2;
            if (startCol < 0) startCol = 0;

            // Incolliamo alla riga 2 (subito sotto il bordo superiore)
            pasteObject(rightCanvas, ownedVisual, 2, startCol);
        }
    } else {
        // Opzionale: Messaggio "No Jokers Owned" se vuoto
        std::vector<std::string> emptyMsg = createMsgBox("NO JOKERS OWNED", GREY);
        int startCol = (rightColWidth - getVisualLength(emptyMsg[0])) / 2;
        pasteObject(rightCanvas, emptyMsg, 4, startCol);
    }

    // --- SEZIONE 2: SEPARATORE E JOKER DEL NEGOZIO (Al Centro) ---
    
    // Genera i joker se non esistono
    if (shopJokers.empty()) {
        generateShopJokers();
    }

    // Calcoliamo dove posizionare il blocco centrale
    int shopContentStartRow = 0;
    int shopContentHeight = 0;

    if (!shopJokers.empty()) {
        std::vector<std::string> shopVisual = getCombinedJokersVisual(shopJokers);
        shopContentHeight = (int)shopVisual.size();
        
        // Centratura Verticale: (Totale - Altezza) / 2
        shopContentStartRow = (totalRows - shopContentHeight) / 2;
        
        // Centratura Orizzontale
        int visualW = getVisualLength(shopVisual[0]);
        int startCol = (rightColWidth - visualW) / 2;
        if (startCol < 0) startCol = 0;

        // Incolla i Joker dello Shop
        pasteObject(rightCanvas, shopVisual, shopContentStartRow, startCol);
        
    } else {
        // Caso SOLD OUT
        std::vector<std::string> msg = createMsgBox("SOLD OUT", ORANGE);
        shopContentHeight = (int)msg.size();
        shopContentStartRow = (totalRows - shopContentHeight) / 2;
        
        int startCol = (rightColWidth - getVisualLength(msg[0])) / 2;
        pasteObject(rightCanvas, msg, shopContentStartRow, startCol);
    }

    // --- SEZIONE 3: COMANDI (Sotto i Joker dello Shop) ---
    // Posizioniamo i bottoni un po' sotto i joker dello shop
    int buttonsRow = shopContentStartRow + shopContentHeight + 2;

    // Bottone NEXT
    std::vector<std::string> nextBox = createMsgBox("!next to continue", ORANGE);
    int nextW = getVisualLength(nextBox[0]);
    
    // Bottone BUY
    std::string buyTxt = "!shop <1-" + to_string_98(shopJokers.size()) + "> to buy";
    if (shopJokers.empty()) buyTxt = "Check back later";
    std::vector<std::string> buyBox = createMsgBox(buyTxt, BLUE);
    int buyW = getVisualLength(buyBox[0]);

    // Posizioniamo i bottoni affiancati o uno sopra l'altro? Facciamo uno sopra l'altro per pulizia
    // Centriamo i bottoni
    int nextCol = (rightColWidth - nextW) / 2;
    int buyCol = (rightColWidth - buyW) / 2;

    pasteObject(rightCanvas, buyBox, buttonsRow, buyCol);
    pasteObject(rightCanvas, nextBox, buttonsRow + 4, nextCol); // 4 righe sotto il buy

    // --- STAMPA FINALE (OUTPUT) ---
    std::string msg = "";
    // Header
    for(int i=0; i<10; i++) msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        // Pannello Sinistro (Stats)
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        
        int vLen = (int)leftRaw.length(); // Nota: qui assumiamo raw length per il padding spazi, getVisualLength se leftRaw ha colori
        int paddingNeeded = leftColWidth - vLen;
        if (paddingNeeded < 0) paddingNeeded = 0;

        std::string leftPanel = leftColor + std::string(paddingNeeded, ' ');
        
        // Pannello Destro (Canvas)
        std::string rightPanel = rightCanvas[row];
        
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }
    // Footer
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}