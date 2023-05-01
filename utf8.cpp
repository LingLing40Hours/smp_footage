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
