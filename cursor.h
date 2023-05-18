#ifndef CURSOR_H
#define CURSOR_H

enum ClearCodes {
    CLEAR_FROM_CURSOR_TO_END,
    CLEAR_FROM_CURSOR_TO_BEGIN,
    CLEAR_ALL
};

//positions must be non-negative
void printSMPExecutingChar(int beat, int charIndex, int &cursorRow, int &cursorCol);
void printAnimationChar(int beat, int &cursorRow, int &cursorCol, int x, int y);
void printChar(char c, int &cursorRow, int &cursorCol, int x, int y);
void showMessage(std::string message, int &cursorRow, int &cursorCol, int x, int y);
void printMessage(std::string message, int &cursorRow, int &cursorCol, int x, int y, int dx, int dy);
void printCursor(int &cursorRow, int &cursorCol, int x, int y);
void moveCursor(int &cursorRow, int &cursorCol, int x, int y);
void moveUp(int positions);
void moveDown(int positions);
void moveRight(int positions);
void moveLeft(int positions);
void moveTo(int row, int col);
void clearScreen();
void clearScreenToBotton();
void clearScreenToTop();
void clearLine();
void clearLineToRight();
void clearLineToLeft();

#endif