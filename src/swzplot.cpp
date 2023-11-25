#include "swzplot.hpp"
#include<cctype>

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*---------*-
// -*- Color -*-
// -*---------*-
std::string Color::defaultSpec = "black";

void Color::init_colors(){
    this->m_colorspecs = {};
    // - basic color
    this->m_colorspecs[ColorEnum::Black] = {"k", "black"};
    this->m_colorspecs[ColorEnum::Red] = {"r", "red"};
    this->m_colorspecs[ColorEnum::Blue] = {"b", "blue"};
    this->m_colorspecs[ColorEnum::Green] = {"g", "green"};
    this->m_colorspecs[ColorEnum::Cyan] = {"c", "cyan"};
    this->m_colorspecs[ColorEnum::Magenta] = {"m", "magenta"};
    this->m_colorspecs[ColorEnum::Yellow] = {"y", "yellow"};
    this->m_colorspecs[ColorEnum::White] = {"w", "white"};
    // - dark color
    this->m_colorspecs[ColorEnum::DarkRed] = {"dr", "darkred"};
    this->m_colorspecs[ColorEnum::DarkBlue] = {"db", "darkblue"};
    this->m_colorspecs[ColorEnum::DarkGreen] = {"dg", "darkgreen"};
    this->m_colorspecs[ColorEnum::DarkCyan] = {"dc", "darkcyan"};
    this->m_colorspecs[ColorEnum::DarkMagenta] = {"dm", "darkmagenta"};
    this->m_colorspecs[ColorEnum::DarkYellow] = {"dy", "darkyellow"};
    // - light color
    this->m_colorspecs[ColorEnum::LightRed] = {"lr", "lightblue"};
    this->m_colorspecs[ColorEnum::LightBlue] = {"lb", "lightblue"};
    this->m_colorspecs[ColorEnum::LightGreen] = {"lg", "lightgreen"};
    this->m_colorspecs[ColorEnum::LightCyan] = {"lc", "lightcyan"};
    this->m_colorspecs[ColorEnum::LightMagenta] = {"lm", "lightmagenta"};
    this->m_colorspecs[ColorEnum::LightYellow] = {"ly", "lightyellow"};
    // - universal color
    this->m_colorspecs[ColorEnum::UniRed] = {"ur", "universalred"};
    this->m_colorspecs[ColorEnum::UniBlue] = {"ub", "universalblue"};
    this->m_colorspecs[ColorEnum::UniGreen] = {"ug", "universalgreen"};
    this->m_colorspecs[ColorEnum::UniYellow] = {"uy", "universalyellow"};
    this->m_colorspecs[ColorEnum::UniCyan] = {"uc", "skyblue"};
    this->m_colorspecs[ColorEnum::UniOrange] = {"uo", "universalorange"};
    this->m_colorspecs[ColorEnum::UniPink] = {"up", "pink"};
    this->m_colorspecs[ColorEnum::UniMagenta] = {"um", "purple"};
    this->m_colorspecs[ColorEnum::UniBrown] = {"ubr", "brown"};

    // -*-
    this->m_rgbcolors = {};
    this->m_rgbcolors[ColorEnum::Black] = {.r=0.f, .g=0.f, .b=0.f };
    this->m_rgbcolors[ColorEnum::Red] = {.r=1.f, .g=0.f, .b=0.f };
    this->m_rgbcolors[ColorEnum::Blue] = {.r=0.f, .g=0.f, .b=1.f };
    this->m_rgbcolors[ColorEnum::Green] = {.r=0.f, .g=1.f, .b=0.f };
    this->m_rgbcolors[ColorEnum::Cyan] = {.r=0.f, .g=1.f, .b=1.f };
    this->m_rgbcolors[ColorEnum::Magenta] = {.r=1.f, .g=0.f, .b=1.f };
    this->m_rgbcolors[ColorEnum::Yellow] = {.r=1.f, .g=1.f, .b=0.f };
    this->m_rgbcolors[ColorEnum::White] = {.r=1.f, .g=1.f, .b=1.f };

    float h = 0.6f;
    this->m_rgbcolors[ColorEnum::DarkRed] = {.r=h, .g=0.f, .b=0.f };
    this->m_rgbcolors[ColorEnum::DarkBlue] = {.r=0.f, .g=0.f, .b=h };
    this->m_rgbcolors[ColorEnum::DarkGreen] = {.r=0.f, .g=h, .b=0.f };
    this->m_rgbcolors[ColorEnum::DarkCyan] = {.r=0.f, .g=h, .b=h };
    this->m_rgbcolors[ColorEnum::DarkMagenta] = {.r=h, .g=0.f, .b=h };
    this->m_rgbcolors[ColorEnum::DarkYellow] = {.r=h, .g=h, .b=0.f };

    h = 0.5f;
    this->m_rgbcolors[ColorEnum::LightRed] = {.r=1.f, .g=h, .b=h };
    this->m_rgbcolors[ColorEnum::LightBlue] = {.r=h, .g=h, .b=1.f };
    this->m_rgbcolors[ColorEnum::LightGreen] = {.r=h, .g=1.f, .b=h };
    this->m_rgbcolors[ColorEnum::LightCyan] = {.r=h, .g=1.f, .b=1.f };
    this->m_rgbcolors[ColorEnum::LightMagenta] = {.r=1.f, .g=h, .b=1.f };
    this->m_rgbcolors[ColorEnum::LightYellow] = {.r=1.f, .g=1.f, .b=h };

    this->m_rgbcolors[ColorEnum::UniRed] = {.r=1.f, .g=0.2f, .b=0.f };
    this->m_rgbcolors[ColorEnum::UniBlue] = {.r=0.f, .g=0.25f, .b=1.f };
    this->m_rgbcolors[ColorEnum::UniGreen] = {.r=0.2f, .g=0.6f, .b=0.4f };
    this->m_rgbcolors[ColorEnum::UniYellow] = {.r=1.f, .g=1.f, .b=1.f };
    this->m_rgbcolors[ColorEnum::UniCyan] = {.r=0.4f, .g=0.8f, .b=1.f };
    this->m_rgbcolors[ColorEnum::UniPink] = {.r=1.f, .g=0.6f, .b=0.6f };
    this->m_rgbcolors[ColorEnum::UniOrange] = {.r=1.f, .g=0.6f, .b=0.f };
    this->m_rgbcolors[ColorEnum::UniPurple] = {.r=0.6f, .g=0.f, .b=0.4f };
    this->m_rgbcolors[ColorEnum::UniBrown] = {.r=0.4f, .g=0.2f, .b=0.f };
}


// 


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-