/*
    Created 3/24/2023 by Xiangyu Chen
    Program to generate music video for Song of Source Monitor Plus

    Entire rectangle must remain visible in terminal, else cursor may move to incorrect locations
*/
#include <iostream>
#include <cassert>
#include <chrono>
#include <cmath>
#include <thread>
#include <vector>
#include <string>
#include <iterator>
#include <regex>
#include "text.h"
#include "lyrics.h"
#include "cursor.h"
#include "utf8.h"
#include "time.h"
#include "globals.h"


void replace(std::string &s, char c1, char c2);
void loadScreen(std::string &screen);
void loadLyrics();
void loadFrameSMP();
void loadFramePVZ();
void loadFrameWave();
void loadSMPRowOffset();
void lineSplit(std::string &s, unsigned int length);
void printWaves();
void printSMP();
void printLyrics();
void printVideo();
int printSMPExecuting(int sixteenth, int &cursorRow, int &cursorCol);
void printAnimationChar(int beat, int &cursorRow, int &cursorCol, int x, int y);
void printChar(char c, int &cursorRow, int &cursorCol, int x, int y);
int printSMPBeat(int sixteenth, int &cursorRow, int &cursorCol);
int printSMPLogo(int sixteenth, int &cursorRow, int &cursorCol);
int updateLyrics(int &timingIndex, int &durationIndex, int &sixteenthOffset, int &lyricIndex, int &cursorDisplayRow, int &cursorDisplayCol);
int printLyrics(int sixteenth, int &cursorRow, int &cursorCol);
void pushLyrics();
void printTimeTotals();
void printTimeSizes();
std::string removeANSI(std::string s);

int main(void)
{
    //printTimeTotals();
    //printTimeSizes();
    loadLyrics();
    loadFrameSMP();
    loadSMPRowOffset();
    printVideo();

    //loadFrameSMP();
    //loadFramePVZ();
    //loadFrameWave();

    //printWaves();
    //printSMP();
    //PVZ transition
    //printLyrics();

    return 0;
}

void replace(std::string &s, char c1, char c2) {
    for (unsigned int i=0; i<s.length(); ++i) {
        if (s[i] == c1) {
            s[i] = c2;
        }
    }
}

void loadScreen(std::string &screen) {
    for (int row=0; row < WINDOW_HEIGHT; ++row) {
        std::string line(WINDOW_WIDTH, ' ');
        screen += line + '\n';
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

void loadFrameSMP() {
    int x0 = (SMP_WIDTH - LOGO_WIDTH) / 2;
    int y0 = (SMP_HEIGHT - LOGO_HEIGHT) / 2;

    // top filler
    for (int row = 0; row < y0; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        frameSMP += line + '\n';
    }

    // SMP
    for (int row = 0; row < LOGO_HEIGHT; ++row)
    {
        // left filler
        for (int x = 0; x < x0; ++x) frameSMP += ' ';

        // SMP row
        for (int x = 0; x < LOGO_WIDTH; ++x) frameSMP += SMP[row][x];

        // right filler
        for (int x = x0 + LOGO_WIDTH; x < SMP_WIDTH; ++x) frameSMP += ' ';

        frameSMP += '\n';
    }

    // bottom filler
    for (int row = y0 + LOGO_HEIGHT; row < SMP_HEIGHT; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        frameSMP += line + '\n';
    }
}

void loadFramePVZ() {
    // top filler
    for (int row = 0; row < LOGO_Y0; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        framePVZ += line + '\n';
    }

    // SMP
    for (int row = 0; row < LOGO_HEIGHT; ++row)
    {
        // left filler
        for (int x = 0; x < LOGO_X0; ++x)
            framePVZ += ' ';

        // SMP row
        for (int x = 0; x < LOGO_WIDTH; ++x)
            framePVZ += PVZ[row][x];

        // right filler
        for (int x = LOGO_X0 + LOGO_WIDTH; x < SMP_WIDTH; ++x)
            framePVZ += ' ';

        framePVZ += '\n';
    }

    // bottom filler
    for (int row = LOGO_Y0 + LOGO_HEIGHT; row < SMP_HEIGHT; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        framePVZ += line + '\n';
    }
}

//without newlines
void loadFrameWave () {
    std::string box(SMP_AREA, '|');
    frameWave += box;
}

void loadSMPRowOffset() {
    SMP_Y0 = std::ceil((double)executing.length()/SMP_WIDTH) + 1;
    LOGO_Y0 = (SMP_HEIGHT - SMP_Y0 - LOGO_HEIGHT) / 2;
    //std::cout << "smp row offset: " << std::to_string(SMP_ROW_OFFSET) << std::endl;
}

void lineSplit(std::string &s, unsigned int length) {
    for (unsigned int i=(unsigned int)(lengthU8(s)-lengthU8(s)%length); i != 0; i-=length) {
        insertU8(s, i, "\n");
    }
    if (substrU8(s, lengthU8(s)-1, 1) == "\n")
        s = substrU8(s, 0, lengthU8(s)-1);
}

void printWaves() {
    int cursorRow = 0, cursorCol = 0;
    unsigned int waveTextLength = 1;
    for (unsigned int wave=0; wave < WAVE_COUNT; ++wave) {
        for (int eighth = 0; eighth < EPW; ++eighth) {
            // find runtime
            auto startTime = std::chrono::steady_clock::now();

            // find string
            unsigned int lengthIncludingNewlines = waveTextLength + (waveTextLength-1)/SMP_WIDTH;
            frameWave = substrU8(animationWaves[wave], (unsigned int)(EPW-1-eighth), 1) + substrU8(frameWave, 0, SMP_AREA-1);
            std::string s = substrU8(frameWave, 0, waveTextLength);
            lineSplit(s, SMP_WIDTH);

            //debugging
            if (lengthU8(frameWave) != SMP_AREA) {
                std::cout << lengthU8(frameWave) << std::endl;
            }
            assert(lengthU8(frameWave) == SMP_AREA);
            assert(substrU8(s, lengthU8(s)-1, 1) != "\n");
            if (lengthU8(s) != lengthIncludingNewlines) {
                std::cout << lengthIncludingNewlines << std::endl;
                std::cout << lengthU8(s) << std::endl;
            }
            assert(lengthU8(s) == lengthIncludingNewlines);

            // move cursor
            if (cursorCol != 0) moveLeft(cursorCol);
            if (cursorRow != 0) moveUp(cursorRow);

            // calculate next shift
            cursorRow = (waveTextLength - 1) / SMP_WIDTH;
            cursorCol = (waveTextLength - 1) % SMP_WIDTH + 1;
            assert(cursorRow >= 0);          

            auto endTime = std::chrono::steady_clock::now();
            auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            long string_delay = timeNS.count();

            // delay with runtime removed
            wait8ths(1, string_delay + TIMER_DELAY);

            // print string
            //std::cout << s << std::endl;
            std::cout << s << std::flush;

            waveTextLength = std::min(waveTextLength+1, (unsigned int)SMP_AREA);
        }
    }
}

void printSMP() {
    unsigned int measure = 1;
    int cursorRow = 0, cursorCol = 0;
    bool drawSMP = true;
    while (measure <= SMP_MEASURE_COUNT) {
        for (unsigned int beat=0; beat < animationLOGO.length(); ++beat) {
            //find runtime
            auto startTime = std::chrono::steady_clock::now();

            // find string
            char bar = animationLOGO[beat];
            unsigned int lengthIncludingNewlines = measure + (measure-1)/SMP_WIDTH;
            std::string s = "";
            if (drawSMP) {
                s = frameSMP.substr(0, lengthIncludingNewlines);
                replace(s, ' ', bar);
            }
            else {
                std::string line(SMP_WIDTH, bar);
                for (int row=0; row < SMP_HEIGHT; ++row) {
                    s += line + '\n';
                }
                s = s.substr(0, lengthIncludingNewlines);
            }

            // move cursor
            if (cursorCol != 0) moveLeft(cursorCol);
            if (cursorRow != 0) moveUp(cursorRow);

            // calculate next shift
            cursorRow = (measure - 1) / SMP_WIDTH;
            cursorCol = (measure - 1) % SMP_WIDTH + 1;
            assert(cursorRow >= 0);

            auto endTime = std::chrono::steady_clock::now();
            auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            long string_delay = timeNS.count();

            // delay with runtime removed
            waitBeats(1, string_delay + TIMER_DELAY);

            // print string
            std::cout << s << std::flush;
        }
        ++measure;
    }
}

void printLyrics() {
    int cursorCol = 0;
    long string_delay = 0;
    std::vector<unsigned int> verseTimings = timings[0];
    for (unsigned int lyricItr=0; lyricItr < verseTimings.size(); ++lyricItr) {
        //wait
        wait16ths(verseTimings[lyricItr], string_delay + TIMER_DELAY);

        //print
        auto startTime = std::chrono::steady_clock::now();

        std::string text = lyricsAll[lyricItr];
        if (text[0] == '%') {
            clearLineToLeft();
            moveLeft(cursorCol);
            cursorCol = 0;
            text = text.substr(1, lyricsAll[lyricItr].length());
        }
        else if (text[0] == '\n') {
            cursorCol = -1;
        }
        std::cout << text << std::flush;
        cursorCol += text.length();

        auto endTime = std::chrono::steady_clock::now();
        auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        string_delay = timeNS.count();
    }
    std::cout << std::endl;
}

//sixteenth starting from 1
void printVideo() {
    int cursorRow=0, cursorCol=0;
    int cursorDisplayRow=2, cursorDisplayCol=0;
    int timingIndex=0, durationIndex=0, sixteenthOffset=0, lyricIndex=-1;
    for (int sixteenth=1; sixteenth <= SONG_SIXTEENTH_COUNT; ++sixteenth) {
        int total_delay = TIMER_DELAY;
        total_delay += printSMPExecuting(sixteenth, cursorRow, cursorCol);
        total_delay += printSMPBeat(sixteenth, cursorRow, cursorCol);
        total_delay += printSMPLogo(sixteenth, cursorRow, cursorCol);
        total_delay += updateLyrics(timingIndex, durationIndex, sixteenthOffset, lyricIndex, cursorDisplayRow, cursorDisplayCol);
        total_delay += printLyrics(sixteenth, cursorRow, cursorCol);
        //std::string message1 = "durationIndex: " + std::to_string(durationIndex);
        //printMessage(message1, cursorRow, cursorCol, 0, 0);
        //std::string message2 = "sixteenthOffset: " + std::to_string(sixteenthOffset) + ", lyricIndex: " + std::to_string(lyricIndex);
        //printMessage(message2, cursorRow, cursorCol, LYRIC_WIDTH, (sixteenth-128)%WINDOW_HEIGHT);
        //printMessage(message2, cursorRow, cursorCol, 0, 0);
        //std::string message3 = "timingIndex: " + std::to_string(timingIndex);
        //printMessage(message3, cursorRow, cursorCol, 0, 0);

        //moveCursor(cursorRow, cursorCol, cursorDisplayRow, cursorDisplayCol);

        //print
        std::cout << std::flush;

        //wait
        wait16ths(1, total_delay);
    }
    moveCursor(cursorRow, cursorCol, 0, 0); //end here
}

//return total delay in ns
int printSMPExecuting(int sixteenth, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    if (sixteenth == 1) { //initial print
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
    if (sixteenth%4 == 1) {
        int beat = (sixteenth-1)/4;
        int measure = beat/4;
        std::vector<int> beatIndices;
        if (beat >= 209 && beat <= 877) { //index 20, m52
            beatIndices.push_back(20);
        }
        if (beat >= 193 && beat <= 881) { //index 24, m48
            beatIndices.push_back(24);
        }
        if (beat >= 177 && beat <= 885)  { //index 34, m44
            beatIndices.push_back(34);
        }
        if (beat >= 161 && beat <= 889) { //index 40, m40
            beatIndices.push_back(40);
        }
        if (beat >= 145 && beat <= 893) { //index 46, m36
            beatIndices.push_back(46);
        }

        for (unsigned int beatIndexItr=0; beatIndexItr < beatIndices.size(); ++beatIndexItr) {
            int index = beatIndices[beatIndexItr];
            int row = index/SMP_WIDTH;
            int col = NTE_WIDTH + index - row*SMP_WIDTH;
            printAnimationChar(beat, cursorRow, cursorCol, col, row+LYRIC_HEIGHT);
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

void printAnimationChar(int beat, int &cursorRow, int &cursorCol, int x, int y) {
    unsigned int beatIndex = (unsigned)beat%4;
    char c = animationLOGO[beatIndex];
    printChar(c, cursorRow, cursorCol, x, y);
}

void printChar(char c, int &cursorRow, int &cursorCol, int x, int y) {
    moveCursor(cursorRow, cursorCol, x, y);
    std::cout << c;
    ++cursorCol;
}

//return total_delay in ns
int printSMPBeat(int sixteenth, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();
    int beat = (sixteenth-1)/4;
    int measure = beat/4;
    if (sixteenth%4 == 1 && measure >= 32 && (measure < 224 || (measure == 224 && beat%4 == 0))) {
        int charCount = measure - 32 + 1;
        int row = LYRIC_HEIGHT + SMP_Y0;
        int col = NTE_WIDTH;
        while (charCount != 0) {
            if (row < WINDOW_HEIGHT) {
                printAnimationChar(beat, cursorRow, cursorCol, col, row);
            }
            ++col;
            if (col == WINDOW_WIDTH) {
                col = NTE_WIDTH;
                ++row;
            }
            --charCount;
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printSMPLogo(int sixteenth, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    int beat = (sixteenth-1)/4;
    int measure = beat/4;
    int charCount = measure - 32 + 1;
    int row = LYRIC_HEIGHT + SMP_Y0;
    int col = NTE_WIDTH;
    for (int charItr=0; charItr < charCount; ++charItr) {
        if (row >= LYRIC_HEIGHT+SMP_Y0+LOGO_Y0 && row < LYRIC_HEIGHT+SMP_Y0+LOGO_Y0+LOGO_HEIGHT && col >= NTE_WIDTH+LOGO_X0 && col < NTE_WIDTH+LOGO_X0+LOGO_WIDTH) {
            char c = SMP[row-LYRIC_HEIGHT-SMP_Y0-LOGO_Y0][col-NTE_WIDTH-LOGO_X0];
            if (c != ' ') {
                printChar(c, cursorRow, cursorCol, col, row);
            }
        }

        ++col;
        if (col == WINDOW_WIDTH) {
            col = NTE_WIDTH;
            ++row;
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int updateLyrics(int &timingIndex, int &durationIndex, int &sixteenthOffset, int &lyricIndex, int &cursorDisplayRow, int &cursorDisplayCol) {
    auto startTime = std::chrono::steady_clock::now();
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

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printLyrics(int sixteenth, int &cursorRow, int &cursorCol) {
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

void printTimeTotals() {
    for (int timingItr=0; timingItr < timings.size(); ++timingItr) {
        int total = 0;
        std::vector<unsigned int> durations = timings[timingItr];
        for (int durationItr=0; durationItr < durations.size(); ++durationItr) {
            total += durations[durationItr];
        }
        std::cout << total << std::endl;
    }
}

void printTimeSizes() {
    for (int timingItr=0; timingItr < timings.size(); ++timingItr) {
        std::cout << timings[timingItr].size() << std::endl;
    }
}

std::string removeANSI(std::string s) {
    std::regex pattern("\e\\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]"); // Matches ANSI escape codes
    std::string ans = std::regex_replace(s, pattern, ""); // Removes escape codes
    return ans;
}
