#include <command.hpp>

int FindCommand(std::string command) {
    const std::vector<std::array<std::string, 2>> commands = {
        {"save", "saves project into color document"},
        {"load", "loads project and the color with it"},
        {"export.save", "exports project as text for use in games or documents"},
        {"export.load", "loads export and converts it to projects"},
        {"erase", "erases pixel at coordinates"},
        {"color", "use this command to change color"},
        {"brush", "changes brush to another character"},
        {"help", "shows dialog for commands and information"}
    };

    if (command == "brush") {
        Var::var.Style(Extras::extras.Window("Brush:", true)[0]);
    }
    else if (command == "help") {
        std::string help = "";
        for (int i = 0; i < commands.size(); i++) {
            help += (commands[i][0]+" - "+commands[i][1]+"\n");
        }
        Extras::extras.Window(help, false);
    }
    else if (command == "save") {
        std::string name = Extras::extras.Window("Enter New Filename:", true);
        IO::Save(name);
        IO::Load(name);
    }
    else if (command == "export.save") {
        std::string name = Extras::extras.Window("Enter New Filename:", true);
        IO::Export(name);
        IO::Load(name);
    }
    else if (command == "export.load") {
        std::string name = Extras::extras.Window("Enter Load Filename:", true);
        IO::LoadExp(name);
    }
    else if (command == "load") {
        std::string name = Extras::extras.Window("Enter Load Filename:", true);
        IO::Load(name);
    }
    else if (command == "color") {
        int backinc = 0;
        if (Var::var.RetBack()) {
            backinc = 10;
        }
        else {
            backinc = 0;
        }
        
        std::string scolor = Extras::extras.Window("Set Color:", true);

        if (scolor == "black") {
            Var::var.Color(30+backinc);
        }
        else if (scolor == "red") {
            Var::var.Color(31+backinc);
        }
        else if (scolor == "green") {
            Var::var.Color(32+backinc);
        }
        else if (scolor == "yellow") {
            Var::var.Color(33+backinc);
        }
        else if (scolor == "blue") {
            Var::var.Color(34+backinc);
        }
        else if (scolor == "magenta") {
            Var::var.Color(35+backinc);
        }   
        else if (scolor == "cyan") {
            Var::var.Color(36+backinc);
        }
        else if (scolor == "white") {
            Var::var.Color(37+backinc);
        }
        else if (scolor == "back") {
            bool n = Extras::extras.FlipBool(Var::var.RetBack());
            Var::var.Back(n);
        }
        else if (scolor == "bright") {
            bool n = Extras::extras.FlipBool(Var::var.RetBright());
            Var::var.Bright(n);
        }
        else {
            Extras::extras.DrawMessage("Unknown Color");
        }
    }
    else if (command == "quit") {
        IO::Save("quitsave");
        exit(0);
    }
    else {
        Extras::extras.DrawMessage("Unknown Command");
    } 
}