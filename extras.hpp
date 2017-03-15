#pragma once

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>       
#include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>

#include <unistd.h> // for usleep
#include <string>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}


char CurKey() {
    if (_kbhit()) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
    } 
    else {
        return '\0';
    }
}

void DisableKeyPrint() {
  int temp = system("stty -echo");
}

void EnableKeyPrint() {
  int temp = system("stty echo");
}

void TimeDelay(int milliseconds) {
    usleep(milliseconds * 1000);
}
