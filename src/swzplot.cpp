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
// -*- swzplot::AxesBase                                            --
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Colormap& cdata
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
    const Matrix<double>& zmat, const Colormap& cdata
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Colormap& cdata
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
    const Matrix<double>& zmat, const Colormap& cdata
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Matrix<double>& zmat, const Matrix<float>& cmat
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
    const Colormap& cdata
){
    return this->gca()->add<PatchBase>()->patch(xmat, ymat, cdata);
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-