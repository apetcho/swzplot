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

// -*-
void AxesBase::draw(){
    std::unique_lock<std::mutex> lock(this->m_children_mtx);
    if(this->m_children.size() > 0){
        if(this->m_axType==AxesType::Axes2D){ this->draw2d(); }
        else if(this->m_axType==AxesType::Axes3D){ this->draw3d(); }
    }
    if(this->m_axType==AxesType::ColorBar){ this->draw_colorbar(); }
    this->config();

    for(auto iter=this->m_children.begin(); iter != this->m_children.end(); ++iter){
        (*iter)->draw();   
    }

    if(this->m_colorbar_ax){ this->m_colorbar_ax->draw(); }
}

// -*-
void AxesBase::draw2d(){
    float r = 0.01f;
    auto left = this->m_axBBox.left;
    auto bottom = this->m_axBBox.bottom;
    auto width = this->m_axBBox.width;
    auto height = this->m_axBBox.height;

    auto w = static_cast<int>(this->window_width());
    auto h = static_cast<int>(this->window_height());
    glViewport(0, 0, w, h);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glDisable(GL_LINE_STIPPLE);
    gl2psDisable(GL2PS_LINE_STIPPLE);

    int _charw = 6;
    int _charh = 12;
    float _offset = 0.01f;
    int _numchar = 4;

    int _tickdir = 1; // 1 => "in" ; -1 => "out"

    if(this->m_boxed){
        glLineWidth(this->m_linewidth);
        gl2psLineWidth(this->m_linewidth);
        if(this->m_selected){
            glLineWidth(2*this->m_linewidth);
            gl2psLineWidth(this->m_linewidth);
        }
        glColor3f(0.f, 0.f, 0.f);
        glBegin(GL_LINE_LOOP);
            glVertex2d(left, bottom);
            glVertex2d(left+width, bottom);
            glVertex2d(left+width, bottom+height);
            glVertex2d(left, bottom+height);
        glEnd();

        // mouse capture
        if(this->m_selected){
            std::ostringstream stream;
            stream << "Mouse: (" << this->m_xymouse.x;
            stream << ", " << this->m_xymouse.y << ")";
            this->ptext(left, bottom+height+r, stream.str());
        }

        // grid
        auto selectGridLineStyle = [](std::string key){
            std::map<std::string, int> styles = {
                {"-", 1}, {"- -", 2}, {"--", 2}, {":", 3}, {"-.", 4}
            };
            auto iter = styles.find(key);
            int rv;
            if(iter != styles.end()){
                rv = iter->second;
            }else{
                rv = 3;
            }
            return rv;
        };
        int gridlinestyle = selectGridLineStyle(this->m_gridLineStyle);
        if(this->m_xgrid){
            glLineWidth(this->m_linewidth);
            gl2psLineWidth(this->m_linewidth);
            for(size_t i=0; i < this->m_xticks.size(); ++i){
                if(gridlinestyle==1){   // -
                    glDisable(GL_LINE_STIPPLE);
                    gl2psDisable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==2){ // - -
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xf0f0);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==3){ // :
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xcccc);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==4){ 
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0x087f);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }
                glBegin(GL_LINE_STRIP);
                    glVertex2d(this->coord2D_to_xaxis(this->m_xticks[i]), bottom);
                    glVertex2d(this->coord2D_to_xaxis(this->m_xticks[i]), bottom+height);
                glEnd();
            }
        }
        if(this->m_ygrid){
            for(size_t i=0; i < this->m_xticks.size(); ++i){
                if(gridlinestyle==1){   // -
                    glDisable(GL_LINE_STIPPLE);
                    gl2psDisable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==2){ // - -
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xf0f0);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==3){ // :
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xcccc);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }else if(gridlinestyle==4){ 
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0x087f);
                    gl2psEnable(GL2PS_LINE_STIPPLE);
                }
                glBegin(GL_LINE_STRIP);
                    glVertex2d(left, this->coord2D_to_xaxis(this->m_yticks[i]));
                    glVertex2d(left+width, this->coord2D_to_xaxis(this->m_yticks[i]));
                glEnd();
            }
        }

        // ticks
        if(this->m_tickDir=="in"){ _tickdir = 1; }
        if(this->m_tickDir=="out"){ _tickdir = -1; }

        glDisable(GL_LINE_STIPPLE);
        gl2psDisable(GL2PS_LINE_STIPPLE);
        // x-ticks
        for(size_t i=0; i < this->m_xticks.size(); ++i){
            glBegin(GL_LINE_STRIP);
                glVertex2d(this->coord2D_to_xaxis(this->m_xticks[i]), bottom);
                glVertex2d(this->coord2D_to_xaxis(this->m_xticks[i]), bottom+_tickdir*0.01);
            glEnd();
        }
        // x-tick labal
        if(this->m_ticklabelFlag){
            std::ostringstream stream;
            float x, y;
            for(size_t i=0; i < this->m_xticks.size(); ++i){
                stream << std::setw(4) << std::setprecision(1);
                stream << this->m_xticks[i];
                x = (
                    this->coord2D_to_xaxis(this->m_xticks[i]) -
                    static_cast<float>(_numchar*_charw)/this->window_width()/2.0f
                );
                y = bottom - _offset - 1.0f*_charh/this->window_height();
                this->ptext(x, y, stream.str());
                stream.clear();
            }
        }
        // y-tick
        for(size_t i=0; i < this->m_yticks.size(); ++i){
            glBegin(GL_LINE_STRIP);
                glVertex2d(left, this->coord2D_to_yaxis(this->m_yticks[i]));
                glVertex2d(left+_tickdir*0.01, this->coord2D_to_yaxis(this->m_yticks[i]));
            glEnd();
        }
        // y-ticklabel
        if(this->m_ticklabelFlag){
            std::ostringstream stream;
            float x, y;
            for(size_t i=0; i < this->m_yticks.size(); ++i){
                stream << std::setw(4) << std::setprecision(1);
                stream << this->m_yticks[i];
                x = left - static_cast<float>(_numchar*_charw)/this->window_width() - _offset;
                y = this->coord2D_to_yaxis(this->m_yticks[i]) - 0.5*_charh/this->window_height();
                this->ptext(x, y, stream.str());
                stream.clear();
            }
        }
    }

    // - title
    _numchar = this->m_title.length();
    float x = left - width/2.0f - static_cast<float>(_numchar*_charw)/this->window_width()/2.0f;
    float y = bottom + height + _offset;
    this->ptext(x, y, this->m_title);

    // - xlabel
    _numchar = this->m_xlabel.length();
    x = left + width/2.0f-static_cast<float>(_numchar*_charw)/this->window_width()/2.0f;
    y = bottom - _offset + 2.0*_charh/this->window_height();
    this->ptext(x, y, this->m_xlabel);

    // - ylabel
    //_numchar = this->m_ylabel.length();
    x = left;
    y = bottom + height + _offset;
    this->ptext(x, y, this->m_ylabel);

    // - viewport
    int _left = static_cast<int>(this->m_axBBox.left * this->window_width());
    int _bottom = static_cast<int>(this->m_axBBox.bottom*this->window_height());
    int _width = static_cast<int>(this->m_axBBox.width*this->window_width());
    int _height = static_cast<int>(this->m_axBBox.height*this->window_height());
    glViewport(_left, _bottom, _width, _height);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
}

// -*-
void AxesBase::draw3d(){
    auto _left = static_cast<int>(this->m_viewBBox.left*this->window_width());
    auto _bottom = static_cast<int>(this->m_viewBBox.bottom*this->window_height());
    auto _width = static_cast<int>(this->m_viewBBox.width*this->window_width());
    auto _height = static_cast<int>(this->m_viewBBox.height*this->window_height());
    
    glViewport(_left, _bottom, _width, _height);
    glLoadIdentity();
    glOrtho(-1.8, 1.8, -1.8, 1.8, -1.5, 3.);
    auto _xt = this->m_cameraTarget.x;
    auto _yt = this->m_cameraTarget.y;
    float _zt = 0.0;
    if(this->m_cameraTarget.z.has_value()){
        _zt = this->m_cameraTarget.z.value();
    }
    auto _xu = this->m_cameraUpVector.x;
    auto _yu = this->m_cameraUpVector.y;
    float _zu = 0.0;
    if(this->m_cameraUpVector.z.has_value()){
        _zu = this->m_cameraUpVector.z.value();
    }

    gluLookAt(
        std::cos(this->m_cta*M_PI/180)*std::cos(this->m_phi*M_PI/180),
        std::sin(this->m_cta*M_PI/180)*std::cos(this->m_phi*M_PI/180),
        std::sin(this->m_phi*M_PI/180),
        _xt, _yt, _zt, _xu, _yu, _zu
    );

    if(this->m_boxed){
        // tick
        float r1 = 1.05f;
        float r2 = 1.2f;
        float r3 = 1.4f;
        float cta0 = std::fmod(this->m_cta, 360);
        int xsign = 0, ysign = 0;
        if((0 <= cta0) && (cta0 < 90)){
            xsign = 1;
            ysign = 1;
        }
        if((90<cta0) && (cta0<190)){
            xsign = -1;
            ysign = 1;
        }
        if((180 <= cta0) && (cta0 < 270)){
            xsign = -1;
            ysign = -1;
        }
        if((270 <= cta0) && (cta0 < 360)){
            xsign = 1;
            ysign = -1;
        }

        glColor3f(0.f, 0.f, 0.f);

        // - axes
        // x
        glBegin(GL_LINE_STRIP);
            glVertex3d(-1, ysign, -1);
            glVertex3d(1, ysign, -1);
        glEnd();
        // y
        glBegin(GL_LINE_STRIP);
            glVertex3d(xsign, -1, -1);
            glVertex3d(xsign, 1, -1);
        glEnd();
        // z
        glBegin(GL_LINE_STRIP);
            glVertex3d(ysign, -xsign, -1);
            glVertex3d(ysign, -xsign, 1);
        glEnd();

        // x-ticks
        for(size_t i=0; i < this->m_xticks.size(); ++i){
            double x = this->coord3D_to_xaxis(this->m_xticks[i]);
            glBegin(GL_LINE_STRIP);
                glVertex3d(x, ysign, -1);
                glVertex3d(x, ysign*r1, -1);
            glEnd();
        }
        // y-ticks
        for(size_t i=0; i < this->m_yticks.size(); ++i){
            double y = this->coord3D_to_yaxis(this->m_yticks[i]);
            glBegin(GL_LINE_STRIP);
                glVertex3d(xsign, y, -1);
                glVertex3d(xsign*r1, y, -1);
            glEnd();
        }
        // z-ticks
        for(size_t i=0; i < this->m_zticks.size(); ++i){
            double z = this->coord3D_to_zaxis(this->m_zticks[i]);
            glBegin(GL_LINE_STRIP);
                glVertex3d(ysign, -xsign, z);
                glVertex3d(ysign*r1, -xsign, z);
            glEnd();
        }
        // -ticklabel
        if(this->m_ticklabelFlag){
            // xticklabel
            std::ostringstream stream;
            for(size_t i=0; i < this->m_xticks.size(); ++i){
                stream << std::setw(4) << std::setprecision(1);
                stream << this->m_xticks[i];
                double x = this->coord3D_to_xaxis(this->m_xticks[i]);
                this->ptext3c(x, ysign*r2, -1, stream.str());
                stream.clear();
            }
            // yticklabel
            stream.clear();
            for(size_t i=0; i < this->m_yticks.size(); ++i){
                stream << std::setw(4) << std::setprecision(1);
                stream << this->m_yticks[i];
                double y = this->coord3D_to_yaxis(this->m_yticks[i]);
                this->ptext3c(ysign*r2, y, -1, stream.str());
                stream.clear();
            }
            // zticklabel
            stream.clear();
            for(size_t i=0; i < this->m_zticks.size(); ++i){
                stream << std::setw(4) << std::setprecision(1);
                stream << this->m_zticks[i];
                double z = this->coord3D_to_xaxis(this->m_zticks[i]);
                this->ptext3c(ysign*r2, -xsign, z, stream.str());
                stream.clear();
            }
        }
        // {x|y|z}label
        this->ptext3c(0, ysign*r3, -1, "x");
        this->ptext3c(xsign*r3, 0, -1, "y");
        this->ptext3c(ysign*r3, -xsign, 0, "z");
    }
}

// -*-
Axes AxesBase::colorbar(){
    auto result = std::make_shared<AxesBase>(this->m_canvas);
    this->m_colorbar_ax = result;
    auto left = this->m_axBBox.left;
    auto bottom = this->m_axBBox.bottom;
    auto width = this->m_axBBox.width;
    auto height = this->m_axBBox.height;
    auto zmin = this->m_zlim.minval;
    auto zmax = this->m_zlim.maxval;

    //! @todo: use in 3D
    result->m_cmap = this->m_cmap;
    result->m_axType = AxesType::ColorBar;

    float _left = left + width*1.01;
    float _bottom = bottom;
    float _width = width * 0.05;
    float _height = height;
    result->m_axBBox = BBox(_left, _bottom, _width, _height);
    result->m_zlim = DataLim(zmin, zmax);
    result->m_ylim = DataLim(zmin, zmax);
    return result;
}

// -*-
void AxesBase::draw_colorbar(){
    auto left = this->m_axBBox.left;
    auto bottom = this->m_axBBox.bottom;
    auto width = this->m_axBBox.width;
    auto height = this->m_axBBox.height;

    // -
    auto _w = static_cast<int>(this->window_width());
    auto _h = static_cast<int>(this->window_height());
    glViewport(0, 0, _w, _h);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glDisable(GL_LINE_STIPPLE);
    gl2psDisable(GL2PS_LINE_STIPPLE);

    if(this->m_boxed){
        // - box
        glLineWidth(this->m_linewidth);
        gl2psLineWidth(this->m_linewidth);
        glColor3f(0.f, 0.f, 0.f);
        glBegin(GL_LINE_LOOP);
            glVertex2d(left, bottom);
            glVertex2d(left+width, bottom);
            glVertex2d(left+width, bottom+height);
            glVertex2d(left, bottom+height);
        glEnd();

        // z-tick
        for(size_t i=0; i < this->m_zticks.size(); ++i){
            double z = this->coord2D_to_yaxis(this->m_zticks[i]);
            glBegin(GL_LINE_STRIP);
                glVertex2d(left+width, z);
                glVertex2d(left+width+0.01, z);
            glEnd();
        }

        // z-ticklabels
        std::ostringstream stream;
        for(size_t i=0; i < this->m_zticks.size(); ++i){
            stream << std::setw(4) << std::setprecision(1);
            stream << this->m_zticks[i];
            double z = this->coord2D_to_yaxis(this->m_zticks[i]);
            this->ptext(left+width+0.01, z, stream.str());
            stream.clear();
        }
    }

    // -
    std::vector<float> rgb;
    auto n = this->m_cmap.size();
    for(size_t i=0; i < n; ++i){
        rgb = this->m_cmap[i];
        glColor3f(rgb[0], rgb[1], rgb[1]);
        glBegin(GL_QUADS);
            glVertex2d(left, bottom+height*i/n);
            glVertex2d(left+width, bottom+height*i/n);
            glVertex2d(left+width, bottom+height*(i+1)/n);
            glVertex2d(left, bottom+height*(i+1)/n);
        glEnd();
    }
}

// - coordindate transform
double AxesBase::coord2D_to_xaxis(double x){
    double result = 0.0;
    if(this->m_xscale==Scale::Linear){
        auto left = this->m_axBBox.left;
        auto width = this->m_axBBox.width;
        auto x1 = this->m_xlim.minval;
        auto x2 = this->m_xlim.maxval;
        result = left + width*(x-x1)/(x2-x1);
    }else{
        auto x1 = this->m_xlim.minval;
        auto x2 = this->m_xlim.maxval;
        double t = 0.0;
        try{
            t = (std::log10(x) - std::log10(x1))/(std::log10(x2) - std::log10(x1));
        }catch(...){}
        if(x <= 0){
            t = -1.0;
        }
        result = this->m_axBBox.left + this->m_axBBox.width*t;
    }

    return result;
}

// -*-
double AxesBase::coord2D_to_yaxis(double y){
    double result = 0.0;
    auto y1 = this->m_ylim.minval;
    auto y2 = this->m_ylim.maxval;
    auto bottom = this->m_axBBox.bottom;
    auto height = this->m_axBBox.height;
    if(this->m_yscale==Scale::Linear){    
        result = bottom + height*(y-y1)/(y2-y1);
    }else{
        //! @todo: check arg of log10
        result = bottom + height*(std::log10(y)-std::log10(y1))/(
            std::log10(y2)-std::log10(y1)
        );
    }
    return result;
}

// -*-
double AxesBase::coord3D_to_xaxis(double x){
    auto x1 = this->m_xlim.minval;
    auto x2 = this->m_xlim.maxval;
    return 2*(x-x1)/(x2-x1) - 1.0;
}

// -*-
double AxesBase::coord3D_to_yaxis(double y){
    auto y1 = this->m_ylim.minval;
    auto y2 = this->m_ylim.maxval;
    return 2*(y-y1)/(y2-y1) - 1.0;
}

// -*-
double AxesBase::coord3D_to_zaxis(double z){
    auto z1 = this->m_zlim.minval;
    auto z2 = this->m_zlim.maxval;
    return 2.0*(z + z1)/(z2 - z1) - 1.0;
}

// -*-
Axes AxesBase::axis(double xmin, double xmax, double ymin, double ymax){
    if(xmin != xmax){
        this->m_xlim = DataLim(xmin, xmax);
        this->m_xlimMode = AxesLimMode::Manual;
    }
    if(ymin != ymax){
        this->m_ylim = DataLim(ymin, ymax);
        this->m_ylimMode = AxesLimMode::Manual;
    }

    this->m_axType = AxesType::Axes2D;
    return this->share();
}

// -*-
Axes AxesBase::axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax){
    if(xmin != xmax){
        this->m_xlim = DataLim(xmin, xmax);
        this->m_xlimMode = AxesLimMode::Manual;
    }
    if(ymin != ymax){
        this->m_ylim = DataLim(ymin, ymax);
        this->m_ylimMode = AxesLimMode::Manual;
    }
    if(zmin != zmax){
        this->m_zlim = DataLim(zmin, zmax);
        this->m_zlimMode = AxesLimMode::Manual;
    }
    this->m_axType = AxesType::Axes3D;
    return this->share();
}

// -*-
Axes AxesBase::axis(std::string onoff){
    this->axis(onoff=="on");
    return this->share();
}

// -*-
Axes AxesBase::axis(bool onoff){
    this->m_boxed = onoff;
    return this->share();
}

// -*-
Axes AxesBase::grid(bool onoff){
    this->m_xgrid = onoff;
    this->m_ygrid = onoff;
    this->m_zgrid = onoff;
    return this->share();
}

// -*-
Axes AxesBase::grid(std::string onoff){
    this->grid(onoff=="on");
    return this->share();
}

// -*-
Axes AxesBase::ticklabel(bool onoff){
    this->m_ticklabelFlag = onoff;
    return this->share();
}

// -*-
Axes AxesBase::title(std::string label){
    this->m_title = label;
    return this->share();
}

// -*-
Axes AxesBase::xlabel(std::string label){
    this->m_xlabel = label;
    return this->share();
}

// -*-
Axes AxesBase::ylabel(std::string label){
    this->m_ylabel = label;
    return this->share();
}

// -*-
Axes AxesBase::capture_mouse(bool flag){
    this->m_mouse = flag;
    return this->share();
}

// -*-
void AxesBase::ptext(float x, float y, const std::string& text){
    auto _w = static_cast<int>(this->window_width());
    auto _h = static_cast<int>(this->window_height());
    glViewport(0, 0, _w, _h);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    gl2psText(text.c_str(), "Arial", 12);

    for(size_t i=0; i < text.size(); ++i){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

// -*-
void AxesBase::ptext3(float x, float y, float z, const std::string& text){
    glColor3f(0.f, 0.f, 0.f);
    glRasterPos3d(x, y, z);
    gl2psText(text.c_str(), "Arial", 12);
    for(auto i=0; i < text.size(); ++i){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-