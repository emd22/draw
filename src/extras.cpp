#include <extras.hpp>

#include <termios.h>
#include <io.hpp>

static struct termios old, _new;
Extras Extras::extras = Extras();

/* Signal Handler for SIGINT */
void SigIntHandler(int sig_num) {
    /* Reset handler to catch SIGINT next time.
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, SigIntHandler);
}

bool Extras::IToB(int b) {
    if (b == 1) {
        return true;
    }
    else {
        return false;
    }
}

bool Extras::FlipBool(bool b) {
    if (b) {
        return false;
    }
    else {
        return true;
    }
}

std::string Extras::GenSpaces(int n) {
    std::string spaces = "";
    for (int i = 0; i < n; i++) {
        spaces += ' ';
    }
    return spaces;
}

void Extras::ClearMessage() {
    if (Var::var.RetIM()) {
        int oldx = Var::var.RetX();
        int oldy = Var::var.RetY();

        Var::var.CS(true);

        if (Var::var.RetKS() == 12) {
            Extras::extras.CaretPos(0, Extras::extras.TermHeight());
            std::cout << GenSpaces(Extras::extras.TermWidth());
            Extras::extras.CaretPos(oldx, oldy);
            Var::var.CS(false);
            Var::var.IM(false);
            Var::var.KS(0);
        }
    }
}

void Extras::DrawMessage(std::string message) {
    int oldx = Var::var.RetX();
    int oldy = Var::var.RetY();

    Extras::extras.CaretPos(Extras::extras.TermWidth()/2-message.length()/2, Extras::extras.TermHeight());
    Extras::extras.ColorPrint(message, 36, true);
    Extras::extras.CaretPos(oldx, oldy);
    Var::var.IM(true);
}

std::string Extras::Window(std::string message, bool isinput) { //almost modal like thing
    IO::Save(".tempsave"); //saves project temporarily to a file
    std::cout << "\e[1;1H\e[2J"; // clear screen
    if (message.length() < Extras::extras.TermWidth()) {
        Extras::extras.CaretPos(Extras::extras.TermWidth()/2-(message.length()/2), Extras::extras.TermHeight()/2-2); // set caret position to close to middle
    }
    else {
        Extras::extras.CaretPos(0, 0);
    }
    
    std::cout << message; // print out message

    if (isinput) { //if the window needs input
        std::string input = "";
        Extras::extras.CaretPos(Extras::extras.TermWidth()/2-(message.length()/2), Extras::extras.TermHeight()/2-1);
        Var::var.KP(true);
        std::cin >> input;
        Var::var.KP(false);
        IO::Load(".tempsave");
        return input;
    }
    else {
        std::string input;
        std::cin >> input;
        IO::Load(".tempsave");
        return "";
    }
}

/* Initialize new terminal i/o settings */
void Extras::InitTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    _new = old; /* make new settings same as old settings */
    _new.c_lflag &= ~ICANON; /* disable buffered i/o */
    _new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &_new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void Extras::ResetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char Extras::getch_(int echo) {
    char ch;
    InitTermios(echo);
    ch = getchar();
    ResetTermios();
    return ch;
}

/* Read 1 character without echo */
char Extras::Getch(void) {
    return getch_(0);
}

/* Read 1 character with echo */
char Extras::Getche(void) {
    return getch_(1);
}

void Extras::TimeDelay(int milliseconds) {
    usleep(milliseconds * 1000);
}

int Extras::BoolToInt(bool bl) {
    return bl;
}

void Extras::CColorPrint(char message, int color, bool bright) { //prints out color(as character)
    int isbrgt = BoolToInt(bright);
    std::cout << "\033[" << isbrgt << ';' << color << "m" << message << "\033[15m";
    std::cout << "\033[0m\033[15m";
}

void Extras::ColorPrint(std::string message, int color, bool bright) { //prints out color(string)
    int isbrgt = BoolToInt(bright);
    std::cout << "\033[" << isbrgt << ';' << color << "m" << message << "\033[15m";
    std::cout << "\033[0m\033[15m";
}
void Extras::CaretPos(int x, int y) { //sets caret position to {x, y}
    std::printf("\033[%d;%dH", y, x);
}
int Extras::TermHeight() { //gets height of terminal
    struct winsize size;
    ioctl(1/*STDOUT_FILENO*/, TIOCGWINSZ, &size);
    return size.ws_row;
}

int Extras::TermWidth() { // gets width of terminal. Don't ask me how this works, because i have no idea ;)
    struct winsize size;
    ioctl(1/*STDOUT_FILENO*/, TIOCGWINSZ, &size);
    return size.ws_col;
}  