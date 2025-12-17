#include "includes/Balatro.hpp"

void Balatro::generatePackPlanets() {
    this->packPlanets.clear();
    initPlanets(); 

    if (allPlanets.empty()) return;

    // Mescola e seleziona pianeti casuali
    std::vector<IPlanet*> candidates = allPlanets;
    std::random_shuffle(candidates.begin(), candidates.end());

    int amount = 5;
    if (candidates.size() < (size_t)amount) amount = candidates.size();

    for (int i = 0; i < amount; ++i) {
        this->packPlanets.push_back(candidates[i]);
    }
}

void Balatro::planetPackUI() {
    isInPlanetPackUI = 1;
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";

    int totalRows = 58;
    int leftColWidth = 29;
    int rightColWidth = 85; 

    // --- COLORI UI ---
    std::string RESET = "\x0f";
    std::string GREY  = "\x03" "14";
    std::string PURPLE = "\x03" "06"; 
    
    // Inizializza Canvas destro vuoto
    std::vector<std::string> rightCanvas(totalRows, std::string(rightColWidth, ' '));

    // 1. Genera Pianeti se necessario
    if (this->packPlanets.empty()) {
        generatePackPlanets();
    }

    // 2. Disegna HEADER (Celestial Pack)
    std::vector<std::string> headerBox;
    std::string hTop = "\xE2\x95\xAD" + repeat_string(40, "\xE2\x94\x80") + "\xE2\x95\xAE"; // ╭──╮
    std::string hBot = "\xE2\x95\xB0" + repeat_string(40, "\xE2\x94\x80") + "\xE2\x95\xAF"; // ╰──╯
    std::string vLine = "\xE2\x94\x82"; // │

    headerBox.push_back(GREY + hTop + RESET);
    headerBox.push_back(GREY + vLine + RESET + centerText("\x02" "CELESTIAL PACK", 40) + GREY + vLine + RESET);
    headerBox.push_back(GREY + vLine + RESET + centerText("Choose 1 Planet to level up your hands", 40) + GREY + vLine + RESET);
    headerBox.push_back(GREY + hBot + RESET);

    int headerCol = (rightColWidth - 42) / 2;
    pasteObject(rightCanvas, headerBox, 15, headerCol);

    // 3. Disegna PIANETI
    int cardsStartRow = 22;
    
    if (!packPlanets.empty()) {
        // A. Ottieni l'unica striscia combinata di tutti i pianeti
        std::vector<std::string> combinedVisual = getCombinedPlanetsVisual(packPlanets);
        
        // B. Calcola posizione centrata
        int visualLen = getVisualLength(combinedVisual[0]);
        int startCol = (rightColWidth - visualLen) / 2;
        if (startCol < 0) startCol = 0;

        // C. Incolla la striscia combinata
        pasteObject(rightCanvas, combinedVisual, cardsStartRow, startCol);

        // 4. Disegna BOTTONI [ !pick N ] sotto ogni pianeta
        int planetFullWidth = 18; 
        int gap = 1;

        for (size_t i = 0; i < packPlanets.size(); ++i) {
            std::string btnText = "[ !pick " + to_string_98(i + 1) + " ]";
            std::string coloredBtn = PURPLE + "[ !pick " + to_string_98(i + 1) + " ]" + RESET;
            
            int btnLen = getVisualLength(btnText);
            // Centra il bottone rispetto alla larghezza del pianeta (18)
            int btnPad = (planetFullWidth - btnLen) / 2;
            
            // Calcola la X assoluta
            int btnCol = startCol + (i * (planetFullWidth + gap)) + btnPad;
            
            int btnRow = cardsStartRow + (int)combinedVisual.size() + 1;

            if (btnRow < totalRows && btnCol < (int)rightCanvas[btnRow].length()) {
                std::string& line = rightCanvas[btnRow];
                std::string before = line.substr(0, btnCol);
                std::string after = "";
                if (btnCol + btnLen < (int)line.length()) {
                    after = line.substr(btnCol + btnLen);
                }
                line = before + coloredBtn + after;
            }
        }
    }

    // 5. Disegna Bottone SKIP
    std::vector<std::string> skipBox;
    std::string sTop = "\xE2\x95\xAD" + repeat_string(10, "\xE2\x94\x80") + "\xE2\x95\xAE";
    std::string sBot = "\xE2\x95\xB0" + repeat_string(10, "\xE2\x94\x80") + "\xE2\x95\xAF";
    
    skipBox.push_back(GREY + sTop + RESET);
    skipBox.push_back(GREY + vLine + RESET + " SKIP PACK" + GREY + vLine + RESET);
    skipBox.push_back(GREY + sBot + RESET);

    int skipRow = cardsStartRow + 14; 
    int skipCol = (rightColWidth - 12) / 2;
    pasteObject(rightCanvas, skipBox, skipRow, skipCol);

    std::string skipDesc = "!skip (Don't take any Planet)";
    int descLen = getVisualLength(skipDesc);
    int descCol = (rightColWidth - descLen) / 2;
    
    if (skipRow + 4 < totalRows) {
         std::string& line = rightCanvas[skipRow + 4];
         line = line.substr(0, descCol) + GREY + skipDesc + RESET + line.substr(descCol + descLen);
    }

    // 6. Invia Messaggio Finale (Merge Left + Right)
    std::string msg = "";
    
    // Top
    for(int i=0; i<5; i++) msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor); 
        
        int vLen = (int)leftRaw.length(); 
        int paddingNeeded = leftColWidth - vLen;
        if (paddingNeeded < 0) paddingNeeded = 0;

        std::string leftPanel = leftColor + std::string(paddingNeeded, ' ');
        std::string rightPanel = rightCanvas[row];
        
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }
    
    // Bottom
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}
