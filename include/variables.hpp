#pragma once
#include <vector>
#include <string>

#define BACKINC 10

struct Pixel {
    int x;
    int y;
    char style;
    int color;
    int back;
    int bright;
};

class Var {
public:
    void X(int _x) {
        if (_x < 0) {
            x -= -_x;
        }
        else {
            x += _x;
        }
    }
    void Y(int _y) {
        if (_y < 0) {
            y -= -_y;
        }
        else {
            y += _y;
        }
    }
    void Color(int _color) {
        color = _color;
    }
    void Style(char _style) {
        style = _style;
    }
    void Back(bool _back) {
        back = _back;
    }
    void Bright(bool br) {
        bright = br;
    }
    void IM(bool _ismessage) {
        ismessage = _ismessage;
    }
    void CS(bool _countstrokes) {
        countstrokes = _countstrokes;
    }
    void KS(int n) {
        keystrokes = n;
    }
    void KP(bool kp) {
        keyprint = kp;
    }
    void P(Pixel pixel) {
        pixels.push_back(pixel);
    }
    void RemP(int index) {
        pixels.erase(pixels.begin()+index);
    }
    void T(Pixel pixel) {
        tokens.push_back(pixel);
    }
    void CT() {
        tokens.clear();
    }
    void FL(int _x, int _y, char _style) {
        filelines[_y][_x] = _style;
    }
    void AddFL(char add) {
        fileline += add;
    }
    void CFL() {
        fileline = "";
    }
    void PBFLS() {
        filelines.push_back(fileline);
    }
    void ManFLS(int _x, int _y, char _style) {
        filelines[_x][_y] = _style;
    }
    void CFLS() {
        filelines.clear();
    }
    void EFLS(int index, std::string edit) {
        filelines[index] = edit;
    }
    void PBPR(char preset) {
        presets.push_back(preset);
    }

    //--------------------------------------------

    int RetX() {
        return x;
    }
    int RetY() {
        return y;
    }
    int RetColor() {
        return color;
    }
    char RetStyle() {
        return style;
    }
    bool RetBack() {
        return back;
    }
    bool RetBright() {
        return bright;
    }
    bool RetIM() {
        return ismessage;
    }
    bool RetCS() {
        return countstrokes;
    }
    int RetKS() {
        return keystrokes;
    }
    bool RetKP() {
        return keyprint;
    }
    std::vector<Pixel> RetP() {
        return pixels;
    }
    std::vector<Pixel> RetT() {
        return tokens;
    }
    std::vector<std::string> RetFLS() {
        return filelines;
    }
    std::vector<char> RetPR() {
        return presets;
    }

    static Var var;
    
private:
    int x = 0;
    int y = 0;
    int color = 29;
    char style = 'O';
    bool back = false;
    bool bright = false;
    bool ismessage = false;
    bool countstrokes = false;
    bool keyprint = false;
    int keystrokes = 0;
    std::vector<Pixel> pixels;
    std::vector<Pixel> tokens;
    std::string fileline = "";
    std::vector<std::string> filelines;
    std::vector<char> presets;
};