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
