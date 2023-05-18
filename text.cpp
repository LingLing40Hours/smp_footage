#include "text.h"
//_____  _   _  _____
//|      |\ /|  |   |
//|____  | - |  |___|
//    |  |   |  |
//____|  |   |  |

//_____  _   _  _____
//|   |   \ /    ///
//|___|    -    //__
//|
//|___
std::string executing = " *  Executing task: /bin/bash -c ./build/Debug/outDebug";
std::string animationLOGO = "|/-\\";
std::string waveChars = "゜つロabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~1!2@3#4$5%6^7&8*9(0)-_=+[{]}\\|;:'\",<.>/?¡™£¢∞§¶•ªº–≠œ∑´®†¥¨ˆøπ“‘«åß∂ƒ©˙∆˚¬…æΩ≈ç√∫˜µ≤≥÷";
std::string bbgb = "( ゜- ゜)つロ"; //゜つロ
std::vector<std::string> smp;
std::vector<std::string> lyrics;
std::vector<std::string> waves;
std::vector<std::string> stars;
std::vector<int> waveWidths;
std::vector<int> waveLengths;
std::vector<std::string> animationWaves = {
    u8"||||||||||||////||*π~!∆#®!)Ø+_||\\\\\\\\||||||||||||",
    u8"||||||||||||////||);>ßn«∫¬ˆ%^#||\\\\\\\\||||||||||||",
    u8"||||||||||||////||!@(œ÷#¬å˜•™}||\\\\\\\\||||||||||||",
    u8"||||||||||||////||ˆåµ†-´ß¨®ç´µ||\\\\\\\\||||||||||||",
    u8"||||||||||||////||˜ˆ†®π¬!ß3~)#||\\\\\\\\||||||||||||",
    u8"||||||||||||////||*DM#^§å¨(µ≠e||\\\\\\\\||||||||||||",
    u8"||||||||||||////||^∫ß¬O{?µ∂¢¡Q||\\\\\\\\||||||||||||",
    u8"||||||||||||////||ƒ¶†(¢–P:≈∂V…||\\\\\\\\||||||||||||"
};
std::vector<std::string> animationTests = {
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||",
    "||||||||||||////||aaaaaaaaaaaa||\\\\\\\\||||||||||||"
};
char SMP[5][19] = {
    {'_', '_', '_', '_', '_', ' ', ' ', '_', ' ', ' ', ' ', '_', ' ', ' ', '_', '_', '_', '_', '_'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', '|', '\\', ' ', '/', '|', ' ', ' ', '|', ' ', ' ', ' ', '|'},
    {'|', '_', '_', '_', '_', ' ', ' ', '|', ' ', '-', ' ', '|', ' ', ' ', '|', '_', '_', '_', '|'},
    {' ', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', ' '},
    {'_', '_', '_', '_', '|', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', ' ', ' '}
};
char PVZ[5][19] = {
    {'_', '_', '_', '_', '_', ' ', ' ', '_', ' ', ' ', ' ', '_', ' ', ' ', '_', '_', '_', '_', '_'},
    {'|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '\\', ' ', '/', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|'},
    {'|', '_', '_', '_', '|', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', ' ', '|', '_', '_', '_', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'|', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};
std::string frameSMP = "";
std::string framePVZ = "";
std::string frameWave = "";
std::vector<std::vector<std::string>> frameLowTide;
std::vector<std::vector<std::string>> ALPHABET5;

std::vector<std::string> A5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> B5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> C5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> D5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> E5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> F5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> G5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> H5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> I5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> J5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> K5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> L5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> M5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> N5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> O5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> P5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> Q5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> R5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> S5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> T5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> U5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> V5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> W5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> X5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> Y5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};

std::vector<std::string> Z5 = {
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
};
