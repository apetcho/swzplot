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

// -*-
void SurfaceBase::clear(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    this->m_cdataIndex.clear();
    this->m_cdata.clear();
    this->m_vdata.clear();
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-