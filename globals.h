#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <codecvt>

#define BPM 128
#define NSPB (60000000000/BPM)
#define NSPE (NSPB/2)
#define NSPS (NSPB/4)
#define NSPT (NSPB/8)
#define TIMER_DELAY 53674 //49637 //44313 //64500
#define FUNCTION_DELAY 2

#define SONG_MEASURE_COUNT 20
#define SONG_THIRTYSECOND_COUNT 640

#define WINDOW_WIDTH 57
#define WINDOW_HEIGHT 8
#define WINDOW_AREA (WINDOW_WIDTH*WINDOW_HEIGHT)

#define VSLIDE_X0 0
#define VSLIDE_Y0 0
#define VSLIDE_WIDTH WINDOW_WIDTH
#define VSLIDE_HEIGHT WINDOW_HEIGHT
#define VSLIDE_AREA (VSLIDE_WIDTH*VSLIDE_HEIGHT)
#define VBAR_WIDTH 8


extern std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> CONVERTER1;
extern std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> CONVERTER2;
extern std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>::state_type STATE;
extern int DL_WIDTH;
extern int DL_HEIGHT;
extern int DL_X0; //VSLIDE coordinates
extern int DL_Y0;
extern std::vector<std::string> VSLIDE;
extern std::vector<bool> VBARRED;
extern std::vector<std::vector<bool>> SPINNING;

#endif