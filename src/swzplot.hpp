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
#include<optional>
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
// -*- Utility classes and functionalities                          -*-
// -*----------------------------------------------------------------*-
template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
struct Position {
    T x;
    T y;
    std::optional<T> z;

    Position() = default;
    explicit Position(T x, T y): x{x}, y{y}, z(std::nullopt){}
    explicit Position(T x, T y, T z): x{x}, y{y}, z{z}{}
    ~Position() = default;
};

template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
struct DataLim{
    T minval;
    T maxval;

    DataLim() = default;
    explicit DataLim(T min, T max): minval(min), maxval(max){}
    ~DataLim() = default;
};

template<typename T, std::enable_if<std::is_arithmetic_v<T>>>
struct BBox{
    T left;
    T bottom;
    T width;
    T height;

    BBox() = default;
    explicit BBox(T left, T bottom, T width, T height)
    : left{left}, bottom{bottom}, width{width}, height{height}
    {}
    ~BBox() = default;
};

// -*----------------------------------------------------------------*-
// -*- ::axes                                                       -*-
// -*----------------------------------------------------------------*-
class AxesBase: public std::enable_shared_from_this<AxesBase>{
private:
    std::mutex m_childer_mtx;
    Layer m_layer;
    Axes m_colorbar_ax;

public:
    AxesBase(Layer layer);
    ~AxesBase(){};
    Layer gcl(){ return this->m_layer; }

    // -*- 
    float cta0;
    float phi0;
    float cta;
    float phi;
    // -*-
    double xmouse;
    double ymouse;
    bool mouse; // Capture the mouse actions
    int xbuttonDown;
    int ybuttonDown;
    float ctaButtonDown;
    float phiButtonDown;
    bool (*mouse_callback)(int button, int state, int x, int y);

    // double xmin;
    // double xmax;
    
    double ymin;
    double ymax;
    double zmin;
    double zmax;

    void reset_limits();
    void config();
    // - set property -
    Axes set(const std::string& key, const std::string& value);
    Axes colorbar();

    void ptext(float x, float y, const std::string& text);
    void ptext3(float x, float y, float z, const std::string& text);
    void ptext3c(float x, float y, float z, const std::string& text);

    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);

    enum AxesType {
        Axes2D, Axes3D, ColorBar
    };

    enum AxesMode{
        Auto, Maxnual,
    };

    AxesType axType;

    // -*- styles -
    bool box;

private:
    int window_height();
    int window_width();
    void draw2d();
    void draw3d();
    void draw_colorbar();
    // -
    double coord2D_to_xaxis(double x);
    double coord2D_to_yaxis(double y);
    double coord3D_to_xaxis(double x);
    double coord3D_to_yaxis(double y);
    double coord3D_to_zaxis(double z);

public:
    // - get shared_pointer sharing the current instance of this AxesBase object
    Axes share(){
        return shared_from_this();
    }
};

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