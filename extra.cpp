/* ===============old functions===============
void loadScreen(std::string &screen) {
    for (int row=0; row < WINDOW_HEIGHT; ++row) {
        std::string line(WINDOW_WIDTH, ' ');
        screen += line + '\n';
    }
}

void loadFrameSMP() {
    int x0 = (SMP_WIDTH - LOGO_WIDTH) / 2;
    int y0 = (SMP_HEIGHT - LOGO_HEIGHT) / 2;

    // top filler
    for (int row = 0; row < y0; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        frameSMP += line + '\n';
    }

    // SMP
    for (int row = 0; row < LOGO_HEIGHT; ++row)
    {
        // left filler
        for (int x = 0; x < x0; ++x) frameSMP += ' ';

        // SMP row
        for (int x = 0; x < LOGO_WIDTH; ++x) frameSMP += SMP[row][x];

        // right filler
        for (int x = x0 + LOGO_WIDTH; x < SMP_WIDTH; ++x) frameSMP += ' ';

        frameSMP += '\n';
    }

    // bottom filler
    for (int row = y0 + LOGO_HEIGHT; row < SMP_HEIGHT; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        frameSMP += line + '\n';
    }
}

void loadFramePVZ() {
    // top filler
    for (int row = 0; row < LOGO_Y0; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        framePVZ += line + '\n';
    }

    // SMP
    for (int row = 0; row < LOGO_HEIGHT; ++row)
    {
        // left filler
        for (int x = 0; x < LOGO_X0; ++x)
            framePVZ += ' ';

        // SMP row
        for (int x = 0; x < LOGO_WIDTH; ++x)
            framePVZ += PVZ[row][x];

        // right filler
        for (int x = LOGO_X0 + LOGO_WIDTH; x < SMP_WIDTH; ++x)
            framePVZ += ' ';

        framePVZ += '\n';
    }

    // bottom filler
    for (int row = LOGO_Y0 + LOGO_HEIGHT; row < SMP_HEIGHT; ++row)
    {
        std::string line(SMP_WIDTH, ' ');
        framePVZ += line + '\n';
    }
}

void printWaves() {
    int cursorRow = 0, cursorCol = 0;
    unsigned int waveTextLength = 1;
    for (unsigned int wave=0; wave < WAVE_COUNT; ++wave) {
        for (int eighth = 0; eighth < EPW; ++eighth) {
            // find runtime
            auto startTime = std::chrono::steady_clock::now();

            // find string
            unsigned int lengthIncludingNewlines = waveTextLength + (waveTextLength-1)/SMP_WIDTH;
            frameWave = substrU8(animationWaves[wave], (unsigned int)(EPW-1-eighth), 1) + substrU8(frameWave, 0, SMP_AREA-1);
            std::string s = substrU8(frameWave, 0, waveTextLength);
            lineSplit(s, SMP_WIDTH);

            //debugging
            if (WAVE_LENGTH != SMP_AREA) {
                std::cout << WAVE_LENGTH << std::endl;
            }
            assert(WAVE_LENGTH == SMP_AREA);
            assert(substrU8(s, lengthU8(s)-1, 1) != "\n");
            if (lengthU8(s) != lengthIncludingNewlines) {
                std::cout << lengthIncludingNewlines << std::endl;
                std::cout << lengthU8(s) << std::endl;
            }
            assert(lengthU8(s) == lengthIncludingNewlines);

            // move cursor
            if (cursorCol != 0) moveLeft(cursorCol);
            if (cursorRow != 0) moveUp(cursorRow);

            // calculate next shift
            cursorRow = (waveTextLength - 1) / SMP_WIDTH;
            cursorCol = (waveTextLength - 1) % SMP_WIDTH + 1;
            assert(cursorRow >= 0);          

            auto endTime = std::chrono::steady_clock::now();
            auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            long string_delay = timeNS.count();

            // delay with runtime removed
            wait8ths(1, string_delay + TIMER_DELAY);

            // print string
            //std::cout << s << std::endl;
            std::cout << s << std::flush;

            waveTextLength = std::min(waveTextLength+1, (unsigned int)SMP_AREA);
        }
    }
}

void printSMP() {
    unsigned int measure = 1;
    int cursorRow = 0, cursorCol = 0;
    bool drawSMP = true;
    while (measure <= SMP_MEASURE_COUNT) {
        for (unsigned int beat=0; beat < animationLogo.length(); ++beat) {
            //find runtime
            auto startTime = std::chrono::steady_clock::now();

            // find string
            char bar = animationLogo[beat];
            unsigned int lengthIncludingNewlines = measure + (measure-1)/SMP_WIDTH;
            std::string s = "";
            if (drawSMP) {
                s = frameSMP.substr(0, lengthIncludingNewlines);
                replace(s, ' ', bar);
            }
            else {
                std::string line(SMP_WIDTH, bar);
                for (int row=0; row < SMP_HEIGHT; ++row) {
                    s += line + '\n';
                }
                s = s.substr(0, lengthIncludingNewlines);
            }

            // move cursor
            if (cursorCol != 0) moveLeft(cursorCol);
            if (cursorRow != 0) moveUp(cursorRow);

            // calculate next shift
            cursorRow = (measure - 1) / SMP_WIDTH;
            cursorCol = (measure - 1) % SMP_WIDTH + 1;
            assert(cursorRow >= 0);

            auto endTime = std::chrono::steady_clock::now();
            auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
            long string_delay = timeNS.count();

            // delay with runtime removed
            waitBeats(1, string_delay + TIMER_DELAY);

            // print string
            std::cout << s << std::flush;
        }
        ++measure;
    }
}

void printLyrics() {
    int cursorCol = 0;
    long string_delay = 0;
    std::vector<unsigned int> verseTimings = timings[0];
    for (unsigned int lyricItr=0; lyricItr < verseTimings.size(); ++lyricItr) {
        //wait
        wait16ths(verseTimings[lyricItr], string_delay + TIMER_DELAY);

        //print
        auto startTime = std::chrono::steady_clock::now();

        std::string text = lyricsAll[lyricItr];
        if (text[0] == '%') {
            clearLineToLeft();
            moveLeft(cursorCol);
            cursorCol = 0;
            text = text.substr(1, lyricsAll[lyricItr].length());
        }
        else if (text[0] == '\n') {
            cursorCol = -1;
        }
        std::cout << text << std::flush;
        cursorCol += text.length();

        auto endTime = std::chrono::steady_clock::now();
        auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        string_delay = timeNS.count();
    }
    std::cout << std::endl;
}

//return total_delay in ns
int printSMPBeat(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    int sixteenth = thirtysecond/2;
    int beat = sixteenth/4;
    int measure = beat/4;
    if (thirtysecond%8 == 0 && measure >= 32 && (measure < 224 || (measure == 224 && beat%4 == 0))) {
        int charCount = measure - 32 + 1;
        int row = LYRIC_HEIGHT + SMP_Y0;
        int col = NTE_WIDTH;
        while (charCount != 0) {
            if (row < WINDOW_HEIGHT) {
                printAnimationChar(beat, cursorRow, cursorCol, col, row);
            }
            ++col;
            if (col == WINDOW_WIDTH) {
                col = NTE_WIDTH;
                ++row;
            }
            --charCount;
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}

int printSMPLogo(int thirtysecond, int &cursorRow, int &cursorCol) {
    auto startTime = std::chrono::steady_clock::now();

    int sixteenth = thirtysecond/2;
    int beat = sixteenth/4;
    int measure = beat/4;
    int charCount = measure - 32 + 1;
    int row = LYRIC_HEIGHT + SMP_Y0;
    int col = NTE_WIDTH;
    for (int charItr=0; charItr < charCount; ++charItr) {
        if (row >= LYRIC_HEIGHT+SMP_Y0+LOGO_Y0 && row < LYRIC_HEIGHT+SMP_Y0+LOGO_Y0+LOGO_HEIGHT && col >= NTE_WIDTH+LOGO_X0 && col < NTE_WIDTH+LOGO_X0+LOGO_WIDTH) {
            char c = SMP[row-LYRIC_HEIGHT-SMP_Y0-LOGO_Y0][col-NTE_WIDTH-LOGO_X0];
            if (c != ' ') {
                printChar(c, cursorRow, cursorCol, col, row);
            }
        }

        ++col;
        if (col == WINDOW_WIDTH) {
            col = NTE_WIDTH;
            ++row;
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}
*/

/* ===============all chars 1-col wide===============
int printWaves(int sixteenth, int &cursorRow, int &cursorCol, char fillChar) {
    auto startTime = std::chrono::steady_clock::now();

    int beat = (sixteenth-1)/4;
    int measure = beat/4;
    if (measure >= WAVE_M0 && sixteenth < SMP_AREA+LYRIC_AREA+2176) {
        std::string visibleWave;
        if (shownCount <= 48*16) {
            visibleWave = substrU8(frameWave, 48*16-shownCount, shownCount);
        }
        else {
            std::string empty(shownCount-48*16, ' ');
            visibleWave = empty + substrU8(frameWave, 0, 48*16);
        }
        if (lengthU8(visibleWave) > LYRIC_AREA+SMP_AREA) {
            visibleWave = substrU8(visibleWave, 0, LYRIC_AREA+SMP_AREA);
        }

        //lyric window
        std::string lyricWave = substrU8(visibleWave, 0, LYRIC_AREA);
        lineSplit(lyricWave, LYRIC_WIDTH);
        int lyricCount = std::min(shownCount, LYRIC_AREA);
        int dy = std::ceil((double)lyricCount/LYRIC_WIDTH) - 1;
        int dx = lyricCount%LYRIC_WIDTH;
        if (dx == 0) dx += LYRIC_WIDTH;
        printMessage(lyricWave, cursorRow, cursorCol, 0, 0, dx, dy);

        //SMP window
        if (shownCount > LYRIC_AREA) {
            int smpCount = lengthU8(visibleWave)-LYRIC_AREA;
            std::string smpWave = substrU8(visibleWave, LYRIC_AREA, smpCount);
            lineSplit(smpWave, SMP_WIDTH);
            dy = std::ceil((double)smpCount/SMP_WIDTH) - 1;
            dx = smpCount%SMP_WIDTH;
            if (dx == 0) dx += LYRIC_WIDTH;
            printMessage(smpWave, cursorRow, cursorCol, NTE_WIDTH, LYRIC_HEIGHT, dx, dy);
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}*/

/* ===============multiple couts, no startVisibleIndex tracking===============
int printWaves(int sixteenth, int &cursorRow, int &cursorCol, char fillChar) {
    auto startTime = std::chrono::steady_clock::now();

    int beat = (sixteenth-1)/4;
    int measure = beat/4;
    if (measure >= WAVE_M0 && sixteenth < LOW_TIDE_S0+LYRIC_AREA+SMP_AREA) {
        //find visible wave
        int outLength = sixteenth - WAVE_S0;
        int visibleLength = std::min(outLength, WAVE_SIXTEENTH_COUNT);
        int startVisibleIndex = lengthU8(frameWave);
        int tempLength = 0, startWidth;
        while (tempLength < visibleLength) {
            --startVisibleIndex;
            startWidth = charWidth(toChar(substrU8(frameWave, startVisibleIndex, 1)));
            tempLength += startWidth;
        }
        std::string visibleWave;
        if (tempLength > visibleLength) { //add fill
            ++startVisibleIndex;
            std::string fill(startWidth-tempLength+visibleLength, fillChar);
            visibleWave += fill;
        }
        visibleWave += substrU8(frameWave, startVisibleIndex, lengthU8(frameWave)-startVisibleIndex);

        //find starting index
        int startTextIndex = 0;
        if (sixteenth-1 >= LOW_TIDE_S0) {
            startTextIndex = sixteenth - LOW_TIDE_S0;
        }

        //find and move to starting coordinate
        int row, col;
        if (startTextIndex >= LYRIC_AREA) {
            row = (startTextIndex-LYRIC_AREA)/SMP_WIDTH + LYRIC_HEIGHT;
            col = (startTextIndex-LYRIC_AREA) - (row-LYRIC_HEIGHT)*SMP_WIDTH + NTE_WIDTH;
        }
        else {
            row = startTextIndex/LYRIC_WIDTH;
            col = startTextIndex - row*LYRIC_WIDTH;
        }
        moveCursor(cursorRow, cursorCol, col, row);

        //print
        int charIndex=-1, charWidth=0;
        std::string s;
        while (cursorRow < WINDOW_HEIGHT && charIndex < visibleLength) {
            if (charWidth == 0) {
                ++charIndex;
                s = substrU8(visibleWave, charIndex, 1);
                charWidth = stringWidth(s);
                if (cursorCol+charWidth <= LYRIC_WIDTH) { //print char
                    printMessage(s, cursorRow, cursorCol, cursorCol, cursorRow, charWidth, 0);
                    charWidth = 0;
                }
            }
            else { //print fill
                int fillWidth = std::min(charWidth, LYRIC_WIDTH-cursorCol);
                std::string fill(fillWidth, fillChar);
                printMessage(fill, cursorRow, cursorCol, cursorCol, cursorRow, fillWidth, 0);
                charWidth -= fillWidth;
            }

            if (cursorCol == LYRIC_WIDTH) {
                //increment row
                moveCursor(cursorRow, cursorCol, cursorCol, cursorRow+1);
                //reset col
                if (cursorRow < LYRIC_HEIGHT) { //lyric window
                    moveCursor(cursorRow, cursorCol, 0, cursorRow);
                }
                else { //SMP window
                    moveCursor(cursorRow, cursorCol, NTE_WIDTH, cursorRow);
                }
            }
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}
*/

/* ===============single cout, startOutIndex tracking, with \n===============
int printWaves(int thirtysecond, int &cursorRow, int &cursorCol, char fillChar, int &startOutIndex, int &startFillWidth, int &startMaxWidth) {
    auto startTime = std::chrono::steady_clock::now();

    int beat = thirtysecond/8;
    int measure = beat/4;
    if (measure >= WAVE_M0 && thirtysecond < LOW_TIDE_T0+LYRIC_AREA+SMP_AREA) {
        //find outWave
        std::string outWave;
        int outLength = thirtysecond - WAVE_T0;
        int visibleLength = std::min(outLength, WAVE_THIRTYSECOND_COUNT);
        if (startOutIndex == 0) {
            outWave = frameWave;
        }
        else {
            ++startFillWidth;
            if (startFillWidth == startMaxWidth) {
                --startOutIndex;
                startFillWidth = 0;
                startMaxWidth = stringWidth(substrU8(frameWave, startOutIndex, 1));
            }
            std::string fill(startFillWidth, fillChar);
            outWave = fill + substrU8(frameWave, startOutIndex, WAVE_LENGTH-startOutIndex);
        }
        int outWaveLength = lengthU8(outWave);

        //find starting index
        int startTextIndex = 0;
        if (thirtysecond >= LOW_TIDE_T0) {
            startTextIndex = thirtysecond - LOW_TIDE_T0 + 1;
        }

        //find and move to starting coordinate
        int row, col;
        if (startTextIndex >= LYRIC_AREA) {
            row = (startTextIndex-LYRIC_AREA)/SMP_WIDTH + LYRIC_HEIGHT;
            col = (startTextIndex-LYRIC_AREA) - (row-LYRIC_HEIGHT)*SMP_WIDTH + NTE_WIDTH;
        }
        else {
            row = startTextIndex/LYRIC_WIDTH;
            col = startTextIndex - row*LYRIC_WIDTH;
        }
        moveCursor(cursorRow, cursorCol, col, row);

        //construct string
        std::string waves;
        int charIndex=-1, charWidth=0;
        std::string s;
        while (row < WINDOW_HEIGHT && charIndex < outWaveLength) {
            if (charWidth == 0) {
                ++charIndex;
                s = substrU8(outWave, charIndex, 1);
                charWidth = stringWidth(s);
                if (col+charWidth <= LYRIC_WIDTH) { //print char
                    waves += s;
                    col += charWidth;
                    charWidth = 0;
                }
            }
            else { //print fill
                int fillWidth = std::min(charWidth, LYRIC_WIDTH-col);
                std::string fill(fillWidth, fillChar);
                waves += fill;
                col += fillWidth;
                charWidth -= fillWidth;
            }

            if (col == LYRIC_WIDTH) {
                //increment row
                ++row;
                waves += "\n";
                //reset col
                if (row < LYRIC_HEIGHT) { //lyric window
                    col = 0;
                }
                else { //SMP window
                    col = NTE_WIDTH;
                    std::string empty(NTE_WIDTH, ' ');
                    waves += empty;
                }
            }
        }

        //print
        printMessage(waves, cursorRow, cursorCol, cursorCol, cursorRow, col-cursorCol, row-cursorRow);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto timeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    return (int)timeNS.count();
}
*/

/* ===============instruction formatting===============
    std::vector<std::vector<std::string>> ins = {
        {"\nFirst,", " create", " a", " new", " .smp", " file,", " paste", " in", " the", " example", " code", " provided", " in"},
        {"\nsource", "_monitor", "_plus/", "source/", "src,", " and", " save", " it", " to", " your", " source", "_monitor", "_plus/"},
        {"\nW:/", " drive.", " Next,", " ensuring", " the", " build", " configuration", " is", " set", " to", " 'export", " as"},
        {"\nwav',", " click", " the", " compile", " icon", " (located", " 3rd", " from", " 2nd", " from", " right", " from", " left"},
        {"\nin", " the", " toolbar).", " If", " you", " encounter", " a", " \"Backup", " failed", " -", " hardware"},
        {"\ntemperature", " critical\"", " error,", " take", " a", " nap", " and", " retry", " step", " two", " when", " you"},
        {"\nstart", " to", " dream", " about", " juggling", " walnut(s)", " at", " exactly", " 128npm.", " You", " need", " not"},
        {"\nbe", " awake", " when", " performing", " this", " fix.", " Next,", " ensuring", " you", " \e[3mare\e[0m", " awake,", " run"},
        {"\nyour", " compiled", " program", " using", " Valgrind", " to", " remove", " any", " peaks", " over", " 0dB."},
        {"\nFinally,", " head", " to", " https://youtu.be/0BTVcEdnXnE,", " wait", " 426s,", " then", " click"},
        {"\nlike", " and", " subscribe.", " You", " have", " successfully", " made", " use", " of", " source", " monitor"},
        {"\nplus."},
    };
    for (int i=0; i < ins.size(); ++i) {
        std::vector<std::string> irow = ins[i];
        int width = 0;
        for (int j=0; j < irow.size(); ++j) {
            width += lengthU8(removeANSI(irow[j]));
        }
        std::cout << "row " << i << " space: " << 73-width << std::endl;
    }
*/

/* ===============outWave (slow)===============
        int startVisibleIndex = WAVE_LENGTH;
        int tempLength = 0, startWidth;
        while (tempLength < visibleLength) {
            --startVisibleIndex;
            startWidth = charWidth(toChar(substrU8(frameWave, startVisibleIndex, 1)));
            tempLength += startWidth;
        }
        std::string visibleWave;
        if (tempLength > visibleLength) { //add fill
            ++startVisibleIndex;
            std::string fill(startWidth-tempLength+visibleLength, fillChar);
            visibleWave += fill;
        }
        visibleWave += substrU8(frameWave, startVisibleIndex, WAVE_LENGTH-startVisibleIndex);
*/

        //updateSMP(), settle logo chars to SMP
        /*
        for (int settleItr=unsettled.size()-1; settleItr >= 0; --settleItr) {
            std::pair<int, int> coordinate = unsettled[settleItr];
            int smpRow = coordinate.first, smpCol = coordinate.second;
            if (beatIndex(smp[smpRow][smpCol]) == beatIndex(SMP[smpRow-LOGO_Y0][smpCol-LOGO_X0])) {
                smp[smpRow][smpCol] = SMP[smpRow-LOGO_Y0][smpCol-LOGO_X0];
                unsettled.erase(unsettled.begin()+settleItr);
                unsettledBool[smpRow-LOGO_Y0][smpCol-LOGO_X0] = false;
            }
        }*/

//advanceLogoChar(), casework
/*
    if (startLogo[logoRow][logoCol] != ' ' && endLogo[logoRow][logoCol] != ' ' && smp[smpRow][smpCol] != endLogo[logoRow][logoCol]) { //'*'->'*'
        if ((beatIndex(endLogo[logoRow][logoCol])-beatIndex(smp[smpRow][smpCol])+4)%4 == 1) { //match beat while transitioning
            smp[smpRow][smpCol] = nextBeatChar(smp[smpRow][smpCol]);
            if (endLogo[logoRow][logoCol] == '_' && smp[smpRow][smpCol] == '-') { //underscore exception
                smp[smpRow][smpCol] = '_';
            }
            transitioned[smpRow][smpCol] = true;
        }
    }
    else if (startLogo[logoRow][logoCol] == ' ' && endLogo[logoRow][logoCol] != ' ' && smp[smpRow][smpCol] != endLogo[logoRow][logoCol]) { //' '->'*'
        smp[smpRow][smpCol] = nextBeatChar(smp[smpRow][smpCol]);
        if (endLogo[logoRow][logoCol] == '_' && smp[smpRow][smpCol] == '-') { //underscore exception
            smp[smpRow][smpCol] = '_';
        }
        if (smp[smpRow][smpCol] == endLogo[logoRow][logoCol]) {
            transitioned[smpRow][smpCol] = true;
        }
    }
    else if (startLogo[logoRow][logoCol] != ' ' && endLogo[logoRow][logoCol] == ' ') { //'*'->' '
        int i = (mainBeatIndex-beatIndex(smp[smpRow][smpCol])+4)%4;
        if ((mainBeatIndex-beatIndex(smp[smpRow][smpCol])+4)%4 == 1) {
            smp[smpRow][smpCol] = nextBeatChar(smp[smpRow][smpCol]);
            transitioned[smpRow][smpCol] = true;
        }
    }
*/