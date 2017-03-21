#include <io.hpp>

void IO::Save(std::string filename) {
    std::ofstream file("projects/"+filename+".dpj"); //opening file for writing

    std::cout << "\e[1;1H\e[2J"; // clear screen

    if (file.is_open()) { //checking if file is open, else printing its unable to write.
        for (int y = 0; y < Var::var.RetP().size(); y++)  {
            file << '(' << Var::var.RetP()[y].x << ',' << Var::var.RetP()[y].y << ',' << (int)Var::var.RetP()[y].style << ',' << Var::var.RetP()[y].color << ',' << Var::var.RetP()[y].back << ')' << '\n';
        }
        file.close(); //closing file saving changes
    }
    else { Extras::extras.DrawMessage("Cannot write to file"); } //else statement for not being able to open file
}

void IO::ParseLoaded(std::string line) {
    std::vector<int> params;

    for (int i = 0; i < line.length(); i++) {
        int tempj = 0;
        if (line[i] == '(') {
            std::string param;

            for (int j = i+1; line[j] != ')'; j++) {
                if (line[j] == ',') { // look for command in file to split parameters
                    params.push_back(std::stoi(param));
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
            Var::var.T(pixel);
        }
    }
}

void IO::Load(std::string filename) { //Loads creation from file
    Var::var.CT();

    std::string line;
    std::ifstream file("projects/"+filename+".dpj"); //opening file for reading 
    
    int oldcx = Var::var.RetX(); //capturing old x position of cursor
    int oldcy = Var::var.RetY(); //capturing old y position of cursor(caret)

    Extras::extras.CaretPos(0, 0); //setting caret position to {0, 0}

    std::cout << "\e[1;1H\e[2J"; // clear screen

    if (file.is_open()) {
        while (std::getline(file, line)) { //reads through each line in file
            ParseLoaded(line);
        }
        file.close(); // closes file
        
        for (int i = 0; i < Var::var.RetT().size(); i++) {
            int cback = 0;
            Pixel ctok = Var::var.RetT()[i];

            if (ctok.back == 1) {
                cback = BACKINC;
            }

            //std::cout << ctok.x << ',' << ctok.y << ',' << ctok.style << ',' << ctok.color << ',' << ctok.back << '\n';
            //std::cout << "loading:" << i << '\n';
            Extras::extras.CaretPos(ctok.x, ctok.y);
            Extras::extras.CColorPrint(ctok.style, ctok.color+cback, false);
            Var::var.ManFLS(ctok.y, ctok.x, ctok.style);
        }
    }
    else { Extras::extras.DrawMessage("Cannot write to file"); }

    Extras::extras.CaretPos(oldcx, oldcy); //sets caret position to the previous ones (old x, old y)
}

void IO::LoadPresets() { //loads brush presets from hidden file
    std::ifstream file(".brushpresets"); 
    std::string line;

    if (file.is_open()) { //checks if file is open if not, don't add presets
        std::getline(file, line); //gets only first line
        for (int i = 0; i < line.length(); i++) { //scrolls through characters and sets each one to a preset
            Var::var.PBPR(line[i]);
        }
        file.close();
    }
    else { std::cout << "Unable to open presets"; }
}  

 void IO::LoadExp(std::string filename) { //Loads creation from file
    std::string line;
    std::ifstream file("exports/"+filename); //opening file for reading 
    
    int oldcx = Var::var.RetX(); //capturing old x position of cursor
    int oldcy = Var::var.RetY(); //capturing old y position of cursor(caret)

    Extras::extras.CaretPos(0, 0); //setting caret position to {0, 0}

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
                    Var::var.P(pixel);
                }
            }
            std::cout << line; //print line from file onto screen
            Var::var.EFLS(0, line); //sets a string from vector to the one in file(for being able to commit changes to the actual project)
        }
        file.close(); // closes file
    }
    else { Extras::extras.DrawMessage("Cannot find file"); }

    Extras::extras.CaretPos(oldcx, oldcy); //sets caret position to the previous ones (old x, old y)
}

 void IO::Export(std::string filename) { 
    /*
    Exports creation by holding a vector of strings
    and changing the characters in each string as
    it progresses down the window
    */

    std::ofstream file("exports/"+filename); //opening file for writing

    std::cout << "\e[1;1H\e[2J"; // clear screen

    SaveToExp();

    if (file.is_open()) { //checking if file is open, else printing its unable to write.
        for (int i = 0; i < Var::var.RetFLS().size(); i++) { //saving each line from the vector
            file << Var::var.RetFLS()[i] << '\n';
        }
        file.close(); //closing file saving changes
    }
    else { Extras::extras.DrawMessage("Cannot find file"); } //else statement for not being able to open file
}

void IO::SaveToExp() {
    ResetExp();

    for (int i = 0; i < Var::var.RetP().size(); i++) {
        Var::var.ManFLS(Var::var.RetP()[i].x, Var::var.RetP()[i].x, Var::var.RetP()[i].style);
    }
}

 void IO::ResetExp() {
    Var::var.CFLS();

    for (int y = 0; y < Extras::extras.TermHeight(); y++) {
        for (int x = 0; x < Extras::extras.TermWidth(); x++) {
            std::cout << ' '; // fill screen with spaces
            Var::var.AddFL(' ');
        }
        std::cout << '\n'; // add a few newlines to the mix
        Var::var.PBFLS();
        Var::var.CFL();
    } 
}