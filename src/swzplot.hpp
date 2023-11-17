#ifndef SWZPLOT_H
#define SWZPLOT_H


#include<type_traits>
#include<functional>
#include<algorithm>
#include<stdexcept>
#include<exception>
#include<valarray>
#include<iostream>
#include<variant>
#include<chrono>
#include<vector>
#include<string>
#include<memory>
#include<thread>
#include<mutex>
#include<cmath>
#include<list>
#include<map>

#include "GL/freeglut.h"
#include "gl2ps.h"

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-------------------------*-
// -*- axes_t_t ==> AxesBase -*-
// -*-------------------------*-
//! @todo: documentation
class AxesBase;
using Axes = std::shared_ptr<AxesBase>;
using AxesDict = std::map<int, Axes>;

// -*---------------------------*-
// -*- layer_t_t ==> LayerBase -*-
// -*---------------------------*-
//! @todo: documentation
class LayerBase;
using Layer = std::shared_ptr<LayerBase>;
using LayerDict = std::map<std::string, Layer>;

// -*-----------------------------*-
// -*- figure_t_t ==> FigureBase -*-
// -*-----------------------------*-
//! @todo: documentation
class FigureBase;
using Figure = std::shared_ptr<FigureBase>;
using FigureDict = std::map<int, Figure>;

// -*----------------------------*-
// -*- drawing_t_t ==> Drawable -*-
// -*----------------------------*-
class DrawableBase;
using Drawable = std::shared_ptr<DrawableBase>;
using DrawableList = std::list<Drawable>;

// -
class DrawableBase{
protected:
    Axes m_ca;

public:
    DrawableBase(const Axes ax): m_ca(ax){}
    virtual ~DrawableBase() = default;
    // Axes coordination
    double coord2D_to_xaxis(double x);
    double coord2D_to_yaxis(double y);
    double coord3D_to_xaxis(double x);
    double coord3D_to_yaxis(double y);
    double coord3D_to_zaxis(double z);
    virtual void draw() = 0;
    virtual void config() = 0;
    virtual void clear() = 0;
};

// -*----------------------------------------------------------------*-
// -*- ::utils                                                      -*-
// -*----------------------------------------------------------------*-
//! @note: math
enum Scale{
    Linear,
    Logarithm,
};


/**
 * @brief Get the maximum valued element from vector
 * 
 * @tparam T 
 * @param vec 
 * @return T 
 */
template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
T max(const std::vector<T>& vec){
    return *std::max_element(vec.begin(), vec.end());
}

template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
T min(const std::vector<T>& vec){
    return *std::min_element(vec.begin(), vec.end());
}

template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
T max(const std::vector<std::vector<T>>& mat){
    auto result = std::numeric_limits<double>::min();
    for(auto vec: mat){
        result = std::max(result, max(vec));
    }

    return static_cast<T>(result);
}

// -*-
template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
T min(const std::vector<std::vector<T>>& mat){
    auto result = std::numeric_limits<double>::max();
    for(auto vec: mat){
        result = std::min(result, min(vec));
    }
    return static_cast<T>(result);
}

//! @note: Vector
template<typename T>
using Vector = std::vector<T>;

//! @note: Matrix
template<typename T>
using Matrix = Vector<std::vector<T>>;

template<typename T, std::enable_if<std::is_floating_point_v<T>>>
Vector<T> linspace(T minval, T maxval, size_t count=10){
    if(count==0){ return Vector<T>{}; }
    if(count==1){ return Vector<T>{minval}; }
    if(count==2){ return Vector<T>{minval, maxval}; }
    Vector<T> result{};
    auto dx = (maxval - minval)/(count-1);
    for(size_t i=0; i < count; ++i){
        auto value = minval + dx*i;
        result.push_back(value);
    }
    return result;
}

// -*-
Matrix<double> peaks(size_t num);

// -*----------------------------------------------------------------*-
// -*- ::color                                                      -*-
// -*----------------------------------------------------------------*-
// -*-
class Color{
public:
    enum class ColorEnum{
        // - basic
        Black, Red, Blue, Green, Cyan, Magenta, Yellow, White,
        // - dark color
        DarkRed, DarkBlue, DarkGreen, DarkCyan, DarkMagenta, DarkYellow,
        // - light color
        LightRed, LightBlue, LightGreen, LightCyan, LightMagenta, LightYellow,
        // - universal color
        UniRed, UniBlue, UniGreen, UniCyan, UniMagenta, UniYellow,
        UniSkyBlue, UniPink, UniOrange, UniPurple, UniBrown,
    };

    struct Rgb{
        float r;
        float g;
        float b;
    };
private:
    std::variant<std::string, ColorEnum> m_value;
    
public:
    static std::map<ColorEnum, std::pair<std::string, std::string>> colors;
    static std::map<ColorEnum, Rgb> rgbcolors;
    // -*-
    Color(const std::string& colorspec);
    Color(float r, float g, float b);
    ~Color() = default;

    Color::Rgb rgb();
    std::string colorspec();
    Vector<float> to_vector();
};

// -*-
// Vector<float> colorspec_to_rgb(const std::string colorspec);
// std::string rgb_to_colorspec(Vector<float> rgb);

// -*----------------------------------------------------------------*-
// -*- ::axes                                                       -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::color                                                      -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::figure                                                     -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::layer                                                      -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::line                                                       -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::patch                                                      -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::surface                                                    -*-
// -*----------------------------------------------------------------*-

// -*----------------------------------------------------------------*-
// -*- ::text                                                       -*-
// -*----------------------------------------------------------------*-


// -*----------------------------------------------------------------*-
// -*- ::glut                                                       -*-
// -*----------------------------------------------------------------*-


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-

#endif