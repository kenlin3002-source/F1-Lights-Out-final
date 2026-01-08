#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "rlutil.h"

// --- Global Variables ---
double bestTime = 999.0; 
double history[5] = {0}; 
int totalPlays = 0;      

// --- Meme Databases (The Soul of F1) ---
// 1. GOD MODE (Win / Fast)
const char* memesGod[] = {
    "Simply lovely. (Max)",
    "Haha, yes boys! (Max)",
    "Smooth operator~ (Carlos)",
    "Not bad for a number two driver. (Mark)",
    "Lando Nowins? Lando WINS! (Lando)"
};

// 2. COPIUM / EXCUSES (Average / Weird)
const char* memesCopium[] = {
    "Bono, my tyres are gone. (Lewis)",
    "The engine feels good... much slower than before. (Alonso)",
    "I have no power! (George/Everyone)",
    "It's bwoken! It's bwoken! (Lando)",
    "Leave me alone, I know what I'm doing. (Kimi)",
    "Blue flag! Blue flag! Honestly! (Seb)"
};

// 3. PAIN / RAGE (Fail / Slow / Jump Start)
const char* memesPain[] = {
    "I am stupid. I am stupid. (Charles)",
    "NOOOOOOOO! (Charles)",
    "GP2 Engine! GP2! (Alonso)",
    "Karma. (Alonso)",
    "No Mikey, no no Mikey! (Toto)",
    "FOR WHAT?! (Kimi)",
    "*** ***** *** (BEEP) (Yuki)"
};

// --- Save Score ---
void saveScore(double time) {
    if (time < bestTime) bestTime = time;
    for (int i = 4; i > 0; i--) history[i] = history[i - 1];
    history[0] = time;
    if (totalPlays < 5) totalPlays++;
}

// --- Calculate Average ---
double calculateAverage() {
    if (totalPlays == 0) return 0.0;
    double sum = 0;
    for (int i = 0; i < totalPlays; i++) sum += history[i];
    return sum / totalPlays;
}

// --- Rank System ---
const char* getRank(double time) {
    if (time < 0.20) return "Rank: Max Verstappen (World Champion)";
    if (time < 0.23) return "Rank: Lewis Hamilton (Legend)";
    if (time < 0.26) return "Rank: Charles Leclerc (Ferrari Ace)";
    if (time < 0.30) return "Rank: Fernando Alonso (Veteran)";
    if (time < 0.40) return "Rank: Safety Car Driver";
    return "Rank: Grandma Driving"; 
}

// --- Helper: Print Centered ---
void printCentered(int y, const char* text, int color) {
    int x = (tcols() - strlen(text)) / 2;
    if (x < 1) x = 1;
    locate(x, y);
    setColor(color);
    printf("%s", text);
}

// --- Helper: Draw Separator Line ---
void drawLine(int y) {
    const char* line = "--------------------------------";
    printCentered(y, line, GREY);
}

// --- Helper: Draw F1 LOGO (New!) ---
void drawF1Logo(int y) {
    setColor(LIGHTRED);
    // A stylized big "F1"
    printCentered(y,     " ______  __ ", LIGHTRED);
    printCentered(y + 1, "|  ____|/_ |", LIGHTRED);
    printCentered(y + 2, "| |__    | |", LIGHTRED);
    printCentered(y + 3, "|  __|   | |", LIGHTRED);
    printCentered(y + 4, "| |      | |", LIGHTRED);
    printCentered(y + 5, "|_|      |_|", LIGHTRED);
    resetColor();
}

// --- Helper: Get Random Meme ---
// category: 0=God, 1=Copium, 2=Pain
const char* getRandomMeme(int category) {
    int r;
    if (category == 0) {
        // Size of memesGod array is 5
        r = rand() % 5;
        return memesGod[r];
    } else if (category == 1) {
        // Size of memesCopium array is 6
        r = rand() % 6;
        return memesCopium[r];
    } else {
        // Size of memesPain array is 7
        r = rand() % 7;
        return memesPain[r];
    }
}

// --- Helper: Draw Lights ---
void drawLights(int stage) {
    int centerX = tcols() / 2;
    int startX = centerX - 14; 
    int y = 5; 

    for (int i = 0; i < 5; i++) {
        locate(startX + (i * 7), y);
        if (i < stage) {
            setColor(LIGHTRED);
            printf("(@)");
        } else {
            setColor(GREY);
            printf("( )");
        }
    }
    resetColor();
}

// --- Game Loop ---
void playGame() {
    cls();
    printCentered(3, "Get Ready...", WHITE);
    drawLights(0);
    // No car here, keep it clean for lights
    msleep(1000);

    int jumpStart = 0; 

    // 1. Lights On
    for (int i = 1; i <= 5; i++) {
        drawLights(i);
        msleep(1000); 
        if (kbhit()) { getkey(); jumpStart = 1; break; }
    }

    // 2. Random Wait
    if (jumpStart == 0) {
        int waitTime = 200 + rand() % 3000;
        clock_t waitStart = clock();
        while ((clock() - waitStart) * 1000 / CLOCKS_PER_SEC < waitTime) {
            if (kbhit()) { getkey(); jumpStart = 1; break; }
        }
    }

    // 3. Result
    if (jumpStart == 1) {
        setBackgroundColor(RED);
        cls();
        printCentered(8, "JUMP START!", WHITE);
        printCentered(10, "(False Start)", WHITE);
        
        // Random Rage Meme for Jump Start
        printCentered(14, "Team Radio:", YELLOW);
        printCentered(16, getRandomMeme(2), LIGHTCYAN); // Category 2: Pain/Rage
        
        msleep(500); 
    } else {
        setBackgroundColor(GREEN);
        cls();
        printCentered(10, "GO! GO! GO!", WHITE);
        
        clock_t startTime = clock();
        while (!kbhit()); 
        getkey(); 
        clock_t endTime = clock();

        // --- Stats Card ---
        setBackgroundColor(BLACK);
        cls();
        
        double reactionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        saveScore(reactionTime); 
        double avgTime = calculateAverage();

        // Title & Score
        printCentered(3, "You Passed!", WHITE);
        char scoreMsg[50];
        sprintf(scoreMsg, "%.4f s", reactionTime);
        printCentered(5, scoreMsg, LIGHTGREEN);

        // Rank
        printCentered(7, getRank(reactionTime), LIGHTMAGENTA);

        // --- TEAM RADIO MEME SECTION ---
        printCentered(9, "--- Team Radio ---", GREY);
        
        const char* meme;
        // Logic: 
        // Fast (<0.25) -> High chance of God Mode, small chance of Copium
        // Slow (>0.40) -> High chance of Pain
        // Average -> Mix of Copium and God Mode
        
        int r = rand() % 100;
        if (reactionTime < 0.25) {
            // Fast: 80% God Mode, 20% Copium (just for fun)
            if (r < 80) meme = getRandomMeme(0);
            else meme = getRandomMeme(1);
        } else if (reactionTime > 0.40) {
            // Slow: 70% Pain, 30% Copium
            if (r < 70) meme = getRandomMeme(2);
            else meme = getRandomMeme(1);
        } else {
            // Average: Mix of everything
            if (r < 40) meme = getRandomMeme(0); // God
            else if (r < 80) meme = getRandomMeme(1); // Copium
            else meme = getRandomMeme(2); // Pain (Random rage)
        }
        
        printCentered(11, meme, LIGHTCYAN);
        // -------------------------------

        drawLine(13);

        // Stats
        char statsMsg[100];
        sprintf(statsMsg, "Best: %.4fs  |  Avg: %.4fs", bestTime, avgTime);
        printCentered(15, statsMsg, YELLOW);

        drawLine(17);

        // History
        printCentered(19, "Your Recent Attempts", WHITE);
        for (int i = 0; i < totalPlays; i++) {
            char historyMsg[50];
            if (i == 0) sprintf(historyMsg, "%d. %.4f s (New)", i+1, history[i]);
            else sprintf(historyMsg, "%d. %.4f s", i+1, history[i]);
            printCentered(21 + i, historyMsg, GREY);
        }
    }

    resetColor();
    printCentered(28, "Press ANY KEY to return...", WHITE);
    getkey(); 
}

// --- Main Menu ---
int main() {
    srand(time(NULL));
    hidecursor();
    
    while (1) {
        setBackgroundColor(BLACK);
        cls(); 
        
        // --- Draw Menu ---
        printCentered(5, "===============================", LIGHTRED);
        printCentered(6, "      F1 LIGHTS OUT GAME       ", LIGHTRED);
        printCentered(7, "===============================", LIGHTRED);
        
        drawF1Logo(10); // NEW LOGO

        if (bestTime != 999.0) {
            char bestMsg[50];
            sprintf(bestMsg, "Session Best: %.4f s", bestTime);
            printCentered(20, bestMsg, YELLOW);
        }

        printCentered(22, "[SPACE] START GAME", WHITE);
        printCentered(23, "[ESC]   EXIT", WHITE);
        
        int k = getkey(); 
        
        if (k == KEY_SPACE) {
            playGame();
        } else if (k == KEY_ESCAPE) {
            break;
        }
    }

    showcursor();
    return 0;
}