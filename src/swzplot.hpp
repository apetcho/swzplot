#ifndef SWZPLOT_H
#define SWZPLOT_H


#include<type_traits>
#include<functional>
#include<algorithm>
#include<stdexcept>
#include<exception>
#include<valarray>
#include<iostream>
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

//! @note: Array
//! @note: Matrix

// -*----------------------------------------------------------------*-
// -*- ::color                                                      -*-
// -*----------------------------------------------------------------*-

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