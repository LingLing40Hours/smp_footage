#include "globals.h"
#include "text.h"
#include "animations.h"


//assume both chars are beat indicators
int beatGap(char a, char b) {
    if (a == b) {
        return 0;
    }
    int ans = beatIndex(b) - beatIndex(a);
    if (ans <= 0) {
        ans += 4;
    }
    return ans;
}

//assume int [0, 4), char is beat indicator
//produces same result as beatGap(animationLogo[int], char) but slightly cheaper
int beatGap(int aBeatIndex, char b) {
    if (animationLogo[aBeatIndex] == b) {
        return 0;
    }
    int ans = beatIndex(b) - aBeatIndex;
    if (ans <= 0) {
        ans += 4;
    }
    return ans;
}

int beatIndex(char c) {
    if (c == '|') {
        return 0;
    }
    if (c == '/') {
        return 1;
    }
    if (c == '-' || c == '_') {
        return 2;
    }
    if (c == '\\') {
        return 3;
    }
    return -1;
}

char nextBeatChar(char c) {
    if (c == '|') {
        return '/';
    }
    if (c == '/') {
        return '-';
    }
    if (c == '-' || c == '_') {
        return '\\';
    }
    if (c == '\\') {
        return '|';
    }
    return '*';
}

void advanceLogoChar(int mainBeatIndex, int logoRow, int logoCol, char endLogo[5][19]) {
    //assert(logoRow >= 0 && logoRow < LOGO_HEIGHT);
    //assert(logoCol >= 0 && logoCol < LOGO_WIDTH);
    int smpRow = logoRow + LOGO_Y0;
    int smpCol = logoCol + LOGO_X0;
    if (transitioned[smpRow][smpCol]) {
        return;
    }
    
    if (!transitioned[smpRow][smpCol] && (mainBeatIndex-beatIndex(smp[smpRow][smpCol])+4)%4 == 1 && smp[smpRow][smpCol] != endLogo[logoRow][logoCol]) {
        smp[smpRow][smpCol] = nextBeatChar(smp[smpRow][smpCol]);
        if (beatIndex(smp[smpRow][smpCol]) == beatIndex(endLogo[logoRow][logoCol])) {
            smp[smpRow][smpCol] = endLogo[logoRow][logoCol];
        }
    }
    if (smp[smpRow][smpCol] == endLogo[logoRow][logoCol] || (smp[smpRow][smpCol] != '_' && beatIndex(smp[smpRow][smpCol]) == mainBeatIndex && endLogo[logoRow][logoCol] == ' ')) {
        transitioned[smpRow][smpCol] = true;
    }
    //assert(smp[smpRow][smpCol] != ' ' || (smpRow == 5 && smpCol == 16));
}

void advanceLogo(int mainBeatIndex, char endLogo[5][19]) {
    for (int row=0; row < LOGO_HEIGHT; ++row) {
        for (int col=0; col < LOGO_WIDTH; ++col) {
            if (SMP[row][col] != ' ' || PVZ[row][col] != ' ') {
                advanceLogoChar(mainBeatIndex, row, col, endLogo);
            }
        }
    }
}
