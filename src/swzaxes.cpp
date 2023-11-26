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
    double vmax = -vmin;
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

// -*-
void AxesBase::reset_limits(){
    double vmin = std::numeric_limits<double>::max();
    double vmax = -vmin;
    this->m_xdatalim = DataLim(vmin, vmax);
    this->m_ydatalim = DataLim(vmin, vmax);
    this->m_zdatalim = DataLim(vmin, vmax);
}

// -*-
void AxesBase::config(){
    this->reset_limits();
    for(auto iter=this->m_children.begin(); iter != this->m_children.end(); ++iter){
        (*iter)->config();
    }
    float extent = 0.0f;
    float extentLinear = 0.03f;
    bool test = (
        (this->m_xlimMode==AxesLimMode::Auto) &&
        (this->m_xdatalim.maxval > this->m_xdatalim.minval)
    );
    if(test){
        extent = this->m_xscale==Scale::Linear ? extentLinear : 0.f;
        double vmin = this->m_xdatalim.minval - extent*(
            this->m_xdatalim.maxval - this->m_xdatalim.minval
        );
        double vmax = this->m_xdatalim.maxval + extent*(
            this->m_xdatalim.maxval - this->m_xdatalim.minval
        );
        this->m_xlim = DataLim(vmin, vmax);
    }
    test = (
        (this->m_ylimMode==AxesLimMode::Auto) &&
        (this->m_ydatalim.maxval > this->m_ydatalim.minval)
    );
    if(test){
        extent = this->m_yscale == Scale::Linear ? extentLinear : 0.f;
        double vmin = (
            this->m_ydatalim.minval - extent*(
                this->m_ydatalim.maxval - this->m_ydatalim.minval
            )
        );
        double vmax = (
            this->m_ydatalim.maxval + extent*(
                this->m_ydatalim.maxval - this->m_ydatalim.minval
            )
        );
        this->m_ylim = DataLim(vmin, vmax);
    }
    test = (
        (this->m_zlimMode==AxesLimMode::Auto) &&
        (this->m_zdatalim.maxval > this->m_zdatalim.minval)
    );
    if(test){
        extent = (this->m_zscale==Scale::Linear) ? extentLinear : 0.f;
        double vmin = (
            this->m_zdatalim.minval - extent * (
                this->m_zdatalim.maxval - this->m_zdatalim.minval
            )
        );
        double vmax = (
            this->m_zdatalim.maxval + extent*(
                this->m_zdatalim.maxval - this->m_zdatalim.minval
            )
        );
        this->m_zlim = DataLim(vmin, vmax);
    }

    double vmin = this->m_xdatalim.minval;
    double vmax = this->m_xdatalim.maxval;
    this->m_xticks = this->make_ticks(vmin, vmax);

    vmin = this->m_ydatalim.minval;
    vmax = this->m_ydatalim.maxval;
    this->m_yticks = this->make_ticks(vmin, vmax);

    vmin = this->m_zdatalim.minval;
    vmax = this->m_zdatalim.maxval;
    this->m_zticks = this->make_ticks(vmin, vmax);
}

// -*-
bool AxesBase::mouse(int button, int state, int x, int y){
    if(!this->m_mouse){ return false;}
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        this->m_xybutton = Position(x, y);
        if(this->m_axType==AxesType::Axes3D){
            this->m_ctaButtonDown = this->m_cta;
            this->m_phiButtonDown = this->m_phi;
        }
        this->m_xymouse = Position(static_cast<double>(x), static_cast<double>(y));
        return true;
    }
    if(this->mouse_callback!=nullptr){
        this->mouse_callback(button, state, x, y);
    }
    return false;
}

// -*-
bool AxesBase::motion(int x, int y){
    if(this->m_axType==AxesType::Axes3D){
        auto xbtn = this->m_xybutton.x;
        auto ybtn = this->m_xybutton.y;
        this->m_cta = this->m_ctaButtonDown - static_cast<float>(x - xbtn);
        this->m_phi = this->m_phiButtonDown + static_cast<float>(y - ybtn);
        auto myclamp = [](float val, float min, float max) -> float {
            if(val >= max){ val = max; }
            if(val <= min){ val = min; }
            return val;
        };
        this->m_phi = myclamp(this->m_phi, -90.f, 90.f);
        this->m_cta = myclamp(this->m_cta, 0.0f, 360.f);

        return true;
    }
    return false;
}

// -*-
Vector<double> AxesBase::make_ticks(double vmin, double vmax){
    double x = std::abs(vmax-vmin);
    auto z = static_cast<int>(std::log10(x));
    double y = x / std::pow(10.0, static_cast<double>(z));
    double dg = std::pow(10.0, static_cast<double>(z));
    if(y < 2.0){ dg *= 0.2; }
    else if( y < 5.){ dg *= 0.5; }

    double vmin0 = vmin - std::fmod(vmin, dg);

    int j = 0;
    int i;
    Vector<double> result;
    if(vmax > vmin){
        i = -2;
        while(vmax >= (vmin0 + dg*i)){
            if(vmin <= vmin0 + dg*i){
                result.push_back((vmin0 + dg*i));
                ++j;
            }
            ++i;
        }
    }
    if(vmax < vmin){
        i = -2;
        while(vmax <= vmin0 - dg*i){
            if(vmin >= vmin0 - dg*i){
                result.push_back(vmin0 - dg*i);
                ++j;
            }
            ++i;
        }
    }

    return result;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-