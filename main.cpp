/*
    Created 3/24/2023 by waibibabow
    Song of Source Monitor Plus music video

    Entire window must remain visible in terminal, else cursor movement won't work
    Avoid \n to reduce premature flushing
    Reduce number of couts for speed
    Use a custom buffer (and a fast clearBuffer()) next time
*/
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <chrono>
#include <cmath>
#include <math.h>
#include <thread>
#include <vector>
#include <string>
#include <iterator>
#include <regex>
#include <locale>
#include <codecvt>
#include "text.h"
#include "lyrics.h"
#include "cursor.h"
#include "utf8.h"
#include "time.h"
#include "globals.h"


void showTimerDelay();
void showTimeTotals();
void showTimeSizes();
void replace(std::string &s, char c1, char c2);
void loadLyrics();
void clearLyrics();
void loadSMP();
void loadSMPRowOffset();
void loadWaves();
void loadFrameWave();
void loadFrameLowTide();
void lineSplit(std::string &s, unsigned int length);
void printVideo();
int printSMPExecuting(int thirtysecond, int &cursorRow, int &cursorCol);
int updateSMP(int thirtysecond, int &cursorRow, int &cursorCol, std::vector<std::vector<bool>> &reverted, int &revertfrontRow, int &revertfrontCol);
int printSMP(int thirtysecond, int &cursorRow, int &cursorCol);
int updateLyrics(int thirtysecond, int &timingIndex, int &durationIndex, int &sixteenthOffset, int &lyricIndex, int &cursorDisplayRow, int &cursorDisplayCol);
int printLyrics(int thirtysecond, int &cursorRow, int &cursorCol);
void pushLyrics();
std::string removeANSI(std::string s);
int clearWindow(int &cursorRow, int &cursorCol);
int updateMaxDelay(int thirtysecond, int &maxTotalDelay, int totalDelay, int &slowestThirtysecond);
int genericFunction(int thirtysecond, int &cursorRow, int &cursorCol, int totalDelay);
int updateWaves(int thirtysecond, int &cursorRow, int &cursorCol, char fillChar, int &startOutIndex, int &dryLine, std::vector<std::string> &edgeChars, std::vector<int> &edgeCharWidths, std::vector<int> &edgeFillWidths);
int printWaves(int thirtysecond, int &cursorRow, int &cursorCol, int dryLine);
int printLowTide(int thirtysecond, int &cursorRow, int &cursorCol);


int main(void)
{
    //std::ios_base::sync_with_stdio(false);
    srand((unsigned int)time(NULL));
    loadSMPRowOffset();
    loadSMP();
    loadLyrics();
    loadWaves();
    loadFrameWave();
    loadFrameLowTide();
    printVideo();

    return 0;
}

void showTimerDelay() {
    int n = 2000;
    auto startTime = std::chrono::steady_clock::now();
    for (int i=0; i < n; ++i) {
        wait16ths(1, 0);
    }
    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    int td = (timeNS.count()-n*NSPS)/n;
    std::cout << "Timer Delay: " << td << std::endl;
}

void showTimeTotals() {
    for (int timingItr=0; timingItr < timings.size(); ++timingItr) {
        int total = 0;
        std::vector<unsigned int> durations = timings[timingItr];
        for (int durationItr=0; durationItr < durations.size(); ++durationItr) {
            total += durations[durationItr];
        }
        std::cout << total << std::endl;
    }
}

void showTimeSizes() {
    for (int timingItr=0; timingItr < timings.size(); ++timingItr) {
        std::cout << timings[timingItr].size() << std::endl;
    }
}

void replace(std::string &s, char c1, char c2) {
    for (unsigned int i=0; i<s.length(); ++i) {
        if (s[i] == c1) {
            s[i] = c2;
        }
    }
}

void loadLyrics() {
    int rowCount = (int)std::ceil((double)executing.length()/LYRIC_WIDTH);

    //load message rows
    for (int row=0; row < rowCount; ++row) {
        std::string rowString;
        if (row == 0 && LYRIC_WIDTH >= 3) {
            rowString += "\033[7m";
            rowString += executing.substr(0, 3);
            rowString += "\033[0m";
            rowString += executing.substr(3, LYRIC_WIDTH-3);
        }
        else {
            rowString += executing.substr((unsigned)row*LYRIC_WIDTH, LYRIC_WIDTH);
        }
        lyrics.push_back(rowString);
    }

    //std::string empty(LYRIC_WIDTH, ' ');
    for (int row=rowCount; row < LYRIC_HEIGHT; ++row) {
        lyrics.push_back("");
    }
}

void clearLyrics() {
    for (int row=0; row < lyrics.size(); ++row) {
        lyrics[row] = "";
    }
}

void loadSMP() {
    for (int row=SMP_Y0; row < SMP_HEIGHT; ++row) {
        smp.push_back("");
    }
}

void loadSMPRowOffset() {
    SMP_Y0 = std::ceil((double)executing.length()/SMP_WIDTH) + 1;
    LOGO_Y0 = (SMP_HEIGHT-SMP_Y0-LOGO_HEIGHT) / 2;
    SMP_M1 = SMP_WIDTH*(SMP_HEIGHT-SMP_Y0) + SMP_M0;
    LAST_BAR_M0 = std::floor((double)(LOW_TIDE_T0+LYRIC_AREA+SMP_Y0*SMP_WIDTH-SMP_M0)/31);
    REVERT_PER_THIRTYSECOND = std::ceil((double)(LAST_BAR_M0-SMP_M0+1)/REVERT_THIRTYSECOND_COUNT);
    //std::cout << "smp row offset: " << std::to_string(SMP_ROW_OFFSET) << std::endl;
}

void loadWaves() {
    for (int row=0; row < WINDOW_HEIGHT; ++row) {
        waves.push_back("");
        waveWidths.push_back(0);
        waveLengths.push_back(0);
    }
}

//without newlines
void loadFrameWave () {
    //std::string box(SMP_AREA, '|');
    //frameWave += box;

    //random ascii
    bool drunk = false;
    int colCount = 0;
    int waveCharLength = lengthU8(waveChars);
    while (colCount < WAVE_THIRTYSECOND_COUNT) {
        std::string s = substrU8(waveChars, rand()%waveCharLength, 1);
        int width = charWidth(toChar(s));
        if (colCount+width <= WAVE_THIRTYSECOND_COUNT) {
            frameWave += s;
            colCount += width;

            //bbgb
            if (s == "ロ" && colCount >= 13 && !drunk) {
                overwriteColU8(frameWave, colCount-13, bbgb, U'*');
                drunk = true;
            }
        }
    }

    //wave crests
    for (int colItr=0; colItr < WAVE_THIRTYSECOND_COUNT; ++colItr) {
        double pStar = cos((double)(WAVE_THIRTYSECOND_COUNT-colItr)/WAVE_THIRTYSECOND_PERIOD*2*M_PI);
        if (colItr < DEEP_THIRTYSECOND_COUNT) {
            pStar *= 0.57;
        }
        else {
            pStar *= 0.75;
        }
        double r = (double)rand()/(double)RAND_MAX;
        if (r <= pStar) {
            int fillIndex = rand()%(waveCharLength-3)+3;
            std::string s = substrU8(waveChars, fillIndex, 1);
            char32_t fill = toChar(s);
            overwriteColU8(frameWave, colItr, "*", fill);
        }
    }

    //wavelength
    WAVE_LENGTH = lengthU8(frameWave);
}

void loadFrameLowTide() {
    //lyric window
    for (int row=0; row < LYRIC_HEIGHT; ++row) {
        std::vector<std::string> vrow;
        for (int col=0; col < LYRIC_WIDTH; ++col) {
            vrow.push_back(" ");
        }
        frameLowTide.push_back(vrow);
    }

    int executingLength = executing.length();
    for (int row=0; row < SMP_HEIGHT; ++row) {
        //NTE window
        std::vector<std::string> vrow;
        for (int col=0; col < NTE_WIDTH; ++col) {
            vrow.push_back(" ");
        }

        //SMP window
        for (int col=0; col < SMP_WIDTH; ++col) {
            std::string s = " ";
            int index = row*SMP_WIDTH + col;
            int measure = index - SMP_Y0*SMP_WIDTH;
            if (index < executingLength) {
                if (index == 20 || index == 24 || index == 34 || index == 40 || index == 46) {
                    s = "_";
                }
                else {
                    s = substrU8(executing, index, 1);
                }
                if (index < 3) {
                    s = "\033[7m" + s + "\033[0m";
                }
            }
            else if (measure >= 0 && measure <= LAST_BAR_M0-SMP_M0) {
                s = "_";
            }
            vrow.push_back(s);
        }
        frameLowTide.push_back(vrow);
    }
}

void lineSplit(std::string &s, unsigned int length) {
    for (unsigned int i=(unsigned int)(lengthU8(s)-lengthU8(s)%length); i != 0; i-=length) {
        insertU8(s, i, "\n");
    }
    //remove end \n
    if (substrU8(s, lengthU8(s)-1, 1) == "\n")
        s = substrU8(s, 0, lengthU8(s)-1);
}

//sixteenth starting from 1
void printVideo() {
    int cursorRow=0, cursorCol=0;
    //SMP
    std::vector<std::vector<bool>> reverted(SMP_HEIGHT-SMP_Y0, std::vector<bool>(SMP_WIDTH, false));
    int revertfrontRow=0, revertfrontCol=0;
    //lyrics
    int cursorDisplayRow=2, cursorDisplayCol=0;
    int timingIndex=0, durationIndex=0, sixteenthOffset=0, lyricIndex=-1;
    //waves
    int startOutIndex=WAVE_LENGTH-1, dryLine = -1;
    std::vector<std::string> edgeChars(WINDOW_HEIGHT, "");
    std::vector<int> edgeCharWidths(WINDOW_HEIGHT, 0);
    std::vector<int> edgeFillWidths(WINDOW_HEIGHT, 0);

    int maxTotalDelay=0, slowestThirtysecond=0;
    for (int thirtysecond=0; thirtysecond < SONG_THIRTYSECOND_COUNT; ++thirtysecond) {
        int total_delay = TIMER_DELAY;
        //total_delay += FUNCTION_DELAY + clearWindow(cursorRow, cursorCol);
        total_delay += FUNCTION_DELAY + printSMPExecuting(thirtysecond, cursorRow, cursorCol);
        total_delay += FUNCTION_DELAY + updateSMP(thirtysecond, cursorRow, cursorCol, reverted, revertfrontRow, revertfrontCol);
        total_delay += FUNCTION_DELAY + printSMP(thirtysecond, cursorRow, cursorCol);
        total_delay += FUNCTION_DELAY + updateLyrics(thirtysecond, timingIndex, durationIndex, sixteenthOffset, lyricIndex, cursorDisplayRow, cursorDisplayCol);
        total_delay += FUNCTION_DELAY + printLyrics(thirtysecond, cursorRow, cursorCol);
        total_delay += FUNCTION_DELAY + updateWaves(thirtysecond, cursorRow, cursorCol, '*', startOutIndex, dryLine, edgeChars, edgeCharWidths, edgeFillWidths);
        total_delay += FUNCTION_DELAY + printWaves(thirtysecond, cursorRow, cursorCol, dryLine);
        total_delay += FUNCTION_DELAY + printLowTide(thirtysecond, cursorRow, cursorCol);

        //move cursor to lyric window
        //moveCursor(cursorRow, cursorCol, cursorDisplayRow, cursorDisplayCol);

        //print
        std::cout << std::flush;

        //wait
        total_delay += FUNCTION_DELAY + updateMaxDelay(thirtysecond, maxTotalDelay, total_delay, slowestThirtysecond);
        wait32nds(1, total_delay);
    }

    //end
    std::string delayMessage = "max total delay: " + std::to_string(maxTotalDelay) + "ns at t=" + std::to_string(slowestThirtysecond) + "; ";
    if (maxTotalDelay < NSPT) {
        delayMessage += "buffer is sufficient";
    }
    else {
        delayMessage += "under-buffering occurred";
    }
    printMessage(delayMessage, cursorRow, cursorCol, 0, 0, delayMessage.length(), 0);
    moveCursor(cursorRow, cursorCol, 0, 1); //end here
}

//return total delay in ns
int printSMPExecuting(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    if (thirtysecond%2 == 0) {
        int sixteenth = thirtysecond/2;
        if (sixteenth == 0) { //initial print
            int rowCount = (int)std::ceil((double)executing.length()/SMP_WIDTH);

            for (int row=0; row < rowCount; ++row) {
                //move cursor
                moveCursor(cursorRow, cursorCol, NTE_WIDTH, row+LYRIC_HEIGHT);

                if (row == 0 && SMP_WIDTH >= 3) {
                    std::cout << "\033[7m";
                    std::cout << executing.substr(0, 3);
                    std::cout << "\033[0m";
                    std::cout <<  executing.substr(3, SMP_WIDTH-3);
                }
                else {
                    std::cout << executing.substr((unsigned)row*SMP_WIDTH, SMP_WIDTH);
                }
                cursorCol += (row==rowCount-1) ? ((int)executing.length() - row*SMP_WIDTH) : SMP_WIDTH;
            }
        }
        if (sixteenth%4 == 0) {
            int beat = sixteenth/4;
            int measure = beat/4;
            std::vector<int> beatIndices;
            if (beat >= 225 && beat <= 877) { //index 20, m52
                beatIndices.push_back(20);
            }
            if (beat >= 209 && beat <= 881) { //index 24, m48
                beatIndices.push_back(24);
            }
            if (beat >= 193 && beat <= 885)  { //index 34, m44
                beatIndices.push_back(34);
            }
            if (beat >= 177 && beat <= 889) { //index 40, m40
                beatIndices.push_back(40);
            }
            if (beat >= 161 && beat <= 893) { //index 46, m36
                beatIndices.push_back(46);
            }

            for (unsigned int beatIndexItr=0; beatIndexItr < beatIndices.size(); ++beatIndexItr) {
                int index = beatIndices[beatIndexItr];
                int row = index/SMP_WIDTH;
                int col = NTE_WIDTH + index - row*SMP_WIDTH;
                printAnimationChar(beat, cursorRow, cursorCol, col, row+LYRIC_HEIGHT);
            }
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

//return total_delay in ns
int updateSMP(int thirtysecond, int &cursorRow, int &cursorCol, std::vector<std::vector<bool>> &reverted, int &revertfrontRow, int &revertfrontCol) {
    auto startTime = std::chrono::steady_clock::now();

    int beat = thirtysecond/8;
    int measure = beat/4;
    bool flattened = (thirtysecond >= LOW_TIDE_T0+LYRIC_AREA && measure < VERSE6_M0) ? true : false;

    //update reverted
    if (flattened) {
        //sixteenth, revert 0-1 random chars
        if (thirtysecond%2 == 0 && measure >= VERSE5_M0 /*&& measure < VERSE6_M0*/) {
            int revertCount = rand()%42 - 40;
            while (revertCount > 0) {
                int measureIndex = rand()%(LAST_BAR_M0-SMP_M0+1);
                int row = measureIndex/SMP_WIDTH;
                int col = measureIndex - row*SMP_WIDTH;
                if (!reverted[row][col]) {
                    reverted[row][col] = true;
                    --revertCount;
                }
            }
        }

        //verse5, revert 4-5 random chars per downbeat
        if (thirtysecond%32 == 0 && measure >= VERSE5_M0 && measure < INTRO2_M0) {
            int revertCount = rand()%2 + 4;
            while (revertCount != 0) {
                int measureIndex = rand()%(LAST_BAR_M0-SMP_M0+1);
                int row = measureIndex/SMP_WIDTH;
                int col = measureIndex - row*SMP_WIDTH;
                if (!reverted[row][col]) {
                    reverted[row][col] = true;
                    --revertCount;
                }
            }
        }

        //low tide, new measures are reverted
        if (measure > LAST_BAR_M0 /*&& measure < SMP_M1*/) {
            int row = (measure-SMP_M0)/SMP_WIDTH;
            int col = measure-SMP_M0-row*SMP_WIDTH;
            reverted[row][col] = true;
        }

        //end of intro2, revert all
        if (thirtysecond >= REVERT_T0) {
            int revertCount = 0;
            while (revertfrontRow < SMP_HEIGHT-SMP_Y0 && revertCount < REVERT_PER_THIRTYSECOND) {
                reverted[revertfrontRow][revertfrontCol] = true;
                ++revertCount;
                
                ++revertfrontCol;
                if (revertfrontCol == SMP_WIDTH) {
                    revertfrontCol = 0;
                    ++revertfrontRow;
                }
            }
        }
    }

    //update smp
    if (measure >= SMP_M0 && (measure < SMP_M1 || (measure == SMP_M1 && beat%4 == 0))) {
        int charCount = measure - SMP_M0 + 1;
        int row = 0;
        int col = 0;
        std::string srow;
        while (charCount != 0) {
            if (flattened && !reverted[row][col]) {
                srow += '_';
            }
            else if (row >= LOGO_Y0 && row < LOGO_Y0+LOGO_HEIGHT && col >= LOGO_X0 && col < LOGO_X0+LOGO_WIDTH) {
                char c = SMP[row-LOGO_Y0][col-LOGO_X0];
                if (c != ' ') {
                    srow += c;
                }
                else {
                    srow += animationLOGO[beat%4];
                }
            }
            else if (row < SMP_HEIGHT-SMP_Y0) {
                srow += animationLOGO[beat%4];
            }

            ++col;
            if (col == SMP_WIDTH) {
                smp[row] = srow;
                srow = "";
                col = 0;
                ++row;
            }
            --charCount;
        }
        if (row < SMP_HEIGHT-SMP_Y0) {
            smp[row] = srow;
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printSMP(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    int y0 = LYRIC_HEIGHT + SMP_Y0;
    for (int row=0; row < smp.size(); ++row) {
        std::string srow = smp[row];
        printMessage(srow, cursorRow, cursorCol, NTE_WIDTH, y0+row, srow.length(), 0);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int updateLyrics(int thirtysecond, int &timingIndex, int &durationIndex, int &sixteenthOffset, int &lyricIndex, int &cursorDisplayRow, int &cursorDisplayCol) {
    auto startTime = std::chrono::steady_clock::now();

    if (thirtysecond == WAVE_T0+LYRIC_AREA) {
        cursorDisplayRow = 0;
        cursorDisplayCol = 0;
        clearLyrics();
    }
    if (thirtysecond%2 == 0) {
        std::vector<unsigned int> timing = timings[song[timingIndex]];

        //increment time variables
        ++sixteenthOffset;
        while (sixteenthOffset == timing[durationIndex]) {
            sixteenthOffset = 0;
            ++durationIndex;
            if (durationIndex == timing.size()) {
                durationIndex = 0;
                ++timingIndex;
                timing = timings[song[timingIndex]];
            }
            else if (hasLyrics(song[timingIndex])) {
                ++lyricIndex;
                std::string text = lyricsAll[lyricIndex];
                if (text[0] == '%') {
                    lyrics[cursorDisplayRow] = "";
                    cursorDisplayCol = 0;
                    text = text.substr(1, text.length()-1);
                }
                else if (text[0] == '\n') {
                    if (cursorDisplayRow == LYRIC_HEIGHT-1) { //last row, push
                        pushLyrics();
                    }
                    else {
                        ++cursorDisplayRow;
                    }
                    cursorDisplayCol = 0;
                    text = text.substr(1, text.length()-1);
                }

                //add text, go to new row(s) if necessary
                int rowCount = (int)std::ceil((double)text.length()/LYRIC_WIDTH);
                for (int row=0; row < rowCount; ++row) {
                    lyrics[cursorDisplayRow] += text.substr((unsigned)row*LYRIC_WIDTH, LYRIC_WIDTH);
                    if (row == rowCount-1) {
                        cursorDisplayCol += text.length() - row*LYRIC_WIDTH;
                    }
                    else {
                        if (cursorDisplayRow == LYRIC_HEIGHT-1) {
                            pushLyrics();
                        }
                        else {
                            ++cursorDisplayRow;
                        }
                        cursorDisplayCol = 0;
                    }
                }
            }
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printLyrics(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    for (int row=0; row < lyrics.size(); ++row) {
        moveCursor(cursorRow, cursorCol, LYRIC_WIDTH-1, row);
        clearLineToLeft();
        moveCursor(cursorRow, cursorCol, 0, row);
        std::cout << lyrics[row];
        cursorCol += removeANSI(lyrics[row]).length();
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

void pushLyrics() {
    for (int row=0; row < LYRIC_HEIGHT-1; ++row) {
        lyrics[row] = lyrics[row+1];
    }
    lyrics[LYRIC_HEIGHT-1] = "";
}

std::string removeANSI(std::string s) {
    std::regex pattern("\e\\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]"); // Matches ANSI escape codes
    std::string ans = std::regex_replace(s, pattern, ""); // Removes escape codes
    return ans;
}

int clearWindow(int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    std::string empty(WINDOW_WIDTH, ' ');
    for (int row=0; row < WINDOW_HEIGHT; ++row) {
        moveCursor(cursorRow, cursorCol, 0, row);
        std::cout << empty;
        cursorCol += WINDOW_WIDTH;
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int updateMaxDelay(int thirtysecond, int &maxTotalDelay, int totalDelay, int &slowestThirtysecond) {
    auto startTime = std::chrono::steady_clock::now();

    if (totalDelay > maxTotalDelay) {
        maxTotalDelay = totalDelay;
        slowestThirtysecond = thirtysecond;
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int genericFunction(int thirtysecond, int &cursorRow, int &cursorCol, int totalDelay) {
    return totalDelay + thirtysecond;
}

//update waves, waveWidths, waveLengths
int updateWaves(int thirtysecond, int &cursorRow, int &cursorCol, char fillChar, int &startOutIndex, int &dryLine, std::vector<std::string> &edgeChars, std::vector<int> &edgeCharWidths, std::vector<int> &edgeFillWidths) {
    auto startTime = std::chrono::steady_clock::now();

    int measure = thirtysecond/32;
    if (measure >= WAVE_M0 && thirtysecond < LOW_TIDE_T0+LYRIC_AREA+SMP_AREA) {
        //position index of wavefront, dryfront
        int waveColumns = std::min(thirtysecond-WAVE_T0, LYRIC_AREA+SMP_AREA-1);
        int dryColumns = std::max(thirtysecond-LOW_TIDE_T0, -1);

        //row index of wavefront, dryfront
        int waterLine;
        if (waveColumns >= LYRIC_AREA) {
            waterLine = LYRIC_HEIGHT + (waveColumns-LYRIC_AREA)/SMP_WIDTH;
        }
        else {
            waterLine = waveColumns/LYRIC_WIDTH;
        }
        if (dryColumns >= LYRIC_AREA) {
            dryLine = LYRIC_HEIGHT + (dryColumns-LYRIC_AREA)/SMP_WIDTH;
        }
        else {
            dryLine = std::floor((double)dryColumns/LYRIC_WIDTH);
        }

        //find first edge char
        if (startOutIndex >= 0) {
            if (edgeFillWidths[0] == 0) {
                edgeChars[0] = substrU8(frameWave, startOutIndex, 1);
                int width = stringWidth(edgeChars[0]);
                edgeCharWidths[0] = width;
                edgeFillWidths[0] = width;
                --startOutIndex;
            }
            --edgeFillWidths[0];
        }

        //crop 1 col from right, store char to edgeChars if no char stored (edgeFillWidth==0)
        for (int row=std::max(dryLine, 0); row <= waterLine; ++row) {
            //assert(waveWidths[row] == stringWidth(waves[row]));
            int maxWidth = (row < LYRIC_HEIGHT) ? LYRIC_WIDTH : SMP_WIDTH;
            if (waveWidths[row] == maxWidth || (row == dryLine && waveWidths[row] != 0)) {
                std::string s = substrU8(waves[row], waveLengths[row]-1, 1);
                int width = stringWidth(s);

                //store char
                if (row != WINDOW_HEIGHT-1) {
                    if (edgeFillWidths[row+1] == 0) {
                        edgeChars[row+1] = s;
                        edgeCharWidths[row+1] = width;
                        edgeFillWidths[row+1] = width;
                    }
                    --edgeFillWidths[row+1];
                }

                //crop
                std::string fill(width-1, fillChar);
                waves[row] = substrU8(waves[row], 0, waveLengths[row]-1) + fill;
                waveLengths[row] += width - 2;
            }
        }

        //push 1 col from left
        for (int row=std::max(dryLine, 0); row <= waterLine; ++row) {
            if (edgeChars[row] == "") {
                continue; //dryfront in row
            }
            if (edgeFillWidths[row] == 0) { //char
                waves[row] = edgeChars[row] + substrU8(waves[row], edgeCharWidths[row]-1, waveLengths[row]-edgeCharWidths[row]+1);
                edgeChars[row] = "";
                waveLengths[row] += 2 - edgeCharWidths[row];
            }
            else { //fill
                waves[row] = fillChar + waves[row];
                ++waveLengths[row];
            }
            //assert(waveLengths[row] == lengthU8(waves[row]));
        }

        //update waveWidths
        if (thirtysecond >= LOW_TIDE_T0) {
            --waveWidths[dryLine];
        }
        if (thirtysecond < WAVE_T0+LYRIC_AREA+SMP_AREA) {
            ++waveWidths[waterLine];
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printWaves(int thirtysecond, int &cursorRow, int &cursorCol, int dryLine) {
    auto startTime = std::chrono::steady_clock::now();

    int x;
    for (int row=0; row < WINDOW_HEIGHT; ++row) {
        if (row == dryLine) { //right align
            x = (row < LYRIC_HEIGHT) ? (LYRIC_WIDTH-waveWidths[row]) : (WINDOW_WIDTH-waveWidths[row]);
        }
        else { //left align
            x = (row < LYRIC_HEIGHT) ? 0 : NTE_WIDTH;
        }
        printMessage(waves[row], cursorRow, cursorCol, x, row, waveWidths[row], 0);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printLowTide(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    if (thirtysecond >= LOW_TIDE_T0+LYRIC_AREA && thirtysecond < LOW_TIDE_T0+LYRIC_AREA+SMP_AREA) {
        int windowIndex = thirtysecond-LOW_TIDE_T0, row, col;
        if (windowIndex >= LYRIC_AREA) {
            windowIndex -= LYRIC_AREA;
            row = windowIndex/SMP_WIDTH + LYRIC_HEIGHT;
            col = windowIndex - (row-LYRIC_HEIGHT)*SMP_WIDTH + NTE_WIDTH;
        }
        else {
            row = windowIndex/LYRIC_WIDTH;
            col = windowIndex - row*LYRIC_WIDTH;
        }
        printMessage(frameLowTide[row][col], cursorRow, cursorCol, col, row, 1, 0);
        //printMessage("$", cursorRow, cursorCol, col, row, 1, 0);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}
