#ifndef TIME_H
#define TIME_H

#include "globals.h"


enum Timing {
    INTRO=0,
    VERSE,
    VERSE2,
    PAUSE,
    BRIDGE,
    BRIDGE2,
    BRIDGE3,
    CHORUS,
    INSTRUCTIONS,
    OUTRO
};
extern std::vector<std::vector<unsigned int>> timings;
extern std::vector<Timing> song;
extern std::vector<Timing> lyricized;

void waitMeasures(unsigned int m, long offset);
void waitBeats(unsigned int b, long offset);
void wait8ths(unsigned int e, long offset);
void wait16ths(unsigned int s, long offset);
void wait32nds(unsigned int t, long offset);
void waitNanosecs(unsigned int n);
bool hasLyrics(Timing t);

#endif