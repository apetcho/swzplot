#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
void SurfaceBase::draw(){
    if(this->m_axType==AxesType::ContourPlot){ this->draw_contour(); }
    else if(this->m_axType==AxesType::Axes3D){ this->draw3d(); }
    else{ this->draw2d(); }
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-