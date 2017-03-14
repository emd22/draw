#include <iostream>
#include <cstdio>
#include <array>
#include <string>
#include <vector>
#include <fstream>

#include "keypress.hpp"


/*
                                    (made with draw)
                                                             ____                                                                                   
                       _____                                |    \____                                                                              
                      |  __ \                               |         \_                                                                            
                      | |  \ \                              |          /                                                                            
                      | |   \ \                             |         /                                                                             
                      | |   | |                             |        /                                                                              
                      | |   | |  ____   _____ __        __  |       /                                                                               
                      | |   | | |  __| / _   |\ \      / /  |      /                                                                                
                      | |   / / | |   | / \  | \ \    / /   |     /                                                                                 
                      | |__/ /  | |   | \_/  |  \ \/\/ /    |_   /                                                                                  
                      |_____/   |_|    \___/\|   \_/\_/       \_/                                                                                   
                                                            __                                                                                      
                                                           /  \                                                                                     
                             A TERMINAL DRAWING PROGRAM    \__/  

Draw is a terminal application that you can draw your own ascii pictures etc.
The great part about this is that it doesn't constantly refresh and do other
wack things. Instead, the only thing it updates on a regular basis is the key
checker. So, when you want to draw something, it literally only prints that 
character on top of the screen that already has all of the previous ones.
Easy on the computer, and quite simple to use. Remember that this is a really
early work in progress.

Shortcuts:

    c: brings up command dialog. Commands include:
        brush: change brush
        color: change color(back to toggle background color)
        save: saves project
        load: loads project

        load: loads project
        NOTE: saving can't save color currently, and will hopefully be fixed soon.
        also going to a menu will remove all color, because it has to save to a temporary file.      
        
    enter: place pixel
    space: erase                                                               
                                    
*/

void CColorPrint(char message, int color) { //prints out color(as character)
    std::cout << "\033[" << color << "m" << message << "\033[15m";
    std::cout << "\033[0m\033[15m";
}
void ColorPrint(std::string message, int color) { //prints out color(string)
    std::cout << "\033[" << color << "m" << message << "\033[15m";
    std::cout << "\033[0m\033[15m";
}

void CaretPos(int x, int y) { //sets caret position to {x, y}
    std::printf("\033[%d;%dH", y, x);
}
int TermHeight() { //gets height of terminal
  struct winsize size;
  ioctl(1/*STDOUT_FILENO*/, TIOCGWINSZ, &size);
  return size.ws_row;
}
int TermWidth() { // gets width of terminal. Don't ask me how this works, because i have no idea ;)
  struct winsize size;
  ioctl(1/*STDOUT_FILENO*/, TIOCGWINSZ, &size);
  return size.ws_col;
}

class Draw {
public:
    void Init(int _w, int _h) { // Initialize the program
        DisableKeyPrint();
        
        w = _w;
        h = _h;

        /*
        add spaces to string(fileline), then push it to a vector(filelines).
        this vector will later be used to save and load projects.
        */
        
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                std::cout << ' '; // fill screen with spaces
                fileline += ' ';
            }
            std::cout << '\n'; //add a few newlines to the mix
            filelines.push_back(fileline);
            fileline = "";
        } 
    }

    void Save(std::string filename) { 
        /*
        Saves creation by holding a vector of strings
        and changing the characters in each string as
        it progresses down the window
        */

        std::ofstream file(filename); //opening file for writing

        std::cout << "\e[1;1H\e[2J"; // clear screen

        if (file.is_open()) { //checking if file is open, else printing its unable to write.
            for (int i = 0; i < filelines.size(); i++) { //saving each line from the vector
                file << filelines[i] << '\n';
            }
            file.close(); //closing file saving changes
        }
        else { std::cout << "Unable to open file"; } //else statement for not being able to open file
    }

    void Load(std::string filename) { //Loads creation from file
        std::string line;
        std::ifstream file(filename); //opening file for reading 
        
        int oldcx = x; //capturing old x position of cursor
        int oldcy = y; //capturing old y position of cursor(caret)

        CaretPos(0, 0); //setting caret position to {0, 0}

        std::cout << "\e[1;1H\e[2J"; // clear screen

        if (file.is_open()) {
            int linelen = 0; //setting variable linelen that increments by one when reading through lines
            while (std::getline(file,line)) { //reads through each line in file
                linelen++; //increments linelen
                std::cout << line; //print line from file onto screen
                filelines[linelen] = line; //sets a string from vector to the one in file(for being able to commit changes to the actual project)
            }
            file.close(); // closes file
        }
        else { std::cout << "Unable to open file"; }

        CaretPos(oldcx, oldcy); //sets caret position to the previous ones (old x, old y)
    }

    void LoadPresets() { //loads brush presets from hidden file
        std::ifstream file(".brushpresets"); 
        std::string line;

        if (file.is_open()) { //checks if file is open if not, don't add presets(TODO: future bug for assigned keys)
            std::getline(file, line); //gets only first line
            for (int i = 0; i < line.length(); i++) { //scrolls through characters and sets each one to a preset
                presets.push_back(line[i]);
            }
            
            file.close();
        }
        else { std::cout << "Unable to open presets"; }
    }   

    std::string Window(std::string message, bool isinput) { //almost modal like thing
        Save(".tempsave"); //saves project temporarily to a file
        std::cout << "\e[1;1H\e[2J"; // clear screen
        CaretPos(TermWidth()/2-(message.length()/2), TermHeight()/2-2); // set caret position to close to middle
        std::cout << message; // print out message

        if (isinput) { //if the window needs input(i made my own thing besides cin)
            std::string input = "";
            CaretPos(TermWidth()/2-(message.length()/2), TermHeight()/2-1);
            EnableKeyPrint();
            while (true) { //gets it stuck in an infinite loop that only breaks when return is pressed
                char k = CurKey();
                if (k != 0) {
                    input += k;
                }
                if (k == '\n' || k == '\r') {
                    std::cout << "\e[1;1H\e[2J"; // clear screen
                    Load(".tempsave"); //loads previously saved temp file
                    DisableKeyPrint();
                    if (input.length() > 1) {
                        input.erase(0, 1); //remove keypressed before (TODO: fix bug with having to press button to continue)
                    }
                    input.erase(input.length()-1, input.length()); // chops off newline
                    return input;
                }
            }
        }
        else {
            while (true) {
                char k = CurKey();
                if (k == '\n' || k == '\r') {
                    std::cout << "\e[1;1H\e[2J"; // clear screen
                    Load(".tempsave");
                    return "";
                }
            }
            
        }
    }

    void KeyPress() {
        char k = CurKey(); // gets single key input

        if (k == 'w') {
            if (y > 0) { y--; }
        } 
        if (k == 'a') {
            if (x > 0) { x--; }
        }
        if (k == 's') {
            if (y < h) { y++; }
        } 
        if (k == 'd') {
            if (x < w) { x++; }
        }

        //presets check

        if (k == '1') {
            style = presets[0];
        }
        else if (k == '2') {
            style = presets[1];
        }
        else if (k == '3') {
            style = presets[2];
        }
        else if (k == '4') {
            style = presets[3];
        }
        else if (k == '5') {
            style = presets[4];
        }
        else if (k == '6') {
            style = presets[5];
        }
        else if (k == '7') {
            style = presets[6];
        }
        else if (k == '8') {
            style = presets[7];
        }
        else if (k == '9') {
            style = presets[8];
        }
        else if (k == '0') {
            style = presets[9];
        }

        CaretPos(x, y);
         

        if (k == '\n' || k == '\r') {
            CColorPrint(style, color);
            filelines[y-1][x-1] = style;
        }
        if (k == ' ') {
            std::cout << ' ';
            filelines[y-1][x-1] = ' ';
        }

        if (k == 'c') {
            std::string command = Window("Command:", true);
            if (command == "brush") {
                style = Window("Brush:", true)[0];
            }
            else if (command == "save") {
                std::string name = Window("Enter New Filename:", true);
                Save(name);
            }
            else if (command == "load") {
                std::string name = Window("Enter Load Filename:", true);
                Load(name);
            }
            else if (command == "color") {
                int backinc = 0;
                if (back) {
                    backinc = 10;
                }
                else {
                    backinc = 0;
                }

                std::string scolor = Window("Set Color:", true);

                if (scolor == "black") {
                    color = 30+backinc;
                }
                else if (scolor == "red") {
                    color = 31+backinc;
                }
                else if (scolor == "green") {
                    color = 32+backinc;
                }
                else if (scolor == "yellow") {
                    color = 33+backinc;
                }
                else if (scolor == "blue") {
                    color = 34+backinc;
                }
                else if (scolor == "magenta") {
                    color = 35+backinc;
                }   
                else if (scolor == "cyan") {
                    color = 36+backinc;
                }
                else if (scolor == "white") {
                    color = 37+backinc;
                }
                else if (scolor == "back") {
                    if (back) {
                        back = false;
                    }
                    else {
                        back = true;
                    }
                }
                else {
                    std::cout << "unknown color";
                }
            } 
        }
    }

private:
    int x = 1;
    int y = 1;

    std::vector<char> presets;

    int color = 29;
    bool back = false;

    std::string fileline = "";
    std::vector<std::string> filelines;

    char style = 'O';

    int w = 0;
    int h = 0;
};

int main() {
    Draw draw;

    draw.Init(TermWidth(), TermHeight()-1);

    draw.LoadPresets();
    //draw.Load(".drawintro");

    draw.Window("test!", false);

    while (true) {
        draw.KeyPress();  
    }
}