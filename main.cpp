/*
    Created 6/1/2023 by waibibabow
    Digital Nature music video
*/
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <chrono>
#include <cmath>
#include <math.h>
#include <thread>
#include <vector>
#include <list>
#include <string>
#include <iterator>
#include <regex>
#include <locale>
#include <codecvt>
#include "text.h"
#include "animations.h"
#include "lyrics.h"
#include "cursor.h"
#include "utf8.h"
#include "time.h"
#include "smath.h"
#include "globals.h"

void loadDimensionsOffsets();
void loadVSlide();
void loadSpinning();

void printVideo();
int updateMaxDelay(int thirtysecond, int &maxTotalDelay, int totalDelay, int &slowestThirtysecond);
int updateVSlide(int thirtysecond);
int slideXOffset(int t, int maxIndex);
int printVSlide(int thirtysecond, int &cursorRow, int &cursorCol);
void showDigitalLife();
void bresenhamLine(int x0, int y0, int x1, int y1, void (*operation)(int, int));
void updateVBarRow(int x, int y);
void clearVSlide();
void resetVBarred();


int main(void) {
    //showDigitalLife();
    srand((unsigned int)time(NULL));
    loadDimensionsOffsets();
    loadVSlide();
    loadSpinning();
    printVideo();

    return 0;
}

void loadDimensionsOffsets() {
    DL_HEIGHT = digitalLife.size();
    DL_WIDTH = digitalLife[0].size();
    DL_X0 = (VSLIDE_WIDTH - DL_WIDTH)/2;
    DL_Y0 = (VSLIDE_HEIGHT - DL_HEIGHT)/2;
}

void loadVSlide() {
    VSLIDE = std::vector<std::string>(VSLIDE_HEIGHT, std::string(VSLIDE_WIDTH, ' '));
}

void loadSpinning() {
    std::vector<bool> row(VBAR_WIDTH, true);
    row.insert(row.end(), VSLIDE_WIDTH-VBAR_WIDTH, false);
    for (int rowItr=0; rowItr < VSLIDE_HEIGHT; ++rowItr) {
        SPINNING.push_back(row);
    }
}

void printVideo() {
    //cursor
    int cursorRow=0, cursorCol=0;
    //waiting
    int maxTotalDelay=0, slowestThirtysecond=0;

    for (int thirtysecond=0; thirtysecond < SONG_THIRTYSECOND_COUNT; ++thirtysecond) {
        int total_delay = TIMER_DELAY;
        total_delay += FUNCTION_DELAY + updateVSlide(thirtysecond);
        total_delay += FUNCTION_DELAY + printVSlide(thirtysecond, cursorRow, cursorCol);

        //print
        std::cout << std::flush;

        //wait
        total_delay += FUNCTION_DELAY + updateMaxDelay(thirtysecond, maxTotalDelay, total_delay, slowestThirtysecond);
        wait32nds(1, total_delay);
    }

    //end
    std::string delayMessage = "max total delay: " + std::to_string(maxTotalDelay) + "ns at t=" + std::to_string(slowestThirtysecond) + "; ";
    if (maxTotalDelay < NSPT) {
        delayMessage += "no under-buffering";
    }
    else {
        delayMessage += "no! under-buffering!";
    }
    printMessage(delayMessage, cursorRow, cursorCol, 0, 0, delayMessage.length(), 0);
    moveCursor(cursorRow, cursorCol, 0, 1); //end here
}

int updateMaxDelay(int thirtysecond, int &maxTotalDelay, int totalDelay, int &slowestThirtysecond) {
    auto startTime = std::chrono::steady_clock::now();

    if (totalDelay > maxTotalDelay) {
        maxTotalDelay = totalDelay;
        slowestThirtysecond = thirtysecond;
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

//swing from 0 to VSLIDE_WIDTH-VBAR_WIDTH
int updateVSlide(int thirtysecond) {
    auto startTime = std::chrono::steady_clock::now();

    clearVSlide();
    resetVBarred();

    //find vbar endpoints in VSLIDE coordinates
    int x0 = slideXOffset(thirtysecond, VSLIDE_WIDTH-VBAR_WIDTH);
    int x1 = (thirtysecond < 8) ? 0 : slideXOffset(thirtysecond-8, VSLIDE_WIDTH-VBAR_WIDTH);

    //fill vbar using VSLIDE coordinates
    bresenhamLine(x0, 0, x1, VSLIDE_HEIGHT-1, updateVBarRow);

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

//offset is zero at t=0
int slideXOffset(int t, int maxIndex) {
    return std::min((int)((maxIndex+1)/2.0 * (sin((t-8)/16.0*M_PI)+1)), maxIndex);
}

int printVSlide(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    for (int row=0; row < VSLIDE_HEIGHT; ++row) {
        printMessage(VSLIDE[row], cursorRow, cursorCol, VSLIDE_X0, VSLIDE_Y0+row, VSLIDE_WIDTH, 0);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

void showDigitalLife() {
    for (std::vector<std::string> srow : digitalLife) {
        for (std::string s : srow) {
            if (s != "*") {
                std::cout << s;
            }
        }
        std::cout << std::endl;
    }
}

void bresenhamLine(int x0, int y0, int x1, int y1, void (*operation)(int, int)) {
    bool is_steep = abs(y1-y0) > abs(x1-x0);
    if (is_steep) { //flip about y=x
        int temp = y1;
        y1 = x1;
        x1 = temp;

        temp = y0;
        y0 = x0;
        x0 = temp;
    }
    if (x0 > x1) { //swap points
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = -(dx/2);
    int y = y0;
    int y_step = (y0 < y1) ? 1 : -1;

    for (int x=x0; x <= x1; ++x) {
        if (is_steep) {
            operation(y, x);
        }
        else {
            operation(x, y);
        }
        error += dy;
        if (error > 0) {
            y += y_step;
            error -= dx;
        }
    }
}

//x, y in VSLIDE coordinates
//assumes DL_WIDTH > 0
void updateVBarRow(int x, int y) {
    if (VBARRED[y]) {
        return;
    }
    if (y >= DL_Y0 && y < DL_Y0+DL_HEIGHT) {
        //left space
        VSLIDE[y] = std::string(x, ' ');

        //vbar row
        int dly = y - DL_Y0;
        bool inDL = (x >= DL_X0 && x < DL_X0+DL_WIDTH);
        for (int dx=0; dx < VBAR_WIDTH; ++dx) {
            int tx = x + dx;

            //update inDL
            if (tx == DL_X0) {
                inDL = true;
            }
            else if (tx == DL_X0+DL_WIDTH) {
                VSLIDE[y] += std::string(VBAR_WIDTH-dx, '*');
                break;
            }

            //digital life
            if (inDL) {
                int dlx = tx - DL_X0;
                std::string s = digitalLife[dly][dlx];
                if (s == "*") {
                    if (dx == 0) {
                        VSLIDE[y] += s;
                    }
                    continue;
                }
                else if (s != " " && dx+stringWidth(s) <= VBAR_WIDTH) {
                    VSLIDE[y] += s;
                    continue;
                }
            }

            //fill
            VSLIDE[y] += "*";
        }

        //right space
        VSLIDE[y] += std::string(VSLIDE_WIDTH-x-VBAR_WIDTH, ' ');
    }
    else {
        for (int tx=x; tx < x+VBAR_WIDTH; ++tx) {
            VSLIDE[y][tx] = '*';
        }
    }
    VBARRED[y] = true;
    //assert(stringWidth(VSLIDE[y]) == VSLIDE_WIDTH);
}

void clearVSlide() {
    VSLIDE = std::vector<std::string>(VSLIDE_HEIGHT, std::string(VSLIDE_WIDTH, ' '));
}

void resetVBarred() {
    VBARRED = std::vector<bool>(VSLIDE_HEIGHT, false);
}
