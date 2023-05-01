#ifndef TIME_H
#define TIME_H

#include "globals.h"


enum Timing {
    INTRO=0,
    VERSE,
    VERSE2,
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
void wait16ths(unsigned int n, long offset);
bool hasLyrics(Timing t);

#endif