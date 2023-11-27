#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
LineBase::LineBase(const Axes axes)
: DrawableBase(axes)
, m_errorbarFlag(false)
, m_maxCapacity(std::numeric_limits<int>::max())
, m_stopAtMax(false)
, m_xdata{}
, m_ydata{}
, m_zdata{}
, m_color("b")
, m_lineStyle("-")
, m_lineWidth(0.5)
, m_marker("none")
, m_markerSize(6)
, m_markerEdgeColor("k")
, m_markerFaceColor("w")
, m_visible(true)
, m_rgbcolor(Color("blue"))
{}

// -*-
Figure LineBase::gcf(){
    return this->m_ca->canvas()->gcf();
}

// -*-
void LineBase::clear(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    this->m_xerrdata.clear();
    this->m_yerrdata.clear();
}

// -*-
void LineBase::color(float r, float g, float b){
    this->m_color = "";
    this->m_rgbcolor = Color(r, g, b);
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-