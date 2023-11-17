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
template<typename T>
using Tensor = Matrix<std::vector<T>>;

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
private:
    std::mutex m_childer_mtx;
    Canvas m_canvas;
    Axes m_colorbar_ax;

public:
    AxesBase(Canvas layer);
    ~AxesBase(){};
    // Canvas gcl(){ return this->m_canvas; }
    Canvas canvas(){ return this->m_canvas; }

    // -*- START OF PROTECTED API -*-
    // -*- 
    float m_cta0;
    float m_phi0;
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
    float m_ctaButtonDown;
    float m_phiButtonDown;
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

    Vector<double> make_tick(double minval, double maxval);

    AxesType m_axType;

    // -*- styles -
    bool m_boxed; // Axes on/off :: boxFlag
    float m_linewidth;
    std::string m_tickDir; // "in" | "out"
    bool m_is_visible;
    bool m_xgrid; // xgridFlag
    bool m_ygrid; // ygridFlag
    bool m_zgrid; // zgridFlag

    // General information
    DrawableList m_children;  // container for children drawings
    Drawable m_co;            // currently active drawing object
    bool m_selected;          // Axes have been clicked by the user
    BBox<float> m_axBBox;     // ::position
    BBox<float> m_viewBBox;   // ::viewport3d

    // -*- Scale and axes
    std::string m_xAxisLocation;  // "top" | "bottom"
    std::string m_yAxisLocation;  // "left"| "right" 

    DataLim<double> m_xlim;       // x-axis limit
    DataLim<double> m_ylim;       // y-axis limit
    DataLim<double> m_zlim;       // z-axis limit
    AxesLimMode m_xlimMode;
    AxesLimMode m_ylimMode;
    AxesLimMode m_zlimMode;
    Scale m_xscale;
    Scale m_yscale;
    Scale m_zscale;
    Vector<double> m_xticks;
    Vector<double> m_yticks;
    Vector<double> m_zticks;
    bool m_ticklabel;         // ticklabelFlag

    // -
    Position<float> m_cameraPosition;
    Position<float> m_cameraTarget;
    Position<float> m_cameraUpVector;

    // -*- Label
    std::string m_title;
    std::string m_xlabel;
    std::string m_ylabel;
    std::string m_zlabl;
    DataLim<double> m_clim;

    void draw();

    // -*- END OF PROTECTED API -*-

    // -*------------------------------*-
    // -*- Interface (i.e public API) -*-
    // -*------------------------------*-
    // - set axis limit (2D & 3D)
    Axes set_axis(double xmin, double xmax, double ymin, double ymax);
    Axes set_axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    // - toggle axis visibility: "on" | "off" | true | false
    Axes set_axis(std::string onoff);
    Axes set_axis(bool onoff);
    // - toggle grid visibility: "on" | "off" | true | false
    Axes set_grid(std::string onoff);
    Axes set_grid(bool onoff);
    // - toggle ticklabel visibility: true | false
    Axes set_ticklabel(bool onoff);
    // - title
    Axes set_title(std::string label);
    // - [x|y|z]label
    Axes set_xlabel(std::string label);
    Axes set_ylabel(std::string label);
    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    Axes set_capture_mouse(bool flag);

    //! @todo: implement this here
    template<typename T>
    std::shared_ptr<T> add();

    //! @todo: implement this here
    template<typename T>
    std::shared_ptr<T> gco();

    //! @note: clear the figure|axes
    //! @todo: implement this here
    Axes clear();
    // - set color of axes
    void set_color(float r, float g, float b);
    //! @note: It will better our utility classes here for colormap operations
    // - set the colormap of the children's axes
    Vector<float> set_colormap(std::string color, float target);
    void set_colormap(std::string color);
    void set_colormap(const Matrix<float>& colors);

    // - set colormaps
    void grey(){ this->set_colormap("gray"); }
    void jet(){ this->set_colormap("jet"); }
    void hsv(){ this->set_colormap("hsv"); }
    void cool(){ this->set_colormap("cool"); }
    void spring(){ this->set_colormap("spring"); }
    void summer(){ this->set_colormap("summer"); }
    void autumn(){ this->set_colormap("autumn"); }
    void winter(){ this->set_colormap("winter"); }

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
    Surface surface(const Matrix<double>& zmat, const Tensor<float>& cten);
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
        const Tensor<float>& cten
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
        const Tensor<float>& cten
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Tensor<float>& cten);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Tensor<float>& cten
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Tensor<float>& cten
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
    void set_shading(std::string arg);

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
        const Tensor<float>& cten
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
        const Tensor<double>& cten
    );

    Patch bar(const Vector<double>& ydata);
    Patch bar(const Vector<double>& ydata, float width);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

    //! @todo: add font information
    Text text(double x, double y, const std::string message);

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

// public:
    // - get shared_pointer sharing the current instance of this AxesBase object
    Axes share(){
        return shared_from_this();
    }
};



// -*----------------------------------------------------------------*-
// -*- ::layer                                                      -*-
// -*----------------------------------------------------------------*-
class CanvasBase: public std::enable_shared_from_this<CanvasBase>{
private:
    Axes m_ax;
    Axes m_selected_axes;
    bool m_visible;
    Position<float> m_xyButtonDown;

public:
    // -
    std::chrono::steady_clock::time_point timeCliked;
    std::string name;       // or simply ::name
    Figure figure;          // 
    AxesDict axesDict;      // axes

    // -
    CanvasBase(const Figure fig, const std::string& name, bool visible)
    : m_visible{visible}, name{name}, figure{fig}
    {}

    // -
    void draw();
    Axes subplot(unsigned int m, unsigned int n, unsigned p);
    Axes gca();
    Figure gcf();
    Canvas clear();
    void toggle_visibility();
    void set_visibility(bool flag);
    bool is_visible(){ return this->m_visible; }
    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);

    // -*----------------------------*-
    // -*- Interface i.e public API -*-
    // -*----------------------------*-
    template<typename T>
    void set(const std::string key);
    void set(const std::string key);
    template<typename T>
    void set(float key);
    void set(float key);
    template<typename T>
    void set(std::string key, std::string val);
    void set(std::string key, std::string val);
    template<typename T>
    void set(std::string key, float val);
    void set(std::string key, float val);

    // -*-
    // - set axis limit (2D & 3D)
    void set_axis(double xmin, double xmax, double ymin, double ymax);
    void set_axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    // - toggle axis visibility: "on" | "off" | true | false
    void set_axis(std::string onoff);
    void set_axis(bool onoff);
    // - toggle grid visibility: "on" | "off" | true | false
    void set_grid(std::string onoff);
    void set_grid(bool onoff);
    // - toggle ticklabel visibility: true | false
    void set_ticklabel(bool onoff);
    // - title
    void set_title(std::string label);
    // - [x|y|z]label
    void set_xlabel(std::string label);
    void set_ylabel(std::string label);
    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    void capture_mouse(bool flag);

    Axes colorbar();
    void grey();
    void jet();
    void hsv();
    void cool();
    void spring();
    void summer();
    void autumn();
    void winter();

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
    Surface surface(const Matrix<double>& zmat, const Tensor<float>& cten);
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
        const Tensor<float>& cten
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
        const Tensor<float>& cten
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Tensor<float>& cten);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Tensor<float>& cten
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Tensor<float>& cten
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
    void set_shading(std::string arg);

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
        const Tensor<float>& cten
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
        const Tensor<double>& cten
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
    int window_width;
    int window_height;
    Position<int> xyPassive;
    std::string window_name;    // figname
    int window_num;
    BBox<int> windowBBox;   // position[4]
    Canvas current_canvas;  // cl
    Canvas selected_canvas;

    FigureBase(std::string name="plot", bool visible=true);
    ~FigureBase() = default;

    // -
    void set_window_name(); // set_figure_name()
    // -
    Canvas canvas(std::string name="default", bool visible=true);
    Canvas get_current_canvas();
    Figure clear();
    void draw();
    void draw_all(); // draw_layer_list

    // - callback
    void reshape(int width, int height);
    void mouse(int button, int state, int x, int y);
    void passivemotion(int x, int y);
    void keyboard(char key, int x, int y);

    Figure gcf(){ return this->share(); }
    Axes gca();
    Axes subplot(unsigned int m, unsigned int n, unsigned int p);

    // -*----------------------------*-
    // -*- Interface i.e public API -*-
    // -*----------------------------*-
    template<typename T>
    void set(const std::string key);
    void set(const std::string key);
    template<typename T>
    void set(float key);
    void set(float key);
    template<typename T>
    void set(std::string key, std::string val);
    void set(std::string key, std::string val);
    template<typename T>
    void set(std::string key, float val);
    void set(std::string key, float val);

    // -*-
    // - set axis limit (2D & 3D)
    void set_axis(double xmin, double xmax, double ymin, double ymax);
    void set_axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    // - toggle axis visibility: "on" | "off" | true | false
    void set_axis(std::string onoff);
    void set_axis(bool onoff);
    // - toggle grid visibility: "on" | "off" | true | false
    void set_grid(std::string onoff);
    void set_grid(bool onoff);
    // - toggle ticklabel visibility: true | false
    void set_ticklabel(bool onoff);
    // - title
    void set_title(std::string label);
    // - [x|y|z]label
    void set_xlabel(std::string label);
    void set_ylabel(std::string label);
    //! @todo: Axes zlabel(std::string label);
    // - Capture mouse events
    void capture_mouse(bool flag);

    Axes colorbar();
    void grey();
    void jet();
    void hsv();
    void cool();
    void spring();
    void summer();
    void autumn();
    void winter();

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
    Surface surface(const Matrix<double>& zmat, const Tensor<float>& cten);
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
        const Tensor<float>& cten
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
        const Tensor<float>& cten
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Tensor<float>& cten);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Tensor<float>& cten
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Tensor<float>& cten
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
    void set_shading(std::string arg);

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
        const Tensor<float>& cten
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
        const Tensor<double>& cten
    );

    Patch bar(const Vector<double>& ydata);
    Patch bar(const Vector<double>& ydata, float width);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata);
    Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width);

    //! @todo: add font information
    Text text(double x, double y, const std::string message);

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
    bool errorbarFlag;

    void clear();
    void set_color(float r, float g, float b);

    // -*-
    unsigned int maxCapacity;
    bool stopAtMax;
    Vector<double> xdata;
    Vector<double> ydata;
    Vector<double> zdata;
    std::string color;
    std::string lineStyle;  // "-" | "--" | ":" | "-." "none"
    float lineWidth;
    // marker: "none" | "." | "+" | "x" | "d" | "^" | "v" | "o" | "*" | "s"
    std::string marker;
    float markerSize;
    std::string markerEdgeColor;
    std::string markerFaceColor;
    bool visible;

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
    Tensor<float> cdata;

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
        const Tensor<float>& cten
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
        const Tensor<double>& cten
    );

    Patch set(std::string key, std::string val);
    Patch set(std::string key, float val);

    Tensor<float> index_to_trucolor(const Vector<double> indexcolor);
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
    Tensor<float> cdata;
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
    Surface surface(const Matrix<double>& zmat, const Tensor<float>& cten);
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
        const Tensor<float>& cten
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
        const Tensor<float>& cten
    );

    Surface pcolor(const Matrix<double>& cmat);
    Surface pcolor(const Tensor<float>& cten);
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Matrix<double>& cvec
    );
    Surface pcolor(
        const Vector<double>& xvec,
        const Vector<double>& yvec,
        const Tensor<float>& cten
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Matrix<double>& cmat
    );
    Surface pcolor(
        const Matrix<double>& xmat,
        const Matrix<double>& ymat,
        const Tensor<float>& cten
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


// -*----------------------------------------------------------------*-
// -*- ::glut                                                       -*-
// -*----------------------------------------------------------------*-


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-

#endif