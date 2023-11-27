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
{}

// -*-
Figure LineBase::gcf(){
    return this->m_ca->canvas()->gcf();
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-