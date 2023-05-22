#ifndef ANIMATIONS_H
#define ANIMATIONS_H

int beatGap(char a, char b);
int beatGap(int aBeatIndex, char b);
int beatIndex(char c);
char nextBeatChar(char c);
void advanceLogoChar(int mainBeatIndex, int logoRow, int logoCol, char endLogo[5][19]);
void advanceLogo(int mainBeatIndex, char endLogo[5][19]);

#endif