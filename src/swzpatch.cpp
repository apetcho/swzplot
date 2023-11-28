#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namesapce::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
void PatchBase::draw(){
    if(this->m_axType==AxesType::Axes2D){ this->draw2d(); }
    if(this->m_axType==AxesType::Axes3D){ this->draw3d(); }
}

// -*-
void PatchBase::clear(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    this->m_faces.clear();
    this->m_vertices.clear();
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-