#ifndef UTF8_H
#define UTF8_H

#include <string>


size_t lengthU8(std::string s);
std::string substrU8(const std::string& str, unsigned long start, unsigned long leng);
void insertU8(std::string &s, unsigned int index, std::string stuff);
void utf8substr(std::string originalString, int SubStrLength, std::string& csSubstring);

#endif
