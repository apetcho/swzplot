#include "swzplot.hpp"
#include<cctype>

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*---------*-
// -*- Color -*-
// -*---------*-

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
    this->m_rgbcolors[ColorEnum::Black] = Rgb(0.f, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::Red] = Rgb(1.f, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::Blue] = Rgb(0.f, 0.f, 1.f);
    this->m_rgbcolors[ColorEnum::Green] = Rgb(0.f, 1.f, 0.f);
    this->m_rgbcolors[ColorEnum::Cyan] = Rgb(0.f,1.f,1.f);
    this->m_rgbcolors[ColorEnum::Magenta] = Rgb(1.f,0.f, 1.f);
    this->m_rgbcolors[ColorEnum::Yellow] = Rgb(1.f, 1.f, 0.f);
    this->m_rgbcolors[ColorEnum::White] = Rgb(1.f, 1.f,1.f);

    float h = 0.6f;
    this->m_rgbcolors[ColorEnum::DarkRed] = Rgb(h, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::DarkBlue] = Rgb(0.f, 0.f, h);
    this->m_rgbcolors[ColorEnum::DarkGreen] = Rgb(0.f, h, 0.f);
    this->m_rgbcolors[ColorEnum::DarkCyan] = Rgb(0.f, h, h);
    this->m_rgbcolors[ColorEnum::DarkMagenta] = Rgb(h, 0.f, h);
    this->m_rgbcolors[ColorEnum::DarkYellow] = Rgb(h, h, 0.f);

    h = 0.5f;
    this->m_rgbcolors[ColorEnum::LightRed] = Rgb(1.f, h, h);
    this->m_rgbcolors[ColorEnum::LightBlue] = Rgb(h, h, 1.f);
    this->m_rgbcolors[ColorEnum::LightGreen] = Rgb(h, 1.f, h);
    this->m_rgbcolors[ColorEnum::LightCyan] = Rgb(h, 1.f, 1.f);
    this->m_rgbcolors[ColorEnum::LightMagenta] = Rgb(1.f, h, 1.f);
    this->m_rgbcolors[ColorEnum::LightYellow] = Rgb(1.f, 1.f, h);

    this->m_rgbcolors[ColorEnum::UniRed] = Rgb(1.f, 0.2f, 0.f);
    this->m_rgbcolors[ColorEnum::UniBlue] = Rgb(0.f, 0.25f, 1.f);
    this->m_rgbcolors[ColorEnum::UniGreen] = Rgb(0.2f, 0.6f, 0.4f);
    this->m_rgbcolors[ColorEnum::UniYellow] = Rgb(1.f, 1.f, 1.f);
    this->m_rgbcolors[ColorEnum::UniCyan] = Rgb(0.4f, 0.8f, 1.f);
    this->m_rgbcolors[ColorEnum::UniPink] = Rgb(1.f, 0.6f, 0.6f);
    this->m_rgbcolors[ColorEnum::UniOrange] = Rgb(1.f, 0.6f, 0.f);
    this->m_rgbcolors[ColorEnum::UniPurple] = Rgb(0.6f, 0.f, 0.4f);
    this->m_rgbcolors[ColorEnum::UniBrown] = Rgb(0.4f, 0.2f, 0.f);
}


// -*-
Color::Color(const std::string& colorspec){
    this->init_colors();
    this->m_colorspec = colorspec;
    this->m_rgb = {};
    auto ans = this->find_color_enum_by_spec();
    this->m_colorEnum = ans.value_or(ColorEnum::Black);
    auto rgb = this->m_rgbcolors.find(this->m_colorEnum);
    if(rgb == this->m_rgbcolors.end()){
        this->m_rgb = Rgb{};
    }else{
        this->m_rgb.r = rgb->second.r;
        this->m_rgb.g = rgb->second.g;
        this->m_rgb.b = rgb->second.b;
    }
}

// -*-
Color::Color(float r, float g, float b){
    this->init_colors();
    this->m_rgb = Rgb(r, g, b);
    auto ans = this->find_color_enum_by_rgb();
    this->m_colorEnum = ans.value_or(ColorEnum::Black);
    auto spec = this->m_colorspecs.find(this->m_colorEnum);
    if(spec == this->m_colorspecs.end()){
        this->m_colorspec = "black";
    }else{
        this->m_colorspec = spec->second.first;
    }
}

// -*-
void Color::rgb(float& r, float& g, float& b){
    r = this->m_rgb.r;
    g = this->m_rgb.g;
    b = this->m_rgb.b;
}

std::string Color::colorspec(){
    std::ostringstream stream;
    stream << "(" << this->m_rgb.r;
    stream << " " << this->m_rgb.g;
    stream << " " << this->m_rgb.b;
    stream << ")";
    return stream.str(); 
}

// -*-
std::string Color::colorspec(float r, float g, float b){
    std::ostringstream stream;
    stream << "(" << r << " " << g << " " << b << ")";
    return stream.str(); 
}

// -*-
std::string Color::colorspec(std::vector<float> rgb){
    if(rgb.size() != 3){
        throw std::runtime_error("Invalid argument to Color::colorspec()");
    }
    auto r = rgb[0];
    auto g = rgb[1];
    auto b = rgb[2];
    std::ostringstream stream;
    stream << "(" << r << " " << g << " " << b << ")";
    return stream.str(); 
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-