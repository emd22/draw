#include <iostream>
#include <cstdio>
#include <array>
#include <string>
#include <vector>
#include <fstream>

#include "extras.hpp"

#define BACKINC 10

/*
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
        export: exports project to text
        lexport: loads exported file  
        
    enter: place pixel
    space: erase                                                               
                                    
*/

std::vector<std::array<std::string, 2>> commands = {
    {"save", "saves project into color document"},
    {"load", "loads project and the color with it"},
    {"export", "exports project as text for use in games or documents"},
    {"lexport", "loads export and converts it to projects"},
    {"color", "use this command to change color"},
    {"brush", "changes brush to another character"},
    {"help", "shows dialog for commands and information"}
};

struct Pixel {
    int x;
    int y;
    char style;
    int color;
    int back;
};

int BoolToInt(bool bl) {
    return bl;
}

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

    std::string GenSpaces(int n) {
        std::string spaces = "";
        for (int i = 0; i < n; i++) {
            spaces += ' ';
        }
        return spaces;
    }

    void ResetExp() {
        filelines.clear();

        for (int y = 0; y < TermHeight(); y++) {
            for (int x = 0; x < TermWidth(); x++) {
                std::cout << ' '; // fill screen with spaces
                fileline += ' ';
            }
            std::cout << '\n'; // add a few newlines to the mix
            filelines.push_back(fileline);
            fileline = "";
        } 
    }

    void DrawMessage(std::string message) {
        int oldx = x;
        int oldy = y;

        CaretPos(TermWidth()/2-message.length()/2, TermHeight());
        ColorPrint(message, 36);
        CaretPos(oldx, oldy);
        ismessage = true;
    }

    void ClearMessage() {
        if (ismessage) {
            int oldx = x;
            int oldy = y;

            countstrokes = true;

            if (keystrokes == 24) {
                CaretPos(0, TermHeight());
                std::cout << GenSpaces(TermWidth());
                CaretPos(oldx, oldy);
                countstrokes = false;
                ismessage = false;
                keystrokes = 0;
            }
        }
    }

    void SaveToExp() {
        filelines.clear();
        ResetExp();

        for (int i = 0; i < pixels.size(); i++) {
            filelines[pixels[i].y][pixels[i].x] = pixels[i].style;
        }
    }

    void Export(std::string filename) { 
        /*
        Exports creation by holding a vector of strings
        and changing the characters in each string as
        it progresses down the window
        */

        std::ofstream file("exports/"+filename); //opening file for writing

        std::cout << "\e[1;1H\e[2J"; // clear screen

        SaveToExp();

        if (file.is_open()) { //checking if file is open, else printing its unable to write.
            for (int i = 0; i < filelines.size(); i++) { //saving each line from the vector
                file << filelines[i] << '\n';
            }
            file.close(); //closing file saving changes
        }
        else { DrawMessage("Cannot find file"); } //else statement for not being able to open file
    }

    void Save(std::string filename) {
        std::ofstream file("projects/"+filename+".dpg"); //opening file for writing

        std::cout << "\e[1;1H\e[2J"; // clear screen

        if (file.is_open()) { //checking if file is open, else printing its unable to write.
            for (int y = 0; y < pixels.size(); y++)  {
                file << '(' << pixels[y].x << ',' << pixels[y].y << ',' << (int)pixels[y].style << ',' << pixels[y].color << ',' << pixels[y].back << ')';
                file << '\n';
            }
            file.close(); //closing file saving changes
        }
        else { DrawMessage("Cannot find file"); } //else statement for not being able to open file
    }

    void LoadExp(std::string filename) { //Loads creation from file
        std::string line;
        std::ifstream file("exports/"+filename); //opening file for reading 
        
        int oldcx = x; //capturing old x position of cursor
        int oldcy = y; //capturing old y position of cursor(caret)

        CaretPos(0, 0); //setting caret position to {0, 0}

        std::cout << "\e[1;1H\e[2J"; // clear screen

        if (file.is_open()) {
            int linelen = 0; //setting variable linelen that increments by one when reading through lines
            while (std::getline(file,line)) { //reads through each line in file
                linelen++; //increments linelen
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] != ' ') {
                        Pixel pixel;
                        pixel.x = i;
                        pixel.y = linelen;
                        pixel.style = line[i];
                        pixel.color = 39;
                        pixel.back = 0;
                        pixels.push_back(pixel);
                    }
                }
                std::cout << line; //print line from file onto screen
                filelines[linelen] = line; //sets a string from vector to the one in file(for being able to commit changes to the actual project)
            }
            file.close(); // closes file
        }
        else { DrawMessage("Cannot find file"); }

        CaretPos(oldcx, oldcy); //sets caret position to the previous ones (old x, old y)
    }

    void ParseLoaded(std::string line) {
        std::vector<int> params;

        for (int i = 0; i < line.length(); i++) {
            int tempj = 0;
            if (line[i] == '(') {
                std::string param;

                for (int j = i+1; line[j] != ')'; j++) {
                    if (line[j] == ',') { // look for comma in file to split parameters
                        params.push_back(atoi(param.c_str()));
                        param = "";
                    }
                    else {
                        param += line[j];
                    }
                    tempj = j;
                }
                i = tempj;

                Pixel pixel;
                pixel.x = params[0]+1; // create variables from file and load into Pixel struct
                pixel.y = params[1]+1;
                pixel.style = params[2];
                pixel.color = params[3];
                pixel.back = params[4];

                params.clear();
                tokens.push_back(pixel);
            }
        }
    }

    void RemCoords(int x_, int y_) {
        for (int i = 0; i < pixels.size(); i++) {
            Pixel ctok = pixels[i];
            if (ctok.x == x_ && ctok.y == y_) {
                pixels.erase(pixels.begin()+i);
                filelines[y_][x_] = ' ';
                break;
            }
        }
    }

    void Load(std::string filename) { //Loads creation from file
        tokens.clear();

        std::string line;
        std::ifstream file("projects/"+filename+".dpg"); //opening file for reading 
        
        int oldcx = x; //capturing old x position of cursor
        int oldcy = y; //capturing old y position of cursor(caret)

        CaretPos(0, 0); //setting caret position to {0, 0}

        std::cout << "\e[1;1H\e[2J"; // clear screen

        if (file.is_open()) {
            int linelen = 0; //setting variable linelen that increments by one when reading through lines

            while (std::getline(file,line)) { //reads through each line in file
                ParseLoaded(line);
            }
            file.close(); // closes file
            
            for (int i = 0; i < tokens.size(); i++) {
                int cback = 0;
                Pixel ctok = tokens[i];

                if (ctok.back == 1) {
                    cback = BACKINC;
                }
                
                CaretPos(ctok.x, ctok.y);
                CColorPrint(ctok.style, ctok.color+cback);
                filelines[ctok.y][ctok.x] = ctok.style;
            }
        }
        else { std::cout << "Unable to open file"; }

        CaretPos(oldcx, oldcy); //sets caret position to the previous ones (old x, old y)
    }

    void LoadPresets() { //loads brush presets from hidden file
        std::ifstream file(".brushpresets"); 
        std::string line;

        if (file.is_open()) { //checks if file is open if not, don't add presets
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
        if (message.length() < TermWidth()) {
            CaretPos(TermWidth()/2-(message.length()/2), TermHeight()/2-2); // set caret position to close to middle
        }
        else {
            CaretPos(0, 0);
        }
        
        std::cout << message; // print out message

        if (isinput) { //if the window needs input
            std::string input = "";
            CaretPos(TermWidth()/2-(message.length()/2), TermHeight()/2-1);
            EnableKeyPrint();
            std::cin >> input;
            DisableKeyPrint();
            Load(".tempsave");
            return input;
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
            if (y > 1) { y--; }
            if (countstrokes) { keystrokes++; }
        } 
        if (k == 'a') {
            if (x > 1) { x--; }
            if (countstrokes) { keystrokes++; }
        }
        if (k == 's') {
            if (y < h-1) { y++; }
            if (countstrokes) { keystrokes++; }
        } 
        if (k == 'd') {
            if (x < w-1) { x++; }
            if (countstrokes) { keystrokes++; }
        }

        //presets check

        if (k >= '0' && k <= '9') {
            style = presets[k-'0'-1];
        }

        CaretPos(x, y);

        if (k == '\n' || k == '\r') {
            CColorPrint(style, color);

            Pixel pixel;
            pixel.style = style;
            pixel.color = color;
            pixel.back = BoolToInt(back);
            pixel.x = x-1;
            pixel.y = y-1;
            pixels.push_back(pixel);
        }
        if (k == ' ') {
            std::cout << ' ';
            RemCoords(x-1, y-1);
        }

        if (k == 'c') {
            std::string command = Window("Command:", true);
        
            if (command == "brush") {
                style = Window("Brush:", true)[0];
            }
            else if (command == "help") {
                std::string help = "";
                for (int i = 0; i < commands.size(); i++) {
                    help += (commands[i][0]+" - "+commands[i][1]+"\n");
                }
                Window(help, false);
            }
            else if (command == "save") {
                std::string name = Window("Enter New Filename:", true);
                Save("."+name);
                Load("."+name);
            }
            else if (command == "export.save") {
                std::string name = Window("Enter New Filename:", true);
                Export("."+name);
                Load("."+name);
            }
            else if (command == "export.load") {
                std::string name = Window("Enter Load Filename:", true);
                LoadExp("."+name);
            }
            else if (command == "load") {
                std::string name = Window("Enter Load Filename:", true);
                Load("."+name);
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
                    DrawMessage("Unknown Color");
                }
            }
            else {
                DrawMessage("Unknown Command");
            } 
        }
    }

private:
    int x = 1;
    int y = 1;

    int keystrokes = 0;
    bool countstrokes = false;
    bool ismessage = false;

    std::vector<char> presets;
    std::vector<Pixel> tokens;

    int color = 29;
    bool back = false;

    std::string fileline = "";
    std::vector<std::string> filelines;
    std::vector<Pixel> pixels; 

    char style = 'O';

    int w = 0;
    int h = 0;
};

int main() {
    Draw draw;
    draw.Init(TermWidth(), TermHeight());

    draw.LoadPresets();
    //draw.Load(".drawintro");

    while (true) {
        draw.KeyPress();  
        draw.ClearMessage();
    }
}