/// \file swzplot.hpp
/// Includes swzplot plottling library header file.
//
// Copyright (c) 2023 Eyram K. Apetcho
// Distributed under MIT License
//
#ifndef SWZPLOT_H
#define SWZPLOT_H

#include<type_traits>
#include<functional>
#include<algorithm>
#include<stdexcept>
#include<exception>
#include<valarray>
#include<iostream>
#include<sstream>
#include<variant>
#include<cassert>
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
//! @note: renaming
//      - LayerBase ==> CanvasBase
//      - Layer     ==> Canvas
//      - LayerDict ==> CanvasDict
class CanvasBase;
using Canvas = std::shared_ptr<CanvasBase>;
using CanvasDict = std::map<std::string, Canvas>;

// -*-----------------------------*-
// -*- figure_t_t ==> FigureBase -*-
// -*-----------------------------*-
//! @todo: documentation
class FigureBase;
using Figure = std::shared_ptr<FigureBase>;
using FigureDict = std::map<int, Figure>;

// -*-------------------------------*-
// -*- Other forward declarations; -*-
// -*-------------------------------*-
//! @note: LineBase is a DrawableBase
class LineBase;
using Line = std::shared_ptr<LineBase>;

//! @note: PatchBase is a DrawableBase
class PatchBase;
using Patch = std::shared_ptr<PatchBase>;

//! @note: SurfaceBase is a DrawableBase
class SurfaceBase;
using Surface = std::shared_ptr<SurfaceBase>;

//! @note: TextBase is a DrawableBase
class TextBase;
using Text = std::shared_ptr<TextBase>;

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

enum AxesType {
    Axes2D, Axes3D, ColorBar
};

enum AxesLimMode{
    Auto, Maxnual
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

//! @note: Tensor
using Colormap = Matrix<std::vector<float>>;

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

// -*-
// -*-
static std::string tolower(const std::string& name){
    std::string color(name);
    std::string result = "";
    std::string::iterator ptr = color.begin();
    while(ptr != color.end()){
        result += std::tolower(*ptr);
        ptr++;
    }
    return result;
}

static std::string tolower(const char* name){
    std::string color(name);
    std::string result = "";
    std::string::iterator ptr = color.begin();
    while(ptr != color.end()){
        result += std::tolower(*ptr);
        ptr++;
    }
    return result;
}
// -*----------------------------------------------------------------*-
// -*- ::color                                                      -*-
// -*----------------------------------------------------------------*-
// -*-
/** \class Color
 * @brief Define color representation used in swzplot
 * 
 */
class Color{
private:
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
        float r, g, b;

        Rgb(): r(0.f), g(0.f), b(0.f){}
        Rgb(float red, float green, float blue)
        : r(red), g(green), b(blue){}

        bool operator==(const Rgb& other){
            bool ans = (
                this->r==other.r &&
                this->g==other.g &&
                this->b==other.b
            );
            return ans;
        }
    };
    
    //std::variant<std::string, ColorEnum> m_value;
    std::map<ColorEnum, std::pair<std::string, std::string>> m_colorspecs;
    std::map<ColorEnum, Rgb> m_rgbcolors;

    std::string m_colorspec;
    Rgb m_rgb;
    ColorEnum m_colorEnum;

    std::optional<ColorEnum> find_color_enum_by_spec(){
        std::optional<ColorEnum> result = std::nullopt;
        auto spec = tolower(this->m_colorspec);
        for(auto entry: this->m_colorspecs){
            if(entry.second.first == spec || entry.second.second==spec){
                result = entry.first;
            }
        }
        return result;
    }

    std::optional<ColorEnum> find_color_enum_by_rgb(){
        std::optional<ColorEnum> result = std::nullopt;
        for(auto entry: this->m_rgbcolors){
            if(entry.second == this->m_rgb){
                result = entry.first;
            }
        }
        return result;
    }

    void init_colors();
public:
    // -*-
    Color(const std::string& colorspec);
    Color(float r, float g, float b);
    ~Color() = default;

    void rgb(float& r, float& g, float& b);
    std::string colorspec();
    std::string colorspec(float r, float g, float b);
    std::string colorspec(std::vector<float> rgb);
    Vector<float> to_vector();
};

// -*-
// Vector<float> colorspec_to_rgb(const std::string colorspec);
// std::string rgb_to_colorspec(Vector<float> rgb);

// -*----------------------------------------------------------------*-
// -*- Utility classes and functionalities                          -*-
// -*----------------------------------------------------------------*-
template<typename T>
struct Position {
    T x;
    T y;
    std::optional<T> z;

    Position() = default;
    explicit Position(T x, T y): x{x}, y{y}, z(std::nullopt){}
    explicit Position(T x, T y, T z): x{x}, y{y}, z{z}{}
    ~Position() = default;
};

template<typename T>
struct DataLim{
    T minval;
    T maxval;

    DataLim() = default;
    explicit DataLim(T min, T max): minval(min), maxval(max){}
    ~DataLim() = default;
};

template<typename T>
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
public:
    std::vector<std::vector<float>> cmap;
private:
    std::mutex m_children_mtx;
    Canvas m_canvas;
    Axes m_colorbar_ax;

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

// public:
    // - get shared_pointer sharing the current instance of this AxesBase object
    Axes share(){
        return shared_from_this();
    }

public:
    AxesBase(Canvas layer);
    ~AxesBase(){
        this->m_children.clear();
    }; // XXX
    // Canvas gcl(){ return this->m_canvas; }
    Canvas canvas(){ return this->m_canvas; }

    // -*- START OF PROTECTED API -*-
    // ---- default values of the ones below --- 
    float m_cta0;
    float m_phi0;
    // ----- mouse controlled data -----
    float m_cta;
    float m_phi;
    // -*-
    // double xmouse;
    // double ymouse;
    Position<double> m_xymouse;
    bool m_mouse; // Capture the mouse actions
    // int xbuttonDown;
    // int ybuttonDown;
    Position<int> m_xybutton;
    // - View angles when the button is pressed -
    float m_ctaButtonDown;
    float m_phiButtonDown;
    // - Optional user-defined callback when a mouse button is clicked -
    bool (*mouse_callback)(int button, int state, int x, int y);

    // double xmin;
    // double xmax;
    DataLim<double> m_xdatalim;
    // double ymin;
    // double ymax;
    DataLim<double> m_ydatalim;
    // double zmin;
    // double zmax;
    DataLim<double> m_zdatalim;

    // - Reset the axes limits for recalculation -
    void reset_limits();
    // - Configure the axes object to fit the data -
    void config();
    // - set property -
    Axes set(const std::string& key, const std::string& value);
    // - Add legend colorbar to the axes -
    Axes colorbar();

    // - Draw text to the axes -
    void ptext(float x, float y, const std::string& text);
    // - Draw text to the axes in 3D, left adjusted at the coordinates -
    void ptext3(float x, float y, float z, const std::string& text);
    // - Draw text to the axes in 3D, centered to the coordinates -
    void ptext3c(float x, float y, float z, const std::string& text);

    // - Callback when mouse button clicked -
    //! @note: change this to @em{on_mouse_clicked()}
    bool mouse(int button, int state, int x, int y);
    // - Callback when mouse moved with button pressed -
    //! @note: change this to @em{on_mouse_pressed()}
    bool motion(int x, int y);

    // - Draw ticks to the axes -
    //! @note: change this to @em{draw_tick()} 
    Vector<double> make_tick(double minval, double maxval);

    // - Indicates the type of plotted data -
    AxesType m_axType; // type

    // -*- styles -
    bool m_boxed; // Axes on/off :: boxFlag
    float m_linewidth;
    std::string m_tickDir; // "in" | "out"
    bool m_is_visible;
    // - flags to draw grid -
    bool m_xgrid; // xgridFlag
    bool m_ygrid; // ygridFlag
    bool m_zgrid; // zgridFlag

    // General information
    DrawableList m_children;  // container for children drawings
    Drawable m_co;            // currently active drawing object
    bool m_selected;          // Axes have been clicked by the user
    // - Position and size of the axes (left, bottom, width, height) -
    BBox<float> m_axBBox;     // ::position     i.e position[4]
    // - Position and size of axes used by 3D-view (left, bottom, width, height)
    BBox<float> m_viewBBox;   // ::viewport3d   i.e viewport3d[4]

    // -*- Scale and axes
    std::string m_xAxisLocation;  // "top" | "bottom"
    std::string m_yAxisLocation;  // "left"| "right" 

    DataLim<double> m_xlim;       // x-axis limit   :: double[2]
    DataLim<double> m_ylim;       // y-axis limit   :: double[2]
    DataLim<double> m_zlim;       // z-axis limit   :: double[2]
    // - Mode of axes adjustment to data: [AxisLimMode::Auto] | AxisLimMode::manual
    AxesLimMode m_xlimMode;
    AxesLimMode m_ylimMode;
    AxesLimMode m_zlimMode;
    // - Scale of axes: [Scale::Linear] | Scale::Logarithm
    Scale m_xscale;
    Scale m_yscale;
    Scale m_zscale;
    Vector<double> m_xticks;
    Vector<double> m_yticks;
    Vector<double> m_zticks;
    bool m_ticklabelFlag;         // ticklabel

    // -
    Position<float> m_cameraPosition;   // Position of the 3D-view camera
    Position<float> m_cameraTarget;     // Position of the 3D-view target
    Position<float> m_cameraUpVector;   // Orientation of the 3D-view camera

    // -*- Label
    std::string m_title;
    std::string m_xlabel;
    std::string m_ylabel;
    std::string m_zlabl;
    DataLim<double> m_clim;

    // -*- Draw the axes and its children
    void draw();

    // -*- END OF PROTECTED API -*-

    // -*------------------------------*-
    // -*- Interface (i.e public API) -*-
    // -*------------------------------*-
    // - set axis limit (2D & 3D)
    Axes axis(double xmin, double xmax, double ymin, double ymax);
    Axes axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    // - toggle axis visibility: "on" | "off" | true | false
    Axes axis(std::string onoff);
    Axes axis(bool onoff=true);
    // - toggle grid visibility: "on" | "off" | true | false
    Axes grid(std::string onoff);
    Axes grid(bool onoff=true);
    // - toggle ticklabel visibility: true | false
    Axes ticklabel(bool onoff=true);
    // - title
    Axes title(std::string label);
    // - [x|y|z]label
    Axes xlabel(std::string label);
    Axes ylabel(std::string label);
    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    Axes capture_mouse(bool flag);

    // Add a new child drawing to the axes. The added type must inherit
    // DrawableBase
    template<typename T>
    std::shared_ptr<T> add(){
        std::shared_ptr<T> ptr = std::make_shared<T>(this->share());
        this->m_co = std::dynamic_pointer_cast<DrawableBase, T>(ptr);
        assert(this->m_co);
        std::unique_lock<std::mutex> lock(this->m_children_mtx);
        this->m_children.push_back(this->m_co);
        return ptr;
    }

    // Ge the current drawing object. If the type of object requested is
    // incompatible with the currently selected object, or if no object has
    // yet been created, a new one will be create, added and returned.
    template<typename T>
    std::shared_ptr<T> gco(){
        if(!this->m_co){
            return this->add<T>();
        }
        std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T, DrawableBase>(this->m_co);
        return ptr ? ptr : this->add<T>();
    }

    //! @note: clear the figure|axes
    //! @todo: implement this here
    Axes clear(){
        std::unique_lock<std::mutex> lock(this->m_children_mtx);
        this->m_co.reset();
        this->m_children.clear();
        return this->share();
    }

    // - set color of axes
    void color(float r, float g, float b);
    //! @note: It will better our utility classes here for colormap operations
    // - set the colormap of the children's axes
    Vector<float> colormap(std::string color, float target);
    void colormap(std::string color);
    void colormap(const Matrix<float>& colors);

    // - set colormaps
    void gray(){ this->colormap("gray"); }
    void jet(){ this->colormap("jet"); }
    void hsv(){ this->colormap("hsv"); }
    void hot(){ this->colormap("hot"); }
    void cool(){ this->colormap("cool"); }
    void spring(){ this->colormap("spring"); }
    void summer(){ this->colormap("summer"); }
    void autumn(){ this->colormap("autumn"); }
    void winter(){ this->colormap("winter"); }

    Vector<float> map_to_color(double x);

    // - draw vertex
    void vertex(double x, double y);

    void vertex(double x, double y, double z);

    // - plot data
    Line plot(const Vector<double>& yvec);
    Line plot(const Vector<double>& xvec, const Vector<double>& yvec);
    Line plot(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& zvec
    );

    // - log-scale plot
    Line semilogx(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line semilogy(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line loglog(const Vector<double>& xvec, const Vector<double>&  yvec);

    // - vertex
    void vertex(double x, double y, double dx, double dy);
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec
    );
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec,
        const Vector<double>& dyvec
    );

    // - Surface & contour
    Surface surface(const Matrix<double>& zmat);
    Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat);
    Surface surface(const Matrix<double>& zmat, const Colormap& cdata);
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Colormap& cdata);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Colormap& cdata
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );

    // -*-
    Surface contour(const Matrix<double>& zmat);
    Surface contour(const Matrix<double>& zmat, unsigned int n);
    Surface contour(const Matrix<double>& zmat, const Vector<double>& values);
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        unsigned int n
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Vector<double>& values
    );

    Surface mesh(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surf(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );

    // - shading:
    void shading(std::string arg);

    // -
    Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat);
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Patch bar(const Vector<double>& ydata);
    Patch bar(const Vector<double>& ydata, float width);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

    //! @todo: add font information
    Text text(double x, double y, const std::string message);
};

// -*----------------------------------------------------------------*-
// -*- ::layer                                                      -*-
// -*----------------------------------------------------------------*-
class CanvasBase: public std::enable_shared_from_this<CanvasBase>{
private:
    Axes m_ca;
    Axes m_selected_axes;
    bool m_visible;
    Position<float> m_xyButtonDown;

public:
    // -
    std::chrono::steady_clock::time_point timeCliked;
    std::string m_name;       // or simply ::name
    Figure m_figure;          // 
    AxesDict m_axesDict;      // axes

    // -
    CanvasBase(const Figure fig, const std::string& name, bool visible)
    : m_visible{visible}, m_name{name}, m_figure{fig}
    {}

    // -
    void draw();
    Axes subplot(unsigned int m, unsigned int n, unsigned p);
    // -*-
    Axes gca(){
        return this->m_ca ? this->m_ca : this->subplot(1,1,1);
    }

    // -*-
    Figure gcf(){
        return this->m_figure;
    }

    // -*-
    Canvas clear(){
        this->m_ca.reset();
        this->m_axesDict.clear();
        return this->share();
    }

    void toggle_visibility();
    void set_visibility(bool flag);
    bool is_visible(){ return this->m_visible; }
    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);

    // -*----------------------------*-
    // -*- Interface i.e public API -*-
    // -*----------------------------*-
    template<typename T>
    void set(const std::string key){
        this->gca()->gco<T>()->set(key);
    }

    void set(const std::string key);

    template<typename T>
    void set(float key){
        this->gca()->gco<T>()->set(key);
    }

    void set(float key);

    template<typename T>
    void set(std::string key, std::string val){
        this->gca()->gco<T>()->set(key, val);
    }

    void set(std::string key, std::string val);

    template<typename T>
    void set(std::string key, float val){
        this->gca()->gco<T>()->set(key, val);
    }
    
    void set(std::string key, float val);

    // -*-
    // - set axis limit (2D & 3D)
    void axis(double xmin, double xmax, double ymin, double ymax);
    void axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    // - toggle axis visibility: "on" | "off" | true | false
    void axis(std::string onoff);
    void axis(bool onoff);
    // - toggle grid visibility: "on" | "off" | true | false
    void grid(std::string onoff);
    void grid(bool onoff);
    // - toggle ticklabel visibility: true | false
    void ticklabel(bool onoff);
    // - title
    void title(std::string label);
    // - [x|y|z]label
    void xlabel(std::string label);
    void ylabel(std::string label);
    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    void capture_mouse(bool flag);

    // -*-
    Axes colorbar(){
        return this->gca()->colorbar();
    }

    void gray(){
        this->gca()->gray();
    }

    void jet(){
        this->gca()->jet();
    }

    void hsv(){
        this->gca()->hsv();
    }

    void hot(){
        this->gca()->hot();
    }

    void cool(){
        this->gca()->cool();
    }

    void spring(){
        this->gca()->spring();
    }

    void summer(){
        this->gca()->summer();
    }

    void autumn(){
        this->gca()->autumn();
    }

    void winter(){
        this->gca()->winter();
    }

    // - draw vertex
    void vertex(double x, double y);
    void vertex(double x, double y, double z);

    // - plot data
    Line plot(const Vector<double>& yvec);
    Line plot(const Vector<double>& xvec, const Vector<double>& yvec);
    Line plot(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& zvec
    );

    // - log-scale plot
    Line semilogx(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line semilogy(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line loglog(const Vector<double>& xvec, const Vector<double>&  yvec);

    // - vertex
    void vertex(double x, double y, double dx, double dy);
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec
    );
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec,
        const Vector<double>& dyvec
    );

    // ----
    // - Surface & contour
    Surface surface(const Matrix<double>& zmat);
    Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat);
    Surface surface(const Matrix<double>& zmat, const Colormap& cdata);
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Colormap& cdata);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Colormap& cdata
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );

    // -*-
    Surface contour(const Matrix<double>& zmat);
    Surface contour(const Matrix<double>& zmat, unsigned int n);
    Surface contour(const Matrix<double>& zmat, const Vector<double>& values);
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        unsigned int n
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Vector<double>& values
    );

    Surface mesh(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surf(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );

    // - shading:
    void shading(std::string arg);

    // -
    Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat);
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Patch bar(const Vector<double>& ydata);
    Patch bar(const Vector<double>& ydata, float width);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

    //! @todo: add font information
    Text text(double x, double y, const std::string message);

private:
    Canvas share(){
        return shared_from_this();
    }
};

// -*----------------------------------------------------------------*-
// -*- ::figure                                                     -*-
// -*----------------------------------------------------------------*-
class FigureBase: public std::enable_shared_from_this<FigureBase>{
private:
    CanvasDict m_canvasDict;

public:
    // user defined event callbak
    void (*keyboard_callack)(unsigned char, int, int);
    // -
    int m_window_width;
    int m_window_height;
    Position<int> m_xyPassive;
    std::string m_window_name;    // figname
    int m_window_num;
    BBox<int> m_windowBBox;   // position[4]
    Canvas m_current_canvas;  // cl
    Canvas m_selected_canvas;

    FigureBase(std::string name="plot", bool visible=true);
    ~FigureBase() = default;

    // -
    //! @note: ??? set_figure_name()
    void set_window_name(const std::string& name);
    // -
    Canvas canvas(std::string name="default", bool visible=true);
    
    // -*-
    Canvas get_current_canvas(){
        return this->m_current_canvas ? this->m_current_canvas : this->canvas();
    }

    // -*-
    Figure clear(){
        this->m_current_canvas.reset();
        this->m_canvasDict.clear();
        return this->share();
    }

    void draw();
    void draw_all(); // draw_layer_list

    // - callback
    void reshape(int width, int height);
    void mouse(int button, int state, int x, int y);
    void passivemotion(int x, int y);
    void keyboard(char key, int x, int y);

    Figure gcf(){ return this->share(); }
    Axes gca(){ return this->get_current_canvas()->gca(); }
    Axes subplot(unsigned int m, unsigned int n, unsigned int k){
        return this->get_current_canvas()->subplot(m, n, k);
    }

    // -*----------------------------*-
    // -*- Interface i.e public API -*-
    // -*----------------------------*-
    template<typename T>
    void set(const std::string key){ this->gca()->gco<T>()->set(key); }

    void set(const std::string key);

    template<typename T>
    void set(float key){
        this->gca()->gco<T>()->set(key);
    }
    
    void set(float key);

    template<typename T>
    void set(std::string key, std::string val){
        this->gca()->gco<T>()->set(key, val);
    }
    
    void set(std::string key, std::string val);

    template<typename T>
    void set(std::string key, float val){
        this->gca()->gco<T>()->set(key, val);
    }
    
    void set(std::string key, float val);

    // -*-
    // - set axis limit (2D & 3D)
    void set_axis(double xmin, double xmax, double ymin, double ymax){
        this->gca()->axis(xmin, xmax, ymin, ymax);
    }

    void axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax){
        this->gca()->axis(xmin, xmax, ymin, ymax, zmin, zmax);
    }

    // - toggle axis visibility: "on" | "off" | true | false
    void axis(std::string onoff){
        this->gca()->axis(onoff);
    }

    void axis(bool onoff){
        this->gca()->axis(onoff);
    }

    // - toggle grid visibility: "on" | "off" | true | false
    void grid(std::string onoff){
        this->gca()->grid(onoff);
    }

    void grid(bool onoff){
        this->gca()->grid(onoff);
    }

    // - toggle ticklabel visibility: true | false
    void ticklabel(bool onoff){
        this->gca()->ticklabel(onoff);
    }

    // - title
    void title(std::string label){
        this->gca()->title(label);
    }

    // - [x|y|z]label
    void xlabel(std::string label){
        this->gca()->xlabel(label);
    }

    void ylabel(std::string label){
        this->gca()->ylabel(label);
    }

    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    void capture_mouse(bool flag){
        this->gca()->capture_mouse(flag);
    }

    Axes colorbar(){
        return this->gca()->colorbar();
    }

    void gray(){ this->gca()->gray(); }
    void jet(){ this->gca()->jet(); }
    void hsv(){ this->gca()->hsv(); }
    void hot(){ this->gca()->hot(); }
    void cool(){ this->gca()->cool(); }
    void spring(){ this->gca()->spring(); }
    void summer(){ this->gca()->summer(); }
    void autumn(){ this->gca()->autumn(); }
    void winter(){ this->gca()->winter(); }

    // - draw vertex
    void vertex(double x, double y);
    void vertex(double x, double y, double z);

    // - plot data
    Line plot(const Vector<double>& yvec);
    Line plot(const Vector<double>& xvec, const Vector<double>& yvec);
    Line plot(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& zvec
    );

    // - log-scale plot
    Line semilogx(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line semilogy(const Vector<double>& xvec, const Vector<double>&  yvec);
    Line loglog(const Vector<double>& xvec, const Vector<double>&  yvec);

    // - vertex
    void vertex(double x, double y, double dx, double dy);
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec
    );
    void errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& dxvec,
        const Vector<double>& dyvec
    );

    // ----
    // - Surface & contour
    Surface surface(const Matrix<double>& zmat);
    Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat);
    Surface surface(const Matrix<double>& zmat, const Colormap& cdata);
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Colormap& cdata);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Colormap& cdata
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );

    // -*-
    Surface contour(const Matrix<double>& zmat);
    Surface contour(const Matrix<double>& zmat, unsigned int n);
    Surface contour(const Matrix<double>& zmat, const Vector<double>& values);
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        unsigned int n
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Vector<double>& values
    );

    Surface mesh(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surf(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );

    // - shading:
    void shading(std::string arg);

    // -
    Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat);
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cdata
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cdata
    );

    Patch bar(const Vector<double>& ydata);
    Patch bar(const Vector<double>& ydata, float width);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

    //! @todo: add font information
    Text text(double x, double y, const std::string message);
    void print(const std::string filename="out.eps");
    void savefig(const std::string filename="out.eps");

private:
    Figure share(){
        return shared_from_this();
    }
};

// -*----------------------------------------------------------------*-
// -*- ::line                                                       -*-
// -*----------------------------------------------------------------*-
class LineBase: public DrawableBase, public std::enable_shared_from_this<LineBase>{
public:
    // -
    bool m_errorbarFlag;

    void clear();
    void set_color(float r, float g, float b);

    // -*-
    unsigned int m_maxCapacity;
    bool m_stopAtMax;
    Vector<double> m_xdata;
    Vector<double> m_ydata;
    Vector<double> m_zdata;
    std::string m_color;
    std::string m_lineStyle;  // "-" | "--" | ":" | "-." "none"
    float m_lineWidth;
    // marker: "none" | "." | "+" | "x" | "d" | "^" | "v" | "o" | "*" | "s"
    std::string m_marker;
    float m_markerSize;
    std::string m_markerEdgeColor;
    std::string m_markerFaceColor;
    bool m_visible;

    // -
    LineBase(const Axes axes);
    Figure gcf();

    void draw();
    void config();

    Line set_capacity(unsigned int);
    Line stop_at_max(bool flag=true);

    // -*-
    Line vertex(double x, double y);
    
    Line line(const Vector<double>& xvec, const Vector<double>& yvec);
    Line plot(const Vector<double>& yvec);
    Line plot(const Vector<double>& xvec, const Vector<double>& yvec);
    Line plot(const Vector<double>& xvec, const Vector<double>& yvec, const Vector<double>& zvec);
    Line plot(const std::valarray<double>& xvec, const std::valarray<double>& yvec);

    Line semilogx(const Vector<double>& xvec, const Vector<double>& yvec);
    Line semilogy(const Vector<double>& xvec, const Vector<double>& yvec);
    Line loglog(const Vector<double>& xvec, const Vector<double>& yvec);

    Line vertex(double x, double y, double dx, double dy);
    Line errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& evec
    );
    Line errorbar(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Vector<double>& exvec,
        const Vector<double>& eyvec
    );

    // -
    Line vertex(double x, double y, double z);
    Line set(float linewidth);
    Line set(std::string key); // set property
    Line set(std::string key, std::string val);
    Line set(std::string key, float val);

private:
    std::pair<double, double> minmax(const Vector<double>& data, Scale scale);
    std::mutex m_data_mtx;
};

// -*----------------------------------------------------------------*-
// -*- ::patch                                                      -*-
// -*----------------------------------------------------------------*-
class PatchBase: public DrawableBase, public std::enable_shared_from_this<PatchBase>{
public:
    AxesType axesType;
    Matrix<int> faces;
    Matrix<double> vertices;
    Matrix<double> xdata;
    Matrix<double> ydata;
    Matrix<double> zdata;
    Colormap cdata;

    // shading value: <colorspec> | "none" | "flat" | "interp"
    std::string edgeColor;
    std::string faceColor;
    std::string lineStyle; // "-" | "--" | ":" | "-." | "none"
    float lineWidth;

    // -
    PatchBase(const Axes axes)
    : DrawableBase(axes)
    , axesType(AxesType::Axes2D)
    , edgeColor("k")
    , faceColor("r")
    , lineStyle("-")
    , lineWidth(1.f)
    {}

    // -
    void clear();
    void draw();
    Patch bar(const Vector<double>& yvec, float width=0.8);
    Patch bar(const Vector<double>& xvec, const Vector<double>& yvec, float width=0.8);

    // -
    Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat);
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Vector<double>& cvec // Matrix<double>???
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cten
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Vector<double>& cvec
    );
    Patch patch(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cten
    );

    Patch set(std::string key, std::string val);
    Patch set(std::string key, float val);

    Colormap index_to_trucolor(const Vector<double> indexcolor);
    void config();

private:
    void draw2d();
    void draw3d();
    std::mutex m_data_mtx;

};

// -*----------------------------------------------------------------*-
// -*- ::surface                                                    -*-
// -*----------------------------------------------------------------*-
//! @todo: Change Tensor to Colormap
//! @note: According to the previous definition Colormap = Matrix<Vector<float>>;
//! @note: It would be better to do this: Colormap = Vector<Color>;
class SurfaceBase: public DrawableBase, public std::enable_shared_from_this<SurfaceBase>{
public:
    Matrix<double> xdata;
    Matrix<double> ydata;
    Matrix<double> zdata;
    Matrix<double> cdataIndex;
    Colormap cdata;
    Vector<double> vdata;   // V
    std::string faceColor;  // "none" | "flat"
    std::string edgeColor;  // "none" | "flat"
    std::string lineStyle;  // "-" | "--" | ":" | "-." | "none"
    float lineWidth;
    int numContour;
    AxesType axesType;

    // -*-
    SurfaceBase(const Axes axes)
    : DrawableBase(axes)
    , vdata()
    , faceColor("flat")
    , edgeColor("b")
    , lineStyle("-")
    , lineWidth(0.5f)
    , numContour(10)
    , axesType(AxesType::Axes2D)
    {}

    // -
    void clear();
    void draw();

    Surface shading(std::string arg);
    // - Surface & contour
    Surface surface(const Matrix<double>& zmat);
    Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat);
    Surface surface(const Matrix<double>& zmat, const Colormap& cten);
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Colormap& cten
    );

    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Matrix<float>& cmat
    );
    Surface surface(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& zmat,
        const Colormap& cten
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Colormap& cten);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Colormap& cten
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Colormap& cten
    );

    // -*-
    Surface contour(const Matrix<double>& zmat);
    Surface contour(const Matrix<double>& zmat, unsigned int n);
    Surface contour(const Matrix<double>& zmat, const Vector<double>& values);
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        unsigned int n
    );
    Surface contour(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Vector<double>& values
    );

    Surface mesh(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );
    Surface surf(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat
    );

    Surface set(std::string key, std::string val);
    Surface set(std::string key, float val);
    void config();

private:
    void draw2d();
    void draw3d();
    void contourc(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& zmat,
        const Vector<double>& values,
        Matrix<double>& cmat
    );
    void draw_contour();

    std::mutex m_data_mtx;

    // -*-
    struct Contour{
        double x;
        double y;
        int xi;
        int yi;
        int xy;
        int done;
    };
};

// -*----------------------------------------------------------------*-
// -*- ::text                                                       -*-
// -*----------------------------------------------------------------*-
class TextBase: public DrawableBase, public std::enable_shared_from_this<TextBase>{
public:
    std::string message; // String
    Position<float> position;
    void draw();

    TextBase(const Axes axes): DrawableBase(axes){}
    Text text(double x, double y, const std::string message);
    void clear();
    void config();
};

// -*----------------------------------------------------------------*-
// -*- ::glut                                                       -*-
// -*----------------------------------------------------------------*-
namespace glut{
    void initilalize(int &argc, char **argv);
    void register_figure(const Figure fig);
    void set_window_title(int window, std::string name);
}

// -*----------------------------------------------------------------*-
// -*- SWZPLOT PUBLIC API                                           -*-
// -*----------------------------------------------------------------*-
extern Figure currentFigure;
Figure figure();
Figure figure(int id);
Figure figure(std::string name);

Figure gcf(){
    return currentFigure ? currentFigure : figure(0);
}

Axes subplot(unsigned int m, unsigned int n, unsigned int k){
    return gcf()->get_current_canvas()->subplot(m, n, k);
}

//! @todo: legend(...)
Canvas canvas(std::string name="plot", bool visible=true){
    return gcf()->canvas(name, visible);
}

Axes gca(){ return gcf()->get_current_canvas()->gca(); }

// -*-
template<typename T>
void set(const std::string key){ gca()->gco<T>()->set(key); }

// -*-
void set(const std::string key){ gca()->gco<LineBase>()->set(key); }

// -*-
template<typename T>
void set(float key){ gca()->gco<T>()->set(key); }

// -*-
void set(float key){ gca()->gco<LineBase>()->set(key); }

// -*-
template<typename T>
void set(std::string key, std::string val){
    gca()->gco<T>()->set(key, val);
}

// -*-
void set(std::string key, std::string val){
    gca()->gco<LineBase>()->set(key, val);
}

// -*-
template<typename T>
void set(std::string key, float val){
    gca()->gco<T>()->set(key, val);
}

// -*-
void set(std::string key, float val){
    gca()->gco<LineBase>()->set(key, val);
}

// - set axis limit (2D & 3D)
void axis(double xmin, double xmax, double ymin, double ymax){
    gca()->axis(xmin, xmax, ymin, ymax);
}

void axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax){
    gca()->axis(xmin, xmax, ymin, ymax, zmin, zmax);
}

// - toggle axis visibility: "on" | "off" | true | false
void axis(std::string onoff){ gca()->axis(onoff); }

void axis(bool onoff){gca()->axis(onoff); }

// - toggle grid visibility: "on" | "off" | true | false
void grid(std::string onoff){ gca()->grid(onoff); }

void grid(bool onoff){ gca()->grid(onoff); }
// - toggle ticklabel visibility: true | false
void ticklabel(bool onoff){ gca()->ticklabel(onoff); }
// - title
void title(std::string label){ gca()->title(label); }
// - [x|y|z]label
void xlabel(std::string label){ gca()->xlabel(label); }

void ylabel(std::string label){ gca()->ylabel(label); }

//! @todo: Axes zlabel(std::string label);
// - Capture mouse events
void capture_mouse(bool flag){ gca()->capture_mouse(flag); }

// - draw vertex
void vertex(double x, double y){ gca()->gco<LineBase>()->vertex(x, y); }

void vertex(double x, double y, double z){
    gca()->gco<LineBase>()->vertex(x, y, z);
}

// - plot data
Line plot(const Vector<double>& yvec){
    return gca()->add<LineBase>()->plot(yvec);
}

Line plot(const Vector<double>& xvec, const Vector<double>& yvec){
    return gca()->add<LineBase>()->plot(xvec, yvec);
}

Line plot(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& zvec
){
    return gca()->add<LineBase>()->plot(xvec, yvec, zvec);
}

// - log-scale plot
Line semilogx(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->semilogx(xvec, yvec);
}

Line semilogy(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->semilogy(xvec, yvec);
}

Line loglog(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->loglog(xvec, yvec);
}

// - vertex
void vertex(double x, double y, double dx, double dy){
    gca()->gco<LineBase>()->vertex(x, y, dx, dy);
}

void errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec
){
    gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec);
}

void errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec, const Vector<double>& dyvec
){
    gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec, dyvec);
}

// ----
// - Surface & contour
Surface surface(const Matrix<double>& zmat){
    return gca()->add<SurfaceBase>()->surface(zmat);
}

// -*-
Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat){
    return gca()->add<SurfaceBase>()->surface(zmat, cmat);
}

// -*-
Surface surface(const Matrix<double>& zmat, const Colormap& cdata){
    return gca()->add<SurfaceBase>()->surface(zmat, cdata);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Matrix<float>& cmat
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cmat);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cdata);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cmat
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cmat);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cdata);
}

Surface pcolor(const Matrix<double>& cmat){
    return gca()->add<SurfaceBase>()->pcolor(cmat);
}

Surface pcolor(const Colormap& cdata){
    return gca()->add<SurfaceBase>()->pcolor(cdata);
}

Surface pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& cvec
){
    return gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cvec);
}

Surface pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cdata);
}

Surface pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& cmat
){
    return gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cmat);
}

Surface pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cdata);
}

// -*-
Surface contour(const Matrix<double>& zmat){
    return gca()->add<SurfaceBase>()->contour(zmat);
}

Surface contour(const Matrix<double>& zmat, unsigned int n){
    return gca()->add<SurfaceBase>()->contour(zmat, n);
}

Surface contour(const Matrix<double>& zmat, const Vector<double>& values){
    return gca()->add<SurfaceBase>()->contour(zmat, values);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, unsigned int n
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, n);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, values);
}

Surface mesh(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->mesh(xvec, yvec, zmat);
}

Surface surf(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surf(xvec, yvec, zmat);
}

// - shading:
void shading(std::string arg){
    gca()->gco<SurfaceBase>()->shading(arg);
}

// -
Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    return gca()->add<PatchBase>()->patch(xmat, ymat);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, cvec);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Colormap& cdata
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, cdata);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat, zmat);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cdata);
}

Patch bar(const Vector<double>& ydata){
    return gca()->add<PatchBase>()->bar(ydata);
}

Patch bar(const Vector<double>& ydata, float width){
    return gca()->add<PatchBase>()->bar(ydata, width);
}

Patch bar(const Vector<double>& xdata, const Vector<double>& ydata){
    return gca()->add<PatchBase>()->patch(xdata, ydata);
}

Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

//! @todo: add font information
Text text(double x, double y, const std::string message);

Axes colorbar();
void grey();
void jet();
void hsv();
void cool();
void spring();
void summer();
void autumn();
void winter();

void print(std::string filename="out.eps");
void savefig(std::string filename="out.eps");

void append(Line line, std::pair<double, double> point2d);
void append(Axes axes, std::pair<double, double> point2d);
void append(Canvas canvas, std::pair<double, double> point2d);
void append(Figure fig, std::pair<double, double> point2d);
void append(Line line, double x, double y);
void append(Axes axes, double x, double y);
void append(Canvas canvas, double x, double y);
void append(Figure fig, double x, double y);
void append(Line line, Position<double> point2d);
void append(Axes axes, Position<double> point2d);
void append(Canvas canvas, Position<double> point2d);
void append(Figure fig, Position<double> point2d);

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-

#endif