#include <iostream>
#include <string>
#include <cwchar>
#include <locale>
#include <codecvt>
#include "globals.h"
#include "utf8.h"


size_t lengthU8(std::string s) {
    size_t i = 0, j = 0;
	while (s[i]) {
		if ((s[i] & 0xC0) != 0x80)
			j++;
		i++;
	}
	return (j);
}

std::string substrU8(const std::string& str, unsigned long start, unsigned long leng)
{
    if (leng==0) { return ""; }
    unsigned long c, i, ix, q, min=std::string::npos, max=std::string::npos;
    for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
    {
        if (q==start){ min=i; }
        if (q<=start+leng || leng==std::string::npos){ max=i; }

        c = (unsigned char) str[i];
        if      (
                 //c>=0   &&
                 c<=127) i+=0;
        else if ((c & 0xE0) == 0xC0) i+=1;
        else if ((c & 0xF0) == 0xE0) i+=2;
        else if ((c & 0xF8) == 0xF0) i+=3;
        //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
        //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return "";//invalid utf8
    }
    if (q<=start+leng || leng==std::string::npos){ max=i; }
    if (min==std::string::npos || max==std::string::npos) { return ""; }
    return str.substr(min,max-min);
}

void insertU8(std::string &s, unsigned int index, std::string stuff) {
    s = substrU8(s, 0, index) + stuff + substrU8(s, index, lengthU8(s)-index);
}

//index and index+stuff must be in range
void overwriteIndexU8(std::string &s, unsigned int index, std::string stuff) {
    s = substrU8(s, 0, index) + stuff + substrU8(s, index+lengthU8(stuff), lengthU8(s)-index-lengthU8(stuff));
}

//col and col+stuff must be in range, fill must be 1-col wide
void overwriteColU8(std::string &s, unsigned int col, std::string stuff, char32_t fill) {
    std::string ufill = toString(fill);
    int slength = lengthU8(s);
    int startCol = 0, startIndex = 0, startWidth;
    while (startCol < col) {
        if (startIndex == slength) { //col out of range
            return;
        }
        startWidth = charWidth(toChar(substrU8(s, startIndex, 1)));
        startCol += startWidth;
        ++startIndex;
    }

    int end = col + stringWidth(stuff);
    int endCol = startCol, endIndex = startIndex, endWidth;
    while (endCol < end) {
        if (endIndex == slength) { //col+stuff out of range
            return;
        }
        endWidth = charWidth(toChar(substrU8(s, endIndex, 1)));
        endCol += endWidth;
        ++endIndex;
    }

    //find bounding indices, add filler
    if (startCol > col) {
        --startIndex;
        int fillWidth = startWidth-startCol+col;
        std::string sfill;
        for (int colItr=0; colItr < fillWidth; ++colItr) {
            sfill += ufill;
        }
        stuff = sfill + stuff;
    }
    if (endCol > end) {
        int fillWidth = endWidth-endCol+end;
        std::string sfill;
        for (int colItr=0; colItr < fillWidth; ++colItr) {
            sfill += ufill;
        }
        stuff += sfill;
    }
    s = substrU8(s, 0, startIndex) + stuff + substrU8(s, endIndex, lengthU8(s)-endIndex);
}

//an alternative substrU8 to try
void utf8substr(std::string originalString, int SubStrLength, std::string& csSubstring)
{
    int len = 0;
    size_t byteIndex = 0;
    const char* aStr = originalString.c_str();
    size_t origSize = originalString.size();

    for (byteIndex=0; byteIndex < origSize; byteIndex++)
    {
        if((aStr[byteIndex] & 0xc0) != 0x80)
            len += 1;

        if(len >= SubStrLength)
            break;
    }

    csSubstring = originalString.substr(0, byteIndex);
}

char32_t toChar(std::string s) {
    std::u32string s32 = CONVERTER1.from_bytes(s); // Convert to UTF-32 string
    return s32[0];
}

std::string toString(char32_t c) {
    std::string ans = CONVERTER1.to_bytes(c);
    return ans;
}

int charWidth(char32_t c) {
    //std::wstring utf16str = CONVERTER2.to_bytes(c); // Convert to UTF-16 string
    //const wchar_t* utf16data = utf16str.data(); // Get pointer to UTF-16 data
    //wchar_t w = utf16data[0];
    //return wcwidth(w);
    if (c == U'゜' || c == U'つ' || c == U'ロ') {
        return 2;
    }
    return 1;
}

int stringWidth(std::string s) {
    int ans = 0;
    for (int charItr=0; charItr < lengthU8(s); ++charItr) {
        ans += charWidth(toChar(substrU8(s, charItr, 1)));
    }
    return ans;
}