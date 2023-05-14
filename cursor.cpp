#include <iostream>
#include "cursor.h"
#include "text.h"
#include "utf8.h"


int digitCount(int n) {
    int ans = 1;
    n = abs(n);
    while (n >= 10) {
        n /= 10;
        ++ans;
    }
    return ans;
}

//cout is much slower than string construction; avoid single-char prints if possible
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

//for debugging; single-line only, doesn't change cursor position
void showMessage(std::string message, int &cursorRow, int &cursorCol, int x, int y) {
    int row = cursorRow;
    int col = cursorCol;
    moveCursor(cursorRow, cursorCol, x, y);
    std::cout << message << std::flush;

    cursorCol += lengthU8(message);
    moveCursor(cursorRow, cursorCol, col, row);
}

void printMessage(std::string message, int &cursorRow, int &cursorCol, int x, int y, int dx, int dy) {
    moveCursor(cursorRow, cursorCol, x, y);
    std::cout << message;
    cursorRow += dy;
    cursorCol += dx;
}

//print cursor position for debugging
void printCursor(int &cursorRow, int &cursorCol, int x, int y) {
    int row = cursorRow;
    int col = cursorCol;
    moveCursor(cursorRow, cursorCol, x, y);
    std::cout << "cursor row: " << row;
    cursorCol += 12 + digitCount(row);
    moveCursor(cursorRow, cursorCol, x, y+1);
    std::cout << "cursor col: " << col;
    cursorCol += 12 + digitCount(col);

    std::cout << std::flush;
    moveCursor(cursorRow, cursorCol, col, row);
}

void moveCursor(int &cursorRow, int &cursorCol, int x, int y) {
    if (cursorCol < x) {
        moveRight(x-cursorCol);
    }
    else if (cursorCol > x) {
        moveLeft(cursorCol-x);
    }
    if (cursorRow < y) {
        moveDown(y-cursorRow);
    }
    else if (cursorRow > y) {
        moveUp(cursorRow-y);
    }
    cursorCol = x;
    cursorRow = y;
}

void moveUp(int positions) {
    printf("\x1b[%dA", positions);
}

void moveDown(int positions) {
    printf("\x1b[%dB", positions);
}

void moveRight(int positions) {
    printf("\x1b[%dC", positions);
}

void moveLeft(int positions) {
    printf("\x1b[%dD", positions);
}

void moveTo(int row, int col) {
    printf("\x1b[%d;%df", row, col);
}

void clearScreen() {
    printf("\x1b[%dJ", CLEAR_ALL);
}

void clearScreenToBottom() {
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_END);
}

void clearScreenToTop() {
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_BEGIN);
}

void clearLine() {
    printf("\x1b[%dK", CLEAR_ALL);
}

void clearLineToRight() {
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_END);
}

void clearLineToLeft() {
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_BEGIN);
}
