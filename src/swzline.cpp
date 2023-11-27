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
    this->m_y1errdata.clear();
    this->m_y2errdata.clear();
}

// -*-
void LineBase::color(float r, float g, float b){
    this->m_color = "";
    this->m_rgbcolor = Color(r, g, b);
}

// -*-
void LineBase::draw(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    if(this->m_xdata.size()==0){ return; }
    // - transformed coordinates
    float _x, _y;
    // - scaling factor
    float _sx = 0.f;
    float _sy = 0.f;
    std::vector<float> rgb = this->m_rgbcolor.to_vector();
    glColor3f(rgb[0], rgb[1], rgb[2]);

    glLineWidth(this->m_lineWidth);
    glPointSize(this->m_lineWidth);
    gl2psLineWidth(this->m_lineWidth);
    gl2psPointSize(this->m_lineWidth);

    // -*- 2D -*-
    if(this->m_ca->m_axType==AxesType::Axes2D){// 2D
        if(this->m_lineStyle!="none"){ // Line
            if(this->m_lineStyle=="-"){
                glDisable(GL_LINE_STIPPLE);
                gl2psDisable(GL2PS_LINE_STIPPLE);
            }else if(this->m_lineStyle=="- -" || this->m_lineStyle=="--"){
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1, 0xf0f0);
                gl2psEnable(GL2PS_LINE_STIPPLE);
            }else if(this->m_lineStyle==":"){
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1, 0xcccc);
                gl2psEnable(GL2PS_LINE_STIPPLE);
            }else if(this->m_lineStyle=="-."){
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1, 0x087f);
                gl2psEnable(GL2PS_LINE_STIPPLE);
            }

            glBegin(GL_LINE_STRIP);
            for( auto i=0; i < this->m_xdata.size(); ++i){
                _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                glVertex2d(_x, _y);
            }
            glEnd();
        }
        if(this->m_marker != "none"){
            _sx = this->m_markerSize/this->gcf()->m_window_width;
            _sy = this->m_markerSize/this->gcf()->m_window_height;

            glDisable(GL_LINE_STIPPLE);
            gl2psDisable(GL2PS_LINE_STIPPLE);
            if(this->m_marker == "."){
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glPointSize(this->m_lineWidth);
                    glBegin(GL_POINTS);
                        glVertex2d(_x, _y);
                    glEnd();
                }
            }else if(this->m_marker=="+"){
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x - _sx, _y);
                        glVertex2d(_x + _sx, _y);
                    glEnd();

                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x, _y - _sy);
                        glVertex2d(_x, _y + _sy);
                    glEnd();
                }
            }else if(this->m_marker == "x"){
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x-_sx, _y-_sy);
                        glVertex2d(_x+_sx, _y+_sy);
                    glEnd();
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x+_sx, _y-_sy);
                        glVertex2d(_x-_sx, _y+_sy);
                    glEnd();
                }
            }else if(this->m_marker=="d"){ // diamond
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_LOOP);
                        glVertex2d(_x, _y+_sy);
                        glVertex2d(_x+_sx, _y);
                        glVertex2d(_x, _y-_sy);
                        glVertex2d(_x-_sx, _y);
                    glEnd();
                }
            }else if(this->m_marker=="^"){ // up-triangle
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_LOOP);
                        glVertex2d(_x, _y + _sy);
                        glVertex2d(_x+_sx, _y-_sy);
                        glVertex2d(_x-_sx, _y-_sy);
                    glEnd();
                }
            }else if(this->m_marker=="v"){ // down-triangle
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_LOOP);
                        glVertex2d(_x, _y-_sy);
                        glVertex2d(_x+_sx, _y+_sy);
                        glVertex2d(_x-_sx, _y+_sy);
                    glEnd();
                }
            }else if(this->m_marker=="o"){ // circle
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_LOOP);
                        for(auto k=0; k < 20; ++k){
                            auto xc = _x +_sx*std::cos(2*M_PI*(k/20.0));
                            auto yc = _y +_sy*std::sin(2*M_PI*(k/20.0));
                            glVertex2d(xc, yc);
                        }
                    glEnd();
                }
            }else if(this->m_marker=="s"){ // square
                for(auto i =0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_LOOP);
                        glVertex2d(_x-_sx, _y-_sy);
                        glVertex2d(_x-_sx, _y+_sy);
                        glVertex2d(_x+_sx, _y+_sy);
                        glVertex2d(_x+_sx, _y-_sy);
                    glEnd();
                }
            }else{ // marker = "*"
                for(auto i=0; i < this->m_xdata.size(); ++i){
                    _x = this->coord2D_to_xaxis(this->m_xdata[i]);
                    _y = this->coord2D_to_yaxis(this->m_ydata[i]);
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x-_sx, _y);
                        glVertex2d(_x+_sx, _y);
                    glEnd();
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x, _y-_sy);
                        glVertex2d(_x, _y+_sy);
                    glEnd();
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x-_sx, _y-_sy);
                        glVertex2d(_x+_sx, _y+_sy);
                    glEnd();
                    glBegin(GL_LINE_STRIP);
                        glVertex2d(_x+_sx, _y-_sy);
                        glVertex2d(_x-_sx, _y+_sy);
                    glEnd();
                }
            }
        }// -*- marker -*-

        if(this->m_errorbarFlag){
            float _y1, _y2;
            glDisable(GL_LINE_STIPPLE);
            gl2psDisable(GL2PS_LINE_STIPPLE);
            for(auto i=0; i < this->m_xdata.size(); ++i){
                auto xi = this->m_xdata[i];
                auto yi = this->m_ydata[i];
                auto y1 = this->m_y1errdata[i];
                auto y2 = this->m_y2errdata[i];
                _x = this->coord2D_to_xaxis(xi);
                _y = this->coord2D_to_yaxis(yi);
                _y1 = this->coord2D_to_yaxis(yi + y1);
                _y2 = this->coord2D_to_yaxis(yi - y2);
                glBegin(GL_LINE_STRIP);
                    glVertex2d(_x, _y1);
                    glVertex2d(_x, _y2);
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glVertex2d(_x-_sx, _y);
                    glVertex2d(_x+_sx, _y);
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glVertex2d(_x-_sx, _y1);
                    glVertex2d(_x+_sx, _y1);
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glVertex2d(_x-_sx, _y2);
                    glVertex2d(_x+_sx, _y2);
                glEnd();
            }
        }
    }else{// 3D
        glBegin(GL_LINE_STRIP);
        for(auto i=0; i < this->m_xdata.size(); ++i){
            auto x = this->coord3D_to_xaxis(this->m_xdata[i]);
            auto y = this->coord3D_to_xaxis(this->m_ydata[i]);
            auto z = this->coord3D_to_zaxis(this->m_zdata[i]);
            glVertex3d(x, y, z);
        }
        glEnd();
    }
}

// -*-
std::pair<double, double> LineBase::minmax(const Vector<double>& data, Scale scale){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    auto vmin = std::numeric_limits<double>::max();
    auto vmax = -vmin;
    std::pair<double, double> result(vmin, vmax);
    if(scale==Scale::Linear){
        for(auto item=data.begin(); item != data.end(); ++item){
            if(result.first > *item){ result.first = *item; }
            if(result.second < *item){ result.second = *item; }
        }
    }else{
        for(auto item = data.begin(); item != data.end(); ++item){
            if(result.first > *item && *item > 0){ result.first = *item; }
            if(result.second < *item){ result.second = *item; }
        }
    }

    return result;
}

// -*-
void LineBase::config(){
    auto rc = this->minmax(this->m_xdata, this->m_ca->m_xscale);
    double vmin, vmax;
    vmin = std::min(rc.first, this->m_ca->m_xdatalim.minval);
    vmax = std::max(rc.second, this->m_ca->m_xdatalim.maxval);
    this->m_ca->m_xdatalim = DataLim(vmin, vmax);
    rc = this->minmax(this->m_ydata, this->m_ca->m_yscale);
    vmin = std::min(rc.first, this->m_ca->m_ydatalim.minval);
    vmax = std::max(rc.second, this->m_ca->m_ydatalim.maxval);
    this->m_ca->m_ydatalim = DataLim(vmin, vmax);
    rc = this->minmax(this->m_ydata, this->m_ca->m_zscale);
    vmin = std::min(rc.first, this->m_ca->m_zdatalim.minval);
    vmax = std::max(rc.second, this->m_ca->m_zdatalim.maxval);
    this->m_ca->m_zdatalim = DataLim(vmin, vmax);
}

// -*-
Line LineBase::set_capacity(unsigned int cap){
    if(cap < this->m_maxCapacity){
        if(this->m_xdata.size() > cap){
            this->m_xdata.erase(this->m_xdata.begin(), this->m_xdata.end()-cap);
        }
        if(this->m_ydata.size() > cap){
            this->m_ydata.erase(this->m_ydata.begin(), this->m_ydata.end()-cap);
        }
        if(this->m_zdata.size() > cap){
            this->m_zdata.erase(this->m_zdata.begin(), this->m_zdata.end()-cap);
        }
    }
    this->m_maxCapacity = cap;
    return this->share();
}

// -*-
Line LineBase::stop_at_max(bool flag){
    this->m_stopAtMax = flag;
    return this->share();
}

// -*-
Line LineBase::vertex(double x, double y){
    bool ok = (this->m_xdata.size() == this->m_maxCapacity && this->m_stopAtMax);
    if(!ok){
        std::unique_lock<std::mutex> lock(this->m_data_mtx);
        if(this->m_ca->m_xdatalim.minval > x ){
            this->m_ca->m_xdatalim.minval = x;
        }
        if(this->m_ca->m_xdatalim.maxval < x){
            this->m_ca->m_xdatalim.maxval = x;
        }
        if(this->m_ca->m_ydatalim.minval > y ){
            this->m_ca->m_ydatalim.minval = y;
        }
        if(this->m_ca->m_ydatalim.maxval < y){
            this->m_ca->m_ydatalim.maxval = y;
        }
        this->m_xdata.push_back(x);
        if(this->m_xdata.size() > this->m_maxCapacity){
            this->m_xdata.erase(
                this->m_xdata.begin(),
                this->m_xdata.end()-this->m_maxCapacity
            );
        }
        this->m_ydata.push_back(y);
        if(this->m_ydata.size() > this->m_maxCapacity){
            this->m_ydata.erase(
                this->m_ydata.begin(),
                this->m_ydata.end()-this->m_maxCapacity
            );
        }
    }
    return this->share();
}

// -*-
Line LineBase::line(const Vector<double>& xvec, const Vector<double>& yvec){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata = xvec;
    this->m_ydata = yvec;
    return this->share();
}

// -*-
Line LineBase::line(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& zvec
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata = xvec;
    this->m_ydata = yvec;
    this->m_zdata = zvec;
    return this->share();
}

// -*-
Line LineBase::plot(const Vector<double>& yvec){
    auto size = yvec.size();
    Vector<double> xvec(size);
    for(auto i=0; i < size; ++i){
        xvec[i] = 1.0*i/(size - 1);
    }
    return this->line(xvec, yvec);
}

// -*-
Line LineBase::plot(const Vector<double>& xvec, const Vector<double>& yvec){
    return this->line(xvec, yvec);
}

// -*-
Line LineBase::plot(const Vector<double>& xvec, const Vector<double>& yvec, const Vector<double>& zvec){
    this->m_ca->m_axType = AxesType::Axes3D;
    return this->line(xvec, yvec, zvec);
}

// -*-
Line LineBase::plot(const std::valarray<double>& xvec, const std::valarray<double>& yvec){
    Vector<double> xdata, ydata;
    for(auto i=0; i < xvec.size(); ++i){ xdata.push_back(xvec[i]); }
    for(auto i=0; i < yvec.size(); ++i){ ydata.push_back(yvec[i]); }
    return this->line(xdata, ydata);
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-