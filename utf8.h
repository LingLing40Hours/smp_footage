#ifndef UTF8_H
#define UTF8_H

#include <string>


size_t lengthU8(std::string s);
std::string substrU8(const std::string& str, unsigned long start, unsigned long leng);
void insertU8(std::string &s, unsigned int index, std::string stuff);
void overwriteU8(std::string &s, unsigned int index, std::string stuff);
void overwriteColU8(std::string &s, unsigned int col, std::string stuff, char32_t fill);
void utf8substr(std::string originalString, int SubStrLength, std::string& csSubstring);
char32_t toChar(std::string s);
std::string toString(char32_t c);
int charWidth(char32_t c);
int stringWidth(std::string s);

#endif
