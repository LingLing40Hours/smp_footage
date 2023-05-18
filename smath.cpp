#include <cmath>
#include "smath.h"


int floorLog(int base, int n) {
    int ans = 0;
    while (n >= base) {
        n /= base;
        ++ans;
    }
    return ans;
}

int digitCount(int n) {
    int ans = 1;
    n = abs(n);
    while (n >= 10) {
        n /= 10;
        ++ans;
    }
    return ans;
}
