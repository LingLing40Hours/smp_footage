#include <iostream>
#include <vector>
#include <chrono>
#include "time.h"


std::vector<std::vector<unsigned int>> timings = {
    { //8m intro
        128
    },
    { //8m verse
        2, 2, 3, 1, 3, 3,
        4, 2, 3, 1, 3, 3,
        4, 2, 3, 1, 2, 2, 2, 4, 2, 2, 2, 3, 3,
        4, 2, 3, 1, 3, 3,
        4, 2, 3, 1, 3, 3,
        4, 2, 3, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2
    },
    { //8m verse2
        0, 6, 2, 3, 3,
        4, 4, 2, 3, 3, 
        4, 4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 4,
        6, 2, 3, 3,
        4, 4, 2, 3, 3,
        5, 4, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4
    },
    { //1m pause
        16
    },
    { //8m bridge
        2, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 1, 2, 2, 2, 2, 1, 1, 1,
        11, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1
    },
    { //8m bridge2
        2, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 1, 2, 2, 2, 2, 1, 1, 1, 4, 1, 1, 1,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1
    },
    { //8m bridge3
        2, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 2, 1,
        4, 2, 2, 2, 3, 1, 2, 2, 2, 2, 1, 1, 1, 4, 1, 1, 1,
        4, 2, 2, 2, 3, 3,
        4, 2, 2, 2, 3, 2, 1,
        4, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    { //32m chorus /-\|/-\|/-\|/-\|
        512
    },
    { //48m instructions (768)
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 494
    },
    { //9m outro
        144
    }
};

std::vector<Timing> song = {INTRO, VERSE, VERSE, BRIDGE, CHORUS, VERSE, VERSE2, PAUSE, BRIDGE2, INSTRUCTIONS, VERSE, INTRO, VERSE, VERSE, VERSE, BRIDGE3, CHORUS, OUTRO};
std::vector<Timing> lyricized = {VERSE, VERSE2, BRIDGE, BRIDGE2, BRIDGE3, INSTRUCTIONS};

void waitMeasures(unsigned int m, long offset) {
    //std::this_thread::sleep_for(std::chrono::nanoseconds(4*m*NSPB));
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(4*m*NSPB - offset);
    while (std::chrono::steady_clock::now() < end);
}

void waitBeats(unsigned int b, long offset) {
    //std::this_thread::sleep_for(std::chrono::nanoseconds(b*NSPB));
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(b*NSPB - offset);
    while (std::chrono::steady_clock::now() < end);
}

void wait8ths(unsigned int e, long offset) {
    //std::this_thread::sleep_for(std::chrono::nanoseconds(e*NSPE));
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(e*NSPE - offset);
    while (std::chrono::steady_clock::now() < end);
}

void wait16ths(unsigned int s, long offset) {
    //std::this_thread::sleep_for(std::chrono::nanoseconds(s*NSPS));
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(s*NSPS - offset);
    while (std::chrono::steady_clock::now() < end);
}

void wait32nds(unsigned int t, long offset) {
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(t*NSPT - offset);
    while (std::chrono::steady_clock::now() < end);
}

void waitNanosecs(unsigned int n) {
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::nanoseconds(n);
    while (std::chrono::steady_clock::now() < end);
}

bool hasLyrics(Timing t) {
    for (int lyricizedItr=0; lyricizedItr < lyricized.size(); ++lyricizedItr) {
        if (t == lyricized[lyricizedItr]) {
            return true;
        }
    }
    return false;
}
