#include <iostream>
#include <cstdio>
#include <array>
#include <string>
#include <vector>
#include <fstream>

#include <extras.hpp>
#include <command.hpp>
#include <variables.hpp>

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

class Draw {
public:
    void Init(int _w, int _h) { // Initialize the program
        Var::var.KP(false);
        
        w = _w;
        h = _h;

        /*
        add spaces to string(fileline), then push it to a vector(filelines).
        this vector will later be used to export and load projects.
        */
        
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                std::cout << ' '; // fill screen with spaces
                Var::var.AddFL(' ');
            }
            std::cout << '\n'; //add a few newlines to the mix
            Var::var.PBFLS();
            Var::var.CFL();
        } 
    }

    void RemCoords(int x_, int y_) {
        for (int i = 0; i < Var::var.RetP().size(); i++) {
            Pixel ctok = Var::var.RetP()[i];
            if (ctok.x == x_ && ctok.y == y_) {
                Var::var.RemP(i);
                Var::var.ManFLS(x_, y_, ' ');
                break;
            }
        }
    } 

    int KeyPress() {
        char k = Extras::extras.Getch(); // gets single key input

        if (k == 'w') {
            if (Var::var.RetY() > 1) { 
                Var::var.Y(-1); 
            }
            if (Var::var.RetCS()) { 
                Var::var.KS(Var::var.RetKS()+1); 
            }
        } 
        if (k == 'a') {
            if (Var::var.RetX() > 1) { 
                Var::var.X(-1); 
            }
            if (Var::var.RetCS()) { 
                Var::var.KS(Var::var.RetKS()+1); 
            }
        }
        if (k == 's') {
            if (Var::var.RetY() < h-1) { 
                Var::var.Y(+1); 
            }
            if (Var::var.RetCS()) {
                Var::var.KS(Var::var.RetKS()+1); 
            }
        } 
        if (k == 'd') {
            if (Var::var.RetX() < w-1) { 
                Var::var.X(+1); 
            }
            if (Var::var.RetCS()) { 
                Var::var.KS(Var::var.RetKS()+1); 
            }
        }

        //presets check

        if (k >= '0' && k <= '9') {
            Var::var.Style(Var::var.RetPR()[k-'0'-1]);
        }

        Extras::extras.CaretPos(Var::var.RetX(), Var::var.RetY());

        if (k == '\n' || k == '\r') {
            Extras::extras.CColorPrint(Var::var.RetStyle(), Var::var.RetColor(), false);
            Extras::extras.CaretPos(Var::var.RetX(), Var::var.RetY());
            Pixel pixel;
            pixel.style = Var::var.RetStyle();
            pixel.color = Var::var.RetColor();
            pixel.back = Extras::extras.BoolToInt(Var::var.RetBack());
            pixel.x = Var::var.RetX()-1;
            pixel.y = Var::var.RetY()-1;
            Var::var.P(pixel);
            Extras::extras.DrawMessage("pixsize:"+std::to_string(Var::var.RetP().size()));
        }
        if (k == ' ') {
            std::cout << ' ';
            Extras::extras.CaretPos(Var::var.RetX(), Var::var.RetY());
            RemCoords(Var::var.RetX()-1, Var::var.RetY()-1);
        }

        if (k == 'c') {
            FindCommand(Extras::extras.Window("Command:", true));
        }
        return RETTYPE::SUCCESS;
    }

private:
    int w = 0;
    int h = 0;
};

int main() {
    signal(SIGINT, SigIntHandler);

    Draw draw;
    draw.Init(Extras::extras.TermWidth(), Extras::extras.TermHeight());

    IO::LoadPresets();
    //draw.Load(".drawintro");

    while (true) {
        draw.KeyPress();
        Extras::extras.ClearMessage();
    }

}