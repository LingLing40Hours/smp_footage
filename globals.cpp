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
int LOGO_WIDTH = sizeof(SMP[0]);
int LOGO_HEIGHT = (int)sizeof(SMP)/LOGO_WIDTH;
int LOGO_X0 = (SMP_WIDTH - LOGO_WIDTH) / 2;
int LOGO_Y0;
int SMP_Y0;
int SMP_M1;
int LAST_BAR_M0;
int REVERT_PER_THIRTYSECOND;
int WAVE_LENGTH;
std::vector<int> KICK_REVERT_COUNTS = {3, 3, 4, 5, 6, 7, 8, 10};
std::vector<std::vector<bool>> REVERTED;
std::vector<int> SPARKS_DT = {0, 40, 72, 100, 136, 168, 190, 222};