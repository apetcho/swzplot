#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
AxesBase::AxesBase(Canvas canvas): m_canvas(canvas){
    this->m_cta0 = 30;
    this->m_phi0 = 30;
    this->m_cta = this->m_cta0;
    this->m_phi = this->m_phi0;
    this->m_mouse = false;
    this->mouse_callback = nullptr;
    double vmin = std::numeric_limits<double>::max();
    double vmax = -std::numeric_limits<double>::max();
    this->m_xdatalim = DataLim(vmin, vmax);
    this->m_ydatalim = DataLim(vmin, vmax);
    this->m_zdatalim = DataLim(vmin, vmax);
    this->m_axType = AxesType::Axes2D;
    this->m_boxed = true;
    this->m_gridLineStyle = ":";
    this->m_linewidth = 1;
    this->m_tickDir = "in";
    this->m_visible = true;
    this->m_xgrid = false;
    this->m_ygrid = false;
    this->m_selected = false;
    this->m_xAxisLocation = "bottom";
    this->m_yAxisLocation = "left";
    this->m_xlimMode = AxesLimMode::Auto;
    this->m_ylimMode = AxesLimMode::Auto;
    this->m_zlimMode = AxesLimMode::Auto;
    this->m_xscale = Scale::Linear;
    this->m_yscale = Scale::Linear;
    this->m_zscale = Scale::Linear;
    this->m_ticklabelFlag = true;
    this->m_axBBox = BBox(0.13f, 0.11f, 0.775f, 0.815f);
    this->m_viewBBox = BBox(0.0f, 0.0f, 1.0f, 1.0f);
    this->m_cameraPosition = Position(1.f, 1.f, 1.f);
    this->m_cameraTarget = Position(0.f, 0.f, 0.f);
    this->m_cameraUpVector = Position(0.f, 0.f, 1.f);
    this->m_xlim = DataLim(0., 10.);
    this->m_ylim = DataLim(0., 10.);
    this->m_zlim = DataLim(0., 10.);
    this->m_clim = DataLim(0., 0.);
    this->jet();
}

// -*-
int AxesBase::window_height(){
    return this->m_canvas->m_figure->m_window_height;
}

// -*-
int AxesBase::window_width(){
    return this->m_canvas->m_figure->m_window_width;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-