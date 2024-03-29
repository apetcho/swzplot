// #define GL_SILENCE_DEPRECATION

#include "swzplot.hpp"
#include<cctype>

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*---------*-
// -*- Color -*-
// -*---------*-

void Color::init_colors(){
    this->m_colorspecs = {};
    // - basic color
    this->m_colorspecs[ColorEnum::Black] = {"k", "black"};
    this->m_colorspecs[ColorEnum::Red] = {"r", "red"};
    this->m_colorspecs[ColorEnum::Blue] = {"b", "blue"};
    this->m_colorspecs[ColorEnum::Green] = {"g", "green"};
    this->m_colorspecs[ColorEnum::Cyan] = {"c", "cyan"};
    this->m_colorspecs[ColorEnum::Magenta] = {"m", "magenta"};
    this->m_colorspecs[ColorEnum::Yellow] = {"y", "yellow"};
    this->m_colorspecs[ColorEnum::White] = {"w", "white"};
    // - dark color
    this->m_colorspecs[ColorEnum::DarkRed] = {"dr", "darkred"};
    this->m_colorspecs[ColorEnum::DarkBlue] = {"db", "darkblue"};
    this->m_colorspecs[ColorEnum::DarkGreen] = {"dg", "darkgreen"};
    this->m_colorspecs[ColorEnum::DarkCyan] = {"dc", "darkcyan"};
    this->m_colorspecs[ColorEnum::DarkMagenta] = {"dm", "darkmagenta"};
    this->m_colorspecs[ColorEnum::DarkYellow] = {"dy", "darkyellow"};
    // - light color
    this->m_colorspecs[ColorEnum::LightRed] = {"lr", "lightblue"};
    this->m_colorspecs[ColorEnum::LightBlue] = {"lb", "lightblue"};
    this->m_colorspecs[ColorEnum::LightGreen] = {"lg", "lightgreen"};
    this->m_colorspecs[ColorEnum::LightCyan] = {"lc", "lightcyan"};
    this->m_colorspecs[ColorEnum::LightMagenta] = {"lm", "lightmagenta"};
    this->m_colorspecs[ColorEnum::LightYellow] = {"ly", "lightyellow"};
    // - universal color
    this->m_colorspecs[ColorEnum::UniRed] = {"ur", "universalred"};
    this->m_colorspecs[ColorEnum::UniBlue] = {"ub", "universalblue"};
    this->m_colorspecs[ColorEnum::UniGreen] = {"ug", "universalgreen"};
    this->m_colorspecs[ColorEnum::UniYellow] = {"uy", "universalyellow"};
    this->m_colorspecs[ColorEnum::UniCyan] = {"uc", "skyblue"};
    this->m_colorspecs[ColorEnum::UniOrange] = {"uo", "universalorange"};
    this->m_colorspecs[ColorEnum::UniPink] = {"up", "pink"};
    this->m_colorspecs[ColorEnum::UniMagenta] = {"um", "purple"};
    this->m_colorspecs[ColorEnum::UniBrown] = {"ubr", "brown"};

    // -*-
    this->m_rgbcolors = {};
    this->m_rgbcolors[ColorEnum::Black] = Rgb(0.f, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::Red] = Rgb(1.f, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::Blue] = Rgb(0.f, 0.f, 1.f);
    this->m_rgbcolors[ColorEnum::Green] = Rgb(0.f, 1.f, 0.f);
    this->m_rgbcolors[ColorEnum::Cyan] = Rgb(0.f,1.f,1.f);
    this->m_rgbcolors[ColorEnum::Magenta] = Rgb(1.f,0.f, 1.f);
    this->m_rgbcolors[ColorEnum::Yellow] = Rgb(1.f, 1.f, 0.f);
    this->m_rgbcolors[ColorEnum::White] = Rgb(1.f, 1.f,1.f);

    float h = 0.6f;
    this->m_rgbcolors[ColorEnum::DarkRed] = Rgb(h, 0.f, 0.f);
    this->m_rgbcolors[ColorEnum::DarkBlue] = Rgb(0.f, 0.f, h);
    this->m_rgbcolors[ColorEnum::DarkGreen] = Rgb(0.f, h, 0.f);
    this->m_rgbcolors[ColorEnum::DarkCyan] = Rgb(0.f, h, h);
    this->m_rgbcolors[ColorEnum::DarkMagenta] = Rgb(h, 0.f, h);
    this->m_rgbcolors[ColorEnum::DarkYellow] = Rgb(h, h, 0.f);

    h = 0.5f;
    this->m_rgbcolors[ColorEnum::LightRed] = Rgb(1.f, h, h);
    this->m_rgbcolors[ColorEnum::LightBlue] = Rgb(h, h, 1.f);
    this->m_rgbcolors[ColorEnum::LightGreen] = Rgb(h, 1.f, h);
    this->m_rgbcolors[ColorEnum::LightCyan] = Rgb(h, 1.f, 1.f);
    this->m_rgbcolors[ColorEnum::LightMagenta] = Rgb(1.f, h, 1.f);
    this->m_rgbcolors[ColorEnum::LightYellow] = Rgb(1.f, 1.f, h);

    this->m_rgbcolors[ColorEnum::UniRed] = Rgb(1.f, 0.2f, 0.f);
    this->m_rgbcolors[ColorEnum::UniBlue] = Rgb(0.f, 0.25f, 1.f);
    this->m_rgbcolors[ColorEnum::UniGreen] = Rgb(0.2f, 0.6f, 0.4f);
    this->m_rgbcolors[ColorEnum::UniYellow] = Rgb(1.f, 1.f, 1.f);
    this->m_rgbcolors[ColorEnum::UniCyan] = Rgb(0.4f, 0.8f, 1.f);
    this->m_rgbcolors[ColorEnum::UniPink] = Rgb(1.f, 0.6f, 0.6f);
    this->m_rgbcolors[ColorEnum::UniOrange] = Rgb(1.f, 0.6f, 0.f);
    this->m_rgbcolors[ColorEnum::UniPurple] = Rgb(0.6f, 0.f, 0.4f);
    this->m_rgbcolors[ColorEnum::UniBrown] = Rgb(0.4f, 0.2f, 0.f);
}


// -*-
Color::Color(const std::string& colorspec){
    this->init_colors();
    this->m_colorspec = colorspec;
    this->m_rgb = {};
    auto ans = this->find_color_enum_by_spec();
    this->m_colorEnum = ans.value_or(ColorEnum::Black);
    auto rgb = this->m_rgbcolors.find(this->m_colorEnum);
    if(rgb == this->m_rgbcolors.end()){
        this->m_rgb = Rgb{};
    }else{
        this->m_rgb.r = rgb->second.r;
        this->m_rgb.g = rgb->second.g;
        this->m_rgb.b = rgb->second.b;
    }
}

// -*-
Color::Color(float r, float g, float b){
    this->init_colors();
    this->m_rgb = Rgb(r, g, b);
    auto ans = this->find_color_enum_by_rgb();
    this->m_colorEnum = ans.value_or(ColorEnum::Black);
    auto spec = this->m_colorspecs.find(this->m_colorEnum);
    if(spec == this->m_colorspecs.end()){
        this->m_colorspec = "black";
    }else{
        this->m_colorspec = spec->second.first;
    }
}

// -*-
void Color::rgb(float& r, float& g, float& b){
    r = this->m_rgb.r;
    g = this->m_rgb.g;
    b = this->m_rgb.b;
}

std::string Color::colorspec(){
    std::ostringstream stream;
    stream << "(" << this->m_rgb.r;
    stream << " " << this->m_rgb.g;
    stream << " " << this->m_rgb.b;
    stream << ")";
    return stream.str(); 
}

// -*-
std::vector<float> Color::to_vector(){
    float r, g, b;
    this->rgb(r, g, b);
    std::vector<float> result(3);
    result[0] = r;
    result[1] = g;
    result[2] = b;
    return result;
}

// -*-
std::string Color::colorspec(float r, float g, float b){
    std::ostringstream stream;
    stream << "(" << r << " " << g << " " << b << ")";
    return stream.str(); 
}

// -*-
std::string Color::colorspec(std::vector<float> rgb){
    if(rgb.size() != 3){
        throw std::runtime_error("Invalid argument to Color::colorspec()");
    }
    auto r = rgb[0];
    auto g = rgb[1];
    auto b = rgb[2];
    std::ostringstream stream;
    stream << "(" << r << " " << g << " " << b << ")";
    return stream.str(); 
}

// -*----------------------------------------------------------------*-
// -*- swzplot::DrawableBase                                        -*-
// -*----------------------------------------------------------------*-
double DrawableBase::coord2D_to_xaxis(double x){
    double result;
    if(this->m_ca->m_xscale == Scale::Linear){
        auto rv = (x - this->m_ca->m_xlim.minval)/(
            this->m_ca->m_xlim.maxval - this->m_ca->m_xlim.minval
        );
        result = rv;
    }else{
        auto num = (std::log10(x) - std::log10(this->m_ca->m_xlim.maxval));
        auto den = (
            std::log10(this->m_ca->m_xlim.maxval) -
            std::log10(this->m_ca->m_xlim.minval)
        );
        result = num/den;
    }
    return result;
}

// -*-
double DrawableBase::coord2D_to_yaxis(double y){
    double result;
    if(this->m_ca->m_yscale==Scale::Linear){
        double num = y - this->m_ca->m_ylim.minval;
        double den = this->m_ca->m_ylim.maxval - this->m_ca->m_ylim.minval;
        result = num / den;
    }else{
        double num = std::log10(y) - std::log10(this->m_ca->m_ylim.minval);
        double den = (
            std::log10(this->m_ca->m_ylim.maxval) -
            std::log10(this->m_ca->m_ylim.minval)
        );
        result = num / den;
    }
    return result;
}

// -*-
double DrawableBase::coord3D_to_xaxis(double x){
    double num = 2 * (x - this->m_ca->m_xlim.minval);
    double den = this->m_ca->m_xlim.maxval - this->m_ca->m_xlim.minval;
    return (num/den) - 1;
}

// -*-
double DrawableBase::coord3D_to_yaxis(double y){
    double num = 2 * (y - this->m_ca->m_xlim.minval);
    double den = this->m_ca->m_ylim.maxval - this->m_ca->m_ylim.minval;
    return (num/den) - 1;
}

// -*-
double DrawableBase::coord3D_to_zaxis(double z){
    double num = 2 * (z - this->m_ca->m_zlim.minval);
    double den = this->m_ca->m_zlim.maxval - this->m_ca->m_zlim.minval;
    return (num/den) - 1;
}


// -*----------------------------------------------------------------*-
// -*- swzplot::AxesBase                                            -*-
// -*----------------------------------------------------------------*-
// -*-
void AxesBase::vertex(double x, double y){
    this->gco<LineBase>()->vertex(x, y);
}

// -*-
void AxesBase::vertex(double x, double y, double z){
    this->gco<LineBase>()->vertex(x, y, z);
}

// -*-
Line AxesBase::plot(const Vector<double>& ydata){
    return this->add<LineBase>()->plot(ydata);
}

// -*-
Line AxesBase::plot(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->add<LineBase>()->plot(xdata, ydata);
}

// -*-
Line AxesBase::plot(
    const Vector<double>& xdata,
    const Vector<double>& ydata,
    const Vector<double>& zdata
){
    return this->add<LineBase>()->plot(xdata, ydata, zdata);
}

// -*-
Line AxesBase::semilogx(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->add<LineBase>()->semilogx(xdata, ydata);
}

// -*-
Line AxesBase::semilogy(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->add<LineBase>()->semilogy(xdata, ydata);
}

// -*-
Line AxesBase::loglog(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->add<LineBase>()->loglog(xdata, ydata);
}

// -*-
void AxesBase::vertex(double x, double y, double dx, double dy){
    this->gco<LineBase>()->vertex(x, y, dx, dy);
}

// -*-
void AxesBase::errorbar(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Vector<double>& dxvec
){
    this->gco<LineBase>()->errorbar(xvec, yvec, dxvec);
}

// -*-
void AxesBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec, const Vector<double>& dyvec
){
    this->gco<LineBase>()->errorbar(xvec, yvec, dxvec, dyvec);
}

// -*-
Surface AxesBase::surface(const Matrix<double>& zmat){
    return this->add<SurfaceBase>()->surface(zmat);
}

// -*-
Surface AxesBase::surface(const Matrix<double>& zmat, const Matrix<double>& cmat){
    return this->add<SurfaceBase>()->surface(zmat, cmat);
}

// -*-
Surface AxesBase::surface(const Matrix<double>& zmat, const Colormap& cdata){
    return this->add<SurfaceBase>()->surface(zmat, cdata);
}

// -*-
Surface AxesBase::surface(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->add<SurfaceBase>()->surface(xvec, yvec, zmat);
}

// -*-
Surface AxesBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->add<SurfaceBase>()->surface(xvec, yvec, zmat, cmat);
}

// -*-
Surface AxesBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->add<SurfaceBase>()->surface(xvec, yvec, zmat, cdata);
}

// -*-
Surface AxesBase::surface(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->add<SurfaceBase>()->surface(xmat, ymat, zmat);
}

// -*-
Surface AxesBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->add<SurfaceBase>()->surface(xmat, ymat, zmat, cmat);
}

// -*-
Surface AxesBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->add<SurfaceBase>()->surface(xmat, ymat, zmat, cdata);
}

// -*-
Surface AxesBase::pcolor(const Matrix<double>& cmat){
    return this->add<SurfaceBase>()->pcolor(cmat);
}

// -*-
Surface AxesBase::pcolor(const Colormap& cdata){
    return this->add<SurfaceBase>()->pcolor(cdata);
}

// -*-
Surface AxesBase::pcolor(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& cmat
){
    return this->add<SurfaceBase>()->pcolor(xvec, yvec, cmat);
}

// -*-
Surface AxesBase::pcolor(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Colormap& cdata
){
    return this->add<SurfaceBase>()->pcolor(xvec, yvec, cdata);
}

// -*-
Surface AxesBase::pcolor(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Matrix<double>& cmat
){
    return this->add<SurfaceBase>()->pcolor(xmat, ymat, cmat);
}

// -*-
Surface AxesBase::pcolor(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Colormap& cdata
){
    return this->add<SurfaceBase>()->pcolor(xmat, ymat, cdata);
}

// -*-
Surface AxesBase::contour(const Matrix<double>& zmat){
    return this->add<SurfaceBase>()->contour(zmat);
}

// -*-
Surface AxesBase::contour(const Matrix<double>& zmat, unsigned int n){
    return this->add<SurfaceBase>()->contour(zmat, n);
}

// -*-
Surface AxesBase::contour(const Matrix<double>& zmat, const Vector<double>& values){
    return this->add<SurfaceBase>()->contour(zmat, values);
}

// -*-
Surface AxesBase::contour(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->add<SurfaceBase>()->contour(xvec, yvec, zmat);
}

// -*-
Surface AxesBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, unsigned int n
){
    return this->add<SurfaceBase>()->contour(xvec, yvec, zmat, n);
}

// -*-
Surface AxesBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values
){
    return this->add<SurfaceBase>()->contour(xvec, yvec, zmat, values);
}

// -*-
Surface AxesBase::mesh(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->add<SurfaceBase>()->mesh(xvec, yvec, zmat);
}

// -*-
Surface AxesBase::surf(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->add<SurfaceBase>()->surf(xvec, yvec, zmat);
}

// -*-
void AxesBase::shading(std::string arg){
    this->gco<SurfaceBase>()->shading(arg);
}

// -*-
Patch AxesBase::patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    return this->add<PatchBase>()->patch(xmat, ymat);
}

// -*-
Patch AxesBase::patch(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    return this->add<PatchBase>()->patch(xmat, ymat, cvec);
}

// -*-
Patch AxesBase::patch(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Matrix<float>& cdata
){
    return this->add<PatchBase>()->patch(xmat, ymat, cdata);
}

// -*-
Patch AxesBase::patch(
    const Matrix<double>& xmat,
    const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->add<PatchBase>()->patch(xmat, ymat, zmat);
}

// -*-
Patch AxesBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    return this->add<PatchBase>()->patch(xmat, ymat, zmat, cvec);
}

// -*-
Patch AxesBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cdata
){
    return this->add<PatchBase>()->patch(xmat, ymat, zmat, cdata);
}

// -*-
Patch AxesBase::bar(const Vector<double>& ydata){
    return this->add<PatchBase>()->bar(ydata);
}

// -*-
Patch AxesBase::bar(const Vector<double>& ydata, float width){
    return this->add<PatchBase>()->bar(ydata, width);
}

// -*-
Patch AxesBase::bar(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->add<PatchBase>()->bar(xdata, ydata);
}

// -*-
Patch AxesBase::bar(const Vector<double>& xdata, const Vector<double>& ydata, float width){
    return this->add<PatchBase>()->bar(xdata, ydata, width);
}

// -*-
Text AxesBase::text(double x, double y, const std::string message){
    return this->add<TextBase>()->text(x, y, message);
}

// -*----------------------------------------------------------------*-
// -*- swzplot::CanvasBase                                          -*-
// -*----------------------------------------------------------------*-
void CanvasBase::set(const std::string key){
    this->gca()->gco<LineBase>()->set(key);
}

// -*-
void CanvasBase::set(float key){
    this->gca()->gco<LineBase>()->set(key);
}

// -*-
void CanvasBase::set(std::string key, std::string val){
    this->gca()->gco<LineBase>()->set(key, val);
}

// -*-
void CanvasBase::set(std::string key, float val){
    this->gca()->gco<LineBase>()->set(key, val);
}

// -*-
void CanvasBase::axis(double xmin, double xmax, double ymin, double ymax){
    this->gca()->axis(xmin, xmax, ymin, ymax);
}

// -*-
void CanvasBase::axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax){
    this->gca()->axis(xmin, xmax, ymin, ymax, zmin, zmax);
}

// -*-
void CanvasBase::axis(std::string onoff){
    this->gca()->axis(onoff);
}

// -*-
void CanvasBase::axis(bool onoff){
    this->gca()->axis(onoff);
}

// -*-
void CanvasBase::grid(std::string onoff){
    this->gca()->grid(onoff);
}

// -*-
void CanvasBase::grid(bool onoff){
    this->gca()->grid(onoff);
}

// -*-
void CanvasBase::ticklabel(bool onoff){
    this->gca()->ticklabel(onoff);
}

// -*-
void CanvasBase::title(std::string label){
    this->gca()->title(label);
}

// -*-
void CanvasBase::xlabel(std::string label){
    this->gca()->xlabel(label);
}

// -*-
void CanvasBase::ylabel(std::string label){
    this->gca()->ylabel(label);
}

// -*-
void CanvasBase::capture_mouse(bool flag){
    this->gca()->capture_mouse(flag);
}

// -*-
void CanvasBase::vertex(double x, double y){
    this->gca()->gco<LineBase>()->vertex(x, y);
}

// -*-
void CanvasBase::vertex(double x, double y, double z){
    this->gca()->gco<LineBase>()->vertex(x, y, z);
}

// -*-
Line CanvasBase::plot(const Vector<double>& yvec){
    return this->gca()->add<LineBase>()->plot(yvec);
}

// -*-
Line CanvasBase::plot(const Vector<double>& xvec, const Vector<double>& yvec){
    return this->gca()->add<LineBase>()->plot(xvec, yvec);
}

// -*-
Line CanvasBase::plot(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& zvec
){
    return this->gca()->add<LineBase>()->plot(xvec, yvec, zvec);
}

// -*-
Line CanvasBase::semilogx(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->semilogx(xvec, yvec);
}

// -*-
Line CanvasBase::semilogy(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->semilogy(xvec, yvec);
}

// -*-
Line CanvasBase::loglog(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->loglog(xvec, yvec);
}

// -*-
void CanvasBase::vertex(double x, double y, double dx, double dy){
    this->gca()->gco<LineBase>()->vertex(x, y, dx, dy);
}

// -*-
void CanvasBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec
){
    this->gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec);
}

// -*-
void CanvasBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec, const Vector<double>& dyvec
){
    this->gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec, dyvec);
}

// -*-
Surface CanvasBase::surface(const Matrix<double>& zmat){
    return this->gca()->add<SurfaceBase>()->surface(zmat);
}

// -*-
Surface CanvasBase::surface(const Matrix<double>& zmat, const Matrix<double>& cmat){
    return this->gca()->add<SurfaceBase>()->surface(zmat, cmat);
}

// -*-
Surface CanvasBase::surface(const Matrix<double>& zmat, const Colormap& cdata){
    return this->gca()->add<SurfaceBase>()->surface(zmat, cdata);
}

// -*-
Surface CanvasBase::surface(
    const Vector<double>& xvec,
    const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat);
}

// -*-
Surface CanvasBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cmat);
}

// -*-
Surface CanvasBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cdata);
}

// -*-
Surface CanvasBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat);
}

// -*-
Surface CanvasBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cmat);
}

// -*-
Surface CanvasBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cdata);
}

// -*-
Surface CanvasBase::pcolor(const Matrix<double>& cmat){
    return this->gca()->add<SurfaceBase>()->pcolor(cmat);
}

// -*-
Surface CanvasBase::pcolor(const Colormap& cdata){
    return this->gca()->add<SurfaceBase>()->pcolor(cdata);
}

// -*-
Surface CanvasBase::pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& cvec
){
    return this->gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cvec);
}

// -*-
Surface CanvasBase::pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cdata);
}

// -*-
Surface CanvasBase::pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cmat);
}

// -*-
Surface CanvasBase::pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cdata);
}

// -*-
Surface CanvasBase::contour(const Matrix<double>& zmat){
    return this->gca()->add<SurfaceBase>()->contour(zmat);
}

// -*-
Surface CanvasBase::contour(const Matrix<double>& zmat, unsigned int n){
    return this->gca()->add<SurfaceBase>()->contour(zmat, n);
}

// -*-
Surface CanvasBase::contour(const Matrix<double>& zmat, const Vector<double>& values){
    return this->gca()->add<SurfaceBase>()->contour(zmat, values);
}

// -*-
Surface CanvasBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat);
}

// -*-
Surface CanvasBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, unsigned int n
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, n);
}

// -*-
Surface CanvasBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, values);
}

// -*-
Surface CanvasBase::mesh(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->mesh(xvec, yvec, zmat);
}

// -*-
Surface CanvasBase::surf(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surf(xvec, yvec, zmat);
}

// -*-
void CanvasBase::shading(std::string arg){
    this->gca()->gco<SurfaceBase>()->shading(arg);
}

// -*-
Patch CanvasBase::patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat);
}

// -*-
Patch CanvasBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, cvec);
}

// -*-
Patch CanvasBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<float>& cdata
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, cdata);
}

// -*-
Patch CanvasBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat);
}

// -*-
Patch CanvasBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cvec);
}

// -*-
Patch CanvasBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cdata
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cdata);
}

// -*-
Patch CanvasBase::bar(const Vector<double>& ydata){
    return this->gca()->add<PatchBase>()->bar(ydata);
}

// -*-
Patch CanvasBase::bar(const Vector<double>& ydata, float width){
    return this->gca()->add<PatchBase>()->bar(ydata, width);
}

// -*-
Patch CanvasBase::bar(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->gca()->add<PatchBase>()->bar(xdata, ydata);
}

// -*-
Patch CanvasBase::bar(const Vector<double>& xdata, const Vector<double>& ydata, float width){
    return this->gca()->add<PatchBase>()->bar(xdata, ydata, width);
}

// -*-
Text CanvasBase::text(double x, double y, const std::string message){
    return this->gca()->add<TextBase>()->text(x, y, message);
}

// -*-
Axes CanvasBase::subplot(unsigned int m, unsigned int n, unsigned p){
    Axes result = nullptr;
    auto k = p - 1;
    auto item = this->m_axesDict.find(k);
    if(item == this->m_axesDict.end()){

        auto ax = std::make_shared<AxesBase>(this->share());
        this->m_axesDict[k] = ax;
        this->m_ca = ax;
        int ix = k/m;
        int iy = (m-1) - (k % m);
        this->m_ca->m_axBBox.left = (ix + 0.13)/n;
        this->m_ca->m_axBBox.bottom = (ix + 0.11)/m;
        this->m_ca->m_axBBox.width = 0.775/n;
        this->m_ca->m_axBBox.height = 0.815/m;

        this->m_ca->m_viewBBox.left = 1.0*ix/n;
        this->m_ca->m_viewBBox.bottom = 1.0*iy/m;
        this->m_ca->m_viewBBox.width = 1.0/n;
        this->m_ca->m_viewBBox.height = 1.0/m;
    }else{
        this->m_ca = item->second;
    }
    return this->m_ca;
}

// -*-
bool CanvasBase::mouse(int button, int state, int x, int y){
    double _x = static_cast<double>(x)/(this->m_figure->m_window_width);
    double _y = (
        static_cast<double>(this->m_figure->m_window_height-y) /
        this->m_figure->m_window_height
    );
    double rx, ry, mx, my;
    double left, bottom, width, height;
    if(this->m_selected_axes && this->m_selected_axes->m_visible){
        left = this->m_selected_axes->m_axBBox.left;
        bottom = this->m_selected_axes->m_axBBox.bottom;
        width = this->m_selected_axes->m_axBBox.width;
        height = this->m_selected_axes->m_axBBox.height;
        bool check = (
            (left <= _x) && (_x <= (left+width)) &&
            (bottom <= _y) && (_y <= (bottom + height))
        );
        if(this->m_selected_axes->m_mouse && check){
            rx = (_x - left)/width;
            ry = (_y - bottom)/height;
            mx = (
                rx*(this->m_selected_axes->m_xlim.maxval - this->m_selected_axes->m_xlim.minval) +
                this->m_selected_axes->m_xlim.minval
            );
            my = (
                ry*(this->m_selected_axes->m_ylim.maxval - this->m_selected_axes->m_ylim.minval) +
                this->m_selected_axes->m_ylim.minval
            );
            if(this->m_selected_axes->mouse(button, state, mx, my)){
                return true;
            }
        }
    }
    // - axes select
    if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(this->m_selected_axes){
            this->m_selected_axes->m_selected = false;
        }
        for(auto axIter=this->m_axesDict.rbegin(); axIter!=this->m_axesDict.rend(); ++axIter){
            if(this->m_ca->m_visible){
                left = axIter->second->m_axBBox.left;
                bottom = axIter->second->m_axBBox.bottom;
                width = axIter->second->m_axBBox.width;
                height = axIter->second->m_axBBox.height;

                bool test = (
                    (left <= _x) && (_x <= (left + width)) &&
                    (bottom <= _y) && (_y <= (bottom + height)) 
                );
                if(test){
                    this->m_selected_axes = axIter->second;
                    axIter->second->m_selected = true;
                    axIter->second->m_xybutton = Position(x, y);
                    return true;
                }
            }
        }
    }

    return false;
}

// -*-
bool CanvasBase::motion(int x, int y){
    if(this->m_selected_axes){
        return this->m_selected_axes->motion(x, y);
    }
    return false;
}

// -*-
void CanvasBase::toggle_visibility(){
    this->set_visibility(!this->m_visible);
}

// -*-
void CanvasBase::set_visibility(bool flag){
    this->m_visible = flag;
    for(auto axIter=this->m_axesDict.begin(); axIter!=this->m_axesDict.end(); ++axIter){
        axIter->second->m_visible = flag;
    }
}

// -*-
void CanvasBase::draw(){
    if(!this->m_visible){ return; }
    for(auto axIter=this->m_axesDict.begin(); axIter!=this->m_axesDict.end(); ++axIter){
        axIter->second->draw();
    }
}


// -*----------------------------------------------------------------*-
// -*- swzplot::FigureBase                                          -*-
// -*----------------------------------------------------------------*-
void FigureBase::set_window_name(const std::string& name){
    this->m_window_name = name;
    if(this->m_window_num){
        glut::set_window_title(this->m_window_num, name);
    }
}

// -*-
void FigureBase::set(const std::string key){
    this->gca()->gco<LineBase>()->set(key);
}

// -*-
void FigureBase::set(float key){
    this->gca()->gco<LineBase>()->set(key);
}

// -*-
void FigureBase::set(std::string key, std::string val){
    this->gca()->gco<LineBase>()->set(key, val);
}

// -*-
void FigureBase::set(std::string key, float val){
    this->gca()->gco<LineBase>()->set(key, val);
}

// -*-
void FigureBase::vertex(double x, double y){
    this->gca()->gco<LineBase>()->vertex(x, y);
}

// -*-
void FigureBase::vertex(double x, double y, double z){
    this->gca()->gco<LineBase>()->vertex(x, y, z);
}

// -*-
Line FigureBase::plot(const Vector<double>& yvec){
    return this->gca()->add<LineBase>()->plot(yvec);
}

// -*-
Line FigureBase::plot(const Vector<double>& xvec, const Vector<double>& yvec){
    return this->gca()->add<LineBase>()->plot(xvec, yvec);
}

// -*-
Line FigureBase::plot(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& zvec
){
    return this->gca()->add<LineBase>()->plot(xvec, yvec, zvec);
}

// -*-
Line FigureBase::semilogx(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->semilogx(xvec, yvec);
}

// -*-
Line FigureBase::semilogy(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->semilogy(xvec, yvec);
}

// -*-
Line FigureBase::loglog(const Vector<double>& xvec, const Vector<double>&  yvec){
    return this->gca()->add<LineBase>()->loglog(xvec, yvec);
}

// -*-
void FigureBase::vertex(double x, double y, double dx, double dy){
    this->gca()->gco<LineBase>()->vertex(x, y, dx, dy);
}

// -*-
void FigureBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec
){
    this->gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec);
}

// -*-
void FigureBase::errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec, const Vector<double>& dyvec
){
    this->gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec, dyvec);
}

// -*-
Surface FigureBase::surface(const Matrix<double>& zmat){
    return this->gca()->add<SurfaceBase>()->surface(zmat);
}

// -*-
Surface FigureBase::surface(const Matrix<double>& zmat, const Matrix<double>& cmat){
    return this->gca()->add<SurfaceBase>()->surface(zmat, cmat);
}

// -*-
Surface FigureBase::surface(const Matrix<double>& zmat, const Colormap& cdata){
    return this->gca()->add<SurfaceBase>()->surface(zmat, cdata);
}

// -*-
Surface FigureBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat);
}

// -*-
Surface FigureBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cmat);
}

// -*-
Surface FigureBase::surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cdata);
}

// -*-
Surface FigureBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat);
}

// -*-
Surface FigureBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cmat);
}

// -*-
Surface FigureBase::surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cdata);
}

// -*-
Surface FigureBase::pcolor(const Matrix<double>& cmat){
    return this->gca()->add<SurfaceBase>()->pcolor(cmat);
}

// -*-
Surface FigureBase::pcolor(const Colormap& cdata){
    return this->gca()->add<SurfaceBase>()->pcolor(cdata);
}

// -*-
Surface FigureBase::pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& cvec
){
    return this->gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cvec);
}

// -*-
Surface FigureBase::pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cdata);
}

// -*-
Surface FigureBase::pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& cmat
){
    return this->gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cmat);
}

// -*-
Surface FigureBase::pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Colormap& cdata
){
    return this->gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cdata);
}

// -*-
Surface FigureBase::contour(const Matrix<double>& zmat){
    return this->gca()->add<SurfaceBase>()->contour(zmat);
}

// -*-
Surface FigureBase::contour(const Matrix<double>& zmat, unsigned int n){
    return this->gca()->add<SurfaceBase>()->contour(zmat, n);
}

// -*-
Surface FigureBase::contour(const Matrix<double>& zmat, const Vector<double>& values){
    return this->gca()->add<SurfaceBase>()->contour(zmat, values);
}

// -*-
Surface FigureBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat);
}

// -*-
Surface FigureBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, unsigned int n
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, n);
}

// -*-
Surface FigureBase::contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values
){
    return this->gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, values);
}

// -*-
Surface FigureBase::mesh(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->mesh(xvec, yvec, zmat);
}

// -*-
Surface FigureBase::surf(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return this->gca()->add<SurfaceBase>()->surf(xvec, yvec, zmat);
}

// -*-
void FigureBase::shading(std::string arg){
    this->gca()->gco<SurfaceBase>()->shading(arg);
}

// -*-
Patch FigureBase::patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat);
}

// -*-
Patch FigureBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, cvec);
}

// -*-
Patch FigureBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<float>& cdata
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, cdata);
}

// -*-
Patch FigureBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat);
}

// -*-
Patch FigureBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cvec);
}

// -*-
Patch FigureBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cdata
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cdata);
}

// -*-
Patch FigureBase::bar(const Vector<double>& ydata){
    return this->gca()->add<PatchBase>()->bar(ydata);
}

// -*-
Patch FigureBase::bar(const Vector<double>& ydata, float width){
    return this->gca()->add<PatchBase>()->bar(ydata, width);
}

// -*-
Patch FigureBase::bar(const Vector<double>& xdata, const Vector<double>& ydata){
    return this->gca()->add<PatchBase>()->bar(xdata, ydata);
}

// -*-
Patch FigureBase::bar(const Vector<double>& xdata, const Vector<double>& ydata, float width){
    return this->gca()->add<PatchBase>()->bar(xdata, ydata, width);
}

// -*-
Text FigureBase::text(double x, double y, const std::string message){
    return this->gca()->add<TextBase>()->text(x, y, message);
}

// -*-
FigureBase::FigureBase(std::string name, bool visible)
: m_window_name(name), m_visible(visible){
    // -
    this->keyboard_callack = nullptr;
    this->m_xyPassive = Position(100, 0);
    this->m_window_num = 0;
    this->m_windowBBox = BBox(0, 0, 500, 500);
}

// -*-
Canvas FigureBase::canvas(std::string name, bool visible){
    auto entry = this->m_canvasDict.find(name);
    if(entry == this->m_canvasDict.end()){
        this->m_current_canvas = std::make_shared<CanvasBase>(
            this->share(), name, visible
        );
        this->m_canvasDict[name] = this->m_current_canvas;
    }
    return this->m_current_canvas;
}

// -*-
void FigureBase::draw(){
    if(!this->m_visible){ return; }
    glutSetWindow(this->m_window_num);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto _w = this->m_window_width;
    auto _h = this->m_window_height;
    glViewport(0, 0, _w, _h);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 3);
    for(auto entry=this->m_canvasDict.begin(); entry!=this->m_canvasDict.end(); ++entry){
        entry->second->draw();
    }
    this->draw_all();
    glFlush();
    glViewport(0, 0, _w, _h);
    glutSwapBuffers();
}

// -*-
void FigureBase::draw_all(){
    if(this->m_canvasDict.size() <= 1){ return; }

    std::string label;
    int left = 1;
    int width = 20;
    int height = 20;
    int r = 3;

    if(this->m_xyPassive.x < 25){
        auto _w = static_cast<int>(this->m_window_width);
        auto _h = static_cast<int>(this->m_window_height);
        glViewport(0, 0, _w, _h);
        glLoadIdentity();
        auto w = static_cast<double>(_w);
        auto h = static_cast<double>(_h);
        gluOrtho2D(0.0, w, h, 0.0);
        glDisable(GL_LINE_STIPPLE);
        gl2psDisable(GL2PS_LINE_STIPPLE);
        glLineWidth(2.f);
        glColor3d(0., 0., 1.);
        int j = 0;
        auto entry = this->m_canvasDict.begin();
        for(; entry != this->m_canvasDict.end(); ++entry){
            glBegin(GL_LINE_STRIP);
                glVertex2d(left+r, height*j+r);
                glVertex2d(left+r, height*j+height-r);
                glVertex2d(left+width-r, height*j+height-r);
                glVertex2d(left+width-r, height*j+r);
                glVertex2d(left+r, height*j+r);
            glEnd();

            if(entry->second->is_visible()){
                glBegin(GL_LINE_STRIP);
                    glVertex2d(left+9, height*j+5);
                    glVertex2d(left+8, height*j+15);
                    glVertex2d(left+15, height*j+7);
                glEnd();
            }
            glColor3f(0.f, 0.f, 1.0f);
            glRasterPos2f(22.f, height*j+height-6);
            label = entry->second->m_canvas_name;
            gl2psText(label.c_str(), "Arial", 12);
            for(auto i = 0; i < label.size(); ++i){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[i]);
            }
        }        
    }
}

// -*- mouse event handler
void FigureBase::mouse(int button, int state, int x, int y){
    int left = 1;
    int width = 20;
    int height = 20;
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        int j = 0;
        auto entry = this->m_canvasDict.begin();
        for(; entry != this->m_canvasDict.end(); ++entry){
            bool ok = (
                (left < x) && (x < width) &&
                (height*j < y) && (y < height*j + height)
            );
            if(ok){
                entry->second->toggle_visibility();
                bool okay = (
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() -
                        entry->second->timeCliked
                    ).count() < 200
                );
                if(okay){
                    auto keyval = this->m_canvasDict.begin();
                    for(; keyval!=this->m_canvasDict.end(); ++keyval){
                        keyval->second->set_visibility(false);
                    }
                    entry->second->set_visibility(true);
                }
                entry->second->timeCliked = std::chrono::steady_clock::now();
                return;
            }
        }
    }

    auto entry = this->m_canvasDict.rbegin();
    for(; entry != this->m_canvasDict.rend(); ++entry){
        if(entry->second->mouse(button, state, x, y)){
            this->m_selected_canvas = entry->second;
        }
    }
}

// -*- resize event handler
void FigureBase::reshape(int width, int height){
    this->m_window_width = width;
    this->m_window_height = height;
    glViewport(0, 0, width, height);
}

// -*- motion event handler
void FigureBase::motion(int x, int y){
    if(this->m_selected_canvas){
        this->m_selected_canvas->motion(x, y);
    }
}

// -*-
void FigureBase::passivemotion(int x, int y){
    this->m_xyPassive = Position(x, y);
}

// -*-
void FigureBase::keyboard(char key, int x, int y){
    switch(key){
    case 'q':
        glutDestroyWindow(this->m_window_num);
        break;
    case 'p':
        this->print();
        break;
    default:
        break;
    }
    if(this->keyboard_callack){
        this->keyboard_callack(key, x, y);
    }
}

// -*-
void FigureBase::print(const std::string filename){
    //! @todo : allow not only saving .eps file but also .svg, .pdf, .png and .jp[e]g
    //! @note: use std::map<std::string, int> flagSelectors = {{extension, flag}, ...}
    FILE *fp;
    int state = GL2PS_OVERFLOW;
    int bufsize = 0;
    fp = std::fopen(filename.c_str(), "wb");
    std::cout << "Writing '" << filename << "' ...\n";
    while(state == GL2PS_OVERFLOW){
        bufsize += 1024*1024; // XXX
        gl2psBeginPage(
            "test", "gl2ps", NULL,
            GL2PS_EPS, GL2PS_SIMPLE_SORT,
            GL2PS_USE_CURRENT_VIEWPORT, GL_RGBA,
            0, NULL, 0, 0, 0,
            bufsize, fp, filename.c_str()
        );
        this->draw();
        state = gl2psEndPage();
    }
    fclose(fp);
    std::cout << "DONE!" << std::endl;
}

// -*-
void FigureBase::savefig(const std::string filename){
    this->print(filename);
}

// -*----------------------------------------------------------------*-
// -*- swzplot::TextBase                                            -*-
// -*----------------------------------------------------------------*-
Text TextBase::text(double x, double y, const std::string message){
    this->m_position.x = x;
    this->m_position.y = y;
    this->m_message = message;
    return this->share();
}

// -*-
void TextBase::clear(){
    this->m_message.clear();
}

// -*-
void TextBase::draw(){
    glColor3f(0.f, 0.f, 0.f);
    double x, y;
    x = this->coord2D_to_xaxis(this->m_position.x);
    y = this->coord2D_to_yaxis(this->m_position.y);
    glRasterPos2d(x, y);
    gl2psText(this->m_message.c_str(), "Arial", 12);
    for(auto i=0; i < this->m_message.size(); ++i){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, this->m_message[i]);
    }
}


// -*----------------------------------------------------------------*-
// -*- SWZPLOT PUBLIC FUNCTIONAL API                                -*-
// -*----------------------------------------------------------------*-
static FigureDict figureDict;
static Figure currentFigure;

using NamedFigureDict = std::map<std::string, Figure>;
static NamedFigureDict namedFigureDict;

static std::mutex figure_mtx;
static std::mutex namedFigure_mtx;

static int maxFigureNum = 0;

Figure figure(const std::string name){
    std::unique_lock<std::mutex> lock(namedFigure_mtx);
    Figure fig = nullptr;
    auto iter = namedFigureDict.find(name);
    if(iter == namedFigureDict.end()){
        std::cout << "New figure: " << name << std::endl;
        fig = figure();
        fig->set_window_name(name);
        namedFigureDict[name] = fig;
    }else{
        fig = iter->second;
    }
    return fig;
}

Figure figure(){
    return figure(maxFigureNum+1);
}

Figure figure(int num){
    if(num > maxFigureNum){
        maxFigureNum = num;
    }
    std::unique_lock<std::mutex> lock(figure_mtx);
    currentFigure = nullptr;
    auto iter = figureDict.find(num);
    if(iter == figureDict.end()){
        currentFigure = std::make_shared<FigureBase>();
        glut::register_figure(currentFigure);
        figureDict[num] = currentFigure;
    }else{
        currentFigure = iter->second;
    }
    return currentFigure;
}

// -
Figure gcf(){
    return currentFigure ? currentFigure : figure(0);
}

Axes subplot(unsigned int m, unsigned int n, unsigned int k){
    return gcf()->get_current_canvas()->subplot(m, n, k);
}

//! @todo: legend(...)
Canvas canvas(std::string name, bool visible){
    return gcf()->canvas(name, visible);
}

Axes gca(){ return gcf()->get_current_canvas()->gca(); }

// -*-
void set(const std::string key){ gca()->gco<LineBase>()->set(key); }

// -*-
void set(float key){ gca()->gco<LineBase>()->set(key); }


// -*-
void set(std::string key, std::string val){
    gca()->gco<LineBase>()->set(key, val);
}

// -*-
void set(std::string key, float val){
    gca()->gco<LineBase>()->set(key, val);
}

// - set axis limit (2D & 3D)
void axis(double xmin, double xmax, double ymin, double ymax){
    gca()->axis(xmin, xmax, ymin, ymax);
}

void axis(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax){
    gca()->axis(xmin, xmax, ymin, ymax, zmin, zmax);
}

// - toggle axis visibility: "on" | "off" | true | false
void axis(std::string onoff){ gca()->axis(onoff); }

void axis(bool onoff){gca()->axis(onoff); }

// - toggle grid visibility: "on" | "off" | true | false
void grid(std::string onoff){ gca()->grid(onoff); }

void grid(bool onoff){ gca()->grid(onoff); }
// - toggle ticklabel visibility: true | false
void ticklabel(bool onoff){ gca()->ticklabel(onoff); }
// - title
void title(std::string label){ gca()->title(label); }
// - [x|y|z]label
void xlabel(std::string label){ gca()->xlabel(label); }

void ylabel(std::string label){ gca()->ylabel(label); }

//! @todo: Axes zlabel(std::string label);
// - Capture mouse events
void capture_mouse(bool flag){ gca()->capture_mouse(flag); }

// - draw vertex
void vertex(double x, double y){ gca()->gco<LineBase>()->vertex(x, y); }

void vertex(double x, double y, double z){
    gca()->gco<LineBase>()->vertex(x, y, z);
}

// - plot data
Line plot(const Vector<double>& yvec){
    return gca()->add<LineBase>()->plot(yvec);
}

Line plot(const Vector<double>& xvec, const Vector<double>& yvec){
    return gca()->add<LineBase>()->plot(xvec, yvec);
}

Line plot(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& zvec
){
    return gca()->add<LineBase>()->plot(xvec, yvec, zvec);
}

// - log-scale plot
Line semilogx(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->semilogx(xvec, yvec);
}

Line semilogy(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->semilogy(xvec, yvec);
}

Line loglog(const Vector<double>& xvec, const Vector<double>&  yvec){
    return gca()->add<LineBase>()->loglog(xvec, yvec);
}

// - vertex
void vertex(double x, double y, double dx, double dy){
    gca()->gco<LineBase>()->vertex(x, y, dx, dy);
}

void errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec
){
    gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec);
}

void errorbar(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Vector<double>& dxvec, const Vector<double>& dyvec
){
    gca()->gco<LineBase>()->errorbar(xvec, yvec, dxvec, dyvec);
}

// ----
// - Surface & contour
Surface surface(const Matrix<double>& zmat){
    return gca()->add<SurfaceBase>()->surface(zmat);
}

// -*-
Surface surface(const Matrix<double>& zmat, const Matrix<double>& cmat){
    return gca()->add<SurfaceBase>()->surface(zmat, cmat);
}

// -*-
Surface surface(const Matrix<double>& zmat, const Colormap& cdata){
    return gca()->add<SurfaceBase>()->surface(zmat, cdata);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cmat);
}

Surface surface(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->surface(xvec, yvec, zmat, cdata);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<double>& cmat
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cmat);
}

Surface surface(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->surface(xmat, ymat, zmat, cdata);
}

Surface pcolor(const Matrix<double>& cmat){
    return gca()->add<SurfaceBase>()->pcolor(cmat);
}

Surface pcolor(const Colormap& cdata){
    return gca()->add<SurfaceBase>()->pcolor(cdata);
}

Surface pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& cvec
){
    return gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cvec);
}

Surface pcolor(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->pcolor(xvec, yvec, cdata);
}

Surface pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& cmat
){
    return gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cmat);
}

Surface pcolor(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Colormap& cdata
){
    return gca()->add<SurfaceBase>()->pcolor(xmat, ymat, cdata);
}

// -*-
Surface contour(const Matrix<double>& zmat){
    return gca()->add<SurfaceBase>()->contour(zmat);
}

Surface contour(const Matrix<double>& zmat, unsigned int n){
    return gca()->add<SurfaceBase>()->contour(zmat, n);
}

Surface contour(const Matrix<double>& zmat, const Vector<double>& values){
    return gca()->add<SurfaceBase>()->contour(zmat, values);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, unsigned int n
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, n);
}

Surface contour(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values
){
    return gca()->add<SurfaceBase>()->contour(xvec, yvec, zmat, values);
}

Surface mesh(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->mesh(xvec, yvec, zmat);
}

Surface surf(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat
){
    return gca()->add<SurfaceBase>()->surf(xvec, yvec, zmat);
}

// - shading:
void shading(std::string arg){
    gca()->gco<SurfaceBase>()->shading(arg);
}

// -
Patch patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    return gca()->add<PatchBase>()->patch(xmat, ymat);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, cvec);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<float>& cdata
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, cdata);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cvec);
}

Patch patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cdata
){
    return gca()->add<PatchBase>()->patch(xmat, ymat, zmat, cdata);
}

Patch bar(const Vector<double>& ydata){
    return gca()->add<PatchBase>()->bar(ydata);
}

Patch bar(const Vector<double>& ydata, float width){
    return gca()->add<PatchBase>()->bar(ydata, width);
}

Patch bar(const Vector<double>& xdata, const Vector<double>& ydata){
    return gca()->add<PatchBase>()->bar(xdata, ydata);
}

Patch bar(const Vector<double>& xdata, const Vector<double>& ydata, float width){
    return gca()->add<PatchBase>()->bar(xdata, ydata, width);
}

//! @todo: add font information
Text text(double x, double y, const std::string message){
    return gca()->add<TextBase>()->text(x, y, message);
}

Axes colorbar(){
    return gca()->colorbar();
}

void gray(){ gca()->gray(); }
void jet(){ gca()->jet(); }
void hsv(){ gca()->hsv(); }
void hot(){ gca()->hot(); }
void cool(){ gca()->cool(); }
void spring(){ gca()->spring(); }
void summer(){ gca()->summer(); }
void autumn(){ gca()->autumn(); }
void winter(){ gca()->winter(); }

// -*-
void print(std::string filename){
    if(currentFigure){
        currentFigure->print(filename);
    }
}

void savefig(std::string filename){
    print(filename);
}

void append(Line line, std::pair<double, double> point2d){
    line->vertex(point2d.first, point2d.second);
}

void append(Axes axes, std::pair<double, double> point2d){
    axes->gco<LineBase>()->vertex(point2d.first, point2d.second);
}

void append(Canvas canvas, std::pair<double, double> point2d){
    canvas->gca()->gco<LineBase>()->vertex(point2d.first, point2d.second);
}

void append(Figure fig, std::pair<double, double> point2d){
    fig->gca()->gco<LineBase>()->vertex(point2d.first, point2d.second);
}

// void append(Line line, double x, double y){
//     line->vertex(x, y);
// }

// void append(Axes axes, double x, double y){
//     axes->gco<LineBase>()->vertex(x, y);
// }

// void append(Canvas canvas, double x, double y){
//     canvas->gca()->gco<LineBase>()->vertex(x, y);
// }

// void append(Figure fig, double x, double y){
//     fig->gca()->gco<LineBase>()->vertex(x, y);
// }

// void append(Line line, Position<double> point2d){
//     line->vertex(point2d.x, point2d.y);
// }

// void append(Axes axes, Position<double> point2d){
//     axes->gco<LineBase>()->vertex(point2d.x, point2d.y);
// }

// void append(Canvas canvas, Position<double> point2d){
//     canvas->gca()->gco<LineBase>()->vertex(point2d.x, point2d.y);
// }

// void append(Figure fig, Position<double> point2d){
//     fig->gca()->gco<LineBase>()->vertex(point2d.x, point2d.y);
// }

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-