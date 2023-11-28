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


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-