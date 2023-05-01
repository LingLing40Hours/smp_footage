#include <iostream>
#include  "cursor.h"


int digitCount(int n) {
    int ans = 1;
    n = abs(n);
    while (n >= 10) {
        n /= 10;
        ++ans;
    }
    return ans;
}

void printMessage(std::string message, int &cursorRow, int &cursorCol, int x, int y) {
    int row = cursorRow;
    int col = cursorCol;
    moveCursor(cursorRow, cursorCol, x, y);
    std::cout << message << std::flush;

    cursorCol += message.length();
    moveCursor(cursorRow, cursorCol, col, row);
}

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
