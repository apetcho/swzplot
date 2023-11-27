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
, m_mc(Color("blue"))
, m_fc(Color("blue"))
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

// -*-
Line LineBase::semilogx(const Vector<double>& xvec, const Vector<double>& yvec){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_xscale = Scale::Logarithm;
    this->m_xdata = xvec;
    this->m_ydata = yvec;
    return this->share();
}

// -*-
Line LineBase::semilogy(const Vector<double>& xvec, const Vector<double>& yvec){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_yscale = Scale::Logarithm;
    this->m_xdata = xvec;
    this->m_ydata = yvec;
    return this->share();
}

// -*-
Line LineBase::loglog(const Vector<double>& xvec, const Vector<double>& yvec){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_xscale = Scale::Logarithm;
    this->m_ca->m_yscale = Scale::Logarithm;
    this->m_xdata = xvec;
    this->m_ydata = yvec;
    return this->share();
}

// -*-
Line LineBase::vertex(double x, double y, double dy1, double dy2){
    auto ok = (this->m_xdata.size() == this->m_maxCapacity && this->m_stopAtMax);
    if(!ok){
        std::unique_lock<std::mutex> lock(this->m_data_mtx);
        if(this->m_ca->m_xdatalim.minval > x){ this->m_ca->m_xdatalim.minval = x; }
        if(this->m_ca->m_xdatalim.maxval < x){ this->m_ca->m_xdatalim.maxval = x; }
        if(this->m_ca->m_ydatalim.minval > (y+dy1)){
            this->m_ca->m_xdatalim.minval = y+dy1;
        }
        if(this->m_ca->m_ydatalim.maxval < (y-dy2)){
            this->m_ca->m_xdatalim.maxval = (y-dy2);
        }
        this->m_xdata.push_back(x);
        if(this->m_xdata.size() > this->m_maxCapacity){
            auto pos1 = this->m_xdata.begin();
            auto pos2 = this->m_xdata.end();
            this->m_xdata.erase(pos1, pos2-this->m_maxCapacity);
        }
        this->m_ydata.push_back(y);
        if(this->m_ydata.size() > this->m_maxCapacity){
            auto pos1 = this->m_ydata.begin();
            auto pos2 = this->m_ydata.end();
            this->m_ydata.erase(pos1, pos2-this->m_maxCapacity);
        }
        this->m_y1errdata.push_back(dy1);
        if(this->m_y1errdata.size() > this->m_maxCapacity){
            auto pos1 = this->m_y1errdata.begin();
            auto pos2 = this->m_y1errdata.end();
            this->m_y1errdata.erase(pos1, pos2-this->m_maxCapacity);
        }
        this->m_y2errdata.push_back(dy2);
        if(this->m_y2errdata.size() > this->m_maxCapacity){
            auto pos1 = this->m_y2errdata.begin();
            auto pos2 = this->m_y2errdata.end();
            this->m_y2errdata.erase(pos1, pos2-this->m_maxCapacity);
        }
    }
    return this->share();
}

// -*-
Line LineBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& evec
){
    for(auto i=0; i < xvec.size(); ++i){
        this->vertex(xvec[i], yvec[i], evec[i], evec[i]);
    }
    this->m_errorbarFlag = true;
    return this->share();
}

// -*-
Line LineBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& y1vec, const Vector<double>& y2vec
){
    for(auto i=0; i < xvec.size(); ++i){
        auto x = xvec[i];
        auto y = yvec[i];
        auto y1 = y1vec[i];
        auto y2 = y2vec[i];
        this->vertex(x, y, y1, y2);
    }
    this->m_errorbarFlag = true;
    return this->share();
}

// -*- 3D
Line LineBase::vertex(double x, double y, double z){
    auto ok = (this->m_xdata.size() == this->m_maxCapacity && this->m_stopAtMax);
    if(!ok){
        std::unique_lock<std::mutex> lock(this->m_data_mtx);
        if(this->m_ca->m_xdatalim.minval > x){
            this->m_ca->m_xdatalim.minval = x;
        }
        if(this->m_ca->m_xdatalim.maxval < x){
            this->m_ca->m_xdatalim.minval = x;
        }

        if(this->m_ca->m_ydatalim.minval > y){
            this->m_ca->m_ydatalim.minval = y;
        }
        if(this->m_ca->m_ydatalim.maxval < y){
            this->m_ca->m_ydatalim.minval = y;
        }

        if(this->m_ca->m_zdatalim.minval > z){
            this->m_ca->m_zdatalim.minval = z;
        }
        if(this->m_ca->m_zdatalim.maxval < z){
            this->m_ca->m_zdatalim.minval = z;
        }
        this->m_xdata.push_back(x);
        if(this->m_xdata.size() > this->m_maxCapacity){
            auto pos1 = this->m_xdata.begin();
            auto pos2 = this->m_xdata.end();
            this->m_xdata.erase(pos1, pos2-this->m_maxCapacity);
        }
        this->m_ydata.push_back(y);
        if(this->m_ydata.size() > this->m_maxCapacity){
            auto pos1 = this->m_ydata.begin();
            auto pos2 = this->m_ydata.end();
            this->m_ydata.erase(pos1, pos2-this->m_maxCapacity);
        }
        this->m_zdata.push_back(z);
        if(this->m_zdata.size() > this->m_maxCapacity){
            auto pos1 = this->m_zdata.begin();
            auto pos2 = this->m_zdata.end();
            this->m_zdata.erase(pos1, pos2-this->m_maxCapacity);
        }
    }
    return this->share();
}

// -*-
Line LineBase::set(float linewidth){
    this->m_lineWidth = linewidth;
    this->m_markerSize = linewidth;
    return this->share();
}

// -*-
Line LineBase::set(std::string key, std::string val){
    key = tolower(key);
    if(key=="color" || key=="colour"){
        this->m_rgbcolor = Color(val);
        this->m_mc = Color(val);
        this->m_fc = Color(val);
    }else if(key == "marker"){ this->m_marker = val; }
    else if(key == "linestyle" || key == "ls"){ this->m_lineStyle = val; }
    else if(key == "markeredgecolor" || key == "mc"){
        this->m_markerEdgeColor = val;
        this->m_mc = Color(val);
    }else if(key=="markerfacecolor" || key == "fc"){
        this->m_markerFaceColor = val;
        this->m_fc = Color(val);
    }

    return this->share();
}

// -*-
Line LineBase::set(std::string key, float val){
    key = tolower(key);
    if(key=="linewidth" || key == "lw"){ this->m_lineWidth = val; }
    else if(key == "markersize" || key == "ms"){ this->m_markerSize = val; }

    return this->share();
}

// -*-
Line LineBase::set(std::string val){
    if(val=="k"){ this->set("color", "k"); }
    else if(val=="r"){ this->set("color", "r"); }
    else if(val=="b"){ this->set("color", "b"); }
    else if(val=="g"){ this->set("color", "g"); }
    else if(val=="c"){ this->set("color", "c"); }
    else if(val=="m"){ this->set("color", "m"); }
    else if(val=="w"){ this->set("color", "w"); }
    else if(val=="dr"){ this->set("color", "dr"); }
    else if(val=="db"){ this->set("color", "db"); }
    else if(val=="dg"){ this->set("color", "dg"); }
    else if(val=="dc"){ this->set("color", "dc"); }
    else if(val=="dm"){ this->set("color", "dm"); }
    else if(val=="dy"){ this->set("color", "dy"); }
    else if(val=="lr"){ this->set("color", "lr"); }
    else if(val=="lb"){ this->set("color", "lb"); }
    else if(val=="lg"){ this->set("color", "lg"); }
    else if(val=="lc"){ this->set("color", "lc"); }
    else if(val=="lm"){ this->set("color", "lm"); }
    else if(val=="ly"){ this->set("color", "ly"); }
    else if(val=="ur"){ this->set("color", "ur"); }
    else if(val=="ub"){ this->set("color", "ub"); }
    else if(val=="ug"){ this->set("color", "ug"); }
    else if(val=="uy"){ this->set("color", "uy"); }
    else if(val=="up"){ this->set("color", "up"); }
    else if(val=="uo"){ this->set("color", "uo"); }
    else if(val=="um"){ this->set("color", "um"); }
    else if(val=="ubr"){ this->set("color", "ubr"); }
    else if(val=="-"){
        this->set("linestyle", "-");
        this->set("marker", "none");
    }else if(val=="- -" || val=="--"){
        this->set("linestyle", "- -");
        this->set("marker", "none");
    }else if(val==":"){
        this->set("linestyle", ":");
        this->set("marker", "none");
    }else if(val=="-."){
        this->set("linestyle", "-.");
        this->set("marker", "none");
    }else if(val=="."){
        this->set("marker", ".");
        this->set("linestyle", "none");
    }else if(val=="+"){
        this->set("marker", "+");
        this->set("linestyle", "none");
    }else if(val=="x"){
        this->set("marker", "x");
        this->set("linestyle", "none");
    }else if(val=="d"){
        this->set("marker", "d");
        this->set("linestyle", "none");
    }else if(val=="^"){
        this->set("marker", "^");
        this->set("linestyle", "none");
    }else if(val=="v"){
        this->set("marker", "v");
        this->set("linestyle", "none");
    }else if(val=="o"){
        this->set("marker", "o");
        this->set("linestyle", "none");
    }else if(val=="*"){
        this->set("marker", "*");
        this->set("linestyle", "none");
    }else if(val==">"){
        this->set("marker", ">");
        this->set("linestyle", "none");
    }else if(val=="<"){
        this->set("marker", "<");
        this->set("linestyle", "none");
    }else if(val=="p"){
        this->set("marker", "p");
        this->set("linestyle", "none");
    }else if(val=="h"){
        this->set("marker", "h");
        this->set("linestyle", "none");
    }

    return this->share();
}

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-