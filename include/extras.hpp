#pragma once

#include <variables.hpp>
#include <io.hpp>

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>       
#include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>

#include <iostream>
#include <cstdio>
#include <array>
#include <string>
#include <vector>

#include <unistd.h> // for usleep
#include <string>
#include <stdio.h>
#include <signal.h>

void SigIntHandler(int sig_num);


class Extras {
public:
    bool IToB(int b);
    bool FlipBool(bool b);
    std::string GenSpaces(int n);
    void ClearMessage();
    void DrawMessage(std::string message);
    std::string Window(std::string message, bool isinput);
    void InitTermios(int echo);
    void ResetTermios(void);
    char getch_(int echo);
    char Getch(void);
    char Getche(void);
    void TimeDelay(int milliseconds);
    int BoolToInt(bool bl);
    void CColorPrint(char message, int color, bool bright);
    void ColorPrint(std::string message, int color, bool bright);
    void CaretPos(int x, int y);
    int TermHeight();
    int TermWidth();

    static Extras extras;
};
