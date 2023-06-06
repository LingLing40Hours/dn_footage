#include <iostream>
#include <cmath>
#include <string>
#include <locale>
#include <codecvt>
#include "text.h"
#include "globals.h"


std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> CONVERTER1;
std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> CONVERTER2;
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>::state_type STATE;
int DL_WIDTH;
int DL_HEIGHT;
int DL_X0;
int DL_Y0;
std::vector<std::string> VSLIDE;
std::vector<bool> VBARRED = std::vector<bool>(VSLIDE_HEIGHT, false);
std::vector<std::vector<bool>> SPINNING;
